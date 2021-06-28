function y = pccfr_aie_graph(x,block_size,DFE_SPEC)

x_in = x;
ups_shift=14;

if (DFE_SPEC.cfr_fs == 245.76)
    cp_len_1x = length(DFE_SPEC.cpulse200MHz_1x)-2;
    cpulse = DFE_SPEC.cpulse200MHz_4x;
    cpulse_1x = DFE_SPEC.cpulse200MHz_1x;    
    cp_idx = [64,40,41,0];
    cp_len = length(cpulse(5:end-7));
    cp_len_by2 = (length(cpulse(5:end-7))-1)/2;
    cp_phase = [zeros(1,9) cpulse(1,5+3:4:cp_len) ...
                zeros(1,11) cpulse(1,5:4:cp_len_by2+4*16) ...
                zeros(1,10) cpulse(1,5+2:4:cp_len_by2+4*9)];
else
    cp_len_1x = length(DFE_SPEC.cpulse400MHz_1x)-2;
    cpulse = DFE_SPEC.cpulse400MHz_4x;
    cpulse_1x = DFE_SPEC.cpulse400MHz_1x;
    cp_idx = [100,64,65,0];
    cp_len = length(cpulse(5:end-7));
    cp_len_by2 = (length(cpulse(5:end-7))-1)/2;
    cp_phase = [zeros(1,9) cpulse(1,5+3:4:cp_len) ...
                zeros(1,11) cpulse(1,5:4:cp_len_by2+4*17) ...
                zeros(1,9) cpulse(1,5+2:4:cp_len_by2+4*11)];
end

for i = 1:DFE_SPEC.iterations        
    pl_in2x = convergent(upfirdn(x_in,DFE_SPEC.tx_hb23,2,1)/2^15);
    % PL Kernel - Peak Detect
    [ss0,ss1,stats] = pccfr_pl_kernel(pl_in2x,DFE_SPEC.threshold,DFE_SPEC.num_cpg(i),DFE_SPEC.peak_detect_window(i),block_size,cp_len_1x,cp_idx);
    % CPG Compute in AIE
    comp_cp = cpg_compute(cp_phase,DFE_SPEC.num_cpg(i),ss0,ss1);
    iter_x = [zeros(1,block_size+16+DFE_SPEC.peak_detect_window(i)+(length(cpulse_1x)-1)/2) x_in*2^ups_shift];
    % Pulse Cancellation
    x_in = iter_x(1:length(comp_cp)) + comp_cp;
    x_in = convergent(x_in/2^ups_shift);
    fprintf(1,'    PC-CFR Iteration %d: Num of Peaks detected = %d; Num of Peaks Allocated = %d Num CPGs used = %d\n',i,stats.num_peaks_detected,stats.num_peaks_allocated,stats.max_cpg);
end

y=x_in;

end
