
#include "axi2stream.h"

void AXIBursts2PixelStream(
		AXIMM axi,
		U16 WidthInBytes,
		U16 Height,
		U16 StrideInBytes,
		STREAM_PIXELS& stream)
{
#ifndef __SYNTHESIS__
	assert(WidthInBytes<=1920);
#endif

	const int AXIMM_DATA_BUFF_SZ = (1920+(AXIMM_DATA_WIDTH/8)-1)/(AXIMM_DATA_WIDTH/8);
	const int PIXELS_PER_AXIMM = (AXIMM_DATA_WIDTH/8);

	ap_uint<AXIMM_DATA_WIDTH> buff[AXIMM_DATA_BUFF_SZ];

    int yoffset = 0;
	int loopWidth = (WidthInBytes+(AXIMM_DATA_WIDTH/8)-1)/(AXIMM_DATA_WIDTH/8);
	int widthInPix = WidthInBytes;
	int remainPix = widthInPix%(4*(AXIMM_DATA_WIDTH/32));
	remainPix = (remainPix==0) ? (4*(AXIMM_DATA_WIDTH/32)) : remainPix;

	U8 pixel;
	ap_uint<AXIMM_DATA_WIDTH> bytes;

    forEachRow: for (int y = 0; y < Height; y++)
    {
#pragma HLS loop_tripcount max=1080
#pragma HLS loop_flatten off

        aximm2bytes: for (int x = 0; x < loopWidth; x++)
        {
#pragma HLS pipeline II=1
			buff[x] = axi[yoffset+x];
        }
        yoffset += StrideInBytes/(AXIMM_DATA_WIDTH/8);
		
 		bytes2pixels: for (int x = 0; x < loopWidth; x++)
		{
#pragma HLS pipeline II=PIXELS_PER_AXIMM
			bytes = buff[x];
			for (int i=0; i<(AXIMM_DATA_WIDTH/8); i++)
			{
				U8 pixel = bytes(i*8+7, i*8);
				if (x<loopWidth-1 || i<remainPix/1) stream << pixel;
			}
		}
	}
}	


void PixelStream2AXIBursts(
		STREAM_PIXELS& stream,
        U16 WidthInBytes,
        U16 Height,
        U16 StrideInBytes,
        AXIMM aximm)
{

#ifndef __SYNTHESIS__
	assert(WidthInBytes<=1920);
#endif

	const int AXIMM_DATA_BUFF_SZ = (1920+(AXIMM_DATA_WIDTH/8)-1)/(AXIMM_DATA_WIDTH/8);
	const int PIXELS_PER_AXIMM = (AXIMM_DATA_WIDTH/8);

	ap_uint<AXIMM_DATA_WIDTH> buff[AXIMM_DATA_BUFF_SZ];
	
    int yoffset = 0;
	int loopWidth = (WidthInBytes+(AXIMM_DATA_WIDTH/8)-1)/(AXIMM_DATA_WIDTH/8);
	int widthInPix = WidthInBytes;
	int remainPix = widthInPix%(AXIMM_DATA_WIDTH/8);
	remainPix = (remainPix==0) ? (AXIMM_DATA_WIDTH/(8*1)) : (remainPix/1);

    U8 pixel = 0;
	ap_uint<AXIMM_DATA_WIDTH> bytes;

	
	forEachRow: for (int y = 0; y < Height; y++)
	{
#pragma HLS loop_tripcount max=1080
#pragma HLS loop_flatten off

		pixels2bytes: for (int x = 0; x < loopWidth; x++)
		{
#pragma HLS pipeline II=PIXELS_PER_AXIMM
			for (int i=0; i<(AXIMM_DATA_WIDTH/8); i++)
			{
				if (x<loopWidth-1 || i<remainPix) stream >> pixel;
				bytes((8*(i+1))-1, 8*i) = pixel;
			}
			buff[x] = bytes;
		}

        bytes2aximm: for (int x = 0; x < loopWidth; x++)
        {
#pragma HLS pipeline II=1
            aximm[yoffset+x] = buff[x];
        }
        yoffset += StrideInBytes/(AXIMM_DATA_WIDTH/8);
    }	
}



