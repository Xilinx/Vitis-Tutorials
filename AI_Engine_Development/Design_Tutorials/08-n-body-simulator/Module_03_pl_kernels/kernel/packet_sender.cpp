/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "packet_sender.hpp"
ap_uint<32> generateHeader(unsigned int pktType, unsigned int ID){

	ap_uint<32> header=0;
	header(4,0)=ID;
	header(11,5)=0;
	header(14,12)=pktType;
	header[15]=0;
	header(20,16)=-1;//source row
	header(27,21)=-1;//source column
	header(30,28)=0;
	header[31]=header(30,0).xor_reduce()?(ap_uint<1>)0:(ap_uint<1>)1;
	return header;
}

void packet_sender(hls::stream<axis_pkt>& rx,
                   hls::stream<axis_pkt>& tx0,
                   hls::stream<axis_pkt>& tx1,
                   hls::stream<axis_pkt>& tx2,
                   hls::stream<axis_pkt>& tx3,
                   hls::stream<axis_pkt>& tx4,
                   hls::stream<axis_pkt>& tx5,
                   hls::stream<axis_pkt>& tx6,
                   hls::stream<axis_pkt>& tx7,
                   hls::stream<axis_pkt>& tx8,
                   hls::stream<axis_pkt>& tx9,
                   hls::stream<axis_pkt>& tx10,
                   hls::stream<axis_pkt>& tx11,
                   hls::stream<axis_pkt>& tx12,
                   hls::stream<axis_pkt>& tx13,
                   hls::stream<axis_pkt>& tx14,
                   hls::stream<axis_pkt>& tx15,
                   hls::stream<axis_pkt>& tx16,
                   hls::stream<axis_pkt>& tx17,
                   hls::stream<axis_pkt>& tx18,
                   hls::stream<axis_pkt>& tx19,
                   hls::stream<axis_pkt>& tx20,
                   hls::stream<axis_pkt>& tx21,
                   hls::stream<axis_pkt>& tx22,
                   hls::stream<axis_pkt>& tx23,
                   hls::stream<axis_pkt>& tx24,
                   hls::stream<axis_pkt>& tx25,
                   hls::stream<axis_pkt>& tx26,
                   hls::stream<axis_pkt>& tx27,
                   hls::stream<axis_pkt>& tx28,
                   hls::stream<axis_pkt>& tx29,
                   hls::stream<axis_pkt>& tx30,
                   hls::stream<axis_pkt>& tx31,
                   hls::stream<axis_pkt>& tx32,
                   hls::stream<axis_pkt>& tx33,
                   hls::stream<axis_pkt>& tx34,
                   hls::stream<axis_pkt>& tx35,
                   hls::stream<axis_pkt>& tx36,
                   hls::stream<axis_pkt>& tx37,
                   hls::stream<axis_pkt>& tx38,
                   hls::stream<axis_pkt>& tx39,
                   hls::stream<axis_pkt>& tx40,
                   hls::stream<axis_pkt>& tx41,
                   hls::stream<axis_pkt>& tx42,
                   hls::stream<axis_pkt>& tx43,
                   hls::stream<axis_pkt>& tx44,
                   hls::stream<axis_pkt>& tx45,
                   hls::stream<axis_pkt>& tx46,
                   hls::stream<axis_pkt>& tx47,
                   hls::stream<axis_pkt>& tx48,
                   hls::stream<axis_pkt>& tx49,
                   hls::stream<axis_pkt>& tx50,
                   hls::stream<axis_pkt>& tx51,
                   hls::stream<axis_pkt>& tx52,
                   hls::stream<axis_pkt>& tx53,
                   hls::stream<axis_pkt>& tx54,
                   hls::stream<axis_pkt>& tx55,
                   hls::stream<axis_pkt>& tx56,
                   hls::stream<axis_pkt>& tx57,
                   hls::stream<axis_pkt>& tx58,
                   hls::stream<axis_pkt>& tx59,
                   hls::stream<axis_pkt>& tx60,
                   hls::stream<axis_pkt>& tx61,
                   hls::stream<axis_pkt>& tx62,
                   hls::stream<axis_pkt>& tx63,
                   hls::stream<axis_pkt>& tx64,
                   hls::stream<axis_pkt>& tx65,
                   hls::stream<axis_pkt>& tx66,
                   hls::stream<axis_pkt>& tx67,
                   hls::stream<axis_pkt>& tx68,
                   hls::stream<axis_pkt>& tx69,
                   hls::stream<axis_pkt>& tx70,
                   hls::stream<axis_pkt>& tx71,
                   hls::stream<axis_pkt>& tx72,
                   hls::stream<axis_pkt>& tx73,
                   hls::stream<axis_pkt>& tx74,
                   hls::stream<axis_pkt>& tx75,
                   hls::stream<axis_pkt>& tx76,
                   hls::stream<axis_pkt>& tx77,
                   hls::stream<axis_pkt>& tx78,
                   hls::stream<axis_pkt>& tx79,
                   hls::stream<axis_pkt>& tx80,
                   hls::stream<axis_pkt>& tx81,
                   hls::stream<axis_pkt>& tx82,
                   hls::stream<axis_pkt>& tx83,
                   hls::stream<axis_pkt>& tx84,
                   hls::stream<axis_pkt>& tx85,
                   hls::stream<axis_pkt>& tx86,
                   hls::stream<axis_pkt>& tx87,
                   hls::stream<axis_pkt>& tx88,
                   hls::stream<axis_pkt>& tx89,
                   hls::stream<axis_pkt>& tx90,
                   hls::stream<axis_pkt>& tx91,
                   hls::stream<axis_pkt>& tx92,
                   hls::stream<axis_pkt>& tx93,
                   hls::stream<axis_pkt>& tx94,
                   hls::stream<axis_pkt>& tx95,
                   hls::stream<axis_pkt>& tx96,
                   hls::stream<axis_pkt>& tx97,
                   hls::stream<axis_pkt>& tx98,
                   hls::stream<axis_pkt>& tx99
		   )
{
#pragma HLS interface axis port=rx

#pragma HLS interface axis port=tx0
#pragma HLS interface axis port=tx1
#pragma HLS interface axis port=tx2
#pragma HLS interface axis port=tx3
#pragma HLS interface axis port=tx4
#pragma HLS interface axis port=tx5
#pragma HLS interface axis port=tx6
#pragma HLS interface axis port=tx7
#pragma HLS interface axis port=tx8
#pragma HLS interface axis port=tx9
#pragma HLS interface axis port=tx10
#pragma HLS interface axis port=tx11
#pragma HLS interface axis port=tx12
#pragma HLS interface axis port=tx13
#pragma HLS interface axis port=tx14
#pragma HLS interface axis port=tx15
#pragma HLS interface axis port=tx16
#pragma HLS interface axis port=tx17
#pragma HLS interface axis port=tx18
#pragma HLS interface axis port=tx19
#pragma HLS interface axis port=tx20
#pragma HLS interface axis port=tx21
#pragma HLS interface axis port=tx22
#pragma HLS interface axis port=tx23
#pragma HLS interface axis port=tx24
#pragma HLS interface axis port=tx25
#pragma HLS interface axis port=tx26
#pragma HLS interface axis port=tx27
#pragma HLS interface axis port=tx28
#pragma HLS interface axis port=tx29
#pragma HLS interface axis port=tx30
#pragma HLS interface axis port=tx31
#pragma HLS interface axis port=tx32
#pragma HLS interface axis port=tx33
#pragma HLS interface axis port=tx34
#pragma HLS interface axis port=tx35
#pragma HLS interface axis port=tx36
#pragma HLS interface axis port=tx37
#pragma HLS interface axis port=tx38
#pragma HLS interface axis port=tx39
#pragma HLS interface axis port=tx40
#pragma HLS interface axis port=tx41
#pragma HLS interface axis port=tx42
#pragma HLS interface axis port=tx43
#pragma HLS interface axis port=tx44
#pragma HLS interface axis port=tx45
#pragma HLS interface axis port=tx46
#pragma HLS interface axis port=tx47
#pragma HLS interface axis port=tx48
#pragma HLS interface axis port=tx49
#pragma HLS interface axis port=tx50
#pragma HLS interface axis port=tx51
#pragma HLS interface axis port=tx52
#pragma HLS interface axis port=tx53
#pragma HLS interface axis port=tx54
#pragma HLS interface axis port=tx55
#pragma HLS interface axis port=tx56
#pragma HLS interface axis port=tx57
#pragma HLS interface axis port=tx58
#pragma HLS interface axis port=tx59
#pragma HLS interface axis port=tx60
#pragma HLS interface axis port=tx61
#pragma HLS interface axis port=tx62
#pragma HLS interface axis port=tx63
#pragma HLS interface axis port=tx64
#pragma HLS interface axis port=tx65
#pragma HLS interface axis port=tx66
#pragma HLS interface axis port=tx67
#pragma HLS interface axis port=tx68
#pragma HLS interface axis port=tx69
#pragma HLS interface axis port=tx70
#pragma HLS interface axis port=tx71
#pragma HLS interface axis port=tx72
#pragma HLS interface axis port=tx73
#pragma HLS interface axis port=tx74
#pragma HLS interface axis port=tx75
#pragma HLS interface axis port=tx76
#pragma HLS interface axis port=tx77
#pragma HLS interface axis port=tx78
#pragma HLS interface axis port=tx79
#pragma HLS interface axis port=tx80
#pragma HLS interface axis port=tx81
#pragma HLS interface axis port=tx82
#pragma HLS interface axis port=tx83
#pragma HLS interface axis port=tx84
#pragma HLS interface axis port=tx85
#pragma HLS interface axis port=tx86
#pragma HLS interface axis port=tx87
#pragma HLS interface axis port=tx88
#pragma HLS interface axis port=tx89
#pragma HLS interface axis port=tx90
#pragma HLS interface axis port=tx91
#pragma HLS interface axis port=tx92
#pragma HLS interface axis port=tx93
#pragma HLS interface axis port=tx94
#pragma HLS interface axis port=tx95
#pragma HLS interface axis port=tx96
#pragma HLS interface axis port=tx97
#pragma HLS interface axis port=tx98
#pragma HLS interface axis port=tx99
#pragma HLS interface s_axilite port=return bundle=control  
  axis_pkt x;
  ap_axiu<32,0,0,0> tmp;
  
  for(int cu=0; cu<NUM_CU; cu++){
    for (int h = 0; h < PACKET_NUM; h++) 
      { 
	unsigned int ID=packet_ids[h];
	ap_uint<32> header=generateHeader(PKTTYPE,ID); //packet header
	tmp.data=header;
	tmp.keep=-1;
	tmp.last=0;
	
	switch(cu) {
	case 0:tx0.write(tmp);break;
	case 1:tx1.write(tmp);break;
	case 2:tx2.write(tmp);break;
	case 3:tx3.write(tmp);break;
	case 4:tx4.write(tmp);break;
	case 5:tx5.write(tmp);break;
	case 6:tx6.write(tmp);break;
	case 7:tx7.write(tmp);break;
	case 8:tx8.write(tmp);break;
	case 9:tx9.write(tmp);break;
	case 10:tx10.write(tmp);break;
	case 11:tx11.write(tmp);break;
	case 12:tx12.write(tmp);break;
	case 13:tx13.write(tmp);break;
	case 14:tx14.write(tmp);break;
	case 15:tx15.write(tmp);break;
	case 16:tx16.write(tmp);break;
	case 17:tx17.write(tmp);break;
	case 18:tx18.write(tmp);break;
	case 19:tx19.write(tmp);break;
	case 20:tx20.write(tmp);break;
	case 21:tx21.write(tmp);break;
	case 22:tx22.write(tmp);break;
	case 23:tx23.write(tmp);break;
	case 24:tx24.write(tmp);break;
	case 25:tx25.write(tmp);break;
	case 26:tx26.write(tmp);break;
	case 27:tx27.write(tmp);break;
	case 28:tx28.write(tmp);break;
	case 29:tx29.write(tmp);break;
	case 30:tx30.write(tmp);break;
	case 31:tx31.write(tmp);break;
	case 32:tx32.write(tmp);break;
	case 33:tx33.write(tmp);break;
	case 34:tx34.write(tmp);break;
	case 35:tx35.write(tmp);break;
	case 36:tx36.write(tmp);break;
	case 37:tx37.write(tmp);break;
	case 38:tx38.write(tmp);break;
	case 39:tx39.write(tmp);break;
	case 40:tx40.write(tmp);break;
	case 41:tx41.write(tmp);break;
	case 42:tx42.write(tmp);break;
	case 43:tx43.write(tmp);break;
	case 44:tx44.write(tmp);break;
	case 45:tx45.write(tmp);break;
	case 46:tx46.write(tmp);break;
	case 47:tx47.write(tmp);break;
	case 48:tx48.write(tmp);break;
	case 49:tx49.write(tmp);break;
	case 50:tx50.write(tmp);break;
	case 51:tx51.write(tmp);break;
	case 52:tx52.write(tmp);break;
	case 53:tx53.write(tmp);break;
	case 54:tx54.write(tmp);break;
	case 55:tx55.write(tmp);break;
	case 56:tx56.write(tmp);break;
	case 57:tx57.write(tmp);break;
	case 58:tx58.write(tmp);break;
	case 59:tx59.write(tmp);break;
	case 60:tx60.write(tmp);break;
	case 61:tx61.write(tmp);break;
	case 62:tx62.write(tmp);break;
	case 63:tx63.write(tmp);break;
	case 64:tx64.write(tmp);break;
	case 65:tx65.write(tmp);break;
	case 66:tx66.write(tmp);break;
	case 67:tx67.write(tmp);break;
	case 68:tx68.write(tmp);break;
	case 69:tx69.write(tmp);break;
	case 70:tx70.write(tmp);break;
	case 71:tx71.write(tmp);break;
	case 72:tx72.write(tmp);break;
	case 73:tx73.write(tmp);break;
	case 74:tx74.write(tmp);break;
	case 75:tx75.write(tmp);break;
	case 76:tx76.write(tmp);break;
	case 77:tx77.write(tmp);break;
	case 78:tx78.write(tmp);break;
	case 79:tx79.write(tmp);break;
	case 80:tx80.write(tmp);break;
	case 81:tx81.write(tmp);break;
	case 82:tx82.write(tmp);break;
	case 83:tx83.write(tmp);break;
	case 84:tx84.write(tmp);break;
	case 85:tx85.write(tmp);break;
	case 86:tx86.write(tmp);break;
	case 87:tx87.write(tmp);break;
	case 88:tx88.write(tmp);break;
	case 89:tx89.write(tmp);break;
	case 90:tx90.write(tmp);break;
	case 91:tx91.write(tmp);break;
	case 92:tx92.write(tmp);break;
	case 93:tx93.write(tmp);break;
	case 94:tx94.write(tmp);break;
	case 95:tx95.write(tmp);break;
	case 96:tx96.write(tmp);break;
	case 97:tx97.write(tmp);break;
	  case 98:tx98.write(tmp);break;
	case 99:tx99.write(tmp);break;
	}
	for(int i = 0; i < PACKET_LEN; i++)
	  {
#pragma HLS PIPELINE II=1
	    x=rx.read();
	    x.last = (i==PACKET_LEN-1);
	    
	    switch(cu) {
	    case 0:tx0.write(x);break;
	    case 1:tx1.write(x);break;
	    case 2:tx2.write(x);break;
	    case 3:tx3.write(x);break;
	    case 4:tx4.write(x);break;
	    case 5:tx5.write(x);break;
	    case 6:tx6.write(x);break;
	    case 7:tx7.write(x);break;
	    case 8:tx8.write(x);break;
	    case 9:tx9.write(x);break;
	    case 10:tx10.write(x);break;
	    case 11:tx11.write(x);break;
	    case 12:tx12.write(x);break;
	    case 13:tx13.write(x);break;
	    case 14:tx14.write(x);break;
	    case 15:tx15.write(x);break;
	    case 16:tx16.write(x);break;
	    case 17:tx17.write(x);break;
	    case 18:tx18.write(x);break;
	    case 19:tx19.write(x);break;
	    case 20:tx20.write(x);break;
	    case 21:tx21.write(x);break;
	    case 22:tx22.write(x);break;
	    case 23:tx23.write(x);break;
	    case 24:tx24.write(x);break;
	    case 25:tx25.write(x);break;
	    case 26:tx26.write(x);break;
	    case 27:tx27.write(x);break;
	    case 28:tx28.write(x);break;
	    case 29:tx29.write(x);break;
	    case 30:tx30.write(x);break;
	    case 31:tx31.write(x);break;
	    case 32:tx32.write(x);break;
	    case 33:tx33.write(x);break;
	    case 34:tx34.write(x);break;
	    case 35:tx35.write(x);break;
	    case 36:tx36.write(x);break;
	    case 37:tx37.write(x);break;
	    case 38:tx38.write(x);break;
	    case 39:tx39.write(x);break;
	    case 40:tx40.write(x);break;
	    case 41:tx41.write(x);break;
	    case 42:tx42.write(x);break;
	    case 43:tx43.write(x);break;
	    case 44:tx44.write(x);break;
	    case 45:tx45.write(x);break;
	    case 46:tx46.write(x);break;
	    case 47:tx47.write(x);break;
	    case 48:tx48.write(x);break;
	    case 49:tx49.write(x);break;
	    case 50:tx50.write(x);break;
	    case 51:tx51.write(x);break;
	    case 52:tx52.write(x);break;
	    case 53:tx53.write(x);break;
	    case 54:tx54.write(x);break;
	    case 55:tx55.write(x);break;
	    case 56:tx56.write(x);break;
	    case 57:tx57.write(x);break;
	    case 58:tx58.write(x);break;
	    case 59:tx59.write(x);break;
	    case 60:tx60.write(x);break;
	    case 61:tx61.write(x);break;
	    case 62:tx62.write(x);break;
	    case 63:tx63.write(x);break;
	    case 64:tx64.write(x);break;
	    case 65:tx65.write(x);break;
	    case 66:tx66.write(x);break;
	    case 67:tx67.write(x);break;
	    case 68:tx68.write(x);break;
	    case 69:tx69.write(x);break;
	    case 70:tx70.write(x);break;
	    case 71:tx71.write(x);break;
	    case 72:tx72.write(x);break;
	    case 73:tx73.write(x);break;
	    case 74:tx74.write(x);break;
	    case 75:tx75.write(x);break;
	    case 76:tx76.write(x);break;
	    case 77:tx77.write(x);break;
	    case 78:tx78.write(x);break;
	    case 79:tx79.write(x);break;
	    case 80:tx80.write(x);break;
	    case 81:tx81.write(x);break;
	    case 82:tx82.write(x);break;
	    case 83:tx83.write(x);break;
	    case 84:tx84.write(x);break;
	    case 85:tx85.write(x);break;
	    case 86:tx86.write(x);break;
	    case 87:tx87.write(x);break;
	    case 88:tx88.write(x);break;
	    case 89:tx89.write(x);break;
	    case 90:tx90.write(x);break;
	    case 91:tx91.write(x);break;
	    case 92:tx92.write(x);break;
	    case 93:tx93.write(x);break;
	    case 94:tx94.write(x);break;
	    case 95:tx95.write(x);break;
	    case 96:tx96.write(x);break;
	    case 97:tx97.write(x);break;
	    case 98:tx98.write(x);break;
	    case 99:tx99.write(x);break;
	    }
	    
	  }
      }
  }
}
