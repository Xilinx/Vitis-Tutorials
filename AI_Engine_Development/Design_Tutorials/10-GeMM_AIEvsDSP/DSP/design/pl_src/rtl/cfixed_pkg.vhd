--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;
use IEEE.MATH_REAL.all;
use IEEE.MATH_COMPLEX.all;

package cfixed_pkg is 
  use work.sfixed_pkg.all; -- VHDL-2008 fixed_pkg with no saturation, rounding or warning messages
                           -- this gives us UFIXED, SFIXED, operators and conversion functions
 
  constant PI:REAL:=3.1415926535897932;
--  type BOOLEAN_VECTOR is array(NATURAL range <>) of BOOLEAN;
  type INTEGER_VECTOR is array(INTEGER range <>) of INTEGER;
  type REAL_VECTOR is array(INTEGER range <>) of REAL;
  type REAL_MATRIX is array(INTEGER range <>) of REAL_VECTOR;
  type COMPLEX_VECTOR is array(INTEGER range <>) of COMPLEX;
  type UNSIGNED_VECTOR is array(INTEGER range <>) of UNSIGNED;

-- SFIXED is now the standard VHDL-2008 type defined in fixed_generic_pkg, we do not need a user defined type anymore but this is what SFIXED really is
--  type SFIXED is array(INTEGER range <>) of STD_LOGIC; -- arbitrary precision fixed point signed number, like SIGNED but lower bound can be negative
  type SFIXED_VECTOR is array(INTEGER range <>) of SFIXED; -- unconstrained array of SFIXED
  type SFIXED_MATRIX is array(INTEGER range <>) of SFIXED_VECTOR;
  type CFIXED is record RE,IM:SFIXED; end record; -- arbitrary precision fixed point complex signed number
  type CFIXED_VECTOR is array(INTEGER range <>) of CFIXED; -- unconstrained array of CFIXED
  type CFIXED_MATRIX is array(INTEGER range <>) of CFIXED_VECTOR; -- unconstrained array of CFIXED_VECTOR
  type COMPLEX_MATRIX is array(INTEGER range <>) of COMPLEX_VECTOR;

  function MIN(A,B:INTEGER) return INTEGER;
  function MIN(A,B,C:INTEGER) return INTEGER;
  function MIN(A,B,C,D:INTEGER) return INTEGER;
  function MAX(A,B:INTEGER) return INTEGER;
  function MAX(A,B,C:INTEGER) return INTEGER;
  function MAX(A,B,C,D:INTEGER) return INTEGER;
  function MED(A,B,C:INTEGER) return INTEGER;

  function "*"(X:SFIXED;Y:STD_LOGIC) return SFIXED; -- multiply by 0 or 1 with SFIXED(X'high downto X'low) result
  function TO_SFIXED(I:SIGNED;H,L:INTEGER;SHIFT:INTEGER:=0) return SFIXED; -- convert SIGNED to SFIXED(H downto L) and resize
  function TO_SFIXED(I:SIGNED;X:SFIXED;SHIFT:INTEGER:=0) return SFIXED; -- convert SIGNED to SFIXED(X'high downto X'low) and resize
--redefine standard VHDL-2008 SFIXED shift function to make them full precision
  function SHIFT_RIGHT(X:SFIXED;N:INTEGER) return SFIXED; -- returns SFIXED(X'high-N downto X'low-N) result
  function SHIFT_LEFT(X:SFIXED;N:INTEGER) return SFIXED; -- returns SFIXED(X'high+N downto X'low+N) result
  function TO_SFIXED_VECTOR(C:REAL_VECTOR;HL:SFIXED) return SFIXED_VECTOR; -- returns SFIXED_VECTOR(HL'high downto HL'low) result
  function TO_REAL_VECTOR(C:SFIXED_VECTOR) return REAL_VECTOR; -- returns REAL_VECTOR result

  function RE(X:CFIXED) return SFIXED; -- returns SFIXED(X'high/2 downto X'low/2) result
  function IM(X:CFIXED) return SFIXED; -- returns SFIXED(X'high/2 downto X'low/2) result
  function "+"(X,Y:CFIXED) return CFIXED; -- full precision add with CFIXED(MAX(X'high,Y'high)+2 downto MIN(X'low,Y'low)) result
  function "-"(X,Y:CFIXED) return CFIXED; -- full precision subtract with CFIXED(MAX(X'high,Y'high)+2 downto MIN(X'low,Y'low)) result
  function "*"(X,Y:CFIXED) return CFIXED; -- full precision multiply with CFIXED(X'high+Y'high+2 downto X'low+Y'low) result
  function "*"(X:CFIXED;Y:SFIXED) return CFIXED; -- full precision multiply with CFIXED(X'high+Y'high downto X'low+Y'low) result
  function "*"(X:SFIXED;Y:CFIXED) return CFIXED;
  function RESIZE(X:CFIXED;H,L:INTEGER) return CFIXED; -- resizes X and returns CFIXED(RE(H downto L),IM(H downto L))
  function RESIZE(X:CFIXED;HL:CFIXED) return CFIXED; -- resizes X to match HL and returns CFIXED(RE(HL.RE'high downto HL.RE'low),IM(RE(HL.IM'high downto HL.IM'low))
  function PLUS_i_TIMES(X:CFIXED) return CFIXED; -- returns CFIXED(X'high+2 downto X'low) result
  function "-"(X:CFIXED) return CFIXED; -- full precision negate with CFIXED(X'high+2 downto X'low) result
  function MINUS_i_TIMES(X:CFIXED) return CFIXED; -- returns CFIXED(X'high+2 downto X'low) result
  function X_PLUS_i_TIMES_Y(X,Y:CFIXED;RND:CFIXED) return CFIXED; -- returns CFIXED(MAX(X'high,Y'high)+2 downto MIN(X'low,Y'low)) result
  function X_MINUS_i_TIMES_Y(X,Y:CFIXED;RND:CFIXED) return CFIXED; -- returns CFIXED(MAX(X'high,Y'high)+2 downto MIN(X'low,Y'low)) result
  function SWAP(X:CFIXED) return CFIXED; -- returns CFIXED(X'high downto X'low) result
  function CONJ(X:CFIXED) return CFIXED; -- returns CFIXED(X'high+2 downto X'low) result
  function SHIFT_RIGHT(X:CFIXED;N:INTEGER) return CFIXED; -- returns CFIXED(X'high-N downto X'low-N) result
  function SHIFT_LEFT(X:CFIXED;N:INTEGER) return CFIXED; -- returns CFIXED(X'high+N downto X'low+N) result
  function TO_CFIXED(R,I:REAL;H,L:INTEGER) return CFIXED; -- returns CFIXED(RE(H downto L),IM(H downto L)) result
  function TO_CFIXED(R,I:REAL;HL:CFIXED) return CFIXED; -- returns CFIXED(RE(HL.RE'high downto HL.RE'low),IM(RE(HL.IM'high downto HL.IM'low)) result
  function TO_CFIXED(C:COMPLEX;HL:CFIXED) return CFIXED; -- returns CFIXED(RE(HL.RE'high downto HL.RE'low),IM(RE(HL.IM'high downto HL.IM'low)) result
  function TO_CFIXED(R,I:SFIXED) return CFIXED; -- returns CFIXED(2*MAX(R'high,I'high)+1 downto 2*MIN(R'low,I'low)) result
  function TO_COMPLEX(C:CFIXED) return COMPLEX; -- returns COMPLEX result
  function TO_CFIXED_VECTOR(C:COMPLEX_VECTOR;HL:CFIXED) return CFIXED_VECTOR; -- returns CFIXED_VECTOR(RE(HL.RE'high downto HL.RE'low),IM(RE(HL.IM'high downto HL.IM'low)) result
  function TO_COMPLEX_VECTOR(C:CFIXED_VECTOR) return COMPLEX_VECTOR; -- returns COMPLEX_VECTOR result
  function "*"(R:REAL;C:COMPLEX_VECTOR) return COMPLEX_VECTOR; -- returns R*C
  function LOG2(N:INTEGER) return INTEGER; -- returns ceil(log2(N))
  function ZERO(U:UNSIGNED) return UNSIGNED; -- returns 0 with the same range as U
  function ZERO(S:SFIXED) return SFIXED; -- returns 0 with the same range as S
  function ZERO(C:CFIXED) return CFIXED; -- returns 0 with the same range as C
  function ZERO(SV:SFIXED_VECTOR) return SFIXED_VECTOR; -- returns 0 with the same range as SV
  function ZERO(CV:CFIXED_VECTOR) return CFIXED_VECTOR; -- returns 0 with the same range as CV
end package cfixed_pkg;

package body cfixed_pkg is
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
  
  function "*"(X:SFIXED;Y:STD_LOGIC) return SFIXED is
  begin
    if Y='1' then
      return X;
    else
      return TO_SFIXED(0.0,X);
    end if;
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
  end;

  function SHIFT_RIGHT(X:SFIXED;N:INTEGER) return SFIXED is
    variable R:SFIXED(X'high-N downto X'low-N);
  begin
    for K in R'range loop
      R(K):=X(K+N);
    end loop;
    return R;
  end;
--redefine standard VHDL-2008 SFIXED shift function to make them full precision
  function SHIFT_LEFT(X:SFIXED;N:INTEGER) return SFIXED is
    variable R:SFIXED(X'high+N downto X'low+N);
  begin
    for K in R'range loop
      R(K):=X(K-N);
    end loop;
    return R;
  end;

  function TO_SFIXED_VECTOR(C:REAL_VECTOR;HL:SFIXED) return SFIXED_VECTOR is
    variable R:SFIXED_VECTOR(C'range)(HL'high downto HL'low);
  begin
    for K in C'range loop
      R(K):=TO_SFIXED(C(K),HL);
    end loop;
    return R;
  end;

  function TO_REAL_VECTOR(C:SFIXED_VECTOR) return REAL_VECTOR is
    variable R:REAL_VECTOR(C'range);
  begin
    for K in C'range loop
      R(K):=TO_REAL(C(K));
    end loop;
    return R;
  end;


  function RE(X:CFIXED) return SFIXED is
  begin
    return X.RE;
  end;

  function IM(X:CFIXED) return SFIXED is
  begin
    return X.IM;
  end;

  function "+"(X,Y:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE+Y.RE,X.IM+Y.IM);
  end;
  
  function "-"(X,Y:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE-Y.RE,X.IM-Y.IM);
  end;
  
  function "*"(X,Y:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE*Y.RE-X.IM*Y.IM,X.RE*Y.IM+X.IM*Y.RE);
  end;

  function "*"(X:CFIXED;Y:SFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE*Y,X.IM*Y);
  end;

  function "*"(X:SFIXED;Y:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X*Y.RE,X*Y.IM);
  end;

  function RESIZE(X:CFIXED;H,L:INTEGER) return CFIXED is
  begin
    return TO_CFIXED(RESIZE(X.RE,H,L),RESIZE(X.IM,H,L));
  end;
  
  function RESIZE(X:CFIXED;HL:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(RESIZE(X.RE,HL.RE),RESIZE(X.IM,HL.IM));
  end;
  
  function PLUS_i_TIMES(X:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(-X.IM,X.RE);
  end;
  
  function "-"(X:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(-X.RE,-X.IM);
  end;
  
  function MINUS_i_TIMES(X:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.IM,-X.RE);
  end;
  
  function X_PLUS_i_TIMES_Y(X,Y:CFIXED;RND:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE-Y.IM+RND.RE,X.IM+Y.RE+RND.IM);
  end;
  
  function X_MINUS_i_TIMES_Y(X,Y:CFIXED;RND:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE+Y.IM+RND.RE,X.IM-Y.RE+RND.IM);
  end;
  
  function SWAP(X:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.IM,X.RE);
  end;
  
  function CONJ(X:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(X.RE,-X.IM);
  end;
  
  function SHIFT_RIGHT(X:CFIXED;N:INTEGER) return CFIXED is
  begin
    return TO_CFIXED(SHIFT_RIGHT(RE(X),N),SHIFT_RIGHT(IM(X),N));
  end;
  
  function SHIFT_LEFT(X:CFIXED;N:INTEGER) return CFIXED is
  begin
    return TO_CFIXED(SHIFT_LEFT(RE(X),N),SHIFT_LEFT(IM(X),N));
  end;
  
  function TO_CFIXED(R,I:REAL;H,L:INTEGER) return CFIXED is
  begin
    return TO_CFIXED(TO_SFIXED(R,H,L),TO_SFIXED(I,H,L));
  end;
  
  function TO_CFIXED(R,I:REAL;HL:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(TO_SFIXED(R,HL.RE'high,HL.RE'low),TO_SFIXED(I,HL.IM'high,HL.IM'low));
  end;
  
  function TO_CFIXED(C:COMPLEX;HL:CFIXED) return CFIXED is
  begin
    return TO_CFIXED(C.RE,C.IM,HL);
  end;
  
  function TO_CFIXED(R,I:SFIXED) return CFIXED is
    constant H:INTEGER:=MAX(R'high,I'high);
    constant L:INTEGER:=MIN(R'low,I'low);
    variable C:CFIXED(RE(H downto L),IM(H downto L));
  begin
    C.RE:=RESIZE(R,H,L);
    C.IM:=RESIZE(I,H,L);
    return C;
  end;

  function TO_COMPLEX(C:CFIXED) return COMPLEX is
    variable R:COMPLEX;
  begin
    R.RE:=TO_REAL(C.RE);
    R.IM:=TO_REAL(C.IM);
    return R;
  end;
  
  function TO_CFIXED_VECTOR(C:COMPLEX_VECTOR;HL:CFIXED) return CFIXED_VECTOR is
    variable R:CFIXED_VECTOR(C'range)(RE(HL.RE'high downto HL.RE'low),IM(HL.IM'high downto HL.IM'low));
  begin
    for K in C'range loop
      R(K):=TO_CFIXED(C(K),HL);
    end loop;
    return R;
  end;

  function TO_COMPLEX_VECTOR(C:CFIXED_VECTOR) return COMPLEX_VECTOR is
    variable R:COMPLEX_VECTOR(C'range);
  begin
    for K in C'range loop
      R(K):=TO_COMPLEX(C(K));
    end loop;
    return R;
  end;

  function "*"(R:REAL;C:COMPLEX_VECTOR) return COMPLEX_VECTOR is
    variable X:COMPLEX_VECTOR(C'range);
  begin
    for K in C'range loop
      X(K):=R*C(K);
    end loop;
    return X;
  end;

  function LOG2(N:INTEGER) return INTEGER is
    variable TEMP:INTEGER;
    variable RESULT:INTEGER;
  begin
    TEMP:=N;
    RESULT:=0;
    while TEMP>1 loop
      RESULT:=RESULT+1;
      TEMP:=(TEMP+1)/2;
    end loop;  
    return RESULT; 
  end; 

  function ZERO(U:UNSIGNED) return UNSIGNED is -- returns 0 with the same range as S
  begin
    return TO_UNSIGNED(0,U'length); 
  end; 

  function ZERO(S:SFIXED) return SFIXED is -- returns 0 with the same range as S
  begin
    return TO_SFIXED(0.0,S); 
  end; 

  function ZERO(C:CFIXED) return CFIXED is -- returns 0 with the same range as C
  begin
    return TO_CFIXED(0.0,0.0,C); 
  end; 

  function ZERO(SV:SFIXED_VECTOR) return SFIXED_VECTOR is -- returns 0 with the same range as SV
    variable RESULT:SFIXED_VECTOR(SV'range)(SV(SV'low)'range);
  begin
    for K in SV'range loop
      RESULT(K):=TO_SFIXED(0.0,SV(K)); 
    end loop;
    return RESULT; 
  end; 

  function ZERO(CV:CFIXED_VECTOR) return CFIXED_VECTOR is -- returns 0 with the same range as CV
    variable RESULT:CFIXED_VECTOR(CV'range)(RE(CV(CV'low).RE'range),IM(CV(CV'low).IM'range));
  begin
    for K in CV'range loop
      RESULT(K):=TO_CFIXED(0.0,0.0,CV(K)); 
    end loop;
    return RESULT; 
  end; 
end package body cfixed_pkg;
