/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ___HLS__VIDEO__
#error hls_video_histogram.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef _HLS_VIDEO_HISTOGRAM_
#define _HLS_VIDEO_HISTOGRAM_

namespace hls {

template<int SRC_T, int DST_T,int ROW, int COL, int N>
void Equalize(
	Mat<ROW, COL, SRC_T>	&_src,
	Mat<ROW, COL, DST_T>	&_dst,
    ap_uint<8> (&map)[N]
                      )
{
    // Assumed 8 bits per pixel
    const int NUM_STATES=4; // must be a even num
    Window<1,NUM_STATES,ap_uint<8> > addr_win;
    // The histogram needs to be able to count up to the number of pixels in the image.
    ap_uint<BitWidth<ROW*COL>::Value> hist_out[N];
    Window<1,NUM_STATES,ap_uint<BitWidth<ROW*COL>::Value> > hist_win;
    ap_uint<BitWidth<ROW*COL>::Value> hist;
    ap_uint<8> addr;
    ap_uint<8> addr_last;
    ap_uint<BitWidth<ROW*COL>::Value> hist_last;
    ap_uint<8> addr_flag;
    ap_uint<BitWidth<ROW*COL>::Value> hist_flag;
    ap_uint<8> addr_w;
    ap_uint<BitWidth<ROW*COL>::Value> hist_w;

    for(int i=0;i<NUM_STATES;i++) {
    #pragma HLS UNROLL
        addr_win(0,i)=i;// initialize with NUM_STATES different address
        hist_win(0,i)=0;
    }

    for(int i=0;i<N;i++)
        hist_out[i]=0;

    int cols=_src.cols;
    int rows=_src.rows;
    assert(rows<=ROW);
    assert(cols<=COL);
 loop_height: for(int i=0;i<rows;i++)
    {
    loop_width: for(int j=0;j<cols;j++)
        {
#pragma HLS PIPELINE
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS DEPENDENCE array inter false
            ap_uint<4> flag=NUM_STATES;
            HLS_TNAME(SRC_T) tempsrc=0;
            HLS_TNAME(DST_T) tempdst=0;
            _src.data_stream[0].read(tempsrc);
            tempdst=map[tempsrc];
            _dst.data_stream[0]<<tempdst;

            for (int m=0; m<NUM_STATES; m++) {
                if (tempsrc==addr_win(0,m)) {
                    flag = m;
                    break;
                }
            }

            latency_region:{
            #pragma HLS latency min=0 max=1
            addr_last = addr_win(0,NUM_STATES-1);
            hist_last = hist_win(0,NUM_STATES-1)+1;

            for (int m=NUM_STATES-1; m>0; m--) {
                addr = addr_win(0,m-1);
                hist = hist_win(0,m-1);
                if (m==NUM_STATES/2) {
                    addr_w = addr;
                    if (m==flag+1) {
                        hist_w = hist+1;
                    } else {
                        hist_w = hist;
                    }
                }
                if (m==flag+1) {
                    addr_flag = addr;
                    hist_flag = hist+1;
                    addr_win(0,m) = addr_flag;
                    hist_win(0,m) = hist_flag;
                } else {
                    addr_win(0,m) = addr;
                    hist_win(0,m) = hist;
                }
            }

            if (flag==NUM_STATES) {
                hist_win(0,0) = hist_out[tempsrc]+1;
                addr_win(0,0) = tempsrc;
            } else if (flag==NUM_STATES-1) {
                addr_win(0,0) = addr_last;
                hist_win(0,0) = hist_last;
            } else if (flag>=NUM_STATES/2) {
                addr_win(0,0) = addr_flag;
                hist_win(0,0) = hist_flag;
            } else {
                addr_win(0,0) = addr_w;
                hist_win(0,0) = hist_w;
            }

            hist_out[addr_w] = hist_w;
            }//latency
        }
    }
    for (int m=0; m<NUM_STATES/2; m++) {
    #pragma HLS PIPELINE
        hist_out[addr_win(0,m)]=hist_win(0,m);
    }

    // floating point probably makes sense here, because cols*rows could be large or small.
    // This could probably be only 8 mantissa bits or so, though.
    float scale = 1.f; //255.0f/(cols*rows);
    ap_uint<BitWidth<ROW*COL>::Value> sum=0;
    bool flag = false;
 loop_normalize: for(int i=0;i<N;i++)
    {
#pragma HLS PIPELINE
        // to keep consistent with opencv's implementation
        if (!flag) {
            if (hist_out[i] != 0) {
                flag = true;
                scale = 255.0f/(cols*rows - hist_out[i]);
                map[i] = 0;
                continue;
            }
        }

        sum+=hist_out[i];
        ap_uint<8> val=sr_cast< ap_uint<8> > (sum*scale);
        map[i]=val;
    }
    map[0]=0;
}

static  ap_uint<8> lut[256];
template<int SRC_T, int DST_T,int ROW, int COL>
void EqualizeHist(
		Mat<ROW, COL, SRC_T>	&_src,
		Mat<ROW, COL, DST_T>	&_dst)
{ 
#pragma HLS INLINE
    Equalize(_src, _dst, lut);
}
}
#endif


