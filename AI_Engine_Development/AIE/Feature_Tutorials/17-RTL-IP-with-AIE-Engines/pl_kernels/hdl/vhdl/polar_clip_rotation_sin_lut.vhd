
--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity polar_clip_rotation_sin_lut_rom is 
    generic(
             DWIDTH     : integer := 16; 
             AWIDTH     : integer := 6; 
             MEM_SIZE    : integer := 64
    ); 
    port (
          addr0      : in std_logic_vector(AWIDTH-1 downto 0); 
          ce0       : in std_logic; 
          q0         : out std_logic_vector(DWIDTH-1 downto 0);
          clk       : in std_logic
    ); 
end entity; 


architecture rtl of polar_clip_rotation_sin_lut_rom is 

signal addr0_tmp : std_logic_vector(AWIDTH-1 downto 0); 
type mem_array is array (0 to MEM_SIZE-1) of std_logic_vector (DWIDTH-1 downto 0); 
signal mem : mem_array := (
    0 => "1000000101000110", 1 => "1110110111111011", 2 => "1010010110001101", 
    3 => "0101101010010010", 4 => "1000011110110100", 5 => "0010101110111101", 
    6 => "1101101011010000", 7 => "0111101001111011", 8 => "1000000010111101", 
    9 => "0000110110111011", 10 => "1011111010100000", 11 => "0110111000001100", 
    12 => "1001011000101100", 13 => "0100100000000001", 14 => "1111101000011011", 
    15 => "0111111111011101", 16 => "1000000000000100", 17 => "1111110111100110", 
    18 => "1011000110001000", 19 => "0110010100100000", 20 => "1000111000010110", 
    21 => "0011101001100000", 22 => "1110101001011010", 23 => "0111111000101000", 
    24 => "1000001101110000", 25 => "0001110101111000", 26 => "1100110011010110", 
    27 => "0111010101010101", 28 => "1001111111110110", 29 => "0101010010011110", 
    30 => "0000101000010010", 31 => "0111111110011010", 32 => "1000000001100110", 
    33 => "1111010111101110", 34 => "1010101101100010", 35 => "0110000000001010", 
    36 => "1000101010101011", 37 => "0011001100101010", 38 => "1110001010001000", 
    39 => "0111110010010000", 40 => "1000000111011000", 41 => "0001010110100110", 
    42 => "1100010110100000", 43 => "0111000111101010", 44 => "1001101011100000", 
    45 => "0100111001111000", 46 => "0000001000011010", 47 => "0111111111111100", 
    48 => "1000000000100011", 49 => "0000010111100101", 50 => "1011011111111111", 
    51 => "0110100111010100", 52 => "1001000111110100", 53 => "0100000101100000", 
    54 => "1111001001000101", 55 => "0111111101000011", 56 => "1000010110000101", 
    57 => "0010010100110000", 58 => "1101010001000011", 59 => "0111100001001100", 
    60 => "1010010101101110", 61 => "0101101001110011", 62 => "0001001000000101", 
    63 => "0111111010111010" );


begin 


memory_access_guard_0: process (addr0) 
begin
      addr0_tmp <= addr0;
--synthesis translate_off
      if (CONV_INTEGER(addr0) > mem_size-1) then
           addr0_tmp <= (others => '0');
      else 
           addr0_tmp <= addr0;
      end if;
--synthesis translate_on
end process;

p_rom_access: process (clk)  
begin 
    if (clk'event and clk = '1') then
        if (ce0 = '1') then 
            q0 <= mem(CONV_INTEGER(addr0_tmp)); 
        end if;
    end if;
end process;

end rtl;

Library IEEE;
use IEEE.std_logic_1164.all;

entity polar_clip_rotation_sin_lut is
    generic (
        DataWidth : INTEGER := 16;
        AddressRange : INTEGER := 64;
        AddressWidth : INTEGER := 6);
    port (
        reset : IN STD_LOGIC;
        clk : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR(AddressWidth - 1 DOWNTO 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR(DataWidth - 1 DOWNTO 0));
end entity;

architecture arch of polar_clip_rotation_sin_lut is
    component polar_clip_rotation_sin_lut_rom is
        port (
            clk : IN STD_LOGIC;
            addr0 : IN STD_LOGIC_VECTOR;
            ce0 : IN STD_LOGIC;
            q0 : OUT STD_LOGIC_VECTOR);
    end component;



begin
    polar_clip_rotation_sin_lut_rom_U :  component polar_clip_rotation_sin_lut_rom
    port map (
        clk => clk,
        addr0 => address0,
        ce0 => ce0,
        q0 => q0);

end architecture;


