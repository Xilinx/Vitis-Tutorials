function y = fir_199t_sym_aie(coeffs,x,block_size,shift)

iter = block_size/8;
lbuff_v16 = zeros(1,16);
rbuff_v16 = zeros(1,16);
decr = 25;
incr = 0;
lbuff_v16(1:8)  = x(1+incr*8:8+incr*8);
rbuff_v16(9:16) = x(1+decr*8:8+decr*8);
decr = decr-1;
incr = incr+1;
lbuff_v16(9:16) = x(1+incr*8:8+incr*8);
rbuff_v16(1:8)  = x(1+decr*8:8+decr*8);
for i = 1:iter    
    acc = zeros(2,4);
    coeff_incr = 0;
    coeff_v16(1:8)  = coeffs(1+coeff_incr*8:8+coeff_incr*8);
    coeff_incr = coeff_incr+1;
    coeff_v16(9:16) = coeffs(1+coeff_incr*8:8+coeff_incr*8);
    for j = 1:6
        acc(1,:) = mac4_sym(acc(1,:),lbuff_v16,0,[0 1 2 3],1,rbuff_v16,6,coeff_v16,0,[0 0 0 0],1);
        acc(2,:) = mac4_sym(acc(2,:),lbuff_v16,4,[0 1 2 3],1,rbuff_v16,10,coeff_v16,0,[0 0 0 0],1); 
        acc(2,:) = mac4_sym(acc(2,:),lbuff_v16,8,[0 1 2 3],1,rbuff_v16,6,coeff_v16,4,[0 0 0 0],1);
        decr = decr-1;    
        rbuff_v16(9:16) = x(1+decr*8:8+decr*8);
        acc(1,:) = mac4_sym(acc(1,:),lbuff_v16,4,[0 1 2 3],1,rbuff_v16,2,coeff_v16,4,[0 0 0 0],1);        
        incr = incr+1;
        coeff_incr = coeff_incr+1;
        lbuff_v16(1:8) = x(1+incr*8:8+incr*8);
        coeff_v16(1:8) = coeffs(1+coeff_incr*8:8+coeff_incr*8);        
        acc(1,:) = mac4_sym(acc(1,:),lbuff_v16,8,[0 1 2 3],1,rbuff_v16,14,coeff_v16,8,[0 0 0 0],1);
        acc(2,:) = mac4_sym(acc(2,:),lbuff_v16,12,[0 1 2 3],1,rbuff_v16,2,coeff_v16,8,[0 0 0 0],1);
        acc(2,:) = mac4_sym(acc(2,:),lbuff_v16,0,[0 1 2 3],1,rbuff_v16,14,coeff_v16,12,[0 0 0 0],1); 
        decr = decr-1;    
        rbuff_v16(1:8) = x(1+decr*8:8+decr*8);    
        acc(1,:) = mac4_sym(acc(1,:),lbuff_v16,12,[0 1 2 3],1,rbuff_v16,10,coeff_v16,12,[0 0 0 0],1);               
        incr = incr+1;
        coeff_incr = coeff_incr+1;
        coeff_v16(9:16) = coeffs(1+coeff_incr*8:8+coeff_incr*8);
        lbuff_v16(9:16) = x(1+incr*8:8+incr*8);
    end       
    acc(1,:) = mac4_sym(acc(1,:),lbuff_v16,0,[0 1 2 3],1,rbuff_v16,6,coeff_v16,0,[0 0 0 0],1);
    acc(2,:) = mac4_sym(acc(2,:),lbuff_v16,4,[0 1 2 3],1,rbuff_v16,10,coeff_v16,0,[0 0 0 0],1);
    incr = incr-12;
    decr = decr+14;   
    lbuff_v16(1:8) = x(1+incr*8:8+incr*8);
    rbuff_v16(9:16) = x(1+decr*8:8+decr*8);
    incr = incr+1;
    decr = decr-1;
    lbuff_v16(9:16) = x(1+incr*8:8+incr*8);
    rbuff_v16(1:8) = x(1+decr*8:8+decr*8);    
    y(1+(i-1)*8:(i-1)*8+4) = reshape(rnd_sym_zero(acc(1,:)/2^shift),1,4);
    y(5+(i-1)*8:(i-1)*8+8) = reshape(rnd_sym_zero(acc(2,:)/2^shift),1,4);    
end

end