--******************************************************************************
-- Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
-- Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
-- SPDX-License-Identifier: MIT
--******************************************************************************
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity stream_in is
generic (
    C_S_AXI_CONTROL_ADDR_WIDTH : INTEGER := 6;
    C_S_AXI_CONTROL_DATA_WIDTH : INTEGER := 32 );
port (
    ap_clk : IN STD_LOGIC;
    ap_rst_n : IN STD_LOGIC;
    s_TDATA : IN STD_LOGIC_VECTOR (31 downto 0);
    s_TVALID : IN STD_LOGIC;
    s_TREADY : OUT STD_LOGIC;
    s_TKEEP : IN STD_LOGIC_VECTOR (3 downto 0);
    s_TSTRB : IN STD_LOGIC_VECTOR (3 downto 0);
    s_TLAST : IN STD_LOGIC_VECTOR (0 downto 0);
    s_axi_control_AWVALID : IN STD_LOGIC;
    s_axi_control_AWREADY : OUT STD_LOGIC;
    s_axi_control_AWADDR : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_ADDR_WIDTH-1 downto 0);
    s_axi_control_WVALID : IN STD_LOGIC;
    s_axi_control_WREADY : OUT STD_LOGIC;
    s_axi_control_WDATA : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    s_axi_control_WSTRB : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_DATA_WIDTH/8-1 downto 0);
    s_axi_control_ARVALID : IN STD_LOGIC;
    s_axi_control_ARREADY : OUT STD_LOGIC;
    s_axi_control_ARADDR : IN STD_LOGIC_VECTOR (C_S_AXI_CONTROL_ADDR_WIDTH-1 downto 0);
    s_axi_control_RVALID : OUT STD_LOGIC;
    s_axi_control_RREADY : IN STD_LOGIC;
    s_axi_control_RDATA : OUT STD_LOGIC_VECTOR (C_S_AXI_CONTROL_DATA_WIDTH-1 downto 0);
    s_axi_control_RRESP : OUT STD_LOGIC_VECTOR (1 downto 0);
    s_axi_control_BVALID : OUT STD_LOGIC;
    s_axi_control_BREADY : IN STD_LOGIC;
    s_axi_control_BRESP : OUT STD_LOGIC_VECTOR (1 downto 0));
    
end;


architecture behav of stream_in is 
    attribute CORE_GENERATION_INFO : STRING;
    attribute CORE_GENERATION_INFO of behav : architecture is
    "stream_in_stream_in,hls_ip_2023_2,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xcvc1902-vsva2197-2MP-e-S,HLS_INPUT_CLOCK=3.200000,HLS_INPUT_ARCH=others,HLS_SYN_CLOCK=2.893000,HLS_SYN_LAT=-1,HLS_SYN_TPT=none,HLS_SYN_MEM=2,HLS_SYN_DSP=0,HLS_SYN_FF=218,HLS_SYN_LUT=412,HLS_VERSION=2023_2}";
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_fsm_state1 : STD_LOGIC_VECTOR (1 downto 0) := "01";
    constant ap_ST_fsm_pp0_stage0 : STD_LOGIC_VECTOR (1 downto 0) := "10";
    constant ap_const_boolean_1 : BOOLEAN := true;
    constant ap_const_lv32_1 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000001";
    constant ap_const_boolean_0 : BOOLEAN := false;
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant C_S_AXI_DATA_WIDTH : INTEGER := 32;
    constant ap_const_lv1_0 : STD_LOGIC_VECTOR (0 downto 0) := "0";

attribute shreg_extract : string;
    signal ap_rst_reg_2 : STD_LOGIC := '1';
attribute shreg_extract of ap_rst_reg_2 : signal is "no";
    signal ap_rst_reg_1 : STD_LOGIC := '1';
attribute shreg_extract of ap_rst_reg_1 : signal is "no";
    signal ap_rst_n_inv : STD_LOGIC := '1';
attribute shreg_extract of ap_rst_n_inv : signal is "no";
    signal in_num : STD_LOGIC_VECTOR (31 downto 0);
    signal err_num_i : STD_LOGIC_VECTOR (31 downto 0);
    signal err_num_o : STD_LOGIC_VECTOR (31 downto 0);
    signal mem_data_address0 : STD_LOGIC_VECTOR (9 downto 0);
    signal mem_data_ce0 : STD_LOGIC;
    signal mem_data_q0 : STD_LOGIC_VECTOR (31 downto 0);
    signal s_TDATA_blk_n : STD_LOGIC;
    signal ap_CS_fsm : STD_LOGIC_VECTOR (1 downto 0) := "01";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_CS_fsm_pp0_stage0 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage0 : signal is "none";
    signal ap_enable_reg_pp0_iter0 : STD_LOGIC := '0';
    signal ap_block_pp0_stage0 : BOOLEAN;
    signal empty_reg_208_0 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_block_state2_pp0_stage0_iter0 : BOOLEAN;
    signal ap_block_pp0_stage0_11001 : BOOLEAN;
    signal icmp_ln26_fu_179_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal icmp_ln26_reg_218 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_CS_fsm_state1 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state1 : signal is "none";
    signal ap_enable_reg_pp0_iter1 : STD_LOGIC := '0';
    signal ap_block_pp0_stage0_subdone : BOOLEAN;
    signal ap_enable_reg_pp0_iter2 : STD_LOGIC := '0';
    signal zext_ln26_fu_151_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal i_fu_66 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    signal i_3_fu_144_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal add_ln27_fu_187_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_block_pp0_stage0_01001 : BOOLEAN;
    signal index_fu_138_p1 : STD_LOGIC_VECTOR (9 downto 0);
    signal ap_NS_fsm : STD_LOGIC_VECTOR (1 downto 0);
    signal ap_ST_fsm_state1_blk : STD_LOGIC;
    signal ap_str_blocking_cur_n : STD_LOGIC;
    signal ap_ext_blocking_n : STD_LOGIC;
    signal ap_str_blocking_n : STD_LOGIC;
    signal ap_int_blocking_n : STD_LOGIC;
    signal ap_ext_blocking_n_reg : STD_LOGIC;
    signal ap_str_blocking_n_reg : STD_LOGIC;
    signal ap_int_blocking_n_reg : STD_LOGIC;
    signal ap_idle_pp0 : STD_LOGIC;
    signal ap_enable_pp0 : STD_LOGIC;
    signal regslice_both_s_V_data_V_U_apdone_blk : STD_LOGIC;
    signal s_TDATA_int_regslice : STD_LOGIC_VECTOR (31 downto 0);
    signal s_TVALID_int_regslice : STD_LOGIC;
    signal s_TREADY_int_regslice : STD_LOGIC;
    signal regslice_both_s_V_data_V_U_ack_in : STD_LOGIC;
    signal regslice_both_s_V_keep_V_U_apdone_blk : STD_LOGIC;
    signal s_TKEEP_int_regslice : STD_LOGIC_VECTOR (3 downto 0);
    signal regslice_both_s_V_keep_V_U_vld_out : STD_LOGIC;
    signal regslice_both_s_V_keep_V_U_ack_in : STD_LOGIC;
    signal regslice_both_s_V_strb_V_U_apdone_blk : STD_LOGIC;
    signal s_TSTRB_int_regslice : STD_LOGIC_VECTOR (3 downto 0);
    signal regslice_both_s_V_strb_V_U_vld_out : STD_LOGIC;
    signal regslice_both_s_V_strb_V_U_ack_in : STD_LOGIC;
    signal regslice_both_s_V_last_V_U_apdone_blk : STD_LOGIC;
    signal s_TLAST_int_regslice : STD_LOGIC_VECTOR (0 downto 0);
    signal regslice_both_s_V_last_V_U_vld_out : STD_LOGIC;
    signal regslice_both_s_V_last_V_U_ack_in : STD_LOGIC;
    signal ap_ce_reg : STD_LOGIC;
    
    signal stall_start_ext : STD_LOGIC;
    signal stall_done_ext :  STD_LOGIC;
    signal stall_start_str : STD_LOGIC;
    signal stall_done_str :  STD_LOGIC;
    signal stall_start_int : STD_LOGIC;
    signal stall_done_int :  STD_LOGIC;

    component stream_in_mem_data_ROM_AUTO_1R IS
    generic (
        DataWidth : INTEGER;
        AddressRange : INTEGER;
        AddressWidth : INTEGER );
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR (9 downto 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR (31 downto 0) );
    end component;


    component stream_in_control_s_axi IS
    generic (
        C_S_AXI_ADDR_WIDTH : INTEGER;
        C_S_AXI_DATA_WIDTH : INTEGER );
    port (
        AWVALID : IN STD_LOGIC;
        AWREADY : OUT STD_LOGIC;
        AWADDR : IN STD_LOGIC_VECTOR (C_S_AXI_ADDR_WIDTH-1 downto 0);
        WVALID : IN STD_LOGIC;
        WREADY : OUT STD_LOGIC;
        WDATA : IN STD_LOGIC_VECTOR (C_S_AXI_DATA_WIDTH-1 downto 0);
        WSTRB : IN STD_LOGIC_VECTOR (C_S_AXI_DATA_WIDTH/8-1 downto 0);
        ARVALID : IN STD_LOGIC;
        ARREADY : OUT STD_LOGIC;
        ARADDR : IN STD_LOGIC_VECTOR (C_S_AXI_ADDR_WIDTH-1 downto 0);
        RVALID : OUT STD_LOGIC;
        RREADY : IN STD_LOGIC;
        RDATA : OUT STD_LOGIC_VECTOR (C_S_AXI_DATA_WIDTH-1 downto 0);
        RRESP : OUT STD_LOGIC_VECTOR (1 downto 0);
        BVALID : OUT STD_LOGIC;
        BREADY : IN STD_LOGIC;
        BRESP : OUT STD_LOGIC_VECTOR (1 downto 0);
        ACLK : IN STD_LOGIC;
        ARESET : IN STD_LOGIC;
        ACLK_EN : IN STD_LOGIC;
        in_num : IN STD_LOGIC_VECTOR (31 downto 0);
        err_num_o : IN STD_LOGIC_VECTOR (31 downto 0);
        err_num_i : OUT STD_LOGIC_VECTOR (31 downto 0) );
    end component;


    component stream_in_regslice_both IS
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
    mem_data_U : component stream_in_mem_data_ROM_AUTO_1R
    generic map (
        DataWidth => 32,
        AddressRange => 1024,
        AddressWidth => 10)
    port map (
        clk => ap_clk,
        reset => ap_rst_n_inv,
        address0 => mem_data_address0,
        ce0 => mem_data_ce0,
        q0 => mem_data_q0);

    control_s_axi_U : component stream_in_control_s_axi
    generic map (
        C_S_AXI_ADDR_WIDTH => C_S_AXI_CONTROL_ADDR_WIDTH,
        C_S_AXI_DATA_WIDTH => C_S_AXI_CONTROL_DATA_WIDTH)
    port map (
        AWVALID => s_axi_control_AWVALID,
        AWREADY => s_axi_control_AWREADY,
        AWADDR => s_axi_control_AWADDR,
        WVALID => s_axi_control_WVALID,
        WREADY => s_axi_control_WREADY,
        WDATA => s_axi_control_WDATA,
        WSTRB => s_axi_control_WSTRB,
        ARVALID => s_axi_control_ARVALID,
        ARREADY => s_axi_control_ARREADY,
        ARADDR => s_axi_control_ARADDR,
        RVALID => s_axi_control_RVALID,
        RREADY => s_axi_control_RREADY,
        RDATA => s_axi_control_RDATA,
        RRESP => s_axi_control_RRESP,
        BVALID => s_axi_control_BVALID,
        BREADY => s_axi_control_BREADY,
        BRESP => s_axi_control_BRESP,
        ACLK => ap_clk,
        ARESET => ap_rst_n_inv,
        ACLK_EN => ap_const_logic_1,
        in_num => in_num,
        err_num_o => err_num_o,
        err_num_i => err_num_i);

    regslice_both_s_V_data_V_U : component stream_in_regslice_both
    generic map (
        DataWidth => 32)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => s_TDATA,
        vld_in => s_TVALID,
        ack_in => regslice_both_s_V_data_V_U_ack_in,
        data_out => s_TDATA_int_regslice,
        vld_out => s_TVALID_int_regslice,
        ack_out => s_TREADY_int_regslice,
        apdone_blk => regslice_both_s_V_data_V_U_apdone_blk);

    regslice_both_s_V_keep_V_U : component stream_in_regslice_both
    generic map (
        DataWidth => 4)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => s_TKEEP,
        vld_in => s_TVALID,
        ack_in => regslice_both_s_V_keep_V_U_ack_in,
        data_out => s_TKEEP_int_regslice,
        vld_out => regslice_both_s_V_keep_V_U_vld_out,
        ack_out => s_TREADY_int_regslice,
        apdone_blk => regslice_both_s_V_keep_V_U_apdone_blk);

    regslice_both_s_V_strb_V_U : component stream_in_regslice_both
    generic map (
        DataWidth => 4)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => s_TSTRB,
        vld_in => s_TVALID,
        ack_in => regslice_both_s_V_strb_V_U_ack_in,
        data_out => s_TSTRB_int_regslice,
        vld_out => regslice_both_s_V_strb_V_U_vld_out,
        ack_out => s_TREADY_int_regslice,
        apdone_blk => regslice_both_s_V_strb_V_U_apdone_blk);

    regslice_both_s_V_last_V_U : component stream_in_regslice_both
    generic map (
        DataWidth => 1)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        data_in => s_TLAST,
        vld_in => s_TVALID,
        ack_in => regslice_both_s_V_last_V_U_ack_in,
        data_out => s_TLAST_int_regslice,
        vld_out => regslice_both_s_V_last_V_U_vld_out,
        ack_out => s_TREADY_int_regslice,
        apdone_blk => regslice_both_s_V_last_V_U_apdone_blk);





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


    ap_enable_reg_pp0_iter2_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst_n_inv = '1') then
                ap_enable_reg_pp0_iter2 <= ap_const_logic_0;
            else
                if ((ap_const_boolean_0 = ap_block_pp0_stage0_subdone)) then 
                    ap_enable_reg_pp0_iter2 <= ap_enable_reg_pp0_iter1;
                elsif ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
                    ap_enable_reg_pp0_iter2 <= ap_const_logic_0;
                end if; 
            end if;
        end if;
    end process;


    ap_ext_blocking_n_reg_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
        end if;
    end process;

    ap_int_blocking_n_reg_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
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

    ap_str_blocking_n_reg_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            ap_str_blocking_n_reg <= ap_str_blocking_n;
        end if;
    end process;

    i_fu_66_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
                i_fu_66 <= ap_const_lv32_0;
            elsif (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
                i_fu_66 <= i_3_fu_144_p2;
            end if; 
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                empty_reg_208_0 <= s_TDATA_int_regslice;
                icmp_ln26_reg_218 <= icmp_ln26_fu_179_p2;
            end if;
        end if;
    end process;
    ap_ext_blocking_n_reg <= '1';
    ap_int_blocking_n_reg <= '1';

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
    add_ln27_fu_187_p2 <= std_logic_vector(unsigned(err_num_i) + unsigned(ap_const_lv32_1));
    ap_CS_fsm_pp0_stage0 <= ap_CS_fsm(1);
    ap_CS_fsm_state1 <= ap_CS_fsm(0);
    ap_ST_fsm_state1_blk <= ap_const_logic_0;
        ap_block_pp0_stage0 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_pp0_stage0_01001_assign_proc : process(ap_enable_reg_pp0_iter0, ap_block_state2_pp0_stage0_iter0)
    begin
                ap_block_pp0_stage0_01001 <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_boolean_1 = ap_block_state2_pp0_stage0_iter0));
    end process;


    ap_block_pp0_stage0_11001_assign_proc : process(ap_enable_reg_pp0_iter0, ap_block_state2_pp0_stage0_iter0)
    begin
                ap_block_pp0_stage0_11001 <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_boolean_1 = ap_block_state2_pp0_stage0_iter0));
    end process;


    ap_block_pp0_stage0_subdone_assign_proc : process(ap_enable_reg_pp0_iter0, ap_block_state2_pp0_stage0_iter0)
    begin
                ap_block_pp0_stage0_subdone <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_boolean_1 = ap_block_state2_pp0_stage0_iter0));
    end process;


    ap_block_state2_pp0_stage0_iter0_assign_proc : process(s_TVALID_int_regslice)
    begin
                ap_block_state2_pp0_stage0_iter0 <= (s_TVALID_int_regslice = ap_const_logic_0);
    end process;

    ap_enable_pp0 <= (ap_idle_pp0 xor ap_const_logic_1);
    ap_ext_blocking_n <= (ap_const_logic_1 and ap_const_logic_1);

    ap_idle_pp0_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1, ap_enable_reg_pp0_iter2)
    begin
        if (((ap_enable_reg_pp0_iter2 = ap_const_logic_0) and (ap_enable_reg_pp0_iter1 = ap_const_logic_0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_0))) then 
            ap_idle_pp0 <= ap_const_logic_1;
        else 
            ap_idle_pp0 <= ap_const_logic_0;
        end if; 
    end process;

    ap_int_blocking_n <= (ap_const_logic_1 and ap_const_logic_1);
    ap_str_blocking_cur_n <= (s_TDATA_blk_n);
    ap_str_blocking_n <= (ap_str_blocking_cur_n and ap_const_logic_1);

    err_num_o_assign_proc : process(icmp_ln26_reg_218, ap_CS_fsm_state1, ap_enable_reg_pp0_iter2, add_ln27_fu_187_p2, ap_block_pp0_stage0_01001)
    begin
        if (((ap_enable_reg_pp0_iter2 = ap_const_logic_1) and (icmp_ln26_reg_218 = ap_const_lv1_0) and (ap_const_boolean_0 = ap_block_pp0_stage0_01001))) then 
            err_num_o <= add_ln27_fu_187_p2;
        elsif ((ap_const_logic_1 = ap_CS_fsm_state1)) then 
            err_num_o <= ap_const_lv32_0;
        else 
            err_num_o <= "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        end if; 
    end process;

    i_3_fu_144_p2 <= std_logic_vector(unsigned(i_fu_66) + unsigned(ap_const_lv32_1));
    icmp_ln26_fu_179_p2 <= "1" when (empty_reg_208_0 = mem_data_q0) else "0";
    in_num <= std_logic_vector(unsigned(i_fu_66) + unsigned(ap_const_lv32_1));
    index_fu_138_p1 <= i_fu_66(10 - 1 downto 0);
    mem_data_address0 <= zext_ln26_fu_151_p1(10 - 1 downto 0);

    mem_data_ce0_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            mem_data_ce0 <= ap_const_logic_1;
        else 
            mem_data_ce0 <= ap_const_logic_0;
        end if; 
    end process;


    s_TDATA_blk_n_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0, s_TVALID_int_regslice)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            s_TDATA_blk_n <= s_TVALID_int_regslice;
        else 
            s_TDATA_blk_n <= ap_const_logic_1;
        end if; 
    end process;

    s_TREADY <= regslice_both_s_V_data_V_U_ack_in;

    s_TREADY_int_regslice_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            s_TREADY_int_regslice <= ap_const_logic_1;
        else 
            s_TREADY_int_regslice <= ap_const_logic_0;
        end if; 
    end process;


    stall_done_ext_assign_proc : process(ap_ext_blocking_n, ap_ext_blocking_n_reg)
    begin
        if (((ap_ext_blocking_n_reg = ap_const_logic_0) and (ap_ext_blocking_n = ap_const_logic_1))) then 
            stall_done_ext <= ap_const_logic_1;
        else 
            stall_done_ext <= ap_const_logic_0;
        end if; 
    end process;


    stall_done_int_assign_proc : process(ap_int_blocking_n, ap_int_blocking_n_reg)
    begin
        if (((ap_int_blocking_n_reg = ap_const_logic_0) and (ap_int_blocking_n = ap_const_logic_1))) then 
            stall_done_int <= ap_const_logic_1;
        else 
            stall_done_int <= ap_const_logic_0;
        end if; 
    end process;


    stall_done_str_assign_proc : process(ap_str_blocking_n, ap_str_blocking_n_reg)
    begin
        if (((ap_str_blocking_n_reg = ap_const_logic_0) and (ap_str_blocking_n = ap_const_logic_1))) then 
            stall_done_str <= ap_const_logic_1;
        else 
            stall_done_str <= ap_const_logic_0;
        end if; 
    end process;


    stall_start_ext_assign_proc : process(ap_ext_blocking_n, ap_ext_blocking_n_reg)
    begin
        if (((ap_ext_blocking_n_reg = ap_const_logic_1) and (ap_ext_blocking_n = ap_const_logic_0))) then 
            stall_start_ext <= ap_const_logic_1;
        else 
            stall_start_ext <= ap_const_logic_0;
        end if; 
    end process;


    stall_start_int_assign_proc : process(ap_int_blocking_n, ap_int_blocking_n_reg)
    begin
        if (((ap_int_blocking_n_reg = ap_const_logic_1) and (ap_int_blocking_n = ap_const_logic_0))) then 
            stall_start_int <= ap_const_logic_1;
        else 
            stall_start_int <= ap_const_logic_0;
        end if; 
    end process;


    stall_start_str_assign_proc : process(ap_str_blocking_n, ap_str_blocking_n_reg)
    begin
        if (((ap_str_blocking_n_reg = ap_const_logic_1) and (ap_str_blocking_n = ap_const_logic_0))) then 
            stall_start_str <= ap_const_logic_1;
        else 
            stall_start_str <= ap_const_logic_0;
        end if; 
    end process;

    zext_ln26_fu_151_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(index_fu_138_p1),64));
end behav;
