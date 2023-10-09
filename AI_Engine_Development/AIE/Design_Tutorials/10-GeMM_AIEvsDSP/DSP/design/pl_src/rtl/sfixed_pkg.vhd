--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

/* Use for Sythesis with Vivado 2020.2 and later */
library IEEE;
--version of fixed_pkg with no saturation, rounding or warning messages
package sfixed_pkg is new IEEE.fixed_generic_pkg
   generic map(fixed_round_style=>IEEE.fixed_float_types.fixed_truncate,
               fixed_overflow_style=>IEEE.fixed_float_types.fixed_wrap,
               fixed_guard_bits=>3,
               no_warning=>TRUE);
/* Use for Synthesis with Vivado 2020.1 and earlier 
-------------------------------------------------------------------------------- 
--
-- Module Description: Unconstrained Size Vectors and Matrices of Complex Arbitrary Precision Fixed Point Numbers
--
-------------------------------------------------------------------------------- 
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;
use ieee.math_complex.all;

package SFIXED_PKG is
--!  constant PI:REAL:=3.1415926535897932;
--  type BOOLEAN_VECTOR is array(NATURAL range <>) of BOOLEAN;
--!  type INTEGER_VECTOR is array(INTEGER range <>) of INTEGER;
--!  type REAL_VECTOR is array(INTEGER range <>) of REAL;
--!  type REAL_MATRIX is array(INTEGER range <>) of REAL_VECTOR;
--!  type COMPLEX_VECTOR is array(INTEGER range <>) of COMPLEX;
--!  type UNSIGNED_VECTOR is array(INTEGER range <>) of UNSIGNED;

  type SFIXED is array(INTEGER range <>) of STD_LOGIC; -- arbitrary precision fixed point signed number, like SIGNED but lower bound can be negative

--  function MIN(A,B:INTEGER) return INTEGER;
--  function MIN(A,B,C:INTEGER) return INTEGER;
--  function MIN(A,B,C,D:INTEGER) return INTEGER;
--  function MAX(A,B:INTEGER) return INTEGER;
--  function MAX(A,B,C:INTEGER) return INTEGER;
--  function MAX(A,B,C,D:INTEGER) return INTEGER;
--  function MED(A,B,C:INTEGER) return INTEGER;
  function "+"(X,Y:SFIXED) return SFIXED; -- full precision add with SFIXED(MAX(X'high,Y'high)+1 downto MIN(X'low,Y'low)) result
  function "-"(X,Y:SFIXED) return SFIXED; -- full precision subtract with SFIXED(MAX(X'high,Y'high)+1 downto MIN(X'low,Y'low)) result
  function "-"(X:SFIXED) return SFIXED; -- full precision negate with SFIXED(X'high+1 downto X'low) result
  function "*"(X,Y:SFIXED) return SFIXED; -- full precision multiply with SFIXED(X'high+Y'high+1 downto X'low+Y'low) result
  function "*"(X:SFIXED;Y:STD_LOGIC) return SFIXED; -- multiply by 0 or 1 with SFIXED(X'high downto X'low) result
  function RESIZE(X:SFIXED;H,L:INTEGER) return SFIXED; -- resizes X and returns SFIXED(H downto L)
  function RESIZE(X:SFIXED;HL:SFIXED) return SFIXED; -- resizes X to match HL and returns SFIXED(HL'high downto HL'low)
  function SHIFT_RIGHT(X:SFIXED;N:INTEGER) return SFIXED; -- returns SFIXED(X'high-N downto X'low-N) result
  function SHIFT_LEFT(X:SFIXED;N:INTEGER) return SFIXED; -- returns SFIXED(X'high+N downto X'low+N) result
  function TO_SFIXED(R:REAL;H,L:INTEGER) return SFIXED; -- returns SFIXED(H downto L) result
  function TO_SFIXED(R:REAL;HL:SFIXED) return SFIXED; -- returns SFIXED(HL'high downto HL'low) result
  function TO_SFIXED(I:SIGNED;H,L:INTEGER;SHIFT:INTEGER:=0) return SFIXED; -- convert SIGNED to SFIXED(H downto L) and resize
  function TO_SFIXED(I:SIGNED;X:SFIXED;SHIFT:INTEGER:=0) return SFIXED; -- convert SIGNED to SFIXED(X'high downto X'low) and resize
  function TO_SIGNED(I:SFIXED;X:SFIXED) return SIGNED; -- resize I to X and convert to SIGNED(I'length-1 downto 0), returns SIGNED(SHIFT_RIGHT(RESIZE(I,X),X'low))
  function TO_REAL(S:SFIXED) return REAL; -- returns REAL result
  function TO_01(X:SFIXED) return SFIXED; -- get rid of Xes
end SFIXED_PKG;

package body SFIXED_PKG is
  function MIN(A,B:INTEGER) return INTEGER is
  begin
    if A<B then
      return A;
    else
      return B;
    end if;
  end;
  
  function MIN(A,B,C:INTEGER) return INTEGER is
  begin
    return MIN(MIN(A,B),C);
  end;
  
  function MIN(A,B,C,D:INTEGER) return INTEGER is
  begin
    return MIN(MIN(A,B),MIN(C,D));
  end;
  
  function MAX(A,B:INTEGER) return INTEGER is
  begin
    if A>B then
      return A;
    else
      return B;
    end if;
  end;
  
  function MAX(A,B,C:INTEGER) return INTEGER is
  begin
    return MAX(MAX(A,B),C);
  end;
  
  function MAX(A,B,C,D:INTEGER) return INTEGER is
  begin
    return MAX(MAX(A,B),MAX(C,D));
  end;
  
  function MED(A,B,C:INTEGER) return INTEGER is
  begin
    return MAX(MIN(MAX(A,B),C),MIN(A,B));
  end;
  
  function "+"(X,Y:SFIXED) return SFIXED is
    variable SX,SY,SR:SIGNED(MAX(X'high,Y'high)+1-MIN(X'low,Y'low) downto 0);
    variable R:SFIXED(MAX(X'high,Y'high)+1 downto MIN(X'low,Y'low));
  begin
    for K in SX'range loop
      if K<X'low-Y'low then
        SX(K):='0';           -- zero pad X LSBs
      elsif K>X'high-R'low then
        SX(K):=X(X'high);     -- sign extend X MSBs
      else
        SX(K):=X(R'low+K);
      end if;
    end loop;
    for K in SY'range loop
      if K<Y'low-X'low then
        SY(K):='0';           -- zero pad Y LSBs
      elsif K>Y'high-R'low then
        SY(K):=Y(Y'high);     -- sign extend Y MSBs
      else
        SY(K):=Y(R'low+K);
      end if;
    end loop;
    SR:=SX+SY; -- SIGNED addition
    for K in SR'range loop
      R(R'low+K):=SR(K);
    end loop;
    return R;
  end;
  
  function "-"(X,Y:SFIXED) return SFIXED is
    variable SX,SY,SR:SIGNED(MAX(X'high,Y'high)+1-MIN(X'low,Y'low) downto 0);
    variable R:SFIXED(MAX(X'high,Y'high)+1 downto MIN(X'low,Y'low));
  begin
    for K in SX'range loop
      if K<X'low-Y'low then
        SX(K):='0';           -- zero pad X LSBs
      elsif K>X'high-R'low then
        SX(K):=X(X'high);     -- sign extend X MSBs
      else
        SX(K):=X(R'low+K);
      end if;
    end loop;
    for K in SY'range loop
      if K<Y'low-X'low then
        SY(K):='0';           -- zero pad Y LSBs
      elsif K>Y'high-R'low then
        SY(K):=Y(Y'high);     -- sign extend Y MSBs
      else
        SY(K):=Y(R'low+K);
      end if;
    end loop;
    SR:=SX-SY; -- SIGNED subtraction
    for K in SR'range loop
      R(R'low+K):=SR(K);
    end loop;
    return R;
  end;
  
  function "-"(X:SFIXED) return SFIXED is
    variable SX:SIGNED(X'high-X'low downto 0);
    variable SR:SIGNED(X'high-X'low+1 downto 0);
    variable R:SFIXED(X'high+1 downto X'low);
  begin
    for K in SX'range loop
      SX(K):=X(X'low+K);
    end loop;
    SR:=-RESIZE(SX,SR'length); -- SIGNED negation
    for K in SR'range loop
      R(R'low+K):=SR(K);
    end loop;
    return R;
  end;
  
  function "*"(X,Y:SFIXED) return SFIXED is
    variable SX:SIGNED(X'high-X'low downto 0);
    variable SY:SIGNED(Y'high-Y'low downto 0);
    variable SR:SIGNED(SX'high+SY'high+1 downto 0);
    variable R:SFIXED(X'high+Y'high+1 downto X'low+Y'low);
  begin
    for K in SX'range loop
      SX(K):=X(X'low+K);
    end loop;
    for K in SY'range loop
      SY(K):=Y(Y'low+K);
    end loop;
    SR:=SX*SY; -- SIGNED multiplication
    for K in SR'range loop
      R(R'low+K):=SR(K);
    end loop;
    return R;
  end;
  
  function "*"(X:SFIXED;Y:STD_LOGIC) return SFIXED is
  begin
    if Y='1' then
      return X;
    else
      return TO_SFIXED(0.0,X);
    end if;
  end;
  
  function RESIZE(X:SFIXED;H,L:INTEGER) return SFIXED is
    variable R:SFIXED(H downto L);
  begin
    for K in R'range loop
      if K<X'low then
        R(K):='0';           -- zero pad X LSBs
      elsif K>X'high then
        R(K):=X(X'high);     -- sign extend X MSBs
      else
        R(K):=X(K);
      end if;
    end loop;
    return R;
  end;
  
  function RESIZE(X:SFIXED;HL:SFIXED) return SFIXED is
  begin
    return RESIZE(X,HL'high,HL'low);
  end;
  
  function SHIFT_RIGHT(X:SFIXED;N:INTEGER) return SFIXED is
    variable R:SFIXED(X'high-N downto X'low-N);
  begin
    for K in R'range loop
      R(K):=X(K+N);
    end loop;
    return R;
  end;
  
  function SHIFT_LEFT(X:SFIXED;N:INTEGER) return SFIXED is
    variable R:SFIXED(X'high+N downto X'low+N);
  begin
    for K in R'range loop
      R(K):=X(K-N);
    end loop;
    return R;
  end;

  function TO_SFIXED(R:REAL;H,L:INTEGER) return SFIXED is
    variable RR:REAL;
    variable V:SFIXED(H downto L);
  begin
    assert (R<2.0**H) and (R>=-2.0**H) report "TO_SFIXED vector truncation!" severity warning;
    if R<0.0 then
      V(V'high):='1';
      RR:=R+2.0**V'high;
    else
      V(V'high):='0';
      RR:=R;
    end if;
    for K in V'high-1 downto V'low loop
      if RR>=2.0**K then
        V(K):='1';
        RR:=RR-2.0**K;
      else
        V(K):='0';
      end if;
    end loop;
    return V;
  end;
  
  function TO_SFIXED(R:REAL;HL:SFIXED) return SFIXED is
  begin
    return TO_SFIXED(R,HL'high,HL'low);
  end;

  function TO_SFIXED(I:SIGNED;H,L:INTEGER;SHIFT:INTEGER:=0) return SFIXED is
    variable O:SFIXED(H downto L);
  begin
    for K in O'range loop
      if K<I'low+L+SHIFT then
        O(K):='0';
      elsif K-L-SHIFT<I'length then
        O(K):=I(K-L-SHIFT);
      else
        O(K):=I(I'high);
      end if;
    end loop;
    return O;
  end;

  function TO_SFIXED(I:SIGNED;X:SFIXED;SHIFT:INTEGER:=0) return SFIXED is
    variable O:SFIXED(X'range);
  begin
    return TO_SFIXED(I,X'high,X'low,SHIFT);
--    for K in O'range loop
--      if K<I'low+X'low+SHIFT then
--        O(K):='0';
--      elsif K-X'low-SHIFT<I'length then
--        O(K):=I(K-X'low-SHIFT);
--      else
--        O(K):=I(I'high);
--      end if;
--    end loop;
--    return O;
  end;

  function TO_SIGNED(I:SFIXED;X:SFIXED) return SIGNED is -- SIGNED(SHIFT_RIGHT(RESIZE(I,X),X'low))
    variable O:SIGNED(X'length-1 downto 0);
  begin
    for K in O'range loop
      if K<I'low-X'low then
        O(K):='0';
      elsif K<I'length then
        O(K):=I(K+X'low);
      else
        O(K):=I(I'high);
      end if;
    end loop;
    return O;
  end;
  
  function TO_REAL(S:SFIXED) return REAL is
    variable R:REAL;
  begin
    R:=0.0;
    for K in S'range loop
      if K=S'high then
        if S(K)='1' then
          R:=R-2.0**K;
        end if;
      else
        if S(K)='1' then
          R:=R+2.0**K;
        end if;
      end if;
    end loop;
    return R;
  end;
  
  function TO_01(X:SFIXED) return SFIXED is -- get rid of Xes
    variable R:SFIXED(X'range);
  begin
    for K in X'range loop
      R(K):=TO_01(X(K));
    end loop;
    return R;
  end;
end SFIXED_PKG;
*/
