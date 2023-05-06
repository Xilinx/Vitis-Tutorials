% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

%function [xn, yn, zn] = db_cordic (mode, x0, y0, z0, N)
function [zn] = db_cordic (mode, x0, y0, z0, N)

  %coord_t x, y, xp, yp;
  %phase_t z, zp;
  %bool xneg, yneg, dneg;
  %uint6_t i;

  %static const phase_t atan_2Mi[] = { #include "cordic_atan.h" };

  I = [0 : N];

  atan_2Mi = [atan(2.^(-I))];

  quadrant = 0;
  if ((x0>0) & (y0>0))
      quadrant = 1;
  elseif ((x0<0) & (y0>0))
      quadrant = 2;
  elseif ((x0<0) & (y0<0))
      quadrant = 3;
  elseif ((x0>0) & (y0<0))
      quadrant = 4;
  end

  x=abs(x0);
  y=abs(y0);
  z=z0;

  for i=0 : N

    if (mode==1)
        dneg = (z<0);
    else
        dneg = (y>0);
    end

    if (dneg)
      xp = x + ( y/ 2^i);
      yp = y - ( x / 2^i);
      zp = z + atan_2Mi(i+1);
    else
      xp = x - ( y / 2^i);
      yp = y + ( x / 2^i);
      zp = z - atan_2Mi(i+1);
    end
    x = xp;
    y = yp;
    z = zp;
  end

  if (quadrant==1)
      xn=x;
      yn=y;
      zn=z;
  elseif (quadrant==2)
      xn=x;
      yn=-y;
      zn=pi-z;
  elseif (quadrant==3)
      xn=-x;
      yn=-y;
      zn=-(pi-z);
  elseif (quadrant==4)
      xn=-x;
      yn=-y;
      zn=-z;
  end


end
