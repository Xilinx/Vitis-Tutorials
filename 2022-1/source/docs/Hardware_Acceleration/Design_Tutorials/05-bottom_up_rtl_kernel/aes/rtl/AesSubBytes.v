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
//  This uses four S-box functions to form SubBytes() Transformation block.
//  This block operates on column data.
//                     S0_in
//                     S1_in
//                     S2_in
//                     S3_in

`timescale 1ns/1ps

module AesSubBytes (
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
  function [7:0] s_box;
    input [7:0] in;
    case (in)
      8'd0   : s_box = 8'd99;
      8'd1   : s_box = 8'd124;
      8'd2   : s_box = 8'd119;
      8'd3   : s_box = 8'd123;
      8'd4   : s_box = 8'd242;
      8'd5   : s_box = 8'd107;
      8'd6   : s_box = 8'd111;
      8'd7   : s_box = 8'd197;
      8'd8   : s_box = 8'd48;
      8'd9   : s_box = 8'd1;
      8'd10  : s_box = 8'd103;
      8'd11  : s_box = 8'd43;
      8'd12  : s_box = 8'd254;
      8'd13  : s_box = 8'd215;
      8'd14  : s_box = 8'd171;
      8'd15  : s_box = 8'd118;
      8'd16  : s_box = 8'd202;
      8'd17  : s_box = 8'd130;
      8'd18  : s_box = 8'd201;
      8'd19  : s_box = 8'd125;
      8'd20  : s_box = 8'd250;
      8'd21  : s_box = 8'd89;
      8'd22  : s_box = 8'd71;
      8'd23  : s_box = 8'd240;
      8'd24  : s_box = 8'd173;
      8'd25  : s_box = 8'd212;
      8'd26  : s_box = 8'd162;
      8'd27  : s_box = 8'd175;
      8'd28  : s_box = 8'd156;
      8'd29  : s_box = 8'd164;
      8'd30  : s_box = 8'd114;
      8'd31  : s_box = 8'd192;
      8'd32  : s_box = 8'd183;
      8'd33  : s_box = 8'd253;
      8'd34  : s_box = 8'd147;
      8'd35  : s_box = 8'd38;
      8'd36  : s_box = 8'd54;
      8'd37  : s_box = 8'd63;
      8'd38  : s_box = 8'd247;
      8'd39  : s_box = 8'd204;
      8'd40  : s_box = 8'd52;
      8'd41  : s_box = 8'd165;
      8'd42  : s_box = 8'd229;
      8'd43  : s_box = 8'd241;
      8'd44  : s_box = 8'd113;
      8'd45  : s_box = 8'd216;
      8'd46  : s_box = 8'd49;
      8'd47  : s_box = 8'd21;
      8'd48  : s_box = 8'd4;
      8'd49  : s_box = 8'd199;
      8'd50  : s_box = 8'd35;
      8'd51  : s_box = 8'd195;
      8'd52  : s_box = 8'd24;
      8'd53  : s_box = 8'd150;
      8'd54  : s_box = 8'd5;
      8'd55  : s_box = 8'd154;
      8'd56  : s_box = 8'd7;
      8'd57  : s_box = 8'd18;
      8'd58  : s_box = 8'd128;
      8'd59  : s_box = 8'd226;
      8'd60  : s_box = 8'd235;
      8'd61  : s_box = 8'd39;
      8'd62  : s_box = 8'd178;
      8'd63  : s_box = 8'd117;
      8'd64  : s_box = 8'd9;
      8'd65  : s_box = 8'd131;
      8'd66  : s_box = 8'd44;
      8'd67  : s_box = 8'd26;
      8'd68  : s_box = 8'd27;
      8'd69  : s_box = 8'd110;
      8'd70  : s_box = 8'd90;
      8'd71  : s_box = 8'd160;
      8'd72  : s_box = 8'd82;
      8'd73  : s_box = 8'd59;
      8'd74  : s_box = 8'd214;
      8'd75  : s_box = 8'd179;
      8'd76  : s_box = 8'd41;
      8'd77  : s_box = 8'd227;
      8'd78  : s_box = 8'd47;
      8'd79  : s_box = 8'd132;
      8'd80  : s_box = 8'd83;
      8'd81  : s_box = 8'd209;
      8'd82  : s_box = 8'd0;
      8'd83  : s_box = 8'd237;
      8'd84  : s_box = 8'd32;
      8'd85  : s_box = 8'd252;
      8'd86  : s_box = 8'd177;
      8'd87  : s_box = 8'd91;
      8'd88  : s_box = 8'd106;
      8'd89  : s_box = 8'd203;
      8'd90  : s_box = 8'd190;
      8'd91  : s_box = 8'd57;
      8'd92  : s_box = 8'd74;
      8'd93  : s_box = 8'd76;
      8'd94  : s_box = 8'd88;
      8'd95  : s_box = 8'd207;
      8'd96  : s_box = 8'd208;
      8'd97  : s_box = 8'd239;
      8'd98  : s_box = 8'd170;
      8'd99  : s_box = 8'd251;
      8'd100 : s_box = 8'd67;
      8'd101 : s_box = 8'd77;
      8'd102 : s_box = 8'd51;
      8'd103 : s_box = 8'd133;
      8'd104 : s_box = 8'd69;
      8'd105 : s_box = 8'd249;
      8'd106 : s_box = 8'd2;
      8'd107 : s_box = 8'd127;
      8'd108 : s_box = 8'd80;
      8'd109 : s_box = 8'd60;
      8'd110 : s_box = 8'd159;
      8'd111 : s_box = 8'd168;
      8'd112 : s_box = 8'd81;
      8'd113 : s_box = 8'd163;
      8'd114 : s_box = 8'd64;
      8'd115 : s_box = 8'd143;
      8'd116 : s_box = 8'd146;
      8'd117 : s_box = 8'd157;
      8'd118 : s_box = 8'd56;
      8'd119 : s_box = 8'd245;
      8'd120 : s_box = 8'd188;
      8'd121 : s_box = 8'd182;
      8'd122 : s_box = 8'd218;
      8'd123 : s_box = 8'd33;
      8'd124 : s_box = 8'd16;
      8'd125 : s_box = 8'd255;
      8'd126 : s_box = 8'd243;
      8'd127 : s_box = 8'd210;
      8'd128 : s_box = 8'd205;
      8'd129 : s_box = 8'd12;
      8'd130 : s_box = 8'd19;
      8'd131 : s_box = 8'd236;
      8'd132 : s_box = 8'd95;
      8'd133 : s_box = 8'd151;
      8'd134 : s_box = 8'd68;
      8'd135 : s_box = 8'd23;
      8'd136 : s_box = 8'd196;
      8'd137 : s_box = 8'd167;
      8'd138 : s_box = 8'd126;
      8'd139 : s_box = 8'd61;
      8'd140 : s_box = 8'd100;
      8'd141 : s_box = 8'd93;
      8'd142 : s_box = 8'd25;
      8'd143 : s_box = 8'd115;
      8'd144 : s_box = 8'd96;
      8'd145 : s_box = 8'd129;
      8'd146 : s_box = 8'd79;
      8'd147 : s_box = 8'd220;
      8'd148 : s_box = 8'd34;
      8'd149 : s_box = 8'd42;
      8'd150 : s_box = 8'd144;
      8'd151 : s_box = 8'd136;
      8'd152 : s_box = 8'd70;
      8'd153 : s_box = 8'd238;
      8'd154 : s_box = 8'd184;
      8'd155 : s_box = 8'd20;
      8'd156 : s_box = 8'd222;
      8'd157 : s_box = 8'd94;
      8'd158 : s_box = 8'd11;
      8'd159 : s_box = 8'd219;
      8'd160 : s_box = 8'd224;
      8'd161 : s_box = 8'd50;
      8'd162 : s_box = 8'd58;
      8'd163 : s_box = 8'd10;
      8'd164 : s_box = 8'd73;
      8'd165 : s_box = 8'd6;
      8'd166 : s_box = 8'd36;
      8'd167 : s_box = 8'd92;
      8'd168 : s_box = 8'd194;
      8'd169 : s_box = 8'd211;
      8'd170 : s_box = 8'd172;
      8'd171 : s_box = 8'd98;
      8'd172 : s_box = 8'd145;
      8'd173 : s_box = 8'd149;
      8'd174 : s_box = 8'd228;
      8'd175 : s_box = 8'd121;
      8'd176 : s_box = 8'd231;
      8'd177 : s_box = 8'd200;
      8'd178 : s_box = 8'd55;
      8'd179 : s_box = 8'd109;
      8'd180 : s_box = 8'd141;
      8'd181 : s_box = 8'd213;
      8'd182 : s_box = 8'd78;
      8'd183 : s_box = 8'd169;
      8'd184 : s_box = 8'd108;
      8'd185 : s_box = 8'd86;
      8'd186 : s_box = 8'd244;
      8'd187 : s_box = 8'd234;
      8'd188 : s_box = 8'd101;
      8'd189 : s_box = 8'd122;
      8'd190 : s_box = 8'd174;
      8'd191 : s_box = 8'd8;
      8'd192 : s_box = 8'd186;
      8'd193 : s_box = 8'd120;
      8'd194 : s_box = 8'd37;
      8'd195 : s_box = 8'd46;
      8'd196 : s_box = 8'd28;
      8'd197 : s_box = 8'd166;
      8'd198 : s_box = 8'd180;
      8'd199 : s_box = 8'd198;
      8'd200 : s_box = 8'd232;
      8'd201 : s_box = 8'd221;
      8'd202 : s_box = 8'd116;
      8'd203 : s_box = 8'd31;
      8'd204 : s_box = 8'd75;
      8'd205 : s_box = 8'd189;
      8'd206 : s_box = 8'd139;
      8'd207 : s_box = 8'd138;
      8'd208 : s_box = 8'd112;
      8'd209 : s_box = 8'd62;
      8'd210 : s_box = 8'd181;
      8'd211 : s_box = 8'd102;
      8'd212 : s_box = 8'd72;
      8'd213 : s_box = 8'd3;
      8'd214 : s_box = 8'd246;
      8'd215 : s_box = 8'd14;
      8'd216 : s_box = 8'd97;
      8'd217 : s_box = 8'd53;
      8'd218 : s_box = 8'd87;
      8'd219 : s_box = 8'd185;
      8'd220 : s_box = 8'd134;
      8'd221 : s_box = 8'd193;
      8'd222 : s_box = 8'd29;
      8'd223 : s_box = 8'd158;
      8'd224 : s_box = 8'd225;
      8'd225 : s_box = 8'd248;
      8'd226 : s_box = 8'd152;
      8'd227 : s_box = 8'd17;
      8'd228 : s_box = 8'd105;
      8'd229 : s_box = 8'd217;
      8'd230 : s_box = 8'd142;
      8'd231 : s_box = 8'd148;
      8'd232 : s_box = 8'd155;
      8'd233 : s_box = 8'd30;
      8'd234 : s_box = 8'd135;
      8'd235 : s_box = 8'd233;
      8'd236 : s_box = 8'd206;
      8'd237 : s_box = 8'd85;
      8'd238 : s_box = 8'd40;
      8'd239 : s_box = 8'd223;
      8'd240 : s_box = 8'd140;
      8'd241 : s_box = 8'd161;
      8'd242 : s_box = 8'd137;
      8'd243 : s_box = 8'd13;
      8'd244 : s_box = 8'd191;
      8'd245 : s_box = 8'd230;
      8'd246 : s_box = 8'd66;
      8'd247 : s_box = 8'd104;
      8'd248 : s_box = 8'd65;
      8'd249 : s_box = 8'd153;
      8'd250 : s_box = 8'd45;
      8'd251 : s_box = 8'd15;
      8'd252 : s_box = 8'd176;
      8'd253 : s_box = 8'd84;
      8'd254 : s_box = 8'd187;
      8'd255 : s_box = 8'd22;
    endcase
  endfunction

  assign S0_out = s_box (S0_in);
  assign S1_out = s_box (S1_in);
  assign S2_out = s_box (S2_in);
  assign S3_out = s_box (S3_in);  
  
endmodule