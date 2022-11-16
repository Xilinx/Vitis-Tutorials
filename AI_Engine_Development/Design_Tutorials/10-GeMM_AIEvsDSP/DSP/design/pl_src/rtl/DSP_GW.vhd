--*****************************************************************************
-- © Copyright 2008 - 2021 Xilinx, Inc. All rights reserved.
--
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
--
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
--
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
--
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
--
--*****************************************************************************
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor                : Xilinx
-- \   \   \/     Version               : v2.0
--  \   \         Application           : DSP48E1/DSP48E2/DSP58 generic wrapper
--  /   /         Filename              : DSP_GW.vhd
-- /___/   /\     Date Last Modified    : Sep 14 2020
-- \   \  /  \    Date Created          : Nov 14 2014
--  \___\/\___\
--
--Device            : UltraScale and UltraScale+
--Design Name       : DSP_GW
--Purpose           : DSP48E1/DSP48E2/DSP58 Generic Wrapper makes DSP48/58 primitive instantiation easier
--Reference         : 
--Revision History  : v1.0 - original version
--Revision History  : v1.1 - smart SFIXED resizing
--Revision History  : v1.2 - fix for output resizing
--Revision History  : v2.0 - Versal support
--*****************************************************************************

library IEEE;
use IEEE.STD_LOGIC_1164.all;
--use IEEE.NUMERIC_STD.all;

use work.SFIXED_PKG.all;
use work.CFIXED_PKG.all;

library UNISIM;
use UNISIM.vcomponents.all;

entity DSP_GW is
  generic(X,Y:INTEGER:=-1;
          FAMILY:INTEGER:=2;         -- use 1 for 7-series, 2 for US/US+ and 3 for Versal
          -- Feature Control Attributes: Data Path Selection
          AMULTSEL:STRING:="A";                                      -- Selects A input to multiplier (A, AD)
          A_INPUT:STRING:="DIRECT";                                  -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
          BMULTSEL:STRING:="B";                                      -- Selects B input to multiplier (AD, B)
          B_INPUT:STRING:="DIRECT";                                  -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
          PREADDINSEL:STRING:="A";                                   -- Selects input to preadder (A, B)
          RND:STD_LOGIC_VECTOR(FAMILY/3*10+47 downto 0):=(others=>'0');      -- Rounding Constant
          USE_MULT:STRING:="MULTIPLY";                               -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
          USE_SIMD:STRING:="ONE48";                                  -- SIMD selection (FOUR12, ONE48, TWO24)
          DSP_MODE:STRING:="INT24";                                  -- Only for DSP58 - this attribute configures the DSP for a particular mode of operation. INT24 is for the 27 × 24 fixed-point ALU and alsofor the legacy mode. INT8 is for the three-element 9 × 8 vector dot-product mode
          USE_WIDEXOR:STRING:="FALSE";                               -- Use the Wide XOR function (FALSE, TRUE)
          XORSIMD:STRING:="XOR24_48_96";                             -- Mode of operation for the Wide XOR (XOR12, XOR24_48_96)
          -- Pattern Detector Attributes: Pattern Detection Configuration
          AUTORESET_PATDET:STRING:="NO_RESET";                       -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
          AUTORESET_PRIORITY:STRING:="RESET";                        -- Priority of AUTORESET vs.CEP (CEP, RESET).
          MASK:STD_LOGIC_VECTOR(FAMILY/3*10+47 downto 0):=(FAMILY/3*10+47 downto FAMILY/3*10+46=>'0',others=>'1');     -- 48/58-bit mask value for pattern detect (1=ignore)
          PATTERN:STD_LOGIC_VECTOR(FAMILY/3*10+47 downto 0):=(others=>'0');  -- 48/58-bit pattern match for pattern detect
          SEL_MASK:STRING:="MASK";                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
          SEL_PATTERN:STRING:="PATTERN";                             -- Select pattern value (C, PATTERN)
          USE_PATTERN_DETECT:STRING:="NO_PATDET";                    -- Enable pattern detect (NO_PATDET, PATDET)
          -- Programmable Inversion Attributes: Specifies built-in programmable inversion on specific pins
          IS_ALUMODE_INVERTED:STD_LOGIC_VECTOR(3 downto 0):=4x"0";   -- Optional inversion for ALUMODE
          IS_ASYNC_RST_INVERTED:BIT:='0';                            -- Optional inversion for ASYNC_RST (DSP58 only)
          IS_CARRYIN_INVERTED:BIT:='0';                              -- Optional inversion for CARRYIN
          IS_CLK_INVERTED:BIT:='0';                                  -- Optional inversion for CLK
          IS_INMODE_INVERTED:STD_LOGIC_VECTOR(4 downto 0):=5x"00";   -- Optional inversion for INMODE
          IS_NEGATE_INVERTED:STD_LOGIC_VECTOR(2 downto 0):=3x"0";    -- Optional inversion for NEGATE
          IS_OPMODE_INVERTED:STD_LOGIC_VECTOR(8 downto 0):=9x"000";  -- Optional inversion for OPMODE
          IS_RSTALLCARRYIN_INVERTED:BIT:='0';                        -- Optional inversion for RSTALLCARRYIN
          IS_RSTALUMODE_INVERTED:BIT:='0';                           -- Optional inversion for RSTALUMODE
          IS_RSTA_INVERTED:BIT:='0';                                 -- Optional inversion for RSTA
          IS_RSTB_INVERTED:BIT:='0';                                 -- Optional inversion for RSTB
          IS_RSTCTRL_INVERTED:BIT:='0';                              -- Optional inversion for RSTCTRL
          IS_RSTC_INVERTED:BIT:='0';                                 -- Optional inversion for RSTC
          IS_RSTD_INVERTED:BIT:='0';                                 -- Optional inversion for RSTD
          IS_RSTINMODE_INVERTED:BIT:='0';                            -- Optional inversion for RSTINMODE
          IS_RSTM_INVERTED:BIT:='0';                                 -- Optional inversion for RSTM
          IS_RSTP_INVERTED:BIT:='0';                                 -- Optional inversion for RSTP
          RESET_MODE:STRING:="SYNC";                                 -- "SYNC" or "ASYNC", selects if the enabled registers in the DSP are reset by their register specific synchronous resets or the common ASYNC_RST                       
          -- Register Control Attributes: Pipeline Register Configuration
          ACASCREG:INTEGER:=1;                                       -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
          ADREG:INTEGER:=1;                                          -- Pipeline stages for pre-adder (0-1)
          ALUMODEREG:INTEGER:=1;                                     -- Pipeline stages for ALUMODE (0-1)
          AREG:INTEGER:=1;                                           -- Pipeline stages for A (0-2)
          BCASCREG:INTEGER:=1;                                       -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
          BREG:INTEGER:=1;                                           -- Pipeline stages for B (0-2)
          CARRYINREG:INTEGER:=1;                                     -- Pipeline stages for CARRYIN (0-1)
          CARRYINSELREG:INTEGER:=1;                                  -- Pipeline stages for CARRYINSEL (0-1)
          CREG:INTEGER:=1;                                           -- Pipeline stages for C (0-1)
          DREG:INTEGER:=1;                                           -- Pipeline stages for D (0-1)
          INMODEREG:INTEGER:=1;                                      -- Pipeline stages for INMODE (0-1)
          MREG:INTEGER:=1;                                           -- Multiplier pipeline stages (0-1)
          OPMODEREG:INTEGER:=1;                                      -- Pipeline stages for OPMODE (0-1)
          PREG:INTEGER:=1);                                          -- Number of pipeline stages for P (0-1)
  port(-- Cascade inputs: Cascade Ports
       ACIN:in STD_LOGIC_VECTOR(FAMILY/3*4+29 downto 0):=(others=>'0');         -- 30/34-bit input: A cascade data
       BCIN:in STD_LOGIC_VECTOR(FAMILY/3*6+17 downto 0):=(others=>'0');         -- 18/24-bit input: B cascade
       CARRYCASCIN:in STD_LOGIC:='0';                                -- 1-bit input: Cascade carry
       MULTSIGNIN:in STD_LOGIC:='0';                                 -- 1-bit input: Multiplier sign cascade
       PCIN:in STD_LOGIC_VECTOR(FAMILY/3*10+47 downto 0):=(others=>'0');         -- 48/58-bit input: P cascade
       -- Control inputs: Control Inputs/Status Bits
       ALUMODE:in STD_LOGIC_VECTOR(3 downto 0):=4x"0";               -- 4-bit input: ALU control
       NEGATE: in STD_LOGIC_VECTOR(2 downto 0):="000";               -- Negate the multiplier output (only for DSP58)
       CARRYINSEL:in STD_LOGIC_VECTOR(2 downto 0):=3x"0";            -- 3-bit input: Carry select
       CLK:in STD_LOGIC:='0';                                        -- 1-bit input: Clock
       INMODE:in STD_LOGIC_VECTOR(4 downto 0):=5x"00";               -- 5-bit input: INMODE control
       OPMODE:in STD_LOGIC_VECTOR(8 downto 0):=9x"035";              -- 9-bit input: Operation mode - default is P<=C+A*B
       -- Data inputs: Data Ports
       A:in SFIXED;--(Ahi downto Alo):=(others=>'0');                   -- 30/34-bit input: A data
       B:in SFIXED;--(Bhi downto Blo):=(others=>'0');                   -- 18/24-bit input: B data
       C:in SFIXED;--(Chi downto Clo):=(others=>'0');                   -- 48/58-bit input: C data
       CARRYIN:in STD_LOGIC:='0';                                    -- 1-bit input: Carry-in
       D:in SFIXED;--(Dhi downto Dlo):=(others=>'0');                   -- 27-bit input: D data
       -- Reset/Clock Enable inputs: Reset/Clock Enable Inputs
       CEA1:in STD_LOGIC:='1';                                       -- 1-bit input: Clock enable for 1st stage AREG
       CEA2:in STD_LOGIC:='1';                                       -- 1-bit input: Clock enable for 2nd stage AREG
       CEAD:in STD_LOGIC:='1';                                       -- 1-bit input: Clock enable for ADREG
       CEALUMODE:in STD_LOGIC:='1';                                  -- 1-bit input: Clock enable for ALUMODE
       CEB1:in STD_LOGIC:='1';                                       -- 1-bit input: Clock enable for 1st stage BREG
       CEB2:in STD_LOGIC:='1';                                       -- 1-bit input: Clock enable for 2nd stage BREG
       CEC:in STD_LOGIC:='1';                                        -- 1-bit input: Clock enable for CREG
       CECARRYIN:in STD_LOGIC:='1';                                  -- 1-bit input: Clock enable for CARRYINREG
       CECTRL:in STD_LOGIC:='1';                                     -- 1-bit input: Clock enable for OPMODEREG and CARRYINSELREG
       CED:in STD_LOGIC:='1';                                        -- 1-bit input: Clock enable for DREG
       CEINMODE:in STD_LOGIC:='1';                                   -- 1-bit input: Clock enable for INMODEREG
       CEM:in STD_LOGIC:='1';                                        -- 1-bit input: Clock enable for MREG
       CEP:in STD_LOGIC:='1';                                        -- 1-bit input: Clock enable for PREG
       RSTA:in STD_LOGIC:='0';                                       -- 1-bit input: Reset for AREG
       RSTALLCARRYIN:in STD_LOGIC:='0';                              -- 1-bit input: Reset for CARRYINREG
       RSTALUMODE:in STD_LOGIC:='0';                                 -- 1-bit input: Reset for ALUMODEREG
       RSTB:in STD_LOGIC:='0';                                       -- 1-bit input: Reset for BREG
       RSTC:in STD_LOGIC:='0';                                       -- 1-bit input: Reset for CREG
       RSTCTRL:in STD_LOGIC:='0';                                    -- 1-bit input: Reset for OPMODEREG and CARRYINSELREG
       RSTD:in STD_LOGIC:='0';                                       -- 1-bit input: Reset for DREG and ADREG
       RSTINMODE:in STD_LOGIC:='0';                                  -- 1-bit input: Reset for INMODEREG
       RSTM:in STD_LOGIC:='0';                                       -- 1-bit input: Reset for MREG
       RSTP:in STD_LOGIC:='0';                                       -- 1-bit input: Reset for PREG
       ASYNC_RST: in STD_LOGIC:='0';                                 -- Asynchronousreset for allregisters. Input only valid for DSP58 when attribute RESET_MODE=ASYNC
       -- Cascade outputs: Cascade Ports
       ACOUT:out STD_LOGIC_VECTOR(FAMILY/3*4+29 downto 0);                      -- 30/34-bit output: A port cascade
       BCOUT:out STD_LOGIC_VECTOR(FAMILY/3*6+17 downto 0);                      -- 18/24-bit output: B cascade
       CARRYCASCOUT:out STD_LOGIC;                                   -- 1-bit output: Cascade carry
       MULTSIGNOUT:out STD_LOGIC;                                    -- 1-bit output: Multiplier sign cascade
       PCOUT:out STD_LOGIC_VECTOR(FAMILY/3*10+47 downto 0);                      -- 48/58-bit output: Cascade output
       -- Control outputs: Control Inputs/Status Bits
       OVERFLOW:out STD_LOGIC;                                       -- 1-bit output: Overflow in add/acc
       PATTERNBDETECT:out STD_LOGIC;                                 -- 1-bit output: Pattern bar detect
       PATTERNDETECT:out STD_LOGIC;                                  -- 1-bit output: Pattern detect
       UNDERFLOW:out STD_LOGIC;                                      -- 1-bit output: Underflow in add/acc
       -- Data outputs: Data Ports
       CARRYOUT:out STD_LOGIC_VECTOR(3 downto 0);                    -- 4-bit output: Carry
       P:out SFIXED;--(Phi downto Plo);                                 -- 48/58-bit output: Primary data
       XOROUT:out STD_LOGIC_VECTOR(7 downto 0));                     -- 8-bit output: XOR data
  attribute secure_config:STRING;
  attribute secure_config of DSP_GW:entity is "PROTECT";
end entity;

architecture WRAPPER of DSP_GW is
  signal slvA:STD_LOGIC_VECTOR(FAMILY/3*4+29 downto 0);
  signal slvB:STD_LOGIC_VECTOR(FAMILY/3*6+17 downto 0);
  signal slvD:STD_LOGIC_VECTOR(FAMILY/2*2+24 downto 0);
  signal slvC,slvP:STD_LOGIC_VECTOR(FAMILY/3*10+47 downto 0);
-- resize SFIXED and convert to STD_LOGIC_VECTOR
  function SFIXED_TO_SLV_RESIZE(I:SFIXED;hi,lo:INTEGER) return STD_LOGIC_VECTOR is
    variable O:STD_LOGIC_VECTOR(hi-lo downto 0);
  begin
    for K in O'range loop
      if K<I'low-lo then
        O(K):='0';
      elsif K<I'high-lo then
        O(K):=I(K+lo);
      else
        O(K):=I(I'high);
      end if;
    end loop;
    return O;
  end;
-- convert STD_LOGIC_VECTOR to SFIXED and resize 
  function SLV_TO_SFIXED_RESIZE(I:STD_LOGIC_VECTOR;hi,lo:INTEGER;ofs:INTEGER:=0) return SFIXED is
    variable O:SFIXED(hi downto lo);
  begin
    for K in O'range loop
      if K<I'low+lo+ofs then
        O(K):='0';
      elsif K-lo-ofs<I'length then
        O(K):=I(K-lo-ofs);
      else
        O(K):=I(I'high);
      end if;
    end loop;
    return O;
  end;
  
  function MIN(X,Y:INTEGER) return INTEGER is
  begin
    if X<Y then
      return X;
    else
      return Y;
    end if;
  end;
-- fix USE_SIMD="ONE48" for Versal  
  function FIX_USE_SIMD(S:STRING) return STRING is
  begin
    if S="ONE48" then
      return "ONE58";
    else
      return S;
    end if;
  end;
-- fix XORSIMD="XOR24_48_96" for Versal  
  function FIX_XORSIMD(S:STRING) return STRING is
  begin
    if S="XOR24_48_96" then
      return "XOR24_34_58_116";
    else
      return S;
    end if;
  end;
  
  constant AD_low:INTEGER:=MIN(A'low,D'low);
  constant BD_low:INTEGER:=MIN(B'low,D'low);
  constant CAD_low:INTEGER:=MIN(AD_low+B'low,P'low);
  constant CBD_low:INTEGER:=MIN(BD_low+A'low,P'low);
begin
  slvA<=SFIXED_TO_SLV_RESIZE(A,AD_low+slvA'length-1,AD_low) when PREADDINSEL="A" else
        SFIXED_TO_SLV_RESIZE(A,A'low+slvA'length-1,A'low); -- when PREADDINSEL="B"
  slvB<=SFIXED_TO_SLV_RESIZE(B,B'low+slvB'length-1,B'low) when PREADDINSEL="A" else
        SFIXED_TO_SLV_RESIZE(B,BD_low+slvB'length-1,BD_low); -- when PREADDINSEL="B"
  slvC<=SFIXED_TO_SLV_RESIZE(C,CAD_low+slvC'length-1,CAD_low) when PREADDINSEL="A" else
        SFIXED_TO_SLV_RESIZE(C,CBD_low+slvC'length-1,CBD_low); -- when PREADDINSEL="B"
  slvD<=SFIXED_TO_SLV_RESIZE(D,AD_low+slvD'length-1,AD_low) when PREADDINSEL="A" else
        SFIXED_TO_SLV_RESIZE(D,BD_low+slvD'length-1,BD_low); -- when PREADDINSEL="B"
-- two versions to avoid creating false Vivado critical warnings when no LOC constraints are used
  i1:if (X>=0) and (Y>=0) generate
     begin
       i1:if FAMILY=1 generate
            attribute loc:STRING;
            attribute loc of ds:label is "DSP48E1_X"&INTEGER'image(X)&"Y"&INTEGER'image(Y);
          begin
            ds:DSP48E1 generic map(-- Feature Control Attributes: Data Path Selection
                                   A_INPUT => A_INPUT,                                     -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                                   B_INPUT => B_INPUT,                                     -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
                                   USE_MULT => USE_MULT,                                   -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
                                   USE_SIMD => USE_SIMD,                                   -- SIMD selection (FOUR12, ONE48, TWO24)
                                   -- Pattern Detector Attributes: Pattern Detection Configuration
                                   AUTORESET_PATDET => AUTORESET_PATDET,                   -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
--                                   MASK => MASK,                                           -- 48-bit mask value for pattern detect (1=ignore)
--                                   PATTERN => PATTERN,                                     -- 48-bit pattern match for pattern detect
                                   SEL_MASK => SEL_MASK,                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
                                   SEL_PATTERN => SEL_PATTERN,                             -- Select pattern value (C, PATTERN)
                                   USE_PATTERN_DETECT => USE_PATTERN_DETECT,               -- Enable pattern detect (NO_PATDET, PATDET)
                                   -- Register Control Attributes: Pipeline Register Configuration
                                   ACASCREG => ACASCREG,                                   -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
                                   ADREG => ADREG,                                         -- Pipeline stages for pre-adder (0-1)
                                   ALUMODEREG => ALUMODEREG,                               -- Pipeline stages for ALUMODE (0-1)
                                   AREG => AREG,                                           -- Pipeline stages for A (0-2)
                                   BCASCREG => BCASCREG,                                   -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
                                   BREG => BREG,                                           -- Pipeline stages for B (0-2)
                                   CARRYINREG => CARRYINREG,                               -- Pipeline stages for CARRYIN (0-1)
                                   CARRYINSELREG => CARRYINSELREG,                         -- Pipeline stages for CARRYINSEL (0-1)
                                   CREG => CREG,                                           -- Pipeline stages for C (0-1)
                                   DREG => DREG,                                           -- Pipeline stages for D (0-1)
                                   INMODEREG => INMODEREG,                                 -- Pipeline stages for INMODE (0-1)
                                   MREG => MREG,                                           -- Multiplier pipeline stages (0-1)
                                   OPMODEREG => OPMODEREG,                                 -- Pipeline stages for OPMODE (0-1)
                                   PREG => PREG)                                           -- Number of pipeline stages for P (0-1)
                       port map(-- Cascade inputs: Cascade Ports
                                ACIN => ACIN,                                              -- 30-bit input: A cascade data
                                BCIN => BCIN,                                              -- 18-bit input: B cascade
                                CARRYCASCIN => CARRYCASCIN,                                -- 1-bit input: Cascade carry
                                MULTSIGNIN => MULTSIGNIN,                                  -- 1-bit input: Multiplier sign cascade
                                PCIN => PCIN,                                              -- 48-bit input: P cascade
                                -- Control inputs: Control Inputs/Status Bits
                                ALUMODE => ALUMODE,                                        -- 4-bit input: ALU control
                                CARRYINSEL => CARRYINSEL,                                  -- 3-bit input: Carry select
                                CLK => CLK,                                                -- 1-bit input: Clock
                                INMODE => INMODE,                                          -- 5-bit input: INMODE control
                                OPMODE => OPMODE(6 downto 0),                              -- 7-bit input: Operation mode
                                -- Data inputs: Data Ports
                                A => slvA,                                                 -- 30-bit input: A data
                                B => slvB,                                                 -- 18-bit input: B data
                                C => slvC,                                                 -- 48-bit input: C data
                                CARRYIN => CARRYIN,                                        -- 1-bit input: Carry-in
                                D => slvD(24 downto 0),                                    -- 25-bit input: D data
                                -- Reset/Clock Enable inputs: Reset/Clock Enable Inputs
                                CEA1 => CEA1,                                              -- 1-bit input: Clock enable for 1st stage AREG
                                CEA2 => CEA2,                                              -- 1-bit input: Clock enable for 2nd stage AREG
                                CEAD => CEAD,                                              -- 1-bit input: Clock enable for ADREG
                                CEALUMODE => CEALUMODE,                                    -- 1-bit input: Clock enable for ALUMODE
                                CEB1 => CEB1,                                              -- 1-bit input: Clock enable for 1st stage BREG
                                CEB2 => CEB2,                                              -- 1-bit input: Clock enable for 2nd stage BREG
                                CEC => CEC,                                                -- 1-bit input: Clock enable for CREG
                                CECARRYIN => CECARRYIN,                                    -- 1-bit input: Clock enable for CARRYINREG
                                CECTRL => CECTRL,                                          -- 1-bit input: Clock enable for OPMODEREG and CARRYINSELREG
                                CED => CED,                                                -- 1-bit input: Clock enable for DREG
                                CEINMODE => CEINMODE,                                      -- 1-bit input: Clock enable for INMODEREG
                                CEM => CEM,                                                -- 1-bit input: Clock enable for MREG
                                CEP => CEP,                                                -- 1-bit input: Clock enable for PREG
                                RSTA => RSTA,                                              -- 1-bit input: Reset for AREG
                                RSTALLCARRYIN => RSTALLCARRYIN,                            -- 1-bit input: Reset for CARRYINREG
                                RSTALUMODE => RSTALUMODE,                                  -- 1-bit input: Reset for ALUMODEREG
                                RSTB => RSTB,                                              -- 1-bit input: Reset for BREG
                                RSTC => RSTC,                                              -- 1-bit input: Reset for CREG
                                RSTCTRL => RSTCTRL,                                        -- 1-bit input: Reset for OPMODEREG and CARRYINSELREG
                                RSTD => RSTD,                                              -- 1-bit input: Reset for DREG and ADREG
                                RSTINMODE => RSTINMODE,                                    -- 1-bit input: Reset for INMODEREG
                                RSTM => RSTM,                                              -- 1-bit input: Reset for MREG
                                RSTP => RSTP,                                              -- 1-bit input: Reset for PREG
                                -- Cascade outputs: Cascade Ports
                                ACOUT => ACOUT,                                            -- 30-bit output: A port cascade
                                BCOUT => BCOUT,                                            -- 18-bit output: B cascade
                                CARRYCASCOUT => CARRYCASCOUT,                              -- 1-bit output: Cascade carry
                                MULTSIGNOUT => MULTSIGNOUT,                                -- 1-bit output: Multiplier sign cascade
                                PCOUT => PCOUT,                                            -- 48-bit output: Cascade output
                                -- Control outputs: Control Inputs/Status Bits
                                OVERFLOW => OVERFLOW,                                      -- 1-bit output: Overflow in add/acc
                                PATTERNBDETECT => PATTERNBDETECT,                          -- 1-bit output: Pattern bar detect
                                PATTERNDETECT => PATTERNDETECT,                            -- 1-bit output: Pattern detect
                                UNDERFLOW => UNDERFLOW,                                    -- 1-bit output: Underflow in add/acc
                                -- Data outputs: Data Ports
                                CARRYOUT => CARRYOUT,                                      -- 4-bit output: Carry
                                P => slvP);                                                -- 48-bit output: Primary data
          end generate;
       i2:if FAMILY=2 generate
            attribute loc:STRING;
            attribute loc of ds:label is "DSP48E2_X"&INTEGER'image(X)&"Y"&INTEGER'image(Y);
          begin
            ds:DSP48E2 generic map(-- Feature Control Attributes: Data Path Selection
                                   AMULTSEL => AMULTSEL,                                   -- Selects A input to multiplier (A, AD)
                                   A_INPUT => A_INPUT,                                     -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                                   BMULTSEL => BMULTSEL,                                   -- Selects B input to multiplier (AD, B)
                                   B_INPUT => B_INPUT,                                     -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
                                   PREADDINSEL => PREADDINSEL,                             -- Selects input to preadder (A, B)
                                   RND => RND,                                             -- Rounding Constant
                                   USE_MULT => USE_MULT,                                   -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
                                   USE_SIMD => USE_SIMD,                                   -- SIMD selection (FOUR12, ONE48, TWO24)
                                   USE_WIDEXOR => USE_WIDEXOR,                             -- Use the Wide XOR function (FALSE, TRUE)
                                   XORSIMD => XORSIMD,                                     -- Mode of operation for the Wide XOR (XOR12, XOR24_48_96)
                                   -- Pattern Detector Attributes: Pattern Detection Configuration
                                   AUTORESET_PATDET => AUTORESET_PATDET,                   -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
                                   AUTORESET_PRIORITY => AUTORESET_PRIORITY,               -- Priority of AUTORESET vs.CEP (CEP, RESET).
                                   MASK => MASK,                                           -- 48-bit mask value for pattern detect (1=ignore)
                                   PATTERN => PATTERN,                                     -- 48-bit pattern match for pattern detect
                                   SEL_MASK => SEL_MASK,                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
                                   SEL_PATTERN => SEL_PATTERN,                             -- Select pattern value (C, PATTERN)
                                   USE_PATTERN_DETECT => USE_PATTERN_DETECT,               -- Enable pattern detect (NO_PATDET, PATDET)
                                   -- Programmable Inversion Attributes: Specifies built-in programmable inversion on specific pins
                                   IS_ALUMODE_INVERTED => IS_ALUMODE_INVERTED,             -- Optional inversion for ALUMODE
                                   IS_CARRYIN_INVERTED => IS_CARRYIN_INVERTED,             -- Optional inversion for CARRYIN
                                   IS_CLK_INVERTED => IS_CLK_INVERTED,                     -- Optional inversion for CLK
                                   IS_INMODE_INVERTED => IS_INMODE_INVERTED,               -- Optional inversion for INMODE
                                   IS_OPMODE_INVERTED => IS_OPMODE_INVERTED,               -- Optional inversion for OPMODE
                                   IS_RSTALLCARRYIN_INVERTED => IS_RSTALLCARRYIN_INVERTED, -- Optional inversion for RSTALLCARRYIN
                                   IS_RSTALUMODE_INVERTED => IS_RSTALUMODE_INVERTED,       -- Optional inversion for RSTALUMODE
                                   IS_RSTA_INVERTED => IS_RSTA_INVERTED,                   -- Optional inversion for RSTA
                                   IS_RSTB_INVERTED => IS_RSTB_INVERTED,                   -- Optional inversion for RSTB
                                   IS_RSTCTRL_INVERTED => IS_RSTCTRL_INVERTED,             -- Optional inversion for RSTCTRL
                                   IS_RSTC_INVERTED => IS_RSTC_INVERTED,                   -- Optional inversion for RSTC
                                   IS_RSTD_INVERTED => IS_RSTD_INVERTED,                   -- Optional inversion for RSTD
                                   IS_RSTINMODE_INVERTED => IS_RSTINMODE_INVERTED,         -- Optional inversion for RSTINMODE
                                   IS_RSTM_INVERTED => IS_RSTM_INVERTED,                   -- Optional inversion for RSTM
                                   IS_RSTP_INVERTED => IS_RSTP_INVERTED,                   -- Optional inversion for RSTP
                                   -- Register Control Attributes: Pipeline Register Configuration
                                   ACASCREG => ACASCREG,                                   -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
                                   ADREG => ADREG,                                         -- Pipeline stages for pre-adder (0-1)
                                   ALUMODEREG => ALUMODEREG,                               -- Pipeline stages for ALUMODE (0-1)
                                   AREG => AREG,                                           -- Pipeline stages for A (0-2)
                                   BCASCREG => BCASCREG,                                   -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
                                   BREG => BREG,                                           -- Pipeline stages for B (0-2)
                                   CARRYINREG => CARRYINREG,                               -- Pipeline stages for CARRYIN (0-1)
                                   CARRYINSELREG => CARRYINSELREG,                         -- Pipeline stages for CARRYINSEL (0-1)
                                   CREG => CREG,                                           -- Pipeline stages for C (0-1)
                                   DREG => DREG,                                           -- Pipeline stages for D (0-1)
                                   INMODEREG => INMODEREG,                                 -- Pipeline stages for INMODE (0-1)
                                   MREG => MREG,                                           -- Multiplier pipeline stages (0-1)
                                   OPMODEREG => OPMODEREG,                                 -- Pipeline stages for OPMODE (0-1)
                                   PREG => PREG)                                           -- Number of pipeline stages for P (0-1)
                       port map(-- Cascade inputs: Cascade Ports
                                ACIN => ACIN,                                              -- 30-bit input: A cascade data
                                BCIN => BCIN,                                              -- 18-bit input: B cascade
                                CARRYCASCIN => CARRYCASCIN,                                -- 1-bit input: Cascade carry
                                MULTSIGNIN => MULTSIGNIN,                                  -- 1-bit input: Multiplier sign cascade
                                PCIN => PCIN,                                              -- 48-bit input: P cascade
                                -- Control inputs: Control Inputs/Status Bits
                                ALUMODE => ALUMODE,                                        -- 4-bit input: ALU control
                                CARRYINSEL => CARRYINSEL,                                  -- 3-bit input: Carry select
                                CLK => CLK,                                                -- 1-bit input: Clock
                                INMODE => INMODE,                                          -- 5-bit input: INMODE control
                                OPMODE => OPMODE,                                          -- 9-bit input: Operation mode
                                -- Data inputs: Data Ports
                                A => slvA,                                                 -- 30-bit input: A data
                                B => slvB,                                                 -- 18-bit input: B data
                                C => slvC,                                                 -- 48-bit input: C data
                                CARRYIN => CARRYIN,                                        -- 1-bit input: Carry-in
                                D => slvD,                                                 -- 27-bit input: D data
                                -- Reset/Clock Enable inputs: Reset/Clock Enable Inputs
                                CEA1 => CEA1,                                              -- 1-bit input: Clock enable for 1st stage AREG
                                CEA2 => CEA2,                                              -- 1-bit input: Clock enable for 2nd stage AREG
                                CEAD => CEAD,                                              -- 1-bit input: Clock enable for ADREG
                                CEALUMODE => CEALUMODE,                                    -- 1-bit input: Clock enable for ALUMODE
                                CEB1 => CEB1,                                              -- 1-bit input: Clock enable for 1st stage BREG
                                CEB2 => CEB2,                                              -- 1-bit input: Clock enable for 2nd stage BREG
                                CEC => CEC,                                                -- 1-bit input: Clock enable for CREG
                                CECARRYIN => CECARRYIN,                                    -- 1-bit input: Clock enable for CARRYINREG
                                CECTRL => CECTRL,                                          -- 1-bit input: Clock enable for OPMODEREG and CARRYINSELREG
                                CED => CED,                                                -- 1-bit input: Clock enable for DREG
                                CEINMODE => CEINMODE,                                      -- 1-bit input: Clock enable for INMODEREG
                                CEM => CEM,                                                -- 1-bit input: Clock enable for MREG
                                CEP => CEP,                                                -- 1-bit input: Clock enable for PREG
                                RSTA => RSTA,                                              -- 1-bit input: Reset for AREG
                                RSTALLCARRYIN => RSTALLCARRYIN,                            -- 1-bit input: Reset for CARRYINREG
                                RSTALUMODE => RSTALUMODE,                                  -- 1-bit input: Reset for ALUMODEREG
                                RSTB => RSTB,                                              -- 1-bit input: Reset for BREG
                                RSTC => RSTC,                                              -- 1-bit input: Reset for CREG
                                RSTCTRL => RSTCTRL,                                        -- 1-bit input: Reset for OPMODEREG and CARRYINSELREG
                                RSTD => RSTD,                                              -- 1-bit input: Reset for DREG and ADREG
                                RSTINMODE => RSTINMODE,                                    -- 1-bit input: Reset for INMODEREG
                                RSTM => RSTM,                                              -- 1-bit input: Reset for MREG
                                RSTP => RSTP,                                              -- 1-bit input: Reset for PREG
                                -- Cascade outputs: Cascade Ports
                                ACOUT => ACOUT,                                            -- 30-bit output: A port cascade
                                BCOUT => BCOUT,                                            -- 18-bit output: B cascade
                                CARRYCASCOUT => CARRYCASCOUT,                              -- 1-bit output: Cascade carry
                                MULTSIGNOUT => MULTSIGNOUT,                                -- 1-bit output: Multiplier sign cascade
                                PCOUT => PCOUT,                                            -- 48-bit output: Cascade output
                                -- Control outputs: Control Inputs/Status Bits
                                OVERFLOW => OVERFLOW,                                      -- 1-bit output: Overflow in add/acc
                                PATTERNBDETECT => PATTERNBDETECT,                          -- 1-bit output: Pattern bar detect
                                PATTERNDETECT => PATTERNDETECT,                            -- 1-bit output: Pattern detect
                                UNDERFLOW => UNDERFLOW,                                    -- 1-bit output: Underflow in add/acc
                                -- Data outputs: Data Ports
                                CARRYOUT => CARRYOUT,                                      -- 4-bit output: Carry
                                P => slvP,                                                 -- 48-bit output: Primary data
                                XOROUT => XOROUT);                                         -- 8-bit output: XOR data
          end generate;
       i3:if FAMILY=3 generate
            attribute loc:STRING;
            attribute loc of ds:label is "DSP_X"&INTEGER'image(X)&"Y"&INTEGER'image(Y);
          begin
            ds:DSP58 generic map(-- Feature Control Attributes: Data Path Selection
                                 AMULTSEL => AMULTSEL,                                   -- Selects A input to multiplier (A, AD)
                                 A_INPUT => A_INPUT,                                     -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                                 BMULTSEL => BMULTSEL,                                   -- Selects B input to multiplier (AD, B)
                                 B_INPUT => B_INPUT,                                     -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
                                 DSP_MODE => DSP_MODE,
                                 PREADDINSEL => PREADDINSEL,                             -- Selects input to preadder (A, B)
                                 RESET_MODE => RESET_MODE,
                                 RND => RND,                                             -- Rounding Constant
                                 USE_MULT => USE_MULT,                                   -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
                                 USE_SIMD => FIX_USE_SIMD(USE_SIMD),                     -- SIMD selection (FOUR12, ONE58, TWO24)
                                 USE_WIDEXOR => USE_WIDEXOR,                             -- Use the Wide XOR function (FALSE, TRUE)
                                 XORSIMD => FIX_XORSIMD(XORSIMD),                        -- Mode of operation for the Wide XOR (XOR12, XOR24_34_58_116)
                                 -- Pattern Detector Attributes: Pattern Detection Configuration
                                 AUTORESET_PATDET => AUTORESET_PATDET,                   -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
                                 AUTORESET_PRIORITY => AUTORESET_PRIORITY,               -- Priority of AUTORESET vs.CEP (CEP, RESET).
                                 MASK => MASK,                                           -- 48-bit mask value for pattern detect (1=ignore)
                                 PATTERN => PATTERN,                                     -- 48-bit pattern match for pattern detect
                                 SEL_MASK => SEL_MASK,                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
                                 SEL_PATTERN => SEL_PATTERN,                             -- Select pattern value (C, PATTERN)
                                 USE_PATTERN_DETECT => USE_PATTERN_DETECT,               -- Enable pattern detect (NO_PATDET, PATDET)
                                 -- Programmable Inversion Attributes: Specifies built-in programmable inversion on specific pins
                                 IS_ALUMODE_INVERTED => IS_ALUMODE_INVERTED,             -- Optional inversion for ALUMODE
                                 IS_ASYNC_RST_INVERTED => IS_ASYNC_RST_INVERTED,         -- Optional inversion for ASYNC_RST
                                 IS_CARRYIN_INVERTED => IS_CARRYIN_INVERTED,             -- Optional inversion for CARRYIN
                                 IS_CLK_INVERTED => IS_CLK_INVERTED,                     -- Optional inversion for CLK
                                 IS_INMODE_INVERTED => IS_INMODE_INVERTED,               -- Optional inversion for INMODE
                                 IS_NEGATE_INVERTED => IS_NEGATE_INVERTED,               -- Optional inversion for NEGATE
                                 IS_OPMODE_INVERTED => IS_OPMODE_INVERTED,               -- Optional inversion for OPMODE
                                 IS_RSTALLCARRYIN_INVERTED => IS_RSTALLCARRYIN_INVERTED, -- Optional inversion for RSTALLCARRYIN
                                 IS_RSTALUMODE_INVERTED => IS_RSTALUMODE_INVERTED,       -- Optional inversion for RSTALUMODE
                                 IS_RSTA_INVERTED => IS_RSTA_INVERTED,                   -- Optional inversion for RSTA
                                 IS_RSTB_INVERTED => IS_RSTB_INVERTED,                   -- Optional inversion for RSTB
                                 IS_RSTCTRL_INVERTED => IS_RSTCTRL_INVERTED,             -- Optional inversion for RSTCTRL
                                 IS_RSTC_INVERTED => IS_RSTC_INVERTED,                   -- Optional inversion for RSTC
                                 IS_RSTD_INVERTED => IS_RSTD_INVERTED,                   -- Optional inversion for RSTD
                                 IS_RSTINMODE_INVERTED => IS_RSTINMODE_INVERTED,         -- Optional inversion for RSTINMODE
                                 IS_RSTM_INVERTED => IS_RSTM_INVERTED,                   -- Optional inversion for RSTM
                                 IS_RSTP_INVERTED => IS_RSTP_INVERTED,                   -- Optional inversion for RSTP
                                 -- Register Control Attributes: Pipeline Register Configuration
                                 ACASCREG => ACASCREG,                                   -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
                                 ADREG => ADREG,                                         -- Pipeline stages for pre-adder (0-1)
                                 ALUMODEREG => ALUMODEREG,                               -- Pipeline stages for ALUMODE (0-1)
                                 AREG => AREG,                                           -- Pipeline stages for A (0-2)
                                 BCASCREG => BCASCREG,                                   -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
                                 BREG => BREG,                                           -- Pipeline stages for B (0-2)
                                 CARRYINREG => CARRYINREG,                               -- Pipeline stages for CARRYIN (0-1)
                                 CARRYINSELREG => CARRYINSELREG,                         -- Pipeline stages for CARRYINSEL (0-1)
                                 CREG => CREG,                                           -- Pipeline stages for C (0-1)
                                 DREG => DREG,                                           -- Pipeline stages for D (0-1)
                                 INMODEREG => INMODEREG,                                 -- Pipeline stages for INMODE (0-1)
                                 MREG => MREG,                                           -- Multiplier pipeline stages (0-1)
                                 OPMODEREG => OPMODEREG,                                 -- Pipeline stages for OPMODE (0-1)
                                 PREG => PREG)                                           -- Number of pipeline stages for P (0-1)
                     port map(A=>slvA,
                              ACIN=>ACIN,
                              ALUMODE=>ALUMODE,
                              ASYNC_RST=>ASYNC_RST,
                              B=>slvB,
                              BCIN=>BCIN,
                              C=>slvC,
                              CARRYCASCIN=>CARRYCASCIN,
                              CARRYIN=>CARRYIN,
                              CARRYINSEL=>CARRYINSEL,
                              CEA1=>CEA1,
                              CEA2=>CEA2,
                              CEAD=>CEAD,
                              CEALUMODE=>CEALUMODE,
                              CEB1=>CEB1,
                              CEB2=>CEB2,
                              CEC=>CEC,
                              CECARRYIN=>CECARRYIN,
                              CECTRL=>CECTRL,
                              CED=>CED,
                              CEINMODE=>CEINMODE,
                              CEM=>CEM,
                              CEP=>CEP,
                              CLK=>CLK,
                              D=>slvD,
                              INMODE=>INMODE,
                              MULTSIGNIN=>MULTSIGNIN,
                              NEGATE=>NEGATE,
                              OPMODE=>OPMODE,
                              PCIN=>PCIN,
                              RSTA=>RSTA,
                              RSTALLCARRYIN=>RSTALLCARRYIN,
                              RSTALUMODE=>RSTALUMODE,
                              RSTB=>RSTB,
                              RSTC=>RSTC,
                              RSTCTRL=>RSTCTRL,
                              RSTD=>RSTD,
                              RSTINMODE=>RSTINMODE,
                              RSTM=>RSTM,
                              RSTP=>RSTP,
                              ACOUT=>ACOUT,
                              BCOUT=>BCOUT,
                              CARRYCASCOUT=>CARRYCASCOUT,
                              CARRYOUT=>CARRYOUT,
                              MULTSIGNOUT=>MULTSIGNOUT,
                              OVERFLOW=>OVERFLOW,
                              P=>slvP,
                              PATTERNBDETECT=>PATTERNBDETECT,
                              PATTERNDETECT=>PATTERNDETECT,
                              PCOUT=>PCOUT,
                              UNDERFLOW=>UNDERFLOW,
                              XOROUT=>XOROUT);
          end generate;
     end;
     else generate
     begin
       i1:if FAMILY=1 generate
            ds:DSP48E1 generic map(-- Feature Control Attributes: Data Path Selection
                                   A_INPUT => A_INPUT,                                     -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                                   B_INPUT => B_INPUT,                                     -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
                                   USE_MULT => USE_MULT,                                   -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
                                   USE_SIMD => USE_SIMD,                                   -- SIMD selection (FOUR12, ONE48, TWO24)
                                   -- Pattern Detector Attributes: Pattern Detection Configuration
                                   AUTORESET_PATDET => AUTORESET_PATDET,                   -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
--                                   MASK => MASK,                                           -- 48-bit mask value for pattern detect (1=ignore)
--                                   PATTERN => PATTERN,                                     -- 48-bit pattern match for pattern detect
                                   SEL_MASK => SEL_MASK,                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
                                   SEL_PATTERN => SEL_PATTERN,                             -- Select pattern value (C, PATTERN)
                                   USE_PATTERN_DETECT => USE_PATTERN_DETECT,               -- Enable pattern detect (NO_PATDET, PATDET)
                                   -- Register Control Attributes: Pipeline Register Configuration
                                   ACASCREG => ACASCREG,                                   -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
                                   ADREG => ADREG,                                         -- Pipeline stages for pre-adder (0-1)
                                   ALUMODEREG => ALUMODEREG,                               -- Pipeline stages for ALUMODE (0-1)
                                   AREG => AREG,                                           -- Pipeline stages for A (0-2)
                                   BCASCREG => BCASCREG,                                   -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
                                   BREG => BREG,                                           -- Pipeline stages for B (0-2)
                                   CARRYINREG => CARRYINREG,                               -- Pipeline stages for CARRYIN (0-1)
                                   CARRYINSELREG => CARRYINSELREG,                         -- Pipeline stages for CARRYINSEL (0-1)
                                   CREG => CREG,                                           -- Pipeline stages for C (0-1)
                                   DREG => DREG,                                           -- Pipeline stages for D (0-1)
                                   INMODEREG => INMODEREG,                                 -- Pipeline stages for INMODE (0-1)
                                   MREG => MREG,                                           -- Multiplier pipeline stages (0-1)
                                   OPMODEREG => OPMODEREG,                                 -- Pipeline stages for OPMODE (0-1)
                                   PREG => PREG)                                           -- Number of pipeline stages for P (0-1)
                       port map(-- Cascade inputs: Cascade Ports
                                ACIN => ACIN,                                              -- 30-bit input: A cascade data
                                BCIN => BCIN,                                              -- 18-bit input: B cascade
                                CARRYCASCIN => CARRYCASCIN,                                -- 1-bit input: Cascade carry
                                MULTSIGNIN => MULTSIGNIN,                                  -- 1-bit input: Multiplier sign cascade
                                PCIN => PCIN,                                              -- 48-bit input: P cascade
                                -- Control inputs: Control Inputs/Status Bits
                                ALUMODE => ALUMODE,                                        -- 4-bit input: ALU control
                                CARRYINSEL => CARRYINSEL,                                  -- 3-bit input: Carry select
                                CLK => CLK,                                                -- 1-bit input: Clock
                                INMODE => INMODE,                                          -- 5-bit input: INMODE control
                                OPMODE => OPMODE(6 downto 0),                              -- 7-bit input: Operation mode
                                -- Data inputs: Data Ports
                                A => slvA,                                                 -- 30-bit input: A data
                                B => slvB,                                                 -- 18-bit input: B data
                                C => slvC,                                                 -- 48-bit input: C data
                                CARRYIN => CARRYIN,                                        -- 1-bit input: Carry-in
                                D => slvD(24 downto 0),                                    -- 25-bit input: D data
                                -- Reset/Clock Enable inputs: Reset/Clock Enable Inputs
                                CEA1 => CEA1,                                              -- 1-bit input: Clock enable for 1st stage AREG
                                CEA2 => CEA2,                                              -- 1-bit input: Clock enable for 2nd stage AREG
                                CEAD => CEAD,                                              -- 1-bit input: Clock enable for ADREG
                                CEALUMODE => CEALUMODE,                                    -- 1-bit input: Clock enable for ALUMODE
                                CEB1 => CEB1,                                              -- 1-bit input: Clock enable for 1st stage BREG
                                CEB2 => CEB2,                                              -- 1-bit input: Clock enable for 2nd stage BREG
                                CEC => CEC,                                                -- 1-bit input: Clock enable for CREG
                                CECARRYIN => CECARRYIN,                                    -- 1-bit input: Clock enable for CARRYINREG
                                CECTRL => CECTRL,                                          -- 1-bit input: Clock enable for OPMODEREG and CARRYINSELREG
                                CED => CED,                                                -- 1-bit input: Clock enable for DREG
                                CEINMODE => CEINMODE,                                      -- 1-bit input: Clock enable for INMODEREG
                                CEM => CEM,                                                -- 1-bit input: Clock enable for MREG
                                CEP => CEP,                                                -- 1-bit input: Clock enable for PREG
                                RSTA => RSTA,                                              -- 1-bit input: Reset for AREG
                                RSTALLCARRYIN => RSTALLCARRYIN,                            -- 1-bit input: Reset for CARRYINREG
                                RSTALUMODE => RSTALUMODE,                                  -- 1-bit input: Reset for ALUMODEREG
                                RSTB => RSTB,                                              -- 1-bit input: Reset for BREG
                                RSTC => RSTC,                                              -- 1-bit input: Reset for CREG
                                RSTCTRL => RSTCTRL,                                        -- 1-bit input: Reset for OPMODEREG and CARRYINSELREG
                                RSTD => RSTD,                                              -- 1-bit input: Reset for DREG and ADREG
                                RSTINMODE => RSTINMODE,                                    -- 1-bit input: Reset for INMODEREG
                                RSTM => RSTM,                                              -- 1-bit input: Reset for MREG
                                RSTP => RSTP,                                              -- 1-bit input: Reset for PREG
                                -- Cascade outputs: Cascade Ports
                                ACOUT => ACOUT,                                            -- 30-bit output: A port cascade
                                BCOUT => BCOUT,                                            -- 18-bit output: B cascade
                                CARRYCASCOUT => CARRYCASCOUT,                              -- 1-bit output: Cascade carry
                                MULTSIGNOUT => MULTSIGNOUT,                                -- 1-bit output: Multiplier sign cascade
                                PCOUT => PCOUT,                                            -- 48-bit output: Cascade output
                                -- Control outputs: Control Inputs/Status Bits
                                OVERFLOW => OVERFLOW,                                      -- 1-bit output: Overflow in add/acc
                                PATTERNBDETECT => PATTERNBDETECT,                          -- 1-bit output: Pattern bar detect
                                PATTERNDETECT => PATTERNDETECT,                            -- 1-bit output: Pattern detect
                                UNDERFLOW => UNDERFLOW,                                    -- 1-bit output: Underflow in add/acc
                                -- Data outputs: Data Ports
                                CARRYOUT => CARRYOUT,                                      -- 4-bit output: Carry
                                P => slvP);                                                -- 48-bit output: Primary data
          end generate;
       i2:if FAMILY=2 generate
            ds:DSP48E2 generic map(-- Feature Control Attributes: Data Path Selection
                                   AMULTSEL => AMULTSEL,                                   -- Selects A input to multiplier (A, AD)
                                   A_INPUT => A_INPUT,                                     -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                                   BMULTSEL => BMULTSEL,                                   -- Selects B input to multiplier (AD, B)
                                   B_INPUT => B_INPUT,                                     -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
                                   PREADDINSEL => PREADDINSEL,                             -- Selects input to preadder (A, B)
                                   RND => RND,                                             -- Rounding Constant
                                   USE_MULT => USE_MULT,                                   -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
                                   USE_SIMD => USE_SIMD,                                   -- SIMD selection (FOUR12, ONE48, TWO24)
                                   USE_WIDEXOR => USE_WIDEXOR,                             -- Use the Wide XOR function (FALSE, TRUE)
                                   XORSIMD => XORSIMD,                                     -- Mode of operation for the Wide XOR (XOR12, XOR24_48_96)
                                   -- Pattern Detector Attributes: Pattern Detection Configuration
                                   AUTORESET_PATDET => AUTORESET_PATDET,                   -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
                                   AUTORESET_PRIORITY => AUTORESET_PRIORITY,               -- Priority of AUTORESET vs.CEP (CEP, RESET).
                                   MASK => MASK,                                           -- 48-bit mask value for pattern detect (1=ignore)
                                   PATTERN => PATTERN,                                     -- 48-bit pattern match for pattern detect
                                   SEL_MASK => SEL_MASK,                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
                                   SEL_PATTERN => SEL_PATTERN,                             -- Select pattern value (C, PATTERN)
                                   USE_PATTERN_DETECT => USE_PATTERN_DETECT,               -- Enable pattern detect (NO_PATDET, PATDET)
                                   -- Programmable Inversion Attributes: Specifies built-in programmable inversion on specific pins
                                   IS_ALUMODE_INVERTED => IS_ALUMODE_INVERTED,             -- Optional inversion for ALUMODE
                                   IS_CARRYIN_INVERTED => IS_CARRYIN_INVERTED,             -- Optional inversion for CARRYIN
                                   IS_CLK_INVERTED => IS_CLK_INVERTED,                     -- Optional inversion for CLK
                                   IS_INMODE_INVERTED => IS_INMODE_INVERTED,               -- Optional inversion for INMODE
                                   IS_OPMODE_INVERTED => IS_OPMODE_INVERTED,               -- Optional inversion for OPMODE
                                   IS_RSTALLCARRYIN_INVERTED => IS_RSTALLCARRYIN_INVERTED, -- Optional inversion for RSTALLCARRYIN
                                   IS_RSTALUMODE_INVERTED => IS_RSTALUMODE_INVERTED,       -- Optional inversion for RSTALUMODE
                                   IS_RSTA_INVERTED => IS_RSTA_INVERTED,                   -- Optional inversion for RSTA
                                   IS_RSTB_INVERTED => IS_RSTB_INVERTED,                   -- Optional inversion for RSTB
                                   IS_RSTCTRL_INVERTED => IS_RSTCTRL_INVERTED,             -- Optional inversion for RSTCTRL
                                   IS_RSTC_INVERTED => IS_RSTC_INVERTED,                   -- Optional inversion for RSTC
                                   IS_RSTD_INVERTED => IS_RSTD_INVERTED,                   -- Optional inversion for RSTD
                                   IS_RSTINMODE_INVERTED => IS_RSTINMODE_INVERTED,         -- Optional inversion for RSTINMODE
                                   IS_RSTM_INVERTED => IS_RSTM_INVERTED,                   -- Optional inversion for RSTM
                                   IS_RSTP_INVERTED => IS_RSTP_INVERTED,                   -- Optional inversion for RSTP
                                   -- Register Control Attributes: Pipeline Register Configuration
                                   ACASCREG => ACASCREG,                                   -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
                                   ADREG => ADREG,                                         -- Pipeline stages for pre-adder (0-1)
                                   ALUMODEREG => ALUMODEREG,                               -- Pipeline stages for ALUMODE (0-1)
                                   AREG => AREG,                                           -- Pipeline stages for A (0-2)
                                   BCASCREG => BCASCREG,                                   -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
                                   BREG => BREG,                                           -- Pipeline stages for B (0-2)
                                   CARRYINREG => CARRYINREG,                               -- Pipeline stages for CARRYIN (0-1)
                                   CARRYINSELREG => CARRYINSELREG,                         -- Pipeline stages for CARRYINSEL (0-1)
                                   CREG => CREG,                                           -- Pipeline stages for C (0-1)
                                   DREG => DREG,                                           -- Pipeline stages for D (0-1)
                                   INMODEREG => INMODEREG,                                 -- Pipeline stages for INMODE (0-1)
                                   MREG => MREG,                                           -- Multiplier pipeline stages (0-1)
                                   OPMODEREG => OPMODEREG,                                 -- Pipeline stages for OPMODE (0-1)
                                   PREG => PREG)                                           -- Number of pipeline stages for P (0-1)
                       port map(-- Cascade inputs: Cascade Ports
                                ACIN => ACIN,                                              -- 30-bit input: A cascade data
                                BCIN => BCIN,                                              -- 18-bit input: B cascade
                                CARRYCASCIN => CARRYCASCIN,                                -- 1-bit input: Cascade carry
                                MULTSIGNIN => MULTSIGNIN,                                  -- 1-bit input: Multiplier sign cascade
                                PCIN => PCIN,                                              -- 48-bit input: P cascade
                                -- Control inputs: Control Inputs/Status Bits
                                ALUMODE => ALUMODE,                                        -- 4-bit input: ALU control
                                CARRYINSEL => CARRYINSEL,                                  -- 3-bit input: Carry select
                                CLK => CLK,                                                -- 1-bit input: Clock
                                INMODE => INMODE,                                          -- 5-bit input: INMODE control
                                OPMODE => OPMODE,                                          -- 9-bit input: Operation mode
                                -- Data inputs: Data Ports
                                A => slvA,                                                 -- 30-bit input: A data
                                B => slvB,                                                 -- 18-bit input: B data
                                C => slvC,                                                 -- 48-bit input: C data
                                CARRYIN => CARRYIN,                                        -- 1-bit input: Carry-in
                                D => slvD,                                                 -- 27-bit input: D data
                                -- Reset/Clock Enable inputs: Reset/Clock Enable Inputs
                                CEA1 => CEA1,                                              -- 1-bit input: Clock enable for 1st stage AREG
                                CEA2 => CEA2,                                              -- 1-bit input: Clock enable for 2nd stage AREG
                                CEAD => CEAD,                                              -- 1-bit input: Clock enable for ADREG
                                CEALUMODE => CEALUMODE,                                    -- 1-bit input: Clock enable for ALUMODE
                                CEB1 => CEB1,                                              -- 1-bit input: Clock enable for 1st stage BREG
                                CEB2 => CEB2,                                              -- 1-bit input: Clock enable for 2nd stage BREG
                                CEC => CEC,                                                -- 1-bit input: Clock enable for CREG
                                CECARRYIN => CECARRYIN,                                    -- 1-bit input: Clock enable for CARRYINREG
                                CECTRL => CECTRL,                                          -- 1-bit input: Clock enable for OPMODEREG and CARRYINSELREG
                                CED => CED,                                                -- 1-bit input: Clock enable for DREG
                                CEINMODE => CEINMODE,                                      -- 1-bit input: Clock enable for INMODEREG
                                CEM => CEM,                                                -- 1-bit input: Clock enable for MREG
                                CEP => CEP,                                                -- 1-bit input: Clock enable for PREG
                                RSTA => RSTA,                                              -- 1-bit input: Reset for AREG
                                RSTALLCARRYIN => RSTALLCARRYIN,                            -- 1-bit input: Reset for CARRYINREG
                                RSTALUMODE => RSTALUMODE,                                  -- 1-bit input: Reset for ALUMODEREG
                                RSTB => RSTB,                                              -- 1-bit input: Reset for BREG
                                RSTC => RSTC,                                              -- 1-bit input: Reset for CREG
                                RSTCTRL => RSTCTRL,                                        -- 1-bit input: Reset for OPMODEREG and CARRYINSELREG
                                RSTD => RSTD,                                              -- 1-bit input: Reset for DREG and ADREG
                                RSTINMODE => RSTINMODE,                                    -- 1-bit input: Reset for INMODEREG
                                RSTM => RSTM,                                              -- 1-bit input: Reset for MREG
                                RSTP => RSTP,                                              -- 1-bit input: Reset for PREG
                                -- Cascade outputs: Cascade Ports
                                ACOUT => ACOUT,                                            -- 30-bit output: A port cascade
                                BCOUT => BCOUT,                                            -- 18-bit output: B cascade
                                CARRYCASCOUT => CARRYCASCOUT,                              -- 1-bit output: Cascade carry
                                MULTSIGNOUT => MULTSIGNOUT,                                -- 1-bit output: Multiplier sign cascade
                                PCOUT => PCOUT,                                            -- 48-bit output: Cascade output
                                -- Control outputs: Control Inputs/Status Bits
                                OVERFLOW => OVERFLOW,                                      -- 1-bit output: Overflow in add/acc
                                PATTERNBDETECT => PATTERNBDETECT,                          -- 1-bit output: Pattern bar detect
                                PATTERNDETECT => PATTERNDETECT,                            -- 1-bit output: Pattern detect
                                UNDERFLOW => UNDERFLOW,                                    -- 1-bit output: Underflow in add/acc
                                -- Data outputs: Data Ports
                                CARRYOUT => CARRYOUT,                                      -- 4-bit output: Carry
                                P => slvP,                                                 -- 48-bit output: Primary data
                                XOROUT => XOROUT);                                         -- 8-bit output: XOR data
          end generate;
       i3:if FAMILY=3 generate
            ds:DSP58 generic map(-- Feature Control Attributes: Data Path Selection
                                 AMULTSEL => AMULTSEL,                                   -- Selects A input to multiplier (A, AD)
                                 A_INPUT => A_INPUT,                                     -- Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
                                 BMULTSEL => BMULTSEL,                                   -- Selects B input to multiplier (AD, B)
                                 B_INPUT => B_INPUT,                                     -- Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
                                 DSP_MODE => DSP_MODE,
                                 PREADDINSEL => PREADDINSEL,                             -- Selects input to preadder (A, B)
                                 RESET_MODE => RESET_MODE,
                                 RND => RND,                                             -- Rounding Constant
                                 USE_MULT => USE_MULT,                                   -- Select multiplier usage (DYNAMIC, MULTIPLY, NONE)
                                 USE_SIMD => FIX_USE_SIMD(USE_SIMD),                     -- SIMD selection (FOUR12, ONE58, TWO24)
                                 USE_WIDEXOR => USE_WIDEXOR,                             -- Use the Wide XOR function (FALSE, TRUE)
                                 XORSIMD => FIX_XORSIMD(XORSIMD),                        -- Mode of operation for the Wide XOR (XOR12, XOR24_34_58_116)
                                 -- Pattern Detector Attributes: Pattern Detection Configuration
                                 AUTORESET_PATDET => AUTORESET_PATDET,                   -- NO_RESET, RESET_MATCH, RESET_NOT_MATCH
                                 AUTORESET_PRIORITY => AUTORESET_PRIORITY,               -- Priority of AUTORESET vs.CEP (CEP, RESET).
                                 MASK => MASK,                                           -- 48-bit mask value for pattern detect (1=ignore)
                                 PATTERN => PATTERN,                                     -- 48-bit pattern match for pattern detect
                                 SEL_MASK => SEL_MASK,                                   -- C, MASK, ROUNDING_MODE1, ROUNDING_MODE2
                                 SEL_PATTERN => SEL_PATTERN,                             -- Select pattern value (C, PATTERN)
                                 USE_PATTERN_DETECT => USE_PATTERN_DETECT,               -- Enable pattern detect (NO_PATDET, PATDET)
                                 -- Programmable Inversion Attributes: Specifies built-in programmable inversion on specific pins
                                 IS_ALUMODE_INVERTED => IS_ALUMODE_INVERTED,             -- Optional inversion for ALUMODE
                                 IS_ASYNC_RST_INVERTED => IS_ASYNC_RST_INVERTED,         -- Optional inversion for ASYNC_RST
                                 IS_CARRYIN_INVERTED => IS_CARRYIN_INVERTED,             -- Optional inversion for CARRYIN
                                 IS_CLK_INVERTED => IS_CLK_INVERTED,                     -- Optional inversion for CLK
                                 IS_INMODE_INVERTED => IS_INMODE_INVERTED,               -- Optional inversion for INMODE
                                 IS_NEGATE_INVERTED => IS_NEGATE_INVERTED,               -- Optional inversion for NEGATE
                                 IS_OPMODE_INVERTED => IS_OPMODE_INVERTED,               -- Optional inversion for OPMODE
                                 IS_RSTALLCARRYIN_INVERTED => IS_RSTALLCARRYIN_INVERTED, -- Optional inversion for RSTALLCARRYIN
                                 IS_RSTALUMODE_INVERTED => IS_RSTALUMODE_INVERTED,       -- Optional inversion for RSTALUMODE
                                 IS_RSTA_INVERTED => IS_RSTA_INVERTED,                   -- Optional inversion for RSTA
                                 IS_RSTB_INVERTED => IS_RSTB_INVERTED,                   -- Optional inversion for RSTB
                                 IS_RSTCTRL_INVERTED => IS_RSTCTRL_INVERTED,             -- Optional inversion for RSTCTRL
                                 IS_RSTC_INVERTED => IS_RSTC_INVERTED,                   -- Optional inversion for RSTC
                                 IS_RSTD_INVERTED => IS_RSTD_INVERTED,                   -- Optional inversion for RSTD
                                 IS_RSTINMODE_INVERTED => IS_RSTINMODE_INVERTED,         -- Optional inversion for RSTINMODE
                                 IS_RSTM_INVERTED => IS_RSTM_INVERTED,                   -- Optional inversion for RSTM
                                 IS_RSTP_INVERTED => IS_RSTP_INVERTED,                   -- Optional inversion for RSTP
                                 -- Register Control Attributes: Pipeline Register Configuration
                                 ACASCREG => ACASCREG,                                   -- Number of pipeline stages between A/ACIN and ACOUT (0-2)
                                 ADREG => ADREG,                                         -- Pipeline stages for pre-adder (0-1)
                                 ALUMODEREG => ALUMODEREG,                               -- Pipeline stages for ALUMODE (0-1)
                                 AREG => AREG,                                           -- Pipeline stages for A (0-2)
                                 BCASCREG => BCASCREG,                                   -- Number of pipeline stages between B/BCIN and BCOUT (0-2)
                                 BREG => BREG,                                           -- Pipeline stages for B (0-2)
                                 CARRYINREG => CARRYINREG,                               -- Pipeline stages for CARRYIN (0-1)
                                 CARRYINSELREG => CARRYINSELREG,                         -- Pipeline stages for CARRYINSEL (0-1)
                                 CREG => CREG,                                           -- Pipeline stages for C (0-1)
                                 DREG => DREG,                                           -- Pipeline stages for D (0-1)
                                 INMODEREG => INMODEREG,                                 -- Pipeline stages for INMODE (0-1)
                                 MREG => MREG,                                           -- Multiplier pipeline stages (0-1)
                                 OPMODEREG => OPMODEREG,                                 -- Pipeline stages for OPMODE (0-1)
                                 PREG => PREG)                                           -- Number of pipeline stages for P (0-1)
                     port map(A=>slvA,
                              ACIN=>ACIN,
                              ALUMODE=>ALUMODE,
                              ASYNC_RST=>ASYNC_RST,
                              B=>slvB,
                              BCIN=>BCIN,
                              C=>slvC,
                              CARRYCASCIN=>CARRYCASCIN,
                              CARRYIN=>CARRYIN,
                              CARRYINSEL=>CARRYINSEL,
                              CEA1=>CEA1,
                              CEA2=>CEA2,
                              CEAD=>CEAD,
                              CEALUMODE=>CEALUMODE,
                              CEB1=>CEB1,
                              CEB2=>CEB2,
                              CEC=>CEC,
                              CECARRYIN=>CECARRYIN,
                              CECTRL=>CECTRL,
                              CED=>CED,
                              CEINMODE=>CEINMODE,
                              CEM=>CEM,
                              CEP=>CEP,
                              CLK=>CLK,
                              D=>slvD,
                              INMODE=>INMODE,
                              MULTSIGNIN=>MULTSIGNIN,
                              NEGATE=>NEGATE,
                              OPMODE=>OPMODE,
                              PCIN=>PCIN,
                              RSTA=>RSTA,
                              RSTALLCARRYIN=>RSTALLCARRYIN,
                              RSTALUMODE=>RSTALUMODE,
                              RSTB=>RSTB,
                              RSTC=>RSTC,
                              RSTCTRL=>RSTCTRL,
                              RSTD=>RSTD,
                              RSTINMODE=>RSTINMODE,
                              RSTM=>RSTM,
                              RSTP=>RSTP,
                              ACOUT=>ACOUT,
                              BCOUT=>BCOUT,
                              CARRYCASCOUT=>CARRYCASCOUT,
                              CARRYOUT=>CARRYOUT,
                              MULTSIGNOUT=>MULTSIGNOUT,
                              OVERFLOW=>OVERFLOW,
                              P=>slvP,
                              PATTERNBDETECT=>PATTERNBDETECT,
                              PATTERNDETECT=>PATTERNDETECT,
                              PCOUT=>PCOUT,
                              UNDERFLOW=>UNDERFLOW,
                              XOROUT=>XOROUT);
          end generate;
     end;
     end generate;
  P<=SLV_TO_SFIXED_RESIZE(slvP,P'high,P'low,A'low+B'low-P'low);
end WRAPPER;