--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

use work.SFIXED_PKG.all;
use work.CFIXED_PKG.all; -- for SFIXED_VECTOR

entity FIXGEMM is -- LATENCY from A to O is 3*N+1, from B and C to O is 2*N+1 where N is A'length
  generic(FAMILY:INTEGER:=3;         -- use 1 for 7-series, 2 for US/US+ and 3 for Versal
          --DDR:BOOLEAN:=TRUE;
          DDR:BOOLEAN:=FALSE;
          TRIANGULAR_DELAYS:INTEGER:=1; -- set to 0 to remove A,B,C and O triangular delays
          OX:INTEGER:=0;
          OY:INTEGER:=0);
  port(CLK:in STD_LOGIC;
       --CLKH:in STD_LOGIC;
       A:in SFIXED_VECTOR;
       VAI:BOOLEAN;
       B,C:in SFIXED_VECTOR;
       VBCI:BOOLEAN;
       O:out SFIXED_VECTOR;
       VO:out BOOLEAN);
end FIXGEMM;

architecture FAST of FIXGEMM is
  constant N:INTEGER:=A'length;
  signal CE:STD_LOGIC:='0';
  type STD_LOGIC_MATRIX is array(INTEGER range <>) of STD_LOGIC_VECTOR;
  signal CEA1,CEA2:STD_LOGIC_MATRIX(B'range)(A'range):=(others=>(others=>'0'));
  signal BL:SFIXED_MATRIX(B'range(1))(A'low to A'high+1)(B(B'low)'range):=(others=>(others=>(others=>'0')));
  signal CNT:UNSIGNED(LOG2(A'length)-1 downto 0):=(others=>'0');
  function AINPUT(K:INTEGER) return STRING is
  begin
    if K=0 then
      return "DIRECT";
    else
      return "CASCADE";
    end if;
  end;
begin
  assert A'length=B'length report "Ports A and B must have the same length!" severity error;
  assert A'length=C'length report "Ports A and C must have the same length!" severity error;
  assert A'length=O'length report "Ports A and O must have the same length!" severity error;

  process(CLK)
  begin
    if rising_edge(CLK) then
      if VAI then
        CNT<=CNT+1;
      else
        CNT<=(others=>'0');
      end if;
      if VAI and CNT=A'length-2 then
        CE<='1';
      else
        CE<='0';
      end if;
    end if;
  end process;
  
  lk:for K in B'range generate
     begin
       ad:entity work.SDELAY generic map(SIZE=>(K-A'low)*TRIANGULAR_DELAYS,
                                         DDR=>DDR)
                             port map(CLK=>CLK,
                                      --CLKH=>CLKH,
                                      I=>B(B'high-K+B'low),
                                      O=>BL(K)(0));
     end generate;

  CEA1(A'low)(B'low)<='1' when VAI else '0';
  lj:for J in A'range generate
       signal AJD:SFIXED(A(A'low)'range);
       signal CJD:SFIXED(C(C'low)'range);
       signal AC:STD_LOGIC_MATRIX(B'low to B'high+1)(FAMILY/3*4+29 downto 0);
       signal PC:STD_LOGIC_MATRIX(B'low to B'high+1)(FAMILY/3*10+47 downto 0);
    begin
       bd:entity work.SDELAY generic map(SIZE=>(J-B'low)*TRIANGULAR_DELAYS,
                                         DDR=>DDR)
                             port map(CLK=>CLK,
                                      --CLKH=>CLKH,
                                      I=>A(J),
                                      O=>AJD);
       cd:entity work.SDELAY generic map(SIZE=>(J-C'low+1)*TRIANGULAR_DELAYS,
                                         DDR=>DDR)
                             port map(CLK=>CLK,
                                      --CLKH=>CLKH,
                                      I=>C(J),
                                      O=>CJD);
       AC(AC'low)<=(others=>'0');
       PC(PC'low)<=(others=>'0');
       lk:for K in B'range generate
            signal AI:SFIXED(A(A'low)'range);
            signal ZEROD:SFIXED(A(A'low)'range):=TO_SFIXED(0,A(A'low));
            signal OPMODE:STD_LOGIC_VECTOR(8 downto 0);
            signal CI:SFIXED(C(C'low)'range);
            signal P:SFIXED(O(O'low)'range);
            signal CEA1KJ:STD_LOGIC;
          begin
            process(CLK)
            begin
              if rising_edge(CLK) then
                if J=B'low then
                    if K=A'low then
                      CEA2(K)(J)<=CE;
                    else
                      CEA1(K)(J)<=CEA1(K-1)(J) and not CE;
                      CEA2(K)(J)<=CEA2(K-1)(J);
                    end if;
                else
                    CEA1(K)(J)<=CEA1(K)(J-1);
                    CEA2(K)(J)<=CEA2(K)(J-1);
                end if;
                BL(K)(J+1)<=BL(K)(J);
              end if;
            end process;
            
            AI<=AJD when K=0 else (others=>'0');
            OPMODE<=9x"35" when K=0 else 9x"15"; -- FPA=C+A*B when J=0 else PCIN+A*B
            CI<=CJD when K=0 else (others=>'0');
            CEA1KJ<='0' when CEA1(K)(J)='0' else '1'; -- workaround for simulation 'X'es
            ds:entity work.DSP_GW
--                 generic map(X=>OX+J mod (N/2)+(N/2-1-(J mod (N/2))*2)*(J/(N/2)), -- floorplan the DSP array as (N/2)x(2*N) and mirror the upper half
--                             Y=>OY+K+J/(N/2)*N,
                 generic map(X=>OX+J mod 12+(11-(J mod 12)*2)*(J/12 mod 2), -- floorplan the DSP array as (N/2)x(2*N) and mirror the upper half
                             Y=>OY+K+J/12*40,
                             FAMILY=>FAMILY,
                             AREG=>2,                                                   -- Pipeline stages for A (0-2)
                             A_INPUT=>AINPUT(K))                                        -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                 port map(ACIN=>AC(K),                                                  -- 30/34-bit input: A cascade data
                          A=>AI,                                                        -- 30/34-bit input: A data
                          B=>BL(K)(J),                                                  -- 18/24-bit input: B data
                          C=>CI,                                                        -- 48/58-bit input: C data
                          D=>ZEROD,                                                     -- 27-bit input: D data
                          CEA1=>CEA1KJ,                                                 -- 1-bit input: Clock enable for 1st stage AREG
                          CEA2=>CEA2(K)(J),                                             -- 1-bit input: Clock enable for 2nd stage AREG
                          CLK=>CLK,                                                     -- 1-bit input: Clock
                          INMODE=>5x"10",                                               -- 5-bit input: INMODE control
                          OPMODE=>OPMODE,                                               -- 9-bit input: Operation mode - default is P<=C+A*B
                          PCIN=>PC(K),                                                  -- 48/58-bit input: P cascade
                          ACOUT=>AC(K+1),                                               -- 30/34-bit output: A port cascade
                          P=>P,                                                         -- 48/58-bit output: Primary data
                          PCOUT=>PC(K+1));                                              -- 48/58-bit output: Cascade output
            
            od:if K=B'high generate
                 bo:entity work.SDELAY generic map(SIZE=>(A'high-J)*TRIANGULAR_DELAYS,
                                                   DDR=>DDR)
                                       port map(CLK=>CLK,
                                                --CLKH=>CLKH,
                                                I=>P,
                                                O=>O(J));
               end generate;
          end generate;
     end generate;
     
  bd:entity work.BDELAY generic map(SIZE=>A'length+B'length+1,
                                          DDR=>DDR)
                        port map(CLK=>CLK,
                                 --CLKH=>CLKH,
                                 I=>VBCI,
                                 O=>VO);
end FAST;
