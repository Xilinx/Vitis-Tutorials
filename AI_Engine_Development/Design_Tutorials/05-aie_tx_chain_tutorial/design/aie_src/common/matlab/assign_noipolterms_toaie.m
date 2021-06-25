function [params,aie_params] = assign_noipolterms_toaie(params,aie_params)

M=size(params.term_selection,2);
R=size(params.term_selection,1);

aie_dpd_cores = max(max(params.term_selection));

for i = 1:M
    for j = 1:R
        terms_dist(R-j+1,M-i+1) = params.term_selection(j,i);
        mmp_luts(R-j+1,M-i+1,:) = params.mmp_LUTs(j,i,:);
    end
end
params.mmp_LUTs_T = mmp_luts;
params.term_selection_T = terms_dist;
phs=aie_params.phases;
for num_c = aie_dpd_cores:-1:1
    terms = (terms_dist == num_c);    
    indx1=1;    
    indx2=1;
    lut_8_0 = zeros(8,256);
    lut_8_1 = zeros(8,256);
    for i = 1:M
        indx=1;
        col_sum = sum(terms(:,i)); 
        for j = 1:R
            if (terms(j,i) == 1)                
                if (indx == 1)
                    lut_8_0(indx1,:) = mmp_luts(j,i,:);    
                    indx1=indx1+1; 
                elseif (indx == 2)
                    lut_8_1(indx2,:) = mmp_luts(j,i,:); 
                    indx2=indx2+1;
                elseif (indx == 3)
                    lut_8_1(indx2,:) = mmp_luts(j,i,:); 
                    indx2=indx2+1;
                    indx1=indx1+1;
                end                
                indx=indx+1;                               
            end
        end        
    end
    aie_params.core(num_c).LUTS0 = lut_8_1;
    aie_params.core(num_c).LUTS1 = lut_8_0;
end

end