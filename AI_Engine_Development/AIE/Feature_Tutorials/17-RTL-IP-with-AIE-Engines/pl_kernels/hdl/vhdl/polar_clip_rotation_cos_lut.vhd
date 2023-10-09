
--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity polar_clip_rotation_cos_lut_rom is 
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


architecture rtl of polar_clip_rotation_cos_lut_rom is 

signal addr0_tmp : std_logic_vector(AWIDTH-1 downto 0); 
type mem_array is array (0 to MEM_SIZE-1) of std_logic_vector (DWIDTH-1 downto 0); 
signal mem : mem_array := (
    0 => "1110110111111011", 1 => "0111111010111010", 2 => "0101101010010010", 
    3 => "0101101001110011", 4 => "0010101110111101", 5 => "0111100001001100", 
    6 => "0111101001111011", 7 => "0010010100110000", 8 => "0000110110111011", 
    9 => "0111111101000011", 10 => "0110111000001100", 11 => "0100000101100000", 
    12 => "0100100000000001", 13 => "0110100111010100", 14 => "0111111111011101", 
    15 => "0000010111100101", 16 => "1111110111100110", 17 => "0111111111111100", 
    18 => "0110010100100000", 19 => "0100111001111000", 20 => "0011101001100000", 
    21 => "0111000111101010", 22 => "0111111000101000", 23 => "0001010110100110", 
    24 => "0001110101111000", 25 => "0111110010010000", 26 => "0111010101010101", 
    27 => "0011001100101010", 28 => "0101010010011110", 29 => "0110000000001010", 
    30 => "0111111110011010", 31 to 32=> "1111010111101110", 33 => "0111111110011010", 
    34 => "0110000000001010", 35 => "0101010010011110", 36 => "0011001100101010", 
    37 => "0111010101010101", 38 => "0111110010010000", 39 => "0001110101111000", 
    40 => "0001010110100110", 41 => "0111111000101000", 42 => "0111000111101010", 
    43 => "0011101001100000", 44 => "0100111001111000", 45 => "0110010100100000", 
    46 => "0111111111111100", 47 => "1111110111100110", 48 => "0000010111100101", 
    49 => "0111111111011101", 50 => "0110100111010100", 51 => "0100100000000001", 
    52 => "0100000101100000", 53 => "0110111000001100", 54 => "0111111101000011", 
    55 => "0000110110111011", 56 => "0010010100110000", 57 => "0111101001111011", 
    58 => "0111100001001100", 59 => "0010101110111101", 60 => "0101101001110011", 
    61 => "0101101010010010", 62 => "0111111010111010", 63 => "1110110111111011" );


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

entity polar_clip_rotation_cos_lut is
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

architecture arch of polar_clip_rotation_cos_lut is
    component polar_clip_rotation_cos_lut_rom is
        port (
            clk : IN STD_LOGIC;
            addr0 : IN STD_LOGIC_VECTOR;
            ce0 : IN STD_LOGIC;
            q0 : OUT STD_LOGIC_VECTOR);
    end component;



begin
    polar_clip_rotation_cos_lut_rom_U :  component polar_clip_rotation_cos_lut_rom
    port map (
        clk => clk,
        addr0 => address0,
        ce0 => ce0,
        q0 => q0);

end architecture;


