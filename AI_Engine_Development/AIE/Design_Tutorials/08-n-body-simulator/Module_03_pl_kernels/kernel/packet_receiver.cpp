/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
  
#include "packet_receiver.hpp"

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
	ap_uint<32> ID=0;
	ID(4,0)=header(4,0);
	return ID;
}

void packet_receiver(
        hls::stream<axis_pkt>& rx0,
        hls::stream<axis_pkt>& rx1,
        hls::stream<axis_pkt>& rx2,
        hls::stream<axis_pkt>& rx3,
        hls::stream<axis_pkt>& rx4,
        hls::stream<axis_pkt>& rx5,
        hls::stream<axis_pkt>& rx6,
        hls::stream<axis_pkt>& rx7,
        hls::stream<axis_pkt>& rx8,
        hls::stream<axis_pkt>& rx9,
        hls::stream<axis_pkt>& rx10,
        hls::stream<axis_pkt>& rx11,
        hls::stream<axis_pkt>& rx12,
        hls::stream<axis_pkt>& rx13,
        hls::stream<axis_pkt>& rx14,
        hls::stream<axis_pkt>& rx15,
        hls::stream<axis_pkt>& rx16,
        hls::stream<axis_pkt>& rx17,
        hls::stream<axis_pkt>& rx18,
        hls::stream<axis_pkt>& rx19,
        hls::stream<axis_pkt>& rx20,
        hls::stream<axis_pkt>& rx21,
        hls::stream<axis_pkt>& rx22,
        hls::stream<axis_pkt>& rx23,
        hls::stream<axis_pkt>& rx24,
        hls::stream<axis_pkt>& rx25,
        hls::stream<axis_pkt>& rx26,
        hls::stream<axis_pkt>& rx27,
        hls::stream<axis_pkt>& rx28,
        hls::stream<axis_pkt>& rx29,
        hls::stream<axis_pkt>& rx30,
        hls::stream<axis_pkt>& rx31,
        hls::stream<axis_pkt>& rx32,
        hls::stream<axis_pkt>& rx33,
        hls::stream<axis_pkt>& rx34,
        hls::stream<axis_pkt>& rx35,
        hls::stream<axis_pkt>& rx36,
        hls::stream<axis_pkt>& rx37,
        hls::stream<axis_pkt>& rx38,
        hls::stream<axis_pkt>& rx39,
        hls::stream<axis_pkt>& rx40,
        hls::stream<axis_pkt>& rx41,
        hls::stream<axis_pkt>& rx42,
        hls::stream<axis_pkt>& rx43,
        hls::stream<axis_pkt>& rx44,
        hls::stream<axis_pkt>& rx45,
        hls::stream<axis_pkt>& rx46,
        hls::stream<axis_pkt>& rx47,
        hls::stream<axis_pkt>& rx48,
        hls::stream<axis_pkt>& rx49,
        hls::stream<axis_pkt>& rx50,
        hls::stream<axis_pkt>& rx51,
        hls::stream<axis_pkt>& rx52,
        hls::stream<axis_pkt>& rx53,
        hls::stream<axis_pkt>& rx54,
        hls::stream<axis_pkt>& rx55,
        hls::stream<axis_pkt>& rx56,
        hls::stream<axis_pkt>& rx57,
        hls::stream<axis_pkt>& rx58,
        hls::stream<axis_pkt>& rx59,
        hls::stream<axis_pkt>& rx60,
        hls::stream<axis_pkt>& rx61,
        hls::stream<axis_pkt>& rx62,
        hls::stream<axis_pkt>& rx63,
        hls::stream<axis_pkt>& rx64,
        hls::stream<axis_pkt>& rx65,
        hls::stream<axis_pkt>& rx66,
        hls::stream<axis_pkt>& rx67,
        hls::stream<axis_pkt>& rx68,
        hls::stream<axis_pkt>& rx69,
        hls::stream<axis_pkt>& rx70,
        hls::stream<axis_pkt>& rx71,
        hls::stream<axis_pkt>& rx72,
        hls::stream<axis_pkt>& rx73,
        hls::stream<axis_pkt>& rx74,
        hls::stream<axis_pkt>& rx75,
        hls::stream<axis_pkt>& rx76,
        hls::stream<axis_pkt>& rx77,
        hls::stream<axis_pkt>& rx78,
        hls::stream<axis_pkt>& rx79,
        hls::stream<axis_pkt>& rx80,
        hls::stream<axis_pkt>& rx81,
        hls::stream<axis_pkt>& rx82,
        hls::stream<axis_pkt>& rx83,
        hls::stream<axis_pkt>& rx84,
        hls::stream<axis_pkt>& rx85,
        hls::stream<axis_pkt>& rx86,
        hls::stream<axis_pkt>& rx87,
        hls::stream<axis_pkt>& rx88,
        hls::stream<axis_pkt>& rx89,
        hls::stream<axis_pkt>& rx90,
        hls::stream<axis_pkt>& rx91,
        hls::stream<axis_pkt>& rx92,
        hls::stream<axis_pkt>& rx93,
        hls::stream<axis_pkt>& rx94,
        hls::stream<axis_pkt>& rx95,
        hls::stream<axis_pkt>& rx96,
        hls::stream<axis_pkt>& rx97,
        hls::stream<axis_pkt>& rx98,
        hls::stream<axis_pkt>& rx99,
	hls::stream<axis_pkt>& tx0,
	hls::stream<axis_pkt>& tx1,
	hls::stream<axis_pkt>& tx2,
	hls::stream<axis_pkt>& tx3)
{

#pragma HLS INTERFACE axis port=rx0
#pragma HLS INTERFACE axis port=rx1
#pragma HLS INTERFACE axis port=rx2
#pragma HLS INTERFACE axis port=rx3
#pragma HLS INTERFACE axis port=rx4
#pragma HLS INTERFACE axis port=rx5
#pragma HLS INTERFACE axis port=rx6
#pragma HLS INTERFACE axis port=rx7
#pragma HLS INTERFACE axis port=rx8
#pragma HLS INTERFACE axis port=rx9
#pragma HLS INTERFACE axis port=rx10
#pragma HLS INTERFACE axis port=rx11
#pragma HLS INTERFACE axis port=rx12
#pragma HLS INTERFACE axis port=rx13
#pragma HLS INTERFACE axis port=rx14
#pragma HLS INTERFACE axis port=rx15
#pragma HLS INTERFACE axis port=rx16
#pragma HLS INTERFACE axis port=rx17
#pragma HLS INTERFACE axis port=rx18
#pragma HLS INTERFACE axis port=rx19
#pragma HLS INTERFACE axis port=rx20
#pragma HLS INTERFACE axis port=rx21
#pragma HLS INTERFACE axis port=rx22
#pragma HLS INTERFACE axis port=rx23
#pragma HLS INTERFACE axis port=rx24
#pragma HLS INTERFACE axis port=rx25
#pragma HLS INTERFACE axis port=rx26
#pragma HLS INTERFACE axis port=rx27
#pragma HLS INTERFACE axis port=rx28
#pragma HLS INTERFACE axis port=rx29
#pragma HLS INTERFACE axis port=rx30
#pragma HLS INTERFACE axis port=rx31
#pragma HLS INTERFACE axis port=rx32
#pragma HLS INTERFACE axis port=rx33
#pragma HLS INTERFACE axis port=rx34
#pragma HLS INTERFACE axis port=rx35
#pragma HLS INTERFACE axis port=rx36
#pragma HLS INTERFACE axis port=rx37
#pragma HLS INTERFACE axis port=rx38
#pragma HLS INTERFACE axis port=rx39
#pragma HLS INTERFACE axis port=rx40
#pragma HLS INTERFACE axis port=rx41
#pragma HLS INTERFACE axis port=rx42
#pragma HLS INTERFACE axis port=rx43
#pragma HLS INTERFACE axis port=rx44
#pragma HLS INTERFACE axis port=rx45
#pragma HLS INTERFACE axis port=rx46
#pragma HLS INTERFACE axis port=rx47
#pragma HLS INTERFACE axis port=rx48
#pragma HLS INTERFACE axis port=rx49
#pragma HLS INTERFACE axis port=rx50
#pragma HLS INTERFACE axis port=rx51
#pragma HLS INTERFACE axis port=rx52
#pragma HLS INTERFACE axis port=rx53
#pragma HLS INTERFACE axis port=rx54
#pragma HLS INTERFACE axis port=rx55
#pragma HLS INTERFACE axis port=rx56
#pragma HLS INTERFACE axis port=rx57
#pragma HLS INTERFACE axis port=rx58
#pragma HLS INTERFACE axis port=rx59
#pragma HLS INTERFACE axis port=rx60
#pragma HLS INTERFACE axis port=rx61
#pragma HLS INTERFACE axis port=rx62
#pragma HLS INTERFACE axis port=rx63
#pragma HLS INTERFACE axis port=rx64
#pragma HLS INTERFACE axis port=rx65
#pragma HLS INTERFACE axis port=rx66
#pragma HLS INTERFACE axis port=rx67
#pragma HLS INTERFACE axis port=rx68
#pragma HLS INTERFACE axis port=rx69
#pragma HLS INTERFACE axis port=rx70
#pragma HLS INTERFACE axis port=rx71
#pragma HLS INTERFACE axis port=rx72
#pragma HLS INTERFACE axis port=rx73
#pragma HLS INTERFACE axis port=rx74
#pragma HLS INTERFACE axis port=rx75
#pragma HLS INTERFACE axis port=rx76
#pragma HLS INTERFACE axis port=rx77
#pragma HLS INTERFACE axis port=rx78
#pragma HLS INTERFACE axis port=rx79
#pragma HLS INTERFACE axis port=rx80
#pragma HLS INTERFACE axis port=rx81
#pragma HLS INTERFACE axis port=rx82
#pragma HLS INTERFACE axis port=rx83
#pragma HLS INTERFACE axis port=rx84
#pragma HLS INTERFACE axis port=rx85
#pragma HLS INTERFACE axis port=rx86
#pragma HLS INTERFACE axis port=rx87
#pragma HLS INTERFACE axis port=rx88
#pragma HLS INTERFACE axis port=rx89
#pragma HLS INTERFACE axis port=rx90
#pragma HLS INTERFACE axis port=rx91
#pragma HLS INTERFACE axis port=rx92
#pragma HLS INTERFACE axis port=rx93
#pragma HLS INTERFACE axis port=rx94
#pragma HLS INTERFACE axis port=rx95
#pragma HLS INTERFACE axis port=rx96
#pragma HLS INTERFACE axis port=rx97
#pragma HLS INTERFACE axis port=rx98
#pragma HLS INTERFACE axis port=rx99

#pragma HLS INTERFACE axis port=tx0
#pragma HLS INTERFACE axis port=tx1
#pragma HLS INTERFACE axis port=tx2
#pragma HLS INTERFACE axis port=tx3

#pragma HLS interface s_axilite port=return bundle=control  
  
  axis_pkt x;
  ap_axiu<32,0,0,0> tmp;
  
  for (int cu =0; cu < 100; cu++){
    for (int h = 0; h < PACKET_NUM; h++) 
      { 
	//first word is packet header
	switch(cu){	
        case 0:tmp=rx0.read();break;	
        case 1:tmp=rx1.read();break;	
        case 2:tmp=rx2.read();break;	
        case 3:tmp=rx3.read();break;	
        case 4:tmp=rx4.read();break;	
        case 5:tmp=rx5.read();break;	
        case 6:tmp=rx6.read();break;	
        case 7:tmp=rx7.read();break;	
        case 8:tmp=rx8.read();break;	
        case 9:tmp=rx9.read();break;	
        case 10:tmp=rx10.read();break;	
        case 11:tmp=rx11.read();break;	
        case 12:tmp=rx12.read();break;	
        case 13:tmp=rx13.read();break;	
        case 14:tmp=rx14.read();break;	
        case 15:tmp=rx15.read();break;	
        case 16:tmp=rx16.read();break;	
        case 17:tmp=rx17.read();break;	
        case 18:tmp=rx18.read();break;	
        case 19:tmp=rx19.read();break;	
        case 20:tmp=rx20.read();break;	
        case 21:tmp=rx21.read();break;	
        case 22:tmp=rx22.read();break;	
        case 23:tmp=rx23.read();break;	
        case 24:tmp=rx24.read();break;	
        case 25:tmp=rx25.read();break;	
        case 26:tmp=rx26.read();break;	
        case 27:tmp=rx27.read();break;	
        case 28:tmp=rx28.read();break;	
        case 29:tmp=rx29.read();break;	
        case 30:tmp=rx30.read();break;	
        case 31:tmp=rx31.read();break;	
        case 32:tmp=rx32.read();break;	
        case 33:tmp=rx33.read();break;	
        case 34:tmp=rx34.read();break;	
        case 35:tmp=rx35.read();break;	
        case 36:tmp=rx36.read();break;	
        case 37:tmp=rx37.read();break;	
        case 38:tmp=rx38.read();break;	
        case 39:tmp=rx39.read();break;	
        case 40:tmp=rx40.read();break;	
        case 41:tmp=rx41.read();break;	
        case 42:tmp=rx42.read();break;	
        case 43:tmp=rx43.read();break;	
        case 44:tmp=rx44.read();break;	
        case 45:tmp=rx45.read();break;	
        case 46:tmp=rx46.read();break;	
        case 47:tmp=rx47.read();break;	
        case 48:tmp=rx48.read();break;	
        case 49:tmp=rx49.read();break;	
        case 50:tmp=rx50.read();break;	
        case 51:tmp=rx51.read();break;	
        case 52:tmp=rx52.read();break;	
        case 53:tmp=rx53.read();break;	
        case 54:tmp=rx54.read();break;	
        case 55:tmp=rx55.read();break;	
        case 56:tmp=rx56.read();break;	
        case 57:tmp=rx57.read();break;	
        case 58:tmp=rx58.read();break;	
        case 59:tmp=rx59.read();break;	
        case 60:tmp=rx60.read();break;	
        case 61:tmp=rx61.read();break;	
        case 62:tmp=rx62.read();break;	
        case 63:tmp=rx63.read();break;	
        case 64:tmp=rx64.read();break;	
        case 65:tmp=rx65.read();break;	
        case 66:tmp=rx66.read();break;	
        case 67:tmp=rx67.read();break;	
        case 68:tmp=rx68.read();break;	
        case 69:tmp=rx69.read();break;	
        case 70:tmp=rx70.read();break;	
        case 71:tmp=rx71.read();break;	
        case 72:tmp=rx72.read();break;	
        case 73:tmp=rx73.read();break;	
        case 74:tmp=rx74.read();break;	
        case 75:tmp=rx75.read();break;	
        case 76:tmp=rx76.read();break;	
        case 77:tmp=rx77.read();break;	
        case 78:tmp=rx78.read();break;	
        case 79:tmp=rx79.read();break;	
        case 80:tmp=rx80.read();break;	
        case 81:tmp=rx81.read();break;	
        case 82:tmp=rx82.read();break;	
        case 83:tmp=rx83.read();break;	
        case 84:tmp=rx84.read();break;	
        case 85:tmp=rx85.read();break;	
        case 86:tmp=rx86.read();break;	
        case 87:tmp=rx87.read();break;	
        case 88:tmp=rx88.read();break;	
        case 89:tmp=rx89.read();break;	
        case 90:tmp=rx90.read();break;	
        case 91:tmp=rx91.read();break;	
        case 92:tmp=rx92.read();break;	
        case 93:tmp=rx93.read();break;	
        case 94:tmp=rx94.read();break;	
        case 95:tmp=rx95.read();break;	
        case 96:tmp=rx96.read();break;	
        case 97:tmp=rx97.read();break;	
        case 98:tmp=rx98.read();break;	
        case 99:tmp=rx99.read();break;
        }
	
	unsigned int ID=getPacketId(tmp.data);
	unsigned int channel=packet_ids[ID];
	
	for(int i = 0; i < PACKET_LEN; i++)
	  {
#pragma HLS PIPELINE II=1
	    switch(cu){
	    case 0:x=rx0.read();break;
	    case 1:x=rx1.read();break;
	    case 2:x=rx2.read();break;
	    case 3:x=rx3.read();break;
	    case 4:x=rx4.read();break;
	    case 5:x=rx5.read();break;
	    case 6:x=rx6.read();break;
	    case 7:x=rx7.read();break;
	    case 8:x=rx8.read();break;
	    case 9:x=rx9.read();break;
	    case 10:x=rx10.read();break;
	    case 11:x=rx11.read();break;
	    case 12:x=rx12.read();break;
	    case 13:x=rx13.read();break;
	    case 14:x=rx14.read();break;
	    case 15:x=rx15.read();break;
	    case 16:x=rx16.read();break;
	    case 17:x=rx17.read();break;
	    case 18:x=rx18.read();break;
	    case 19:x=rx19.read();break;
	    case 20:x=rx20.read();break;
	    case 21:x=rx21.read();break;
	    case 22:x=rx22.read();break;
	    case 23:x=rx23.read();break;
	    case 24:x=rx24.read();break;
	    case 25:x=rx25.read();break;
	    case 26:x=rx26.read();break;
	    case 27:x=rx27.read();break;
	    case 28:x=rx28.read();break;
	    case 29:x=rx29.read();break;
	    case 30:x=rx30.read();break;
	    case 31:x=rx31.read();break;
	    case 32:x=rx32.read();break;
	    case 33:x=rx33.read();break;
	    case 34:x=rx34.read();break;
	    case 35:x=rx35.read();break;
	    case 36:x=rx36.read();break;
	    case 37:x=rx37.read();break;
	    case 38:x=rx38.read();break;
	    case 39:x=rx39.read();break;
	    case 40:x=rx40.read();break;
	    case 41:x=rx41.read();break;
	    case 42:x=rx42.read();break;
	    case 43:x=rx43.read();break;
	    case 44:x=rx44.read();break;
	    case 45:x=rx45.read();break;
	    case 46:x=rx46.read();break;
	    case 47:x=rx47.read();break;
	    case 48:x=rx48.read();break;
	    case 49:x=rx49.read();break;
	    case 50:x=rx50.read();break;
	    case 51:x=rx51.read();break;
	    case 52:x=rx52.read();break;
	    case 53:x=rx53.read();break;
	    case 54:x=rx54.read();break;
	    case 55:x=rx55.read();break;
	    case 56:x=rx56.read();break;
	    case 57:x=rx57.read();break;
	    case 58:x=rx58.read();break;
	    case 59:x=rx59.read();break;
	    case 60:x=rx60.read();break;
	    case 61:x=rx61.read();break;
	    case 62:x=rx62.read();break;
	    case 63:x=rx63.read();break;
	    case 64:x=rx64.read();break;
	    case 65:x=rx65.read();break;
	    case 66:x=rx66.read();break;
	    case 67:x=rx67.read();break;
	    case 68:x=rx68.read();break;
	    case 69:x=rx69.read();break;
	    case 70:x=rx70.read();break;
	    case 71:x=rx71.read();break;
	    case 72:x=rx72.read();break;
	    case 73:x=rx73.read();break;
	    case 74:x=rx74.read();break;
	    case 75:x=rx75.read();break;
	    case 76:x=rx76.read();break;
	    case 77:x=rx77.read();break;
	    case 78:x=rx78.read();break;
	    case 79:x=rx79.read();break;
	    case 80:x=rx80.read();break;
	    case 81:x=rx81.read();break;
	    case 82:x=rx82.read();break;
	    case 83:x=rx83.read();break;
	    case 84:x=rx84.read();break;
	    case 85:x=rx85.read();break;
	    case 86:x=rx86.read();break;
	    case 87:x=rx87.read();break;
	    case 88:x=rx88.read();break;
	    case 89:x=rx89.read();break;
	    case 90:x=rx90.read();break;
	    case 91:x=rx91.read();break;
	    case 92:x=rx92.read();break;
	    case 93:x=rx93.read();break;
	    case 94:x=rx94.read();break;
	    case 95:x=rx95.read();break;
	    case 96:x=rx96.read();break;
	    case 97:x=rx97.read();break;
	    case 98:x=rx98.read();break;
	    case 99:x=rx99.read();break;
	    }
	    
	    switch(channel){
	    case 0:tx0.write(x);break;
	    case 1:tx1.write(x);break;
	    case 2:tx2.write(x);break;
	    case 3:tx3.write(x);break;
	    }
	  }
      }
  }
}

