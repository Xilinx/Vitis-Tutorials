function comp_cp = cpg_compute(cp_phase,num_cpg,ss0,ss1)

l=mod(length(ss1),8);
plin_scl=ss1(1:end-l).';
plin_indx=ss0(1:end-l).';
L=length(plin_indx);
spltbits = 5;
for i = 1:8    
    pk_val.scl(:,i) = plin_scl(i:8:end);
    pk_val.loc(:,i) = floor(plin_indx(i:8:end)/2^spltbits);    
    pk_val.off(:,i) = plin_indx(i:8:end)-(pk_val.loc(:,i)*2^spltbits);
end

cpc = cp_phase;
cp1 = cpc;
cp2 = circshift(cpc,[0,-8]);
acc = zeros(1,8);
comp_cp = [];
cpg_jump = floor(8/num_cpg);
for l=1:L/8
    for i = 1:cpg_jump:8
        remind=pk_val.loc(l,i)/4;
        cp_16 = [cp1(remind+1:remind+8) cp2(remind+1:remind+8)];
        offs = bitand(pk_val.off(l,i),15);
        rev_start = floor(pk_val.off(l,i)/2^4);
        if (rev_start == 1)
            cp_8 = cp_16(offs+1:-1:offs-6);
        else
            cp_8 = cp_16(offs+1:offs+8);
        end      
        acc = acc + cp_8*pk_val.scl(l,i);        
    end    
    comp_cp = [comp_cp convergent(acc)];      
    acc = zeros(1,8);
end
end