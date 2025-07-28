#====================================================================
  Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: MIT
====================================================================#

using Printf

function arr2file(x, rmajor, plio_width, fname)
    #=
    Input arguments:
        x           : input array
                    : x[h, w, d] => multiple matrices
                    : x[h, w] => multiple vectors
        rmajor      : bool, True when "row-major"
        plio_width  : PLIO width in {32, 64, 128}
        fname       : file name with extension "csv" or "dat"
    =#
    
    etype = string(eltype(x))               # element type
    iscmplx = occursin("Complex", etype)    # true (non-zero) when complex
    if (match(r"[I]", etype) === nothing)   # base type (looking for "Int")
        btype = "Float"
    else
        btype = "Int"
    end # if-else (match(r"[I]", etype) === nothing)
        
    nbits = parse(Int, match(r"\d+", etype).match)  # number of bits in an element

    if (nbits == 8)         # print width for integers
        dwidth = 6
    elseif (nbits == 16)
        dwidth = 8
    else
        dwidth = 14
    end # if-else (nbits == 8)
    
    dcols = plio_width ÷ nbits  # number of data columns
    
    if (match(r".*\.csv", fname) === nothing)   # file type
        ftype = "dat"
    else
        ftype = "csv"
    end # if-else (match(r".*\.csv", fname) === nothing)
    
    height = x.size[1]  # matrix height
    width = x.size[2]   # matrix width
    
    if (length(x.size) == 3)
        depth = x.size[3]   # matrix depth
    else
        depth = 1
    end # if-else (length(x.size) == 3)

    if debug
        @printf("Input:\n")
        display(x)
        @printf("\n")
        @printf("etype = %s\n", etype)
        @printf("iscmplx = %d\n", iscmplx)
        @printf("btype = %s\n", btype)
        @printf("nbits = %d\n", nbits)
        @printf("dwidth = %d\n", dwidth)
        @printf("dcols = %d\n", dcols)
        @printf("ftype = %s\n", ftype)
        @printf("height = %d\n", height)
        @printf("width = %d\n", width)
        @printf("depth = %d\n", depth)
        @printf("\n")
        @printf("%s:\n", fname)
        @printf("\n")
    end
    
    io = open(fname, "w")   # file ID
    
    cidx = 0    # column index
    if (ftype == "dat")
        for i in 1 : depth
            if (rmajor)
                y = x[:, :, i]
            else
                y = conj(x[:, :, i]')   # transpose without conjugation
                temp = height           # swap height and width
                height = width
                width = temp
            end
            for j = 1 : height
                for k = 1 : width
                    if (iscmplx)
                        if (btype == "Float")                        
                            if ((nbits == 32) && (dcols == 1))                            
                                @printf(io, "%1.12e\n", real(y[j, k]))                                
                                if debug
                                    @printf("%1.12e\n", real(y[j, k]))
                                end                                
                                @printf(io, "%1.12e\n", imag(y[j, k]))                                
                                if debug
                                    @printf("%1.12e\n", imag(y[j, k]))
                                end
                            else
                                @printf(io, "%1.12e  %1.12e  ", real(y[j, k]), imag(y[j, k]))                                
                                if debug
                                    @printf("%1.12e  %1.12e  ", real(y[j, k]), imag(y[j, k]))
                                end                            
                                cidx += 2                            
                                if (cidx == dcols)                                
                                    @printf(io, "\n")                                    
                                    if debug
                                        @printf("\n")
                                    end                                    
                                    cidx = 0                                    
                                end # if (cidx == dcols)                                 
                            end # if-else ((nbits == 32) && (dcols == 1))                                                    
                        else # "Int"                        
                            if ((nbits == 32) && (dcols == 1))
                                @printf(io, "%*d\n", dwidth, real(y[j, k]))
                                if debug
                                    @printf("%*d\n", dwidth, real(y[j, k]))
                                end
                                @printf(io, "%*d\n", dwidth, imag(y[j, k]))
                                if debug
                                    @printf("%*d\n", dwidth, imag(y[j, k]))
                                end
                            else
                                @printf(io, "%*d  %*d", dwidth, real(y[j, k]), dwidth, imag(y[j, k]))
                                if debug
                                    @printf("%*d  %*d", dwidth, real(y[j, k]), dwidth, imag(y[j, k]))
                                end                                
                                cidx += 2                                
                                if (cidx == dcols)                                
                                    @printf(io, "\n")                                    
                                    if debug
                                        @printf("\n")
                                    end                                    
                                    cidx = 0                                    
                                end # if (cidx == dcols)                            
                            end # if ((nbits == 32) && (dcols == 1))                            
                        end # if-else (btype == "Float")                        
                    else # real only                    
                        if (btype == "Float")
                            @printf(io, "%1.12e", y[j, k])
                            if debug
                                @printf("%1.12e", y[j, k])
                            end
                            cidx += 1
                            if (cidx == dcols)
                                @printf(io, "\n")
                                if debug
                                    @printf("\n")
                                end
                                cidx = 0
                            end # if (cidx == dcols)
                        else # "Int"
                            @printf(io, "%*d", dwidth, y[j, k])
                            if debug
                                @printf("%*d", dwidth, y[j, k])
                            end
                            cidx += 1
                            if (cidx == dcols)
                                @printf(io, "\n")
                                if debug
                                    @printf("\n")
                                end
                                cidx = 0
                            end # if (cidx == dcols)
                        end # if-else (btype == "Float")                    
                    end # if-else (iscmplx)
                end # for k = 1 : width
            end # for j = 1 : height
        end # for i in 1 : depth
    else # csv
        # !!! T.B.D. !!!
    end
    
    if debug
        @printf("\n")
        @printf("****************************************************\n")
        @printf("\n")
    end
    
    close(io)
        
end # arr2file()

