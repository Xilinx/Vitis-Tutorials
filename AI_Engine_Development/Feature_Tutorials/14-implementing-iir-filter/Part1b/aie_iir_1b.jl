#=
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
=#

# Ref: https://docs.juliadsp.org/latest/filters/#Filters-filter-design-and-filtering

# coefficient matrix written out as 1d array

using DSP
using PyPlot
using DoubleFloats
using Printf

# --- begin user parameters

fp = 10.0e6             # passband frequency
#fp = 20.0e6             # passband frequency
fs = 100.0e6            # sampling frequency
p = 6                   # no. of poles
rp = 0.1                # passband ripple (dB)
rs = 80.0               # stopband attenuation (dB)
N = 256                 # no. of samples for impulse response
coeff_file_suffix = "a" # file suffix to distinguish different coefficient sets
                        # with the same architecture
#coeff_file_suffix = "b" # 2nd set of coefficients
show_plots = true       # display plots?
write_cmatrix = true    # write C matrix to files?
write_impulse = true    # write impulse response?

# --- end user parameters

# define filter characteristics

responsetype = Lowpass(fp, fs = fs)
designmethod = Elliptic(p, rp, rs)

thefilter = digitalfilter(responsetype, designmethod)
H, w = freqresp(thefilter)
impresp_ref = impresp(thefilter, N)

n = 0 : (N - 1) # sampling index

if show_plots
    close("all")    # close previously open plot windows
    ws = w/pi * (fs/2)
    fig1, ax1 = subplots() 
    ax1.plot(ws, 20*log10.(abs.(H)))
    ax1.grid("on")
    ax1.set_title("Original Filter's Frequency Response")
    ax1.set_xlabel("Frequency (MHz)")
    ax1.set_ylabel("Amplitude (dB)")
    ax1.set_xlim([0, fs/2])
    ax1.set_ylim([-120, 20])    # adjust as necessary

    fig2, ax2 = subplots()
    ax2.plot(impresp_ref)
    ax2.grid("on")
    ax2.set_title("Original Filter's Impulse Response")
    ax2.set_xlabel("Sampling Index")
    ax2.set_ylabel("Amplitude")
    ax2.set_xlim([0, (N - 1)])
    #ax2.set_ylim([-0.1, 0.25])  # adjust as necessary
end # if show_plots

thesos = convert(SecondOrderSections, thefilter)    # convert to SecondOrderSections form
num_sections = length(thesos.biquads)   # no. of SOS

g = zeros(Double64, num_sections, 1)      # stage gains
C = zeros(Double64, 8, 12, num_sections)  # matrix of SIMD coefficients
k = zeros(Double64, 8, 12)

# initialize SIMD coefficient matrix
for w = 1 : num_sections
    a = Vector{Double64}(coefa(thesos.biquads[w]))
    b = Vector{Double64}(coefb(thesos.biquads[w]))
    g[w] = sum(a)/sum(b)                            
    if (w == num_sections)
        g[w] *= thesos.g/prod(g)    # adjust gain of last stage such that prod(g) = thesos.g
    end # if (w == num_sections)

    # a0 is *always* 1.0
    a1 = a[2]   
    a2 = (length(a) < 3) ? 0 : a[3] # handle odd number of poles
    b0 = b[1]
    b1 = b[2]
    b2 = (length(b) < 3) ? 0 : b[3]
    K  = g[w]

    # declare SIMD constants

    # Ky0_*
    k[1,  1] = -a2;    # ym2
    k[1,  2] = -a1;    # ym1
    k[1,  3] = b2*K;   # xm2
    k[1,  4] = b1*K;   # xm1
    k[1,  5] = b0*K;   # x0

    # Ky1_*
    k[2,  1] = a1*a2;              # ym2
    k[2,  2] = a1^2 - a2;          # ym1
    k[2,  3] = -(a1*b2*K);         # xm2
    k[2,  4] = (-(a1*b1) + b2)*K;  # xm1
    k[2,  5] = (-(a1*b0) + b1)*K;  # x0
    k[2,  6] = b0*K;               # x1

    # Ky2_*
    k[3,  1] = a2*(-a1^2 + a2);                    # ym2
    k[3,  2] = -a1^3 + 2*a1*a2;                    # ym1
    k[3,  3] = (a1^2 - a2)*b2*K;                   # xm2
    k[3,  4] = (a1^2*b1 - a2*b1 - a1*b2)*K;        # xm1
    k[3,  5] = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;   # x0
    k[3,  6] = (-(a1*b0) + b1)*K;                  # x1
    k[3,  7] = b0*K;                               # x2

    # Ky3_*
    k[4,  1] = a1*(a1^2 - 2*a2)*a2;                                    # ym2
    k[4,  2] = a1^4 - 3*a1^2*a2 + a2^2;                                # ym1
    k[4,  3] = -(a1*(a1^2 - 2*a2)*b2*K);                               # xm2
    k[4,  4] = (-(a1^3*b1) + 2*a1*a2*b1 + a1^2*b2 - a2*b2)*K;          # xm1
    k[4,  5] = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);  # x0
    k[4,  6] = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;                       # x1
    k[4,  7] = (-(a1*b0) + b1)*K;                                      # x2
    k[4,  8] = b0*K;                                                   # x3

    # Ky4_*
    k[5,  1] = -(a2*(a1^4 - 3*a1^2*a2 + a2^2));                                                # ym2
    k[5,  2] = -(a1*(a1^4 - 4*a1^2*a2 + 3*a2^2));                                              # ym1
    k[5,  3] = (a1^4 - 3*a1^2*a2 + a2^2)*b2*K;                                                 # xm2
    k[5,  4] = (a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 - a1^3*b2 + 2*a1*a2*b2)*K;                    # xm1
    k[5,  5] = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;    # x0
    k[5,  6] = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);                          # x1
    k[5,  7] = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;                                               # x2
    k[5,  8] = (-(a1*b0) + b1)*K;                                                              # x3
    k[5,  9] = b0*K;                                                                           # x4

    # Ky5_*
    k[6,  1] = a1*a2*(a1^4 - 4*a1^2*a2 + 3*a2^2);                                                                  # ym2
    k[6,  2] = a1^6 - 5*a1^4*a2 + 6*a1^2*a2^2 - a2^3;                                                              # ym1
    k[6,  3] = -(a1*(a1^4 - 4*a1^2*a2 + 3*a2^2)*b2*K);                                                             # xm2
    k[6,  4] = (-(a1^5*b1) + 4*a1^3*a2*b1 - 3*a1*a2^2*b1 + a1^4*b2 - 3*a1^2*a2*b2 + a2^2*b2)*K;                    # xm1
    k[6,  5] = (-(a1^5*b0) + a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 + a1^3*(4*a2*b0 - b2) + a1*a2*(-3*a2*b0 + 2*b2))*K;  # x0
    k[6,  6] = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;                        # x1
    k[6,  7] = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))* K);                                             # x2
    k[6,  8] = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;                                                                   # x3
    k[6,  9] = (-(a1*b0) + b1)*K;                                                                                  # x4
    k[6, 10] = b0*K;                                                                                               # x5

    # Ky6_*
    k[7,  1] = a2*(-a1^6 + 5*a1^4*a2 - 6*a1^2*a2^2 + a2^3);                                                                                    # ym2
    k[7,  2] = -a1^7 + 6*a1^5*a2 - 10*a1^3*a2^2 + 4*a1*a2^3;                                                                                   # ym1
    k[7,  3] = (a1^6 - 5*a1^4*a2 + 6*a1^2*a2^2 - a2^3)*b2*K;                                                                                   # xm2
    k[7,  4] = (a1^6*b1 - 5*a1^4*a2*b1 + 6*a1^2*a2^2*b1 - a2^3*b1 - a1^5*b2 + 4*a1^3*a2*b2 - 3*a1*a2^2*b2)*K;                                  # xm1
    k[7,  5] = (a1^6*b0 - a1^5*b1 + 4*a1^3*a2*b1 - 3*a1*a2^2*b1 + 3*a1^2*a2*(2*a2*b0 - b2) + a1^4*(-5*a2*b0 + b2) + a2^2*(-(a2*b0) + b2))*K;   # x0
    k[7,  6] = (-(a1^5*b0) + a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 + a1^3*(4*a2*b0 - b2) + a1*a2*(-3*a2*b0 + 2*b2))*K;                              # x1
    k[7,  7] = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;                                                    # x2
    k[7,  8] = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))* K);                                                                         # x3
    k[7,  9] = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;                                                                                               # x4
    k[7, 10] = (-(a1*b0) + b1)*K;                                                                                                              # x5
    k[7, 11] = b0*K;                                                                                                                           # x6

    # Ky7_*
    k[8,  1] = a1*a2*(a1^6 - 6*a1^4*a2 + 10*a1^2*a2^2 - 4*a2^3);                                                                                                   # ym2
    k[8,  2] = a1^8 - 7*a1^6*a2 + 15*a1^4*a2^2 - 10*a1^2*a2^3 + a2^4;                                                                                              # ym1
    k[8,  3] = -(a1*(a1^6 - 6*a1^4*a2 + 10*a1^2*a2^2 - 4*a2^3)*b2* K);                                                                                             # xm2
    k[8,  4] = (-(a1^7*b1) + 6*a1^5*a2*b1 - 10*a1^3*a2^2*b1 + 4*a1*a2^3*b1 + a1^6*b2 - 5*a1^4*a2*b2 + 6*a1^2*a2^2*b2 - a2^3*b2)*K;                                 # xm1
    k[8,  5] = -((a1^7*b0 - a1^6*b1 + 5*a1^4*a2*b1 - 6*a1^2*a2^2*b1 + a2^3*b1 + 2*a1^3*a2*(5*a2*b0 - 2*b2) + a1^5*(-6*a2*b0 + b2) + a1*a2^2*(-4*a2*b0 + 3*b2))*K); # x0
    k[8,  6] = (a1^6*b0 - a1^5*b1 + 4*a1^3*a2*b1 - 3*a1*a2^2*b1 + 3*a1^2*a2*(2*a2*b0 - b2) + a1^4*(-5*a2*b0 + b2) + a2^2*(-(a2*b0) + b2))*K;                       # x1
    k[8,  7] = (-(a1^5*b0) + a1^4*b1 - 3*a1^2*a2*b1 + a2^2*b1 + a1^3*(4*a2*b0 - b2) + a1*a2*(-3*a2*b0 + 2*b2))*K;                                                  # x2
    k[8,  8] = (a1^4*b0 - a1^3*b1 + 2*a1*a2*b1 + a2*(a2*b0 - b2) + a1^2*(-3*a2*b0 + b2))*K;                                                                        # x3
    k[8,  9] = -((a1^3*b0 - a1^2*b1 + a2*b1 + a1*(-2*a2*b0 + b2))*K);                                                                                              # x4
    k[8, 10] = (a1^2*b0 - a2*b0 - a1*b1 + b2)*K;                                                                                                                   # x5
    k[8, 11] = (-(a1*b0) + b1)*K;                                                                                                                                  # x6
    k[8, 12] = b0*K;                                                                                                                                               # x7

    # copy intermediate results into the C matrix
    maxC = 0
    for u = 1 : 8
        for v = 1 : 12
            maxC = (k[u, v] > maxC) ? k[u, v] : maxC
            C[u, v, w] = k[u, v]
        end # for v = 1 : 12
    end # for u = 1 : 8
    @printf("\nLargest value in SIMD coefficient matrix for stage %d:\n", w)
    @printf("  maxC = %1.23e\n", maxC)

end # for w = 1 : num_sections

if write_cmatrix

    copyright_str = """
    /****************************************************************
    # Support
    GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

    # License   
    Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.    
    You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

    <p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
    ****************************************************************/
    """

    # write it out column-wise
    for u = 1 : num_sections
        fname = string("C", string(u), coeff_file_suffix, ".h")
        def_header_name = string("__C", string(u), uppercase(coeff_file_suffix), "_H__")
        fid = open(fname, "w")
        @printf(fid, "%s\n", copyright_str)
        @printf(fid, "#ifndef %s\n", def_header_name)
        @printf(fid, "  #define %s\n\n", def_header_name)
        @printf(fid, "  // SIMD matrix of coefficients for IIR filter stage %d\n", u)
        @printf(fid, "  alignas(16) const float C%d%s[%d] = {\n\n", u, coeff_file_suffix, (12 * 8))
        for v = 1 : 12
            for w = 1 : 8
                @printf(fid, "    %+1.23e", C[w, v, u])
                if !((w == 8) && (v == 12))
                    @printf(fid, ",")
                end
                @printf(fid, "\t// Ky%d_", (w - 1))
                if (v < 5)
                    if (v < 3)
                        @printf(fid, "ym%d\n", (v == 1) ? 2 : 1)
                    else
                        @printf(fid, "xm%d\n", (v == 3) ? 2 : 1)
                    end
                else
                    @printf(fid, "x%d\n", (v - 5))
                end
                if (w == 8)
                    @printf(fid, "\n")
                end
            end # for w = 1 : 8
        end # for v = 1 : 12
        @printf(fid, "  };\n\n")
        @printf(fid, "#endif // %s\n", def_header_name)
        close(fid)
    end # for u = 1 : num_sections
end # if write_cmatrix

if show_plots
    Hsos, wsos = freqresp(thesos)
    wsosp = wsos/pi * (fs/2)
    fig3, ax3 = subplots()
    ax3.plot(wsosp, 20*log10.(abs.(Hsos)))
    ax3.grid("on")
    ax3.set_title("SOS Frequency Response")
    ax3.set_xlabel("Frequency (MHz)")
    ax3.set_ylabel("Amplitude (dB)")
    ax3.set_xlim([0, fs/2])
    ax3.set_ylim([-120, 20])    # adjust as necessary
end # if show_plots


# verification (check impulse response)
in_data = zeros(Double64, N, 1)     # input data
in_data[1] = 1                      # Dirac delta input
out_data = zeros(Double64, N, 1)    # output data

xreg = zeros(Double64, 16, num_sections)    # xreg[5]: ym2, xreg[6]: ym1, xreg[7]: xm2, xreg[8]: xm1, xreg[9:16]: x0:x7
yreg = zeros(Double64,  8, num_sections)

for u = 1 : 8 : N

    for v = 1 : num_sections

        # transfer input data to internal registers
        for w = 1 : 8
            if v == 1
                xreg[(w + 8), v] = in_data[u + w - 1]
            else
                xreg[(w + 8), v] = yreg[w, (v - 1)]
            end # if v == 1
        end # for v = 1 : 8

        #=
        @printf("u = %d: v = %d\n", u, v)
        for w = 1 : 10
            @printf("  xreg[%d] = %f\n", (11 - w), xreg[11 - w])
        end
        @printf("\n")
        for w = 1 : 10
            @printf("  yreg[%d] = %f\n", (11 - w), yreg[11 - w])
        end
        @printf("\n")
        =#

        # process one section

        for w = 1 : 8
            yreg[w, v] = 0      # clear result register befor multiply-accumulate operation
        end # for w = 1 : 8

        for w = 1 : 12
            for r = 1 : 8
                yreg[r, v] += C[r, w, v] * xreg[(w + 4), v]
            end # for r = 1 : 8
        end # for w = 1 : 12

        if v == num_sections
            for w = 1 : 8
                out_data[u + w - 1] = yreg[w, v]
            end # for w = 1 : 8
        end # if v == num_sections

        # update "tails"
        xreg[5, v] = yreg[ 7, v]    # ym2
        xreg[6, v] = yreg[ 8, v]    # ym1
        xreg[7, v] = xreg[15, v]    # xm2
        xreg[8, v] = xreg[16, v]    # xm1
        
    end # for v = 1 : num_sections
    
end # for u = 1 : 8 : n

if show_plots
    fig4, ax4 = subplots()
    ax4.plot(n, out_data)
    ax4.grid("on")
    ax4.set_title("SOS Impulse Response")
    ax4.set_xlabel("Sampling Index")
    ax4.set_ylabel("Amplitude")
    ax4.set_xlim([0, (N - 1)])
    #ax4.set_ylim([-120, 20])    # adjust as necessary

    impresp_err = impresp_ref - out_data
    fig5, ax5 = subplots()
    ax5.plot(n, impresp_err)
    ax5.grid("on")
    ax5.set_title("Impulse Response Error")
    ax5.set_xlabel("Sampling Index")
    ax5.set_ylabel("Error")
    ax5.set_xlim([0, (N - 1)])
    #ax5.set_ylim([-120, 20])    # adjust as necessary
end # if show_plots

if write_impulse
    fid1 = open("input.dat", "w")
    impresp_fname = string("impresponse_", coeff_file_suffix, ".dat")
    fid2 = open(impresp_fname, "w")
    for u = 1 : N 
        @printf(fid1, "%f\n", in_data[u])
        @printf(fid2, "%+1.20e\n", impresp_ref[u])
    end # for u = 1 : N    
    close(fid1)
    close(fid2)
end # if write_impulse

