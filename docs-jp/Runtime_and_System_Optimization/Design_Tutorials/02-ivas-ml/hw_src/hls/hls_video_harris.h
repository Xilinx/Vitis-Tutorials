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

// This code is partially derived from OpenCV:

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef ___HLS__VIDEO__
#error hls_video_harris.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef _HLS_VIDEO_HARRIS_H_
#define _HLS_VIDEO_HARRIS_H_

namespace hls {

template<int KH,int KW, typename SRC_T>
bool IsMax(Window<KH,KW,SRC_T>  & core_win)
{
#pragma HLS INLINE
    bool isMax=true;
    for(int i=0;i<KH;i++) {
        for(int j=0;j<KW;j++) {
            if(((i!=KH/2)||(j!=KW/2))&&core_win.val[i][j]>core_win.val[KH/2][KW/2])
                isMax=false;
        }
    }
    return isMax;
}
template<int KH, int KW, int SRC_T,int DST_T,int ROWS,int COLS>
void FindMax(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        HLS_TNAME(SRC_T)             threshold
                )
{
    LineBuffer<KH-1,COLS,HLS_TNAME(SRC_T)>         k_buf;
    Window<KH,KW,HLS_TNAME(SRC_T)>                 win;
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>     s;
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(DST_T)>     d;
    int rows= _src.rows;
    int cols= _src.cols;
    int ioffset=KH/2+1;
    int joffset=KH/2+1;
    for(HLS_SIZE_T i=0;i<rows+ioffset;i++)
    {
#pragma HLS LOOP_TRIPCOUNT MAX=ROWS
        for(HLS_SIZE_T j=0;j<cols+joffset;j++)
        {
#pragma HLS DEPENDENCE ARRAY INTER FALSE
#pragma HLS LOOP_TRIPCOUNT MAX=COLS
#pragma HLS LOOP_FLATTEN off
#pragma HLS PIPELINE II=1
            d.val[0]=0;
            for(int r= 0;r<KH;r++)
            {
                for(int c=0;c<KW-1;c++)
                {
                    win.val[r][c]=win.val[r][c+1];//column left shift
                }
            }
            if(i<rows&&j<cols)
            {
                win.val[0][KW-1]=k_buf.val[0][j];
                for(int buf_row= 1;buf_row< KH-1;buf_row++)
                {
                    win.val[buf_row][KW-1]=k_buf.val[buf_row][j];
                    k_buf.val[buf_row-1][j]=k_buf.val[buf_row][j];
                }
                //-------
                _src>>s;
                win.val[KH-1][KW-1]=s.val[0];
                k_buf.val[KH-2][j]=s.val[0];
            }
            else if(j>=cols)
            {

                for(int i= 1;i< KW;i++)
                    win.val[i][KW-1]=0;
            }
            if(win.val[1][1]>=threshold&&i>=KH&&j>=KW)
            {
                bool _ismax=IsMax(win);
                if(_ismax)
                {
                   // d.val[0]=s.val[0];
                    d.val[0]=255;
                }
            }

            if(i>=ioffset&&j>=joffset)
            {
                _dst<<d;
            }
        }
    }
}
template<typename KT, typename ST, int X2_T,int Y2_T,int XY_T,int DST_T,int ROWS,int COLS>
void CalCim(
		Mat<ROWS, COLS, X2_T>	    &_gradx,
		Mat<ROWS, COLS, Y2_T>	    &_grady,
		Mat<ROWS, COLS, XY_T>	    &_gradxy,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        KT k,
        ST scale
        )
{
    Scalar<HLS_MAT_CN(X2_T),HLS_TNAME(X2_T)>     gx;
    Scalar<HLS_MAT_CN(Y2_T),HLS_TNAME(Y2_T)>     gy;
    Scalar<HLS_MAT_CN(XY_T),HLS_TNAME(XY_T)>     gxy;
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> d;
    int rows= _grady.rows;
    int cols= _grady.cols;
    for(HLS_SIZE_T i=0;i<rows;i++)
    {
#pragma HLS LOOP_TRIPCOUNT MAX=ROWS
        for(HLS_SIZE_T j=0;j<cols;j++)
        {
#pragma HLS DEPENDENCE ARRAY INTER FALSE
#pragma HLS LOOP_TRIPCOUNT MAX=COLS
#pragma HLS LOOP_FLATTEN off
#pragma HLS PIPELINE II=1
            d.val[0]=0;
            _gradx>> gx;
            _grady>> gy;
            _gradxy>> gxy;
            HLS_TNAME(DST_T) lgx = sr_cast<HLS_TNAME(DST_T)>(gx.val[0])*HLS_TNAME(DST_T)(scale*scale);
            HLS_TNAME(DST_T) lgy = sr_cast<HLS_TNAME(DST_T)>(gy.val[0])*HLS_TNAME(DST_T)(scale*scale);
            HLS_TNAME(DST_T) lgxy= sr_cast<HLS_TNAME(DST_T)>(gxy.val[0])*HLS_TNAME(DST_T)(scale*scale);
            HLS_TNAME(DST_T) det = lgx*lgy - lgxy*lgxy;
            HLS_TNAME(DST_T) trac= lgx + lgy;

            HLS_TNAME(DST_T) cim = det - k*(trac)*(trac);
            d.val[0] = cim;
            _dst<<d;
        }
    }
}
template<int blockSize,int Ksize, bool NORMALIZE, class BORDERMODEL, int GT, int G2T, typename ST, typename KT,int SRC_T,int DST_T,int ROWS,int COLS>
void Harris_opr(
        Mat<ROWS, COLS, SRC_T>	    &_src,
        Mat<ROWS, COLS, DST_T>	    &_dst,
        KT k,
        ST scale)
{
#pragma HLS INLINE
#pragma HLS dataflow
#ifndef __SYNTHESIS__
    assert(Ksize==3||Ksize==5||Ksize==7);
#endif
    int rows = _src.rows;
    int cols = _src.cols;
    Mat<ROWS,COLS,SRC_T>    gray1(rows,cols);
    Mat<ROWS,COLS,SRC_T>    gray2(rows,cols);

    Mat<ROWS,COLS,GT>       grad_x(rows,cols);
    Mat<ROWS,COLS,GT>       grad_x1(rows,cols);
    Mat<ROWS,COLS,GT>       grad_x2(rows,cols);
    Mat<ROWS,COLS,GT>       grad_x3(rows,cols);
    Mat<ROWS,COLS,GT>       grad_x4(rows,cols);

    Mat<ROWS,COLS,GT>       grad_y(rows,cols);
    Mat<ROWS,COLS,GT>       grad_y1(rows,cols);
    Mat<ROWS,COLS,GT>       grad_y2(rows,cols);
    Mat<ROWS,COLS,GT>       grad_y3(rows,cols);
    Mat<ROWS,COLS,GT>       grad_y4(rows,cols);

    Mat<ROWS,COLS,G2T>      grad_xx(rows,cols);
    Mat<ROWS,COLS,G2T>      grad_yy(rows,cols);
    Mat<ROWS,COLS,G2T>      grad_xy(rows,cols);

    Mat<ROWS,COLS,G2T>      grad_gx(rows,cols);
    Mat<ROWS,COLS,G2T>      grad_gy(rows,cols);
    Mat<ROWS,COLS,G2T>      grad_gxy(rows,cols);

    Duplicate(_src,gray1,gray2);

    Sobel<1,0,Ksize,BORDERMODEL>(gray1,grad_x);
    Duplicate(grad_x,grad_x1,grad_x2);
    Duplicate(grad_x1,grad_x3,grad_x4);
    Sobel<0,1,Ksize,BORDERMODEL>(gray2,grad_y);
    Duplicate(grad_y,grad_y1,grad_y2);
    Duplicate(grad_y1,grad_y3,grad_y4);

    Mul(grad_x3,grad_x4,grad_xx);
    Mul(grad_y3,grad_y4,grad_yy);
    Mul(grad_x2,grad_y2,grad_xy);

    BoxFilter<blockSize,blockSize,NORMALIZE,BORDERMODEL>(grad_xx,grad_gx);
    BoxFilter<blockSize,blockSize,NORMALIZE,BORDERMODEL>(grad_yy,grad_gy);
    BoxFilter<blockSize,blockSize,NORMALIZE,BORDERMODEL>(grad_xy,grad_gxy);

    CalCim(grad_gx,grad_gy,grad_gxy, _dst,k, scale);
}

template<int blockSize,int Ksize,typename KT,int SRC_T,int DST_T,int ROWS,int COLS>
void CornerHarris(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        KT k)
{
#pragma HLS INLINE
    ap_uint<9> sc=(1 << ((Ksize>0 ? Ksize:3)-1))*blockSize;
#ifndef __SYNTHESIS__
    assert(SRC_T==HLS_8UC1||SRC_T==HLS_10UC1||SRC_T==HLS_32FC1);
#endif
    if(SRC_T==HLS_8UC1||SRC_T==HLS_10UC1)
    {
        ap_fixed<25,-7-Ksize> scale = (ap_fixed<25,2>(1.0f)/255) / sc;
        switch(Ksize) {
            case 3:
                Harris_opr<blockSize, Ksize,false,BORDER_REPLICATE, HLS_SC(10+1,1), 
                        HLS_SC(20+1+BitWidth<blockSize*blockSize>::Value,1)>(_src, _dst, k,scale);
                break;
            case 5:
                Harris_opr<blockSize, Ksize,false,BORDER_REPLICATE, HLS_SC(14+1,1), 
                        HLS_SC(28+1+BitWidth<blockSize*blockSize>::Value,1)>(_src, _dst, k,scale);
                break;
            case 7:
                Harris_opr<blockSize, Ksize,false,BORDER_REPLICATE, HLS_SC(18+1,1), 
                        HLS_SC(36+1+BitWidth<blockSize*blockSize>::Value,1)>(_src, _dst, k,scale);
                break;
            default: // by default Ksize=3
                Harris_opr<blockSize, Ksize,false,BORDER_REPLICATE, HLS_SC(10+1,1), 
                        HLS_SC(20+1+BitWidth<blockSize*blockSize>::Value,1)>(_src, _dst, k,scale);
        }
        //Harris_opr<blockSize, Ksize,false,BORDER_REPLICATE, MatType<ap_int<20>,1>::type_id, MatType<ap_int<40>,1>::type_id>(_src, _dst, k,scale);
    }
    else
    {
        float scale = 1.0f / sc;
        Harris_opr<blockSize, Ksize,false,BORDER_REPLICATE, HLS_32FC1, HLS_32FC1>(_src, _dst, k,scale);
    }
}

template<int blockSize,int Ksize,typename KT,int SRC_T,int DST_T,int ROWS,int COLS>
void Harris(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        KT k,
        int threshold)
{
#pragma HLS INLINE
    Mat<ROWS,COLS,HLS_32FC1> res(_src.rows,_src.cols);
    CornerHarris<blockSize,Ksize>(_src, res, k);
    FindMax<3,3>(res,_dst,threshold);
}

}
#endif


