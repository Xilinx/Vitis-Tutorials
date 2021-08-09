//
// Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//  This uses Inverse S-box functions to form InvSubBytes() transformation block.
//  This block operates on column data.
//                     S0_in
//                     S1_in
//                     S2_in
//                     S3_in

`timescale 1ns/1ps

module AesInvSubBytes (
        S0_in,
        S1_in,
        S2_in,
        S3_in,
        
        S0_out,
        S1_out,
        S2_out,
        S3_out
  );

  input  [7:0] S0_in;
  input  [7:0] S1_in;
  input  [7:0] S2_in;
  input  [7:0] S3_in;
                 
  output [7:0] S0_out;
  output [7:0] S1_out;
  output [7:0] S2_out;
  output [7:0] S3_out;
                 
  wire   [7:0] S0_in;
  wire   [7:0] S1_in;
  wire   [7:0] S2_in;
  wire   [7:0] S3_in;
                 
  wire   [7:0] S0_out;
  wire   [7:0] S1_out;
  wire   [7:0] S2_out;
  wire   [7:0] S3_out;  

// --=========================================================================--
// Define S-box function
// --=========================================================================--  
  function [7:0] inv_s_box;
    input [7:0] in;
    case (in)
      8'd0   : inv_s_box = 8'd82;
      8'd1   : inv_s_box = 8'd9;
      8'd2   : inv_s_box = 8'd106;
      8'd3   : inv_s_box = 8'd213;
      8'd4   : inv_s_box = 8'd48;
      8'd5   : inv_s_box = 8'd54;
      8'd6   : inv_s_box = 8'd165;
      8'd7   : inv_s_box = 8'd56;
      8'd8   : inv_s_box = 8'd191;
      8'd9   : inv_s_box = 8'd64;
      8'd10  : inv_s_box = 8'd163;
      8'd11  : inv_s_box = 8'd158;
      8'd12  : inv_s_box = 8'd129;
      8'd13  : inv_s_box = 8'd243;
      8'd14  : inv_s_box = 8'd215;
      8'd15  : inv_s_box = 8'd251;
      8'd16  : inv_s_box = 8'd124;
      8'd17  : inv_s_box = 8'd227;
      8'd18  : inv_s_box = 8'd57;
      8'd19  : inv_s_box = 8'd130;
      8'd20  : inv_s_box = 8'd155;
      8'd21  : inv_s_box = 8'd47;
      8'd22  : inv_s_box = 8'd255;
      8'd23  : inv_s_box = 8'd135;
      8'd24  : inv_s_box = 8'd52;
      8'd25  : inv_s_box = 8'd142;
      8'd26  : inv_s_box = 8'd67;
      8'd27  : inv_s_box = 8'd68;
      8'd28  : inv_s_box = 8'd196;
      8'd29  : inv_s_box = 8'd222;
      8'd30  : inv_s_box = 8'd233;
      8'd31  : inv_s_box = 8'd203;
      8'd32  : inv_s_box = 8'd84;
      8'd33  : inv_s_box = 8'd123;
      8'd34  : inv_s_box = 8'd148;
      8'd35  : inv_s_box = 8'd50;
      8'd36  : inv_s_box = 8'd166;
      8'd37  : inv_s_box = 8'd194;
      8'd38  : inv_s_box = 8'd35;
      8'd39  : inv_s_box = 8'd61;
      8'd40  : inv_s_box = 8'd238;
      8'd41  : inv_s_box = 8'd76;
      8'd42  : inv_s_box = 8'd149;
      8'd43  : inv_s_box = 8'd11;
      8'd44  : inv_s_box = 8'd66;
      8'd45  : inv_s_box = 8'd250;
      8'd46  : inv_s_box = 8'd195;
      8'd47  : inv_s_box = 8'd78;
      8'd48  : inv_s_box = 8'd8;
      8'd49  : inv_s_box = 8'd46;
      8'd50  : inv_s_box = 8'd161;
      8'd51  : inv_s_box = 8'd102;
      8'd52  : inv_s_box = 8'd40;
      8'd53  : inv_s_box = 8'd217;
      8'd54  : inv_s_box = 8'd36;
      8'd55  : inv_s_box = 8'd178;
      8'd56  : inv_s_box = 8'd118;
      8'd57  : inv_s_box = 8'd91;
      8'd58  : inv_s_box = 8'd162;
      8'd59  : inv_s_box = 8'd73;
      8'd60  : inv_s_box = 8'd109;
      8'd61  : inv_s_box = 8'd139;
      8'd62  : inv_s_box = 8'd209;
      8'd63  : inv_s_box = 8'd37;
      8'd64  : inv_s_box = 8'd114;
      8'd65  : inv_s_box = 8'd248;
      8'd66  : inv_s_box = 8'd246;
      8'd67  : inv_s_box = 8'd100;
      8'd68  : inv_s_box = 8'd134;
      8'd69  : inv_s_box = 8'd104;
      8'd70  : inv_s_box = 8'd152;
      8'd71  : inv_s_box = 8'd22;
      8'd72  : inv_s_box = 8'd212;
      8'd73  : inv_s_box = 8'd164;
      8'd74  : inv_s_box = 8'd92;
      8'd75  : inv_s_box = 8'd204;
      8'd76  : inv_s_box = 8'd93;
      8'd77  : inv_s_box = 8'd101;
      8'd78  : inv_s_box = 8'd182;
      8'd79  : inv_s_box = 8'd146;
      8'd80  : inv_s_box = 8'd108;
      8'd81  : inv_s_box = 8'd112;
      8'd82  : inv_s_box = 8'd72;
      8'd83  : inv_s_box = 8'd80;
      8'd84  : inv_s_box = 8'd253;
      8'd85  : inv_s_box = 8'd237;
      8'd86  : inv_s_box = 8'd185;
      8'd87  : inv_s_box = 8'd218;
      8'd88  : inv_s_box = 8'd94;
      8'd89  : inv_s_box = 8'd21;
      8'd90  : inv_s_box = 8'd70;
      8'd91  : inv_s_box = 8'd87;
      8'd92  : inv_s_box = 8'd167;
      8'd93  : inv_s_box = 8'd141;
      8'd94  : inv_s_box = 8'd157;
      8'd95  : inv_s_box = 8'd132;
      8'd96  : inv_s_box = 8'd144;
      8'd97  : inv_s_box = 8'd216;
      8'd98  : inv_s_box = 8'd171;
      8'd99  : inv_s_box = 8'd0;
      8'd100 : inv_s_box = 8'd140;
      8'd101 : inv_s_box = 8'd188;
      8'd102 : inv_s_box = 8'd211;
      8'd103 : inv_s_box = 8'd10;
      8'd104 : inv_s_box = 8'd247;
      8'd105 : inv_s_box = 8'd228;
      8'd106 : inv_s_box = 8'd88;
      8'd107 : inv_s_box = 8'd5;
      8'd108 : inv_s_box = 8'd184;
      8'd109 : inv_s_box = 8'd179;
      8'd110 : inv_s_box = 8'd69;
      8'd111 : inv_s_box = 8'd6;
      8'd112 : inv_s_box = 8'd208;
      8'd113 : inv_s_box = 8'd44;
      8'd114 : inv_s_box = 8'd30;
      8'd115 : inv_s_box = 8'd143;
      8'd116 : inv_s_box = 8'd202;
      8'd117 : inv_s_box = 8'd63;
      8'd118 : inv_s_box = 8'd15;
      8'd119 : inv_s_box = 8'd2;
      8'd120 : inv_s_box = 8'd193;
      8'd121 : inv_s_box = 8'd175;
      8'd122 : inv_s_box = 8'd189;
      8'd123 : inv_s_box = 8'd3;
      8'd124 : inv_s_box = 8'd1;
      8'd125 : inv_s_box = 8'd19;
      8'd126 : inv_s_box = 8'd138;
      8'd127 : inv_s_box = 8'd107;
      8'd128 : inv_s_box = 8'd58;
      8'd129 : inv_s_box = 8'd145;
      8'd130 : inv_s_box = 8'd17;
      8'd131 : inv_s_box = 8'd65;
      8'd132 : inv_s_box = 8'd79;
      8'd133 : inv_s_box = 8'd103;
      8'd134 : inv_s_box = 8'd220;
      8'd135 : inv_s_box = 8'd234;
      8'd136 : inv_s_box = 8'd151;
      8'd137 : inv_s_box = 8'd242;
      8'd138 : inv_s_box = 8'd207;
      8'd139 : inv_s_box = 8'd206;
      8'd140 : inv_s_box = 8'd240;
      8'd141 : inv_s_box = 8'd180;
      8'd142 : inv_s_box = 8'd230;
      8'd143 : inv_s_box = 8'd115;
      8'd144 : inv_s_box = 8'd150;
      8'd145 : inv_s_box = 8'd172;
      8'd146 : inv_s_box = 8'd116;
      8'd147 : inv_s_box = 8'd34;
      8'd148 : inv_s_box = 8'd231;
      8'd149 : inv_s_box = 8'd173;
      8'd150 : inv_s_box = 8'd53;
      8'd151 : inv_s_box = 8'd133;
      8'd152 : inv_s_box = 8'd226;
      8'd153 : inv_s_box = 8'd249;
      8'd154 : inv_s_box = 8'd55;
      8'd155 : inv_s_box = 8'd232;
      8'd156 : inv_s_box = 8'd28;
      8'd157 : inv_s_box = 8'd117;
      8'd158 : inv_s_box = 8'd223;
      8'd159 : inv_s_box = 8'd110;
      8'd160 : inv_s_box = 8'd71;
      8'd161 : inv_s_box = 8'd241;
      8'd162 : inv_s_box = 8'd26;
      8'd163 : inv_s_box = 8'd113;
      8'd164 : inv_s_box = 8'd29;
      8'd165 : inv_s_box = 8'd41;
      8'd166 : inv_s_box = 8'd197;
      8'd167 : inv_s_box = 8'd137;
      8'd168 : inv_s_box = 8'd111;
      8'd169 : inv_s_box = 8'd183;
      8'd170 : inv_s_box = 8'd98;
      8'd171 : inv_s_box = 8'd14;
      8'd172 : inv_s_box = 8'd170;
      8'd173 : inv_s_box = 8'd24;
      8'd174 : inv_s_box = 8'd190;
      8'd175 : inv_s_box = 8'd27;
      8'd176 : inv_s_box = 8'd252;
      8'd177 : inv_s_box = 8'd86;
      8'd178 : inv_s_box = 8'd62;
      8'd179 : inv_s_box = 8'd75;
      8'd180 : inv_s_box = 8'd198;
      8'd181 : inv_s_box = 8'd210;
      8'd182 : inv_s_box = 8'd121;
      8'd183 : inv_s_box = 8'd32;
      8'd184 : inv_s_box = 8'd154;
      8'd185 : inv_s_box = 8'd219;
      8'd186 : inv_s_box = 8'd192;
      8'd187 : inv_s_box = 8'd254;
      8'd188 : inv_s_box = 8'd120;
      8'd189 : inv_s_box = 8'd205;
      8'd190 : inv_s_box = 8'd90;
      8'd191 : inv_s_box = 8'd244;
      8'd192 : inv_s_box = 8'd31;
      8'd193 : inv_s_box = 8'd221;
      8'd194 : inv_s_box = 8'd168;
      8'd195 : inv_s_box = 8'd51;
      8'd196 : inv_s_box = 8'd136;
      8'd197 : inv_s_box = 8'd7;
      8'd198 : inv_s_box = 8'd199;
      8'd199 : inv_s_box = 8'd49;
      8'd200 : inv_s_box = 8'd177;
      8'd201 : inv_s_box = 8'd18;
      8'd202 : inv_s_box = 8'd16;
      8'd203 : inv_s_box = 8'd89;
      8'd204 : inv_s_box = 8'd39;
      8'd205 : inv_s_box = 8'd128;
      8'd206 : inv_s_box = 8'd236;
      8'd207 : inv_s_box = 8'd95;
      8'd208 : inv_s_box = 8'd96;
      8'd209 : inv_s_box = 8'd81;
      8'd210 : inv_s_box = 8'd127;
      8'd211 : inv_s_box = 8'd169;
      8'd212 : inv_s_box = 8'd25;
      8'd213 : inv_s_box = 8'd181;
      8'd214 : inv_s_box = 8'd74;
      8'd215 : inv_s_box = 8'd13;
      8'd216 : inv_s_box = 8'd45;
      8'd217 : inv_s_box = 8'd229;
      8'd218 : inv_s_box = 8'd122;
      8'd219 : inv_s_box = 8'd159;
      8'd220 : inv_s_box = 8'd147;
      8'd221 : inv_s_box = 8'd201;
      8'd222 : inv_s_box = 8'd156;
      8'd223 : inv_s_box = 8'd239;
      8'd224 : inv_s_box = 8'd160;
      8'd225 : inv_s_box = 8'd224;
      8'd226 : inv_s_box = 8'd59;
      8'd227 : inv_s_box = 8'd77;
      8'd228 : inv_s_box = 8'd174;
      8'd229 : inv_s_box = 8'd42;
      8'd230 : inv_s_box = 8'd245;
      8'd231 : inv_s_box = 8'd176;
      8'd232 : inv_s_box = 8'd200;
      8'd233 : inv_s_box = 8'd235;
      8'd234 : inv_s_box = 8'd187;
      8'd235 : inv_s_box = 8'd60;
      8'd236 : inv_s_box = 8'd131;
      8'd237 : inv_s_box = 8'd83;
      8'd238 : inv_s_box = 8'd153;
      8'd239 : inv_s_box = 8'd97;
      8'd240 : inv_s_box = 8'd23;
      8'd241 : inv_s_box = 8'd43;
      8'd242 : inv_s_box = 8'd4;
      8'd243 : inv_s_box = 8'd126;
      8'd244 : inv_s_box = 8'd186;
      8'd245 : inv_s_box = 8'd119;
      8'd246 : inv_s_box = 8'd214;
      8'd247 : inv_s_box = 8'd38;
      8'd248 : inv_s_box = 8'd225;
      8'd249 : inv_s_box = 8'd105;
      8'd250 : inv_s_box = 8'd20;
      8'd251 : inv_s_box = 8'd99;
      8'd252 : inv_s_box = 8'd85;
      8'd253 : inv_s_box = 8'd33;
      8'd254 : inv_s_box = 8'd12;
      8'd255 : inv_s_box = 8'd125;
    endcase
  endfunction

  assign S0_out = inv_s_box (S0_in);
  assign S1_out = inv_s_box (S1_in);
  assign S2_out = inv_s_box (S2_in);
  assign S3_out = inv_s_box (S3_in);  
  
endmodule