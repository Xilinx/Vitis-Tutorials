function [ss0,ss1,stats] = pccfr_pl_kernel(x,threshold,num_cpg,peak_det_win,block_size,cp_len_1x,cp_idx)

group_del=block_size+1;
ss0(1:group_del) = zeros(1,group_del);
ss1(1:group_del) = zeros(1,group_del);
fir_pln = zeros(1,5);
cnt_glbl=0;
h4 = round((2*firpm(6,[0 .2 .8 1],[1 1 0  0]))*2^15);
max_arr = zeros(1,3);
detect_flag_t=0;
threshold_sq=threshold*threshold;
threshold_tol = threshold_sq/128;
threshold_margin = threshold_sq+threshold_tol;
new_cpg_alloc=0;
vec_len=8;
cplen_vec=(cp_len_1x+2-1)/vec_len;
center_pos = (cp_len_1x+2-1)/(vec_len*2);
cpg.ss1(1:8)=zeros(1,8);
cpg.ss0(1:8)=zeros(1,8);
cpg.count(1:8)=ones(1,8)*cplen_vec;
cpg.phase(1:8)=zeros(1,8);
cpg.offset(1:8)=zeros(1,8);
cpg.count_CP_half(1:8)=zeros(1,8);
out_cnt=group_del+1;
count_t=-4;
detect_flag=0;
max_val_t=threshold_sq;
fir_max=0;
max_phase=0;
stats.num_peaks_detected = 0;
stats.num_peaks_allocated = 0;
stats.max_cpg = 0;
div_factor = floor(8/num_cpg);
for i = 1:2:length(x)-1 
    fir_pln(5) = fir_pln(3);
    fir_pln(4) = fir_pln(2);
    fir_pln(3) = fir_pln(1);
    fir_pln(2) = x(i); 
    fir_pln(1) = x(i+1);
    cnt_glbl = cnt_glbl+2;
    fir_o(5) = fir_pln(4);
    fir_o(4) = floor(((fir_pln(5)+fir_pln(2))*h4(1)+(fir_pln(4)+fir_pln(3))*h4(3))/2^15);
    fir_o(3) = fir_pln(3);
    fir_o(2) = floor(((fir_pln(4)+fir_pln(1))*h4(1)+(fir_pln(3)+fir_pln(2))*h4(3))/2^15);
    fir_o(1) = fir_pln(2);    
    mag = abs(fir_o).^2;
    fir_mag = fir_o(5:-1:2);    
    [max_val,max_indx]  = max(mag(5:-1:2));  
    fir_max_t=fir_max;
    max_phase_t=max_phase;
    max_phase = cp_idx(max_indx);
    fir_max = fir_mag(max_indx);
    mval_left = max_arr(2);
    mval_mid =  max_arr(1);
    mval_right = max_val;
    max_arr(2) = max_arr(1);
    max_arr(1) = max_val;
    if ((mval_mid > mval_left) && (mval_mid >= mval_right) && (mval_mid > max_val_t))
        count_t=0;
        detect_flag_t = 1;
        max_loc_t = mod(floor(cnt_glbl/2),vec_len);
        max_val_t = mval_mid;
        peak_sample_t = fir_max_t;
        phase_t = max_phase_t;
    elseif (count_t == peak_det_win-1)
        if (detect_flag_t)
            detect_flag = (max_val_t > threshold_margin);
            max_loc=max_loc_t;            
            peak_sample=peak_sample_t;
            phase=phase_t;
        end
        detect_flag_t=0;
        count_t=0;
        max_val_t=threshold_sq;
    else
        count_t=count_t+1;
    end
    if (detect_flag)
        [mag, cos_fx, sin_fx] = cordic_abs_sin_cos_ba(peak_sample);
        sc_fact = floor(((threshold-mag)*(cos_fx+1i*sin_fx))/2^15);           
        detect_flag=0;
        new_cpg_alloc=1;
        stats.num_peaks_detected = stats.num_peaks_detected+1;
    end
    cpg_indx=bitand(floor(abs(cnt_glbl)/2),7)+1;     
    if (cpg.count(cpg_indx) ~=  cplen_vec)
        cpg.count(cpg_indx)=cpg.count(cpg_indx)+1;
        if(cpg.phase(cpg_indx) == cp_idx(2))
            cpg.ss0(cpg_indx) = (15 - bitand(cpg.offset(cpg_indx),15)) + 16;
            cpg.ss0(cpg_indx) = cpg.ss0(cpg_indx) + ((-cpg.count(cpg_indx) + cpg.phase(cpg_indx))*2^10);
        elseif(cpg.phase(cpg_indx) == cp_idx(4))
            cpg.ss0(cpg_indx) = bitand(cpg.offset(cpg_indx),15)-1;
            cpg.ss0(cpg_indx) = cpg.ss0(cpg_indx) + ((cpg.count(cpg_indx)  + cpg.phase(cpg_indx))*2^10);
        else
            lef_en = (cpg.count(cpg_indx) <= center_pos);
            cpg.count_CP_half(cpg_indx) = cpg.count_CP_half(cpg_indx) - (cpg.count(cpg_indx) > center_pos) + lef_en;
            if(cpg.phase(cpg_indx) == cp_idx(1))
                cpg.ss0(cpg_indx) = lef_en*bitand(cpg.offset(cpg_indx),15)+(1-lef_en)*((15 - bitand(cpg.offset(cpg_indx),15))+16);               
            else
                cpg.ss0(cpg_indx) = lef_en*bitand(cpg.offset(cpg_indx),15)+(1-lef_en)*((16 - bitand(cpg.offset(cpg_indx),15))+16);
            end
            cpg.ss0(cpg_indx) = cpg.ss0(cpg_indx) + ((cpg.count_CP_half(cpg_indx) + cpg.phase(cpg_indx))*2^10);         
        end
    else 
        cpg.ss0(cpg_indx) = 0;
        cpg.ss1(cpg_indx) = 0;
    end    
    ss0(out_cnt)=cpg.ss0(bitand(floor(abs(cnt_glbl)/(2*div_factor)),num_cpg-1)+1);
    ss1(out_cnt)=cpg.ss1(bitand(floor(abs(cnt_glbl)/(2*div_factor)),num_cpg-1)+1);    
    out_cnt=out_cnt+1;
    if (new_cpg_alloc && (bitand(cnt_glbl,15)==14))
        index=0;
        for cpg_cnt = 1:num_cpg
            if (cpg.count(cpg_cnt)==cplen_vec)
                index=cpg_cnt;
                if (cpg_cnt > stats.max_cpg)
                    stats.max_cpg = cpg_cnt;
                end
                stats.num_peaks_allocated = stats.num_peaks_allocated+1;
                break;
            end
        end
        if(index > 0)
            cpg.ss0(index)=8-max_loc;
            cpg.ss1(index)=sc_fact;
            cpg.count(index)=-1;
            cpg.phase(index)=phase;
            cpg.offset(index)=8-max_loc;
            cpg.count_CP_half(index)=-1;
        end
        new_cpg_alloc=0;
    end
end

end
