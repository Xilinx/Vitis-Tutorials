--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

--package bus_multiplexer_pkg is
--        type bus_array_in is array(31 downto 0) of std_logic_vector(15 downto 0);
--        type bus_array_out is array(0 downto 31) of std_logic_vector(39 downto 0);
--end package;
--library IEEE;
--use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;
--use work.bus_multiplexer_pkg.all;

library STD;
use STD.textio.all;
use STD.env.all;

use work.SFIXED_PKG.all;
use work.CFIXED_PKG.all; -- for REAL_MATRIX


-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
entity FIXGEMM_WRAPPER is
  generic(FAMILY:INTEGER:=3;         -- use 1 for 7-series, 2 for US/US+ and 3 for Versal
          N:INTEGER:=32;
          DDR:BOOLEAN:=FALSE;
          --TRIANGULAR_DELAYS:INTEGER:=1; -- set to 0 to remove A,B,C and O triangular delays
          TRIANGULAR_DELAYS:INTEGER:=1;
          OX:INTEGER:=-32;
          OY:INTEGER:=-32
          --OX:INTEGER:=0;
          --OY:INTEGER:=46-N
	  );
  port(
       CLK :in  STD_LOGIC;
       --CLKH:in  STD_LOGIC;
       SVAI :in  STD_LOGIC;
       SVBCI:in  STD_LOGIC;
       SVO  :out STD_LOGIC;
       SA :in STD_LOGIC_VECTOR(N*16-1 downto 0); 
       SB :in STD_LOGIC_VECTOR(N*16-1 downto 0); 
       SC    :in STD_LOGIC_VECTOR(N*40-1 downto 0); 
       SO    :out STD_LOGIC_VECTOR(N*40-1 downto 0)
       );
end FIXGEMM_WRAPPER;

architecture WRAPPER of FIXGEMM_WRAPPER is
  signal A,B:SFIXED_VECTOR(0 to N-1 )(7 downto -8):=(others=>(others=>'0'));
  signal C:SFIXED_VECTOR  (0 to N-1 )(23 downto -16):=(others=>(others=>'0'));
  signal O:SFIXED_VECTOR  (0 to N-1 )(23 downto -16);
  signal VO: BOOLEAN;
  signal VAI: BOOLEAN;
  signal VBCI: BOOLEAN;
  
 begin
  
  process(CLK)
  begin
    if rising_edge(CLK) then
      if SVAI = '1' then
        VAI <= TRUE;
      else 
        VAI <= FALSE;
      end if;
    end if;
  end process;

  process(CLK)
  begin
    if rising_edge(CLK) then
      if SVBCI = '1' then
        VBCI <= TRUE;
      else 
        VBCI <= FALSE;
      end if;
    end if;
  end process;


  process(CLK)
  begin
    if rising_edge(CLK) then
      for K in 0 to N-1 loop
        for J in 0 to 15 loop
           A(K)(J-8)<= SA(J+16*K);
        end loop;
      end loop;
    end if;
  end process;
  
  process(CLK)
  begin
    if rising_edge(CLK) then
      for K in 0 to N-1 loop
        for J in 0 to 15 loop
           B(K)(J-8)<= SB(J+16*K);
        end loop;
      end loop;
    end if;
  end process;
  
  process(CLK)
  begin
    if rising_edge(CLK) then
      for K in 0 to N-1 loop
        for J in 0 to 15 loop
           C(K)(J-8)<= SC(J+16*K);
        end loop;
      end loop;
    end if;
  end process;
  
      
  u1:entity work.FIXGEMM generic map(FAMILY=>FAMILY,         
                                     DDR=>DDR,
                                     TRIANGULAR_DELAYS=>TRIANGULAR_DELAYS, 
				     --TRIANGULAR_DELAYS=>1, -- set to 0 to remove A,B,C and O triangular delays
                                     OX=>OX,
                                     OY=>OY)
                         port map(CLK=>CLK,
                                  A=>A,
                                  VAI=>VAI,
                                  B=>B,
                                  C=>C,
                                  VBCI=>VBCI,
                                  O=>O,
                                  VO=>VO);

  process (CLK)
  begin
    if rising_edge(CLK) then
      for K in 0 to N-1 loop
        for J in 39 downto 0 loop
           if VO then
             SO(J+40*K)<= O(K)(J-16);
           else
             SO(J+40*K)<= '0';
           end if;
        end loop;
      end loop;
    end if;
  end process;


  process(CLK)
  begin
    if rising_edge(CLK) then
      if VO  then
        SVO <= '1';
      else 
        SVO <= '0';
      end if;
    end if;
  end process;

end WRAPPER;
