function [ dpdout] = dpd_noipol(signal, mag, dpd_params, dpd_aie_params)

M=size(dpd_params.term_selection,2);
R=size(dpd_params.term_selection,1);
mag_width = dpd_params.mag_width;
lut_addr_width = dpd_params.lut_addr_width;
mmp_LUTs = dpd_params.mmp_LUTs;

if ~isfield(dpd_aie_params,'lutsw')
   lutsw = zeros(1,length(signal)); 
else
   lutsw = dpd_aie_params.lutsw;
end
if ~isfield(dpd_params,'mmp_LUTs2')
   mmp_LUTs2 = zeros(R,M,256); 
else
   mmp_LUTs2 = dpd_params.mmp_LUTs2;
end

% model of data path filter with RTL finite precision
for m = 1:M % columns    
    col_luts_sum=0;    
    for r = 1:R % rows        
        delm = r - 1;
        delayed_mag = circshift(mag,[0 -delm]);
        % non-interplated LUTs (floor)
        LUT_index = floor(delayed_mag/2^(mag_width-lut_addr_width));              
        % LUT Read
        LUT_res = mmp_LUTs(r, m, 1 + LUT_index); 
        LUT_res2 = mmp_LUTs2(r, m, 1 + LUT_index);
        intLUT(r, :) = LUT_res(1,:);
        intLUT2(r, :) = LUT_res2(1,:);
    end        
    % adder tree    
    col_luts_sum = sum(intLUT,1);  
    col_luts_sum2 = sum(intLUT2,1);
    dely = m - 1;    
    delayed_data = circshift(signal,[0 -dely]);    
    col_luts_sum_t = col_luts_sum.*(1-lutsw)+col_luts_sum2.*lutsw;
    mr_term(m, :) = col_luts_sum_t.*delayed_data;   
end
dpd_out = sum(mr_term,1);
dpdout = aie_srs(dpd_out,dpd_aie_params.aie_output_shift); % saturate and round output from last core
end
