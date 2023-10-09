
--Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
--SPDX-License-Identifier: X11

-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
-- ==============================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity polar_clip_regslice_both is
GENERIC (DataWidth : INTEGER := 32);
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;

    data_in : IN STD_LOGIC_VECTOR (DataWidth-1 downto 0);
    vld_in : IN STD_LOGIC;
    ack_in : OUT STD_LOGIC;
    data_out : OUT STD_LOGIC_VECTOR (DataWidth-1 downto 0);
    vld_out : OUT STD_LOGIC;
    ack_out : IN STD_LOGIC;
    apdone_blk : OUT STD_LOGIC );
end;

architecture behav of polar_clip_regslice_both is
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_const_lv2_0 : STD_LOGIC_VECTOR (1 downto 0) := "00";
    constant ap_const_lv2_1 : STD_LOGIC_VECTOR (1 downto 0) := "01";
    constant ap_const_lv2_2 : STD_LOGIC_VECTOR (1 downto 0) := "10";
    constant ap_const_lv2_3 : STD_LOGIC_VECTOR (1 downto 0) := "11";

    signal B_V_data_1_ack_in : STD_LOGIC;
    signal B_V_data_1_ack_out : STD_LOGIC;
    signal B_V_data_1_state : STD_LOGIC_VECTOR (1 downto 0) := "00";
    signal B_V_data_1_data_in : STD_LOGIC_VECTOR (DataWidth-1 downto 0);
    signal B_V_data_1_data_out : STD_LOGIC_VECTOR (DataWidth-1 downto 0);
    signal B_V_data_1_vld_in : STD_LOGIC;
    signal B_V_data_1_vld_out : STD_LOGIC;
    signal B_V_data_1_payload_A : STD_LOGIC_VECTOR (DataWidth-1 downto 0);
    signal B_V_data_1_payload_B : STD_LOGIC_VECTOR (DataWidth-1 downto 0);
    signal B_V_data_1_sel_rd : STD_LOGIC := '0';
    signal B_V_data_1_sel_wr : STD_LOGIC := '0';
    signal B_V_data_1_sel : STD_LOGIC;
    signal B_V_data_1_load_A : STD_LOGIC;
    signal B_V_data_1_load_B : STD_LOGIC;
    signal B_V_data_1_state_cmp_full : STD_LOGIC;

begin

    B_V_data_1_sel_rd_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                B_V_data_1_sel_rd <= ap_const_logic_0;
            else
                if (((ap_const_logic_1 = B_V_data_1_vld_out) and (ap_const_logic_1 = B_V_data_1_ack_out))) then 
                    B_V_data_1_sel_rd <= not(B_V_data_1_sel_rd);
                end if; 
            end if;
        end if;
    end process;

    B_V_data_1_sel_wr_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                B_V_data_1_sel_wr <= ap_const_logic_0;
            else
                if (((ap_const_logic_1 = B_V_data_1_vld_in) and (ap_const_logic_1 = B_V_data_1_ack_in))) then 
                    B_V_data_1_sel_wr <= not(B_V_data_1_sel_wr);
                end if; 
            end if;
        end if;
    end process;

    B_V_data_1_state_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                B_V_data_1_state <= ap_const_lv2_0;
            else
                if ((((ap_const_lv2_3 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_vld_in) and (ap_const_logic_1 = B_V_data_1_ack_out)) or ((ap_const_lv2_2 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_vld_in)))) then 
                    B_V_data_1_state <= ap_const_lv2_2;
                elsif ((((ap_const_lv2_1 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_ack_out)) or ((ap_const_lv2_3 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_ack_out) and (ap_const_logic_1 = B_V_data_1_vld_in)))) then 
                    B_V_data_1_state <= ap_const_lv2_1;
                elsif ((((ap_const_lv2_1 = B_V_data_1_state) and (ap_const_logic_1 = B_V_data_1_ack_out)) or (not(((ap_const_logic_0 = B_V_data_1_ack_out) and (ap_const_logic_1 = B_V_data_1_vld_in))) and not(((ap_const_logic_0 = B_V_data_1_vld_in) and (ap_const_logic_1 = B_V_data_1_ack_out))) and (ap_const_lv2_3 = B_V_data_1_state)) or ((ap_const_lv2_2 = B_V_data_1_state) and (ap_const_logic_1 = B_V_data_1_vld_in)))) then 
                    B_V_data_1_state <= ap_const_lv2_3;
                else 
                    B_V_data_1_state <= ap_const_lv2_2;
                end if; 
            end if;
        end if;
    end process;


    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = B_V_data_1_load_A)) then
                B_V_data_1_payload_A <= B_V_data_1_data_in;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = B_V_data_1_load_B)) then
                B_V_data_1_payload_B <= B_V_data_1_data_in;
            end if;
        end if;
    end process;

    B_V_data_1_data_out_assign_proc : process(B_V_data_1_payload_A, B_V_data_1_payload_B, B_V_data_1_sel)
    begin
        if ((ap_const_logic_1 = B_V_data_1_sel)) then 
            B_V_data_1_data_out <= B_V_data_1_payload_B;
        else 
            B_V_data_1_data_out <= B_V_data_1_payload_A;
        end if; 
    end process;

    B_V_data_1_ack_in <= B_V_data_1_state(1);

    B_V_data_1_load_A <= (not(B_V_data_1_sel_wr) and B_V_data_1_state_cmp_full);
    B_V_data_1_load_B <= (B_V_data_1_state_cmp_full and B_V_data_1_sel_wr);
    B_V_data_1_sel <= B_V_data_1_sel_rd;
    B_V_data_1_state_cmp_full <= '0' when (B_V_data_1_state = ap_const_lv2_1) else '1';

    B_V_data_1_vld_out <= B_V_data_1_state(0);

    B_V_data_1_vld_in <= vld_in;
    B_V_data_1_data_in <= data_in;
    ack_in <= B_V_data_1_ack_in;
    
    vld_out <= B_V_data_1_vld_out;
    data_out <= B_V_data_1_data_out;
    B_V_data_1_ack_out <= ack_out;

    apdone_blk_assign_proc : process(B_V_data_1_state, ack_out)
    begin
        if(((B_V_data_1_state = ap_const_lv2_3) and (ack_out = ap_const_logic_0)) or (B_V_data_1_state = ap_const_lv2_1)) then
            apdone_blk <= ap_const_logic_1;
        else
            apdone_blk <= ap_const_logic_0;   
        end if; 
    end process;

end behav;

    
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity polar_clip_regslice_both_w1 is
GENERIC (DataWidth : INTEGER := 1);
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;

    data_in : IN STD_LOGIC;
    vld_in : IN STD_LOGIC;
    ack_in : OUT STD_LOGIC;
    data_out : OUT STD_LOGIC;
    vld_out : OUT STD_LOGIC;
    ack_out : IN STD_LOGIC;
    apdone_blk : OUT STD_LOGIC );
end;
architecture behav of polar_clip_regslice_both_w1 is
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_const_lv2_0 : STD_LOGIC_VECTOR (1 downto 0) := "00";
    constant ap_const_lv2_1 : STD_LOGIC_VECTOR (1 downto 0) := "01";
    constant ap_const_lv2_2 : STD_LOGIC_VECTOR (1 downto 0) := "10";
    constant ap_const_lv2_3 : STD_LOGIC_VECTOR (1 downto 0) := "11";

    signal B_V_data_1_ack_in : STD_LOGIC;
    signal B_V_data_1_ack_out : STD_LOGIC;
    signal B_V_data_1_state : STD_LOGIC_VECTOR (1 downto 0) := "00";
    signal B_V_data_1_data_in : STD_LOGIC;
    signal B_V_data_1_data_out : STD_LOGIC;
    signal B_V_data_1_vld_in : STD_LOGIC;
    signal B_V_data_1_vld_out : STD_LOGIC;
    signal B_V_data_1_payload_A : STD_LOGIC;
    signal B_V_data_1_payload_B : STD_LOGIC;
    signal B_V_data_1_sel_rd : STD_LOGIC := '0';
    signal B_V_data_1_sel_wr : STD_LOGIC := '0';
    signal B_V_data_1_sel : STD_LOGIC;
    signal B_V_data_1_load_A : STD_LOGIC;
    signal B_V_data_1_load_B : STD_LOGIC;
    signal B_V_data_1_state_cmp_full : STD_LOGIC;

begin

    B_V_data_1_sel_rd_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                B_V_data_1_sel_rd <= ap_const_logic_0;
            else
                if (((ap_const_logic_1 = B_V_data_1_vld_out) and (ap_const_logic_1 = B_V_data_1_ack_out))) then 
                    B_V_data_1_sel_rd <= not(B_V_data_1_sel_rd);
                end if; 
            end if;
        end if;
    end process;

    B_V_data_1_sel_wr_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                B_V_data_1_sel_wr <= ap_const_logic_0;
            else
                if (((ap_const_logic_1 = B_V_data_1_vld_in) and (ap_const_logic_1 = B_V_data_1_ack_in))) then 
                    B_V_data_1_sel_wr <= not(B_V_data_1_sel_wr);
                end if; 
            end if;
        end if;
    end process;

    B_V_data_1_state_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                B_V_data_1_state <= ap_const_lv2_0;
            else
                if ((((ap_const_lv2_3 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_vld_in) and (ap_const_logic_1 = B_V_data_1_ack_out)) or ((ap_const_lv2_2 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_vld_in)))) then 
                    B_V_data_1_state <= ap_const_lv2_2;
                elsif ((((ap_const_lv2_1 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_ack_out)) or ((ap_const_lv2_3 = B_V_data_1_state) and (ap_const_logic_0 = B_V_data_1_ack_out) and (ap_const_logic_1 = B_V_data_1_vld_in)))) then 
                    B_V_data_1_state <= ap_const_lv2_1;
                elsif ((((ap_const_lv2_1 = B_V_data_1_state) and (ap_const_logic_1 = B_V_data_1_ack_out)) or (not(((ap_const_logic_0 = B_V_data_1_ack_out) and (ap_const_logic_1 = B_V_data_1_vld_in))) and not(((ap_const_logic_0 = B_V_data_1_vld_in) and (ap_const_logic_1 = B_V_data_1_ack_out))) and (ap_const_lv2_3 = B_V_data_1_state)) or ((ap_const_lv2_2 = B_V_data_1_state) and (ap_const_logic_1 = B_V_data_1_vld_in)))) then 
                    B_V_data_1_state <= ap_const_lv2_3;
                else 
                    B_V_data_1_state <= ap_const_lv2_2;
                end if; 
            end if;
        end if;
    end process;

    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = B_V_data_1_load_A)) then
                B_V_data_1_payload_A <= B_V_data_1_data_in;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = B_V_data_1_load_B)) then
                B_V_data_1_payload_B <= B_V_data_1_data_in;
            end if;
        end if;
    end process;

    B_V_data_1_data_out_assign_proc : process(B_V_data_1_payload_A, B_V_data_1_payload_B, B_V_data_1_sel)
    begin
        if ((ap_const_logic_1 = B_V_data_1_sel)) then 
            B_V_data_1_data_out <= B_V_data_1_payload_B;
        else 
            B_V_data_1_data_out <= B_V_data_1_payload_A;
        end if; 
    end process;

    B_V_data_1_ack_in <= B_V_data_1_state(1);

    B_V_data_1_load_A <= (not(B_V_data_1_sel_wr) and B_V_data_1_state_cmp_full);
    B_V_data_1_load_B <= (B_V_data_1_state_cmp_full and B_V_data_1_sel_wr);
    B_V_data_1_sel <= B_V_data_1_sel_rd;
    B_V_data_1_state_cmp_full <= '0' when (B_V_data_1_state = ap_const_lv2_1) else '1';

    B_V_data_1_vld_out <= B_V_data_1_state(0);

    B_V_data_1_vld_in <= vld_in;
    B_V_data_1_data_in <= data_in;
    ack_in <= B_V_data_1_ack_in;
    
    vld_out <= B_V_data_1_vld_out;
    data_out <= B_V_data_1_data_out;
    B_V_data_1_ack_out <= ack_out;

    apdone_blk_assign_proc : process(B_V_data_1_state, ack_out)
    begin
        if(((B_V_data_1_state = ap_const_lv2_3) and (ack_out = ap_const_logic_0)) or (B_V_data_1_state = ap_const_lv2_1)) then
            apdone_blk <= ap_const_logic_1;
        else
            apdone_blk <= ap_const_logic_0;   
        end if; 
    end process;

end behav;


