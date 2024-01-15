--******************************************************************************
-- Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
-- Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
-- SPDX-License-Identifier: MIT
--******************************************************************************

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity stream_out is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst_n : IN STD_LOGIC;
    out_r_TDATA : OUT STD_LOGIC_VECTOR (31 downto 0);
    out_r_TVALID : OUT STD_LOGIC;
    out_r_TREADY : IN STD_LOGIC;
    out_r_TKEEP : OUT STD_LOGIC_VECTOR (3 downto 0);
    out_r_TSTRB : OUT STD_LOGIC_VECTOR (3 downto 0);
    out_r_TLAST : OUT STD_LOGIC_VECTOR (0 downto 0) );
end;


architecture behav of stream_out is 
    attribute CORE_GENERATION_INFO : STRING;
    attribute CORE_GENERATION_INFO of behav : architecture is
    "stream_out_stream_out,hls_ip_2023_2,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xcvc1902-vsva2197-2MP-e-S,HLS_INPUT_CLOCK=3.200000,HLS_INPUT_ARCH=others,HLS_SYN_CLOCK=1.361000,HLS_SYN_LAT=-1,HLS_SYN_TPT=none,HLS_SYN_MEM=0,HLS_SYN_DSP=0,HLS_SYN_FF=39,HLS_SYN_LUT=76,HLS_VERSION=2023_2}";
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_fsm_state1 : STD_LOGIC_VECTOR (1 downto 0) := "01";
    constant ap_ST_fsm_pp0_stage0 : STD_LOGIC_VECTOR (1 downto 0) := "10";
    constant ap_const_boolean_1 : BOOLEAN := true;
    constant ap_const_lv32_1 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000001";
    constant ap_const_boolean_0 : BOOLEAN := false;
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant ap_const_lv4_F : STD_LOGIC_VECTOR (3 downto 0) := "1111";
    constant ap_const_lv1_0 : STD_LOGIC_VECTOR (0 downto 0) := "0";

attribute shreg_extract : string;
    signal ap_rst_reg_2 : STD_LOGIC := '1';
attribute shreg_extract of ap_rst_reg_2 : signal is "no";
    signal ap_rst_reg_1 : STD_LOGIC := '1';
attribute shreg_extract of ap_rst_reg_1 : signal is "no";
    signal ap_rst_n_inv : STD_LOGIC := '1';
attribute shreg_extract of ap_rst_n_inv : signal is "no";
    signal out_r_TDATA_blk_n : STD_LOGIC;
    signal ap_CS_fsm : STD_LOGIC_VECTOR (1 downto 0) := "01";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_CS_fsm_pp0_stage0 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage0 : signal is "none";
    signal ap_enable_reg_pp0_iter0 : STD_LOGIC := '0';
    signal ap_block_pp0_stage0 : BOOLEAN;
    signal ap_enable_reg_pp0_iter1 : STD_LOGIC := '0';
    signal ap_block_state2_pp0_stage0_iter0 : BOOLEAN;
    signal ap_block_state3_pp0_stage0_iter1 : BOOLEAN;
    signal ap_block_pp0_stage0_11001 : BOOLEAN;
    signal ap_CS_fsm_state1 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state1 : signal is "none";
    signal ap_block_pp0_stage0_subdone : BOOLEAN;
    signal tmp_data_fu_50 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    signal add_ln17_fu_96_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_block_pp0_stage0_01001 : BOOLEAN;
    signal ap_NS_fsm : STD_LOGIC_VECTOR (1 downto 0);
    signal ap_ST_fsm_state1_blk : STD_LOGIC;
    signal ap_idle_pp0 : STD_LOGIC;
    signal ap_enable_pp0 : STD_LOGIC;
    signal regslice_both_out_r_V_data_V_U_apdone_blk : STD_LOGIC;
    signal out_r_TVALID_int_regslice : STD_LOGIC;
    signal out_r_TREADY_int_regslice : STD_LOGIC;
    signal regslice_both_out_r_V_data_V_U_vld_out : STD_LOGIC;
    signal regslice_both_out_r_V_keep_V_U_apdone_blk : STD_LOGIC;
    signal regslice_both_out_r_V_keep_V_U_ack_in_dummy : STD_LOGIC;
    signal regslice_both_out_r_V_keep_V_U_vld_out : STD_LOGIC;
    signal regslice_both_out_r_V_strb_V_U_apdone_blk : STD_LOGIC;
    signal out_r_TSTRB_int_regslice : STD_LOGIC_VECTOR (3 downto 0);
    signal regslice_both_out_r_V_strb_V_U_ack_in_dummy : STD_LOGIC;
    signal regslice_both_out_r_V_strb_V_U_vld_out : STD_LOGIC;
    signal regslice_both_out_r_V_last_V_U_apdone_blk : STD_LOGIC;
    signal regslice_both_out_r_V_last_V_U_ack_in_dummy : STD_LOGIC;
    signal regslice_both_out_r_V_last_V_U_vld_out : STD_LOGIC;
    signal ap_ce_reg : STD_LOGIC;

    component stream_out_regslice_both IS
    generic (
        DataWidth : INTEGER );
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
    end component;



begin
    regslice_both_out_r_V_data_V_U : component stream_out_regslice_both
    generic map (
        DataWidth => 32)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => tmp_data_fu_50,
        vld_in => out_r_TVALID_int_regslice,
        ack_in => out_r_TREADY_int_regslice,
        data_out => out_r_TDATA,
        vld_out => regslice_both_out_r_V_data_V_U_vld_out,
        ack_out => out_r_TREADY,
        apdone_blk => regslice_both_out_r_V_data_V_U_apdone_blk);

    regslice_both_out_r_V_keep_V_U : component stream_out_regslice_both
    generic map (
        DataWidth => 4)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => ap_const_lv4_F,
        vld_in => out_r_TVALID_int_regslice,
        ack_in => regslice_both_out_r_V_keep_V_U_ack_in_dummy,
        data_out => out_r_TKEEP,
        vld_out => regslice_both_out_r_V_keep_V_U_vld_out,
        ack_out => out_r_TREADY,
        apdone_blk => regslice_both_out_r_V_keep_V_U_apdone_blk);

    regslice_both_out_r_V_strb_V_U : component stream_out_regslice_both
    generic map (
        DataWidth => 4)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => out_r_TSTRB_int_regslice,
        vld_in => out_r_TVALID_int_regslice,
        ack_in => regslice_both_out_r_V_strb_V_U_ack_in_dummy,
        data_out => out_r_TSTRB,
        vld_out => regslice_both_out_r_V_strb_V_U_vld_out,
        ack_out => out_r_TREADY,
        apdone_blk => regslice_both_out_r_V_strb_V_U_apdone_blk);

    regslice_both_out_r_V_last_V_U : component stream_out_regslice_both
    generic map (
        DataWidth => 1)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => ap_const_lv1_0,
        vld_in => out_r_TVALID_int_regslice,
        ack_in => regslice_both_out_r_V_last_V_U_ack_in_dummy,
        data_out => out_r_TLAST,
        vld_out => regslice_both_out_r_V_last_V_U_vld_out,
        ack_out => out_r_TREADY,
        apdone_blk => regslice_both_out_r_V_last_V_U_apdone_blk);





    ap_CS_fsm_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst_n_inv = '1') then
                ap_CS_fsm <= ap_ST_fsm_state1;
            else
                ap_CS_fsm <= ap_NS_fsm;
            end if;
        end if;
    end process;


    ap_enable_reg_pp0_iter0_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst_n_inv = '1') then
                ap_enable_reg_pp0_iter0 <= ap_const_logic_0;
            else
                if ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
                    ap_enable_reg_pp0_iter0 <= ap_const_logic_1;
                end if; 
            end if;
        end if;
    end process;


    ap_enable_reg_pp0_iter1_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst_n_inv = '1') then
                ap_enable_reg_pp0_iter1 <= ap_const_logic_0;
            else
                if ((ap_const_boolean_0 = ap_block_pp0_stage0_subdone)) then 
                    ap_enable_reg_pp0_iter1 <= ap_enable_reg_pp0_iter0;
                elsif ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
                    ap_enable_reg_pp0_iter1 <= ap_const_logic_0;
                end if; 
            end if;
        end if;
    end process;


    ap_rst_n_inv_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            ap_rst_n_inv <= ap_rst_reg_1;
        end if;
    end process;

    ap_rst_reg_1_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            ap_rst_reg_1 <= ap_rst_reg_2;
        end if;
    end process;

    ap_rst_reg_2_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
                        ap_rst_reg_2 <= not(ap_rst_n);
        end if;
    end process;

    tmp_data_fu_50_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
                tmp_data_fu_50 <= ap_const_lv32_0;
            elsif (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
                tmp_data_fu_50 <= add_ln17_fu_96_p2;
            end if; 
        end if;
    end process;

    ap_NS_fsm_assign_proc : process (ap_CS_fsm)
    begin
        case ap_CS_fsm is
            when ap_ST_fsm_state1 => 
                ap_NS_fsm <= ap_ST_fsm_pp0_stage0;
            when ap_ST_fsm_pp0_stage0 => 
                ap_NS_fsm <= ap_ST_fsm_pp0_stage0;
            when others =>  
                ap_NS_fsm <= "XX";
        end case;
    end process;
    add_ln17_fu_96_p2 <= std_logic_vector(unsigned(tmp_data_fu_50) + unsigned(ap_const_lv32_1));
    ap_CS_fsm_pp0_stage0 <= ap_CS_fsm(1);
    ap_CS_fsm_state1 <= ap_CS_fsm(0);
    ap_ST_fsm_state1_blk <= ap_const_logic_0;
        ap_block_pp0_stage0 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_pp0_stage0_01001_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1, ap_block_state2_pp0_stage0_iter0, ap_block_state3_pp0_stage0_iter1)
    begin
                ap_block_pp0_stage0_01001 <= (((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (ap_const_boolean_1 = ap_block_state3_pp0_stage0_iter1)) or ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_boolean_1 = ap_block_state2_pp0_stage0_iter0)));
    end process;


    ap_block_pp0_stage0_11001_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1, ap_block_state2_pp0_stage0_iter0, ap_block_state3_pp0_stage0_iter1, out_r_TREADY_int_regslice)
    begin
                ap_block_pp0_stage0_11001 <= (((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state3_pp0_stage0_iter1) or (out_r_TREADY_int_regslice = ap_const_logic_0))) or ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state2_pp0_stage0_iter0) or (out_r_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_pp0_stage0_subdone_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1, ap_block_state2_pp0_stage0_iter0, ap_block_state3_pp0_stage0_iter1, out_r_TREADY_int_regslice)
    begin
                ap_block_pp0_stage0_subdone <= (((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state3_pp0_stage0_iter1) or (out_r_TREADY_int_regslice = ap_const_logic_0))) or ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state2_pp0_stage0_iter0) or (out_r_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_state2_pp0_stage0_iter0_assign_proc : process(out_r_TREADY_int_regslice)
    begin
                ap_block_state2_pp0_stage0_iter0 <= (out_r_TREADY_int_regslice = ap_const_logic_0);
    end process;


    ap_block_state3_pp0_stage0_iter1_assign_proc : process(out_r_TREADY_int_regslice)
    begin
                ap_block_state3_pp0_stage0_iter1 <= (out_r_TREADY_int_regslice = ap_const_logic_0);
    end process;

    ap_enable_pp0 <= (ap_idle_pp0 xor ap_const_logic_1);

    ap_idle_pp0_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1)
    begin
        if (((ap_enable_reg_pp0_iter1 = ap_const_logic_0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_0))) then 
            ap_idle_pp0 <= ap_const_logic_1;
        else 
            ap_idle_pp0 <= ap_const_logic_0;
        end if; 
    end process;


    out_r_TDATA_blk_n_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0, ap_enable_reg_pp0_iter1, out_r_TREADY_int_regslice)
    begin
        if ((((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0)) or ((ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0)))) then 
            out_r_TDATA_blk_n <= out_r_TREADY_int_regslice;
        else 
            out_r_TDATA_blk_n <= ap_const_logic_1;
        end if; 
    end process;

    out_r_TSTRB_int_regslice <= "XXXX";
    out_r_TVALID <= regslice_both_out_r_V_data_V_U_vld_out;

    out_r_TVALID_int_regslice_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            out_r_TVALID_int_regslice <= ap_const_logic_1;
        else 
            out_r_TVALID_int_regslice <= ap_const_logic_0;
        end if; 
    end process;

end behav;
