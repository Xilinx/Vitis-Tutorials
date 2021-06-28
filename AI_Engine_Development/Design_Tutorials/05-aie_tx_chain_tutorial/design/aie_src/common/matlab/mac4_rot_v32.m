function v8acc = mac4_rot_v32(acc,scd,rot,xbuff,xstart,xoffsets,xstep,zbuff,zstart,zoffsets,zstep)
if (rot == 0)
    acc_tmp = acc;
else
    acc_tmp = [acc(rot+1:8) scd(1:rot)];
end
[indx_ln_zbuff,indx_ln_xbuff] = get_indcs(zstart,zoffsets,zstep,xstart,xoffsets,xstep);

acc_tmp(5) = acc_tmp(5) + ...
             xbuff(indx_ln_xbuff(1,1)) * zbuff(indx_ln_zbuff(1,1)) + ...
             xbuff(indx_ln_xbuff(1,2)) * zbuff(indx_ln_zbuff(1,2));
acc_tmp(6) = acc_tmp(6) + ...
             xbuff(indx_ln_xbuff(2,1)) * zbuff(indx_ln_zbuff(2,1)) + ...
             xbuff(indx_ln_xbuff(2,2)) * zbuff(indx_ln_zbuff(2,2));
acc_tmp(7) = acc_tmp(7) + ...
             xbuff(indx_ln_xbuff(3,1)) * zbuff(indx_ln_zbuff(3,1)) + ...
             xbuff(indx_ln_xbuff(3,2)) * zbuff(indx_ln_zbuff(3,2));
acc_tmp(8) = acc_tmp(8) + ...
             xbuff(indx_ln_xbuff(4,1)) * zbuff(indx_ln_zbuff(4,1)) + ...
             xbuff(indx_ln_xbuff(4,2)) * zbuff(indx_ln_zbuff(4,2));

v8acc = acc_tmp;

end

function [z,x] = get_indcs(zstart,zoffsets,zstep,xstart,xoffsets,xstep)
    z = get_indc_circ(zstart,zoffsets,zstep);
    x = get_indc_circ(xstart,xoffsets,xstep);
end

function indx = get_indc_circ(start,offsets,step)
    for i=1:length(offsets)
        for j=1:2
            indx(i,j) = start+1+offsets(i)+step*(j-1);
            if (indx(i,j) > 32)
                indx(i,j) = indx(i,j)-32;
            elseif (indx(i,j) < 1)    
                indx(i,j) = 32+indx(i,j);
            end
        end
    end
end