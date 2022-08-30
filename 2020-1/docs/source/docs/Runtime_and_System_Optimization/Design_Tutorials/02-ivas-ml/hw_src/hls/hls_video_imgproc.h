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
// opencv/modules/imgproc/src/morph.cpp

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

/*
 * HLS Video Image Processing Functions Header File
 */

#ifndef ___HLS__VIDEO__
#error hls_video_improc.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef _HLS_VIDEO_IMGPROC_H_
#define _HLS_VIDEO_IMGPROC_H_

//#include <cmath>
//#include <cstdlib>

namespace hls {

#ifndef __SYNTHESIS__
inline void assert_cn(int SRC_T,int DST_T)
{
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
}
inline void assert_cn(int SRC1_T,int SRC2_T,int DST_T)
{
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
}
template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
inline void assert_size(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
}
template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
inline void assert_cn_size(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert_cn(SRC1_T,SRC2_T,DST_T);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
}
template<int ROWS, int COLS, int SRC_T, int DST_T>
inline void assert_size(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
}
template<int ROWS, int COLS, int SRC_T, int DST_T>
inline void assert_cn_size(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst)
{
    assert_cn(SRC_T,DST_T);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
}
#endif
class erode_kernel
{
    public:
    template<typename SRC_T,typename DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>					
    void apply(Window<F_HEIGHT,F_WIDTH,FILTER_T>	        &_kernel_filter,
            Window<F_HEIGHT,F_WIDTH,SRC_T>	                &_kernel_pixel,
            DST_T               				&out)
    {
#pragma HLS INLINE
        out = hls::numeric_limits<DST_T>::max();
    loop_height: for( int m=0;m< F_HEIGHT;m++)
        {
        loop_width: for( int n=0;n<F_WIDTH;n++)
            {
                DST_T temp;
                if(_kernel_filter.val[F_HEIGHT-m-1][F_WIDTH-1-n]>0)
                {
                    temp=_kernel_pixel.val[F_HEIGHT-m-1][(F_WIDTH-1-n)];
                    if (temp<out)
                        out=temp;
                }
            }
        }
    }
};

class dilate_kernel
{
    public:
    template<typename SRC_T,typename DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>					
    void apply(Window<F_HEIGHT,F_WIDTH,FILTER_T>	&_kernel_filter,
            Window<F_HEIGHT,F_WIDTH,SRC_T> 	&_kernel_pixel,
            DST_T			        	&out)
    {
#pragma HLS INLINE
    out = hls::numeric_limits<DST_T>::min();
    loop_height: for( int m=0;m< F_HEIGHT;m++)
        {
        loop_width: for( int n=0;n<F_WIDTH;n++)
            {
                DST_T temp;
                if(_kernel_filter.val[F_HEIGHT-m-1][F_WIDTH-1-n]>0)
                {
                    temp=_kernel_pixel.val[F_HEIGHT-m-1][(F_WIDTH-1-n)];
                    if (temp>out)
                        out=temp;
                }
            }
        }
    }
};

// For integer types, use reasonable ap_fixed defaults.
template <typename SRC_T, typename FILTER_T, int SIZE=0>
struct filter2d_traits {
    typedef typename fixed_type<SRC_T>::T SRC_CAST_T;
    typedef typename fixed_type<FILTER_T>::T FILTER_CAST_T;
    typedef typename x_traits<SRC_CAST_T, FILTER_CAST_T >::MULT_T MULT_T;
    typedef typename x_traits_d<MULT_T, SIZE>::ACCUM_T ACCUM_T;
    typedef typename x_traits<ACCUM_T, ap_fixed<1,1> >::MULT_T TEMP_T;
};
// For floating point types, we can make some assumptions to get II=1 accumulation.
template <int SIZE , int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O>
struct filter2d_traits<float, ap_fixed<W, I, _AP_Q, _AP_O>, SIZE> {
    typedef float SRC_CAST_T;
    typedef float FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};
template <int SIZE, int W>
struct filter2d_traits<float, ap_int<W>, SIZE> {
    typedef float SRC_CAST_T;
    typedef ap_int<W> FILTER_CAST_T;
    typedef float MULT_T;
    typedef float ACCUM_T;
    typedef float TEMP_T;
};
template <int SIZE>
struct filter2d_traits<unsigned char,float, SIZE> {
    typedef unsigned char SRC_CAST_T;
    typedef float FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};
template <int SIZE>
struct filter2d_traits<float, unsigned char, SIZE> {
    typedef float SRC_CAST_T;
    typedef unsigned char FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};
template <int SIZE>
struct filter2d_traits<float, float, SIZE> {
    typedef float SRC_CAST_T;
    typedef float FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};
template <int SIZE>
struct filter2d_traits<double, double, SIZE> {
    typedef double SRC_CAST_T;
    typedef double FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};
template <int SIZE>
struct filter2d_traits<float, double, SIZE> {
    typedef float SRC_CAST_T;
    typedef double FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};
template <int SIZE>
struct filter2d_traits<double, float, SIZE> {
    typedef double SRC_CAST_T;
    typedef float FILTER_CAST_T;
    typedef ap_fixed<64,32,AP_RND> MULT_T;
    typedef ap_fixed<64,32,AP_RND> ACCUM_T;
    typedef ap_fixed<64,32,AP_RND> TEMP_T;
};

class filter2d_kernel
{
    public:
    template<typename SRC_T,typename DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>
    void apply(Window<F_HEIGHT,F_WIDTH,FILTER_T>    &_kernel_filter,
               Window<F_HEIGHT,F_WIDTH,SRC_T>       &_kernel_pixel,
               DST_T                    &out,
               const bool               cast)
    {
#pragma HLS INLINE
	    typename filter2d_traits<SRC_T, FILTER_T>::SRC_CAST_T src_v;
	    typename filter2d_traits<SRC_T, FILTER_T>::FILTER_CAST_T filter_v; 
        typename filter2d_traits<SRC_T, FILTER_T, F_HEIGHT*F_WIDTH-1>::ACCUM_T sum=0;
        typename filter2d_traits<SRC_T, FILTER_T>::MULT_T temp=0;
        loop_height: for(int m = 0; m < F_HEIGHT; m++) {
            loop_width: for(int n = 0; n < F_WIDTH; n++) {
                src_v = _kernel_pixel.val[F_HEIGHT-m-1][F_WIDTH-1-n];
                filter_v = _kernel_filter.val[m][n];
                temp = src_v * filter_v;
                sum = sum + temp;
            }
        }
        out=sum;
    }

    template<typename SRC_T,typename DST_T,typename FILTER_T, int F_HEIGHT, int F_WIDTH>
    void apply(Window<F_HEIGHT,F_WIDTH,FILTER_T>    &_kernel_filter,
               Window<F_HEIGHT,F_WIDTH,SRC_T>       &_kernel_pixel,
               DST_T                    &out)
    {
#pragma HLS INLINE
        typename filter2d_traits<SRC_T, FILTER_T, F_HEIGHT*F_WIDTH-1>::ACCUM_T sum=0;
	    apply(_kernel_filter,_kernel_pixel,sum,true);
	    out=sr_cast<DST_T>(sum);
    }
};

template <typename SRC_T,typename SIZE_T,typename POINT_T,int HEIGHT,int WIDTH>
void getStructuringElement(
        int 				shape,
        Size_<SIZE_T> 	        ksize,
        Point_<POINT_T>            anchor,
        Window<HEIGHT,WIDTH,SRC_T> &result)
{
#pragma HLS inline
    int i, j;
    int r = 0, c = 0;
    ap_fixed<31,11,AP_RND> inv_r2 = 0;

    if( ksize.width==1&&ksize.height == 1 )
        shape = MORPH_RECT;

    if( shape == MORPH_ELLIPSE )
    {
        r = ksize.height/2;
        c = ksize.width/2;
        if(r!=0)
        {
            inv_r2 =(ap_fixed<31,11,AP_RND>) 1/(r*r) ;
        }
        else
            inv_r2=0;
    }
 loop_height: for( i = 0; i < ksize.height; i++ )
    {
        int j1 = 0, j2 = 0;
        if( shape == MORPH_RECT || (shape ==MORPH_CROSS && i == anchor.y) )
            j2 = ksize.width;
        else if( shape == MORPH_CROSS ) {
            j1 = anchor.x;
            j2 = j1 + 1;
        } else
        {
            int dy = i - r;
            if( abs(dy) <= r )
            {
			  ap_fixed<12,12,AP_RND> dxx = (c*::hls::sqrt(double(((r-dy)*(r+dy))*inv_r2)));///////saturate_cast<int>  nearly rand
                int dx=dxx;
                j1 = (c - dx)> 0?( c - dx):0;
                j2 =  (c + dx + 1)< ksize.width?(c + dx + 1):ksize.width;
            }
        }
    loop_width: for( j = 0; j < ksize.width; j++ ) {
            if(shape == MORPH_RECT || (j >= j1 && j < j2)) {
                result.val[i][j] = 1;
            } else {
                result.val[i][j] = 0;
            }
        }
    }
}


template<typename P_T,typename W_T,int W_HEIGHT, int W_WIDTH,typename BUF_T,int BUF_HEIGHT,int BUF_WIDTH >
void fill_pixelkernel(
        LineBuffer<BUF_HEIGHT,BUF_WIDTH,BUF_T>  &col_buf,
        Window<W_HEIGHT,W_WIDTH,W_T>            &kernel,
        Point_<P_T>                             curp,
        int borderType, int ref,
        int imgheight,int constval=0, bool forceconst = false)
{
#pragma HLS INLINE
 loop_fill: for(int buf_row= 0;buf_row<W_HEIGHT;buf_row++)
    {
        int  locy;
        int t = borderInterpolate(curp.y,imgheight,borderType);
        locy = ref - t;
        curp.y--;
        kernel.val[buf_row][0] = (t < 0 || forceconst) ? constval : col_buf.val[locy][0];
    }
}

template<typename OPR_KERNEL, typename BORDERMODE>
class filter_opr {
public:
template<int SRC_T, int DST_T, typename KN_T, typename POINT_T,
         int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
static void filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>		&kernel,
        Point_<POINT_T>				        _anchor,
        int rows,int cols)
    {
#pragma HLS inline
    OPR_KERNEL                                              fk_opr;
    // The main processing window
    Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>		        src_kernel_win[HLS_MAT_CN(SRC_T)];
    // The main line buffer
    LineBuffer<2*K_HEIGHT,IMG_WIDTH,HLS_TNAME(SRC_T)>	    k_buf[HLS_MAT_CN(SRC_T)];
    // A small buffer keeping a few pixels from the line
    // buffer, so that we can complete right borders correctly.
    Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>           	right_border_buf[HLS_MAT_CN(SRC_T)];
    // Temporary storage for reading from the line buffers.
    Window<K_HEIGHT,1,HLS_TNAME(SRC_T)>			            col_buf[HLS_MAT_CN(SRC_T)];

    assert(rows >= 8);
    assert(cols >= 8);
    assert(rows <= IMG_HEIGHT);
    assert(cols <= IMG_WIDTH);

#pragma HLS ARRAY_PARTITION variable=col_buf complete dim=0
#pragma HLS ARRAY_PARTITION variable=&kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_win complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=right_border_buf complete dim=0

    Point_<POINT_T> anchor;
    anchor.x=_anchor.x;
    anchor.y=_anchor.y;
    Size_<int> kernel_size;
    kernel_size.width=K_WIDTH;
    kernel_size.height=K_HEIGHT;
    normalizeAnchor(anchor,kernel_size);
    assert(anchor.x >= 0);
    assert(anchor.x <= K_WIDTH-1);
    assert(anchor.y >= 0);
    assert(anchor.y <= K_HEIGHT-1);

    int start_row; // start to output(actually output 0 on i=start_row+1)
    int row_index; // the index in k_buf for src input, determine k_buf height by 2*K_HEIGHT-row_index
    int col_index; // determine the right_border_buf width by col_index-0
    int stop_row; // stop shifting down the linebuffer
    get_parameters<K_HEIGHT, K_WIDTH>(rows, anchor, start_row, stop_row, row_index, col_index);
    assert(start_row >= 0);
    assert(start_row <= K_HEIGHT-1);

    int heightloop= rows+start_row+1;
    int widthloop = cols+K_WIDTH-1;
    Point_<int> ImagLoc;
    int channels=HLS_MAT_CN(SRC_T);

 loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
    loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
// This DEPENDENCE pragma is necessary because the border mode handling is not affine.
#pragma HLS DEPENDENCE array inter false
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
channelloop: for(HLS_CHANNEL_T k= 0; k < channels; k++)
                {
                    ImagLoc.x=j-anchor.x;
                    int x = borderInterpolate(ImagLoc.x, cols, BORDERMODE::value);
                    assert(x < cols);

                    // Fetch the column from the line buffer or right_border_buf to shift into the window.
                    for(int buf_row=0; buf_row<K_HEIGHT; buf_row++) {
                        if((i<stop_row) && (ImagLoc.x>=cols)) {
                            col_buf[k](buf_row,0)=right_border_buf[k](buf_row,cols-1-x);
                        } else {
                            col_buf[k](buf_row,0)=k_buf[k](K_HEIGHT+buf_row,x);
                        }
                    }

                    // Read in image stream and fill in k_buf with two different ways
                    if(ImagLoc.x >= 0 && ImagLoc.x < cols) {
                        if(i<=start_row) { // top border
                            HLS_TNAME(SRC_T) src_temp;
                            _src.data_stream[k] >> src_temp;
                            for(int buf_row=2*K_HEIGHT-1; buf_row>=0; buf_row--) {
                                int y = borderInterpolate(2*K_HEIGHT-1-anchor.y-buf_row, rows, BORDERMODE::value);
                                if(y==i) {
                                    k_buf[k](buf_row,x)=src_temp;
                                }
                            }
                        } else if(i<stop_row) {
                            // back up right border pixels with a window
                            //right_border_buf[k].shift_right();
                            for(int buf_row=0; buf_row<K_HEIGHT; buf_row++) {
                                for(int buf_col=K_WIDTH-1; buf_col>0; buf_col--) {
                                    if(buf_col <= col_index) {
                                         right_border_buf[k](buf_row,buf_col)=right_border_buf[k](buf_row,buf_col-1);
                                    }
                                }
                                right_border_buf[k](buf_row,0)=col_buf[k](buf_row,0);
                            }
			                      //k_buf[k].shift_down(x);
                            for(int buf_row=2*K_HEIGHT-1; buf_row>0; buf_row--) {
                                if(buf_row>row_index) {
                                    k_buf[k](buf_row,x)=k_buf[k](buf_row-1,x);
                                }
                            }
                            if(i<rows) {
                                _src.data_stream[k] >> k_buf[k](row_index,x);
                            }            
                        }            
                    }            

                    // column left shift
		            src_kernel_win[k].shift_right();

                    for(int buf_row=0; buf_row<K_HEIGHT; buf_row++){
                        if(i<=stop_row) {
                            src_kernel_win[k](buf_row,0)=col_buf[k](buf_row,0);
                        }
                        else {
                            int y = borderInterpolate((i-(stop_row-rows+1)-buf_row), rows, BORDERMODE::value);
                            src_kernel_win[k](buf_row,0)=col_buf[k](rows-1-y,0);
                        }
                    }
 
                    // figure out the output image pixel value
                    if(i > start_row && j >= K_WIDTH-1) {
        		        HLS_TNAME(DST_T) temp;
                        fk_opr.apply(kernel,src_kernel_win[k],temp);
                        _dst.data_stream[k]<<temp;
                    }
                }
            }
        }
    }

private:
template<int K_HEIGHT, int K_WIDTH, typename POINT_T>
static void get_parameters(int rows, Point_<POINT_T> anchor, int &start_row, int &stop_row, int &row_index, int &col_index)
    {
#pragma HLS inline
        switch(BORDERMODE::value) {
            case BORDER_REPLICATE::value:
                start_row=K_HEIGHT-1-anchor.y;
                row_index=K_HEIGHT;
                col_index=0;
                stop_row=rows;
                break;
            case BORDER_REFLECT::value:
                col_index=K_WIDTH-1-anchor.x-1;
                if(anchor.y<=K_HEIGHT/2) {
                    start_row=K_HEIGHT-1-anchor.y;
                    row_index=K_HEIGHT;
                    stop_row=rows;
                } else {
                    start_row=anchor.y-1;
                    row_index=2*K_HEIGHT-2*anchor.y;
                    stop_row=rows+K_HEIGHT-row_index;
                }
                break;
            case BORDER_REFLECT_101::value:
                col_index=K_WIDTH-1-anchor.x;
                if(anchor.y<=K_HEIGHT/2) {
                    start_row=K_HEIGHT-1-anchor.y;
                    row_index=K_HEIGHT;
                    stop_row=rows;
                } else {
                    start_row=anchor.y;
                    row_index=2*K_HEIGHT-2*anchor.y-1;
                    stop_row=rows+K_HEIGHT-row_index;
                }
                break;
            default: //BORDER_DEFAULT::value:
                col_index=K_WIDTH-1-anchor.x;
                if(anchor.y<=K_HEIGHT/2) {
                    start_row=K_HEIGHT-1-anchor.y;
                    row_index=K_HEIGHT;
                    stop_row=rows;
                } else {
                    start_row=anchor.y;
                    row_index=2*K_HEIGHT-2*anchor.y-1;
                    stop_row=rows+K_HEIGHT-row_index;
                }
                break;
        }
    }

};

template<typename OPR_KERNEL>
class filter_opr<OPR_KERNEL, BORDER_CONSTANT> {
public:
template<int SRC_T, int DST_T, typename KN_T, typename POINT_T,
         int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
    static void filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>		&kernel,
        Point_<POINT_T>				_anchor,
        int rows,int cols)
{
    HLS_TNAME(SRC_T) fillvalue = 0;
#pragma HLS inline
    OPR_KERNEL                                          fk_opr;
    // The main processing window
    Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>		    src_kernel_win[HLS_MAT_CN(SRC_T)];
    // The main line buffer
    LineBuffer<K_HEIGHT,IMG_WIDTH,HLS_TNAME(SRC_T)>	    k_buf[HLS_MAT_CN(SRC_T)];
    // Temporary storage for src_ker_win, to call insert_right(), use a 1 x K_HEIGHT window
    Window<1,K_HEIGHT,HLS_TNAME(SRC_T)>		            col_buf[HLS_MAT_CN(SRC_T)];

#pragma HLS ARRAY_PARTITION variable=&kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_win complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=col_buf complete dim=0

    assert(rows <= IMG_HEIGHT);
    assert(cols <= IMG_WIDTH);

    // used for buffer image pixel
    Point_<POINT_T> anchor;
    anchor.x=_anchor.x;
    anchor.y=_anchor.y;
    Size_<int> kernel_size;
    kernel_size.width=K_WIDTH;
    kernel_size.height=K_HEIGHT;
    normalizeAnchor(anchor,kernel_size);
    assert(anchor.x >= 0);
    assert(anchor.x <= K_WIDTH-1);
    assert(anchor.y >= 0);
    assert(anchor.y <= K_HEIGHT-1);

    int start_row=K_HEIGHT-1-anchor.y;
    int start_col=K_WIDTH-1-anchor.x;
    int heightloop= rows+start_row;
    int widthloop = cols+start_col;
    Point_<int> ImagLoc;
    int channels=HLS_MAT_CN(SRC_T);

 loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
    loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
channelloop:	for(HLS_CHANNEL_T k= 0; k < channels; k++)
                {
                    HLS_TNAME(SRC_T) src_temp;
                    if((j<cols) && (i<rows)) {
                        _src.data_stream[k] >> src_temp;
                    }

                    if(j<cols) {
                        for(int buf_row=K_HEIGHT-1; buf_row>0; buf_row--) {
                            col_buf[k](0,buf_row)=k_buf[k](buf_row-1,j);
                        }
                    } else {
                        for(int buf_row=K_HEIGHT-1; buf_row>=0; buf_row--) {
                            col_buf[k](0,buf_row)=fillvalue;
                        }
                    }

                    if(j<cols) {
                        if(i==0) {
                            for(int buf_row=K_HEIGHT-1; buf_row>0; buf_row--) {
                                k_buf[k](buf_row,j)=fillvalue;
                                col_buf[k](0,buf_row)=fillvalue;
                            }
                            k_buf[k](0,j)=src_temp;
                            col_buf[k](0,0)=src_temp;
                        } else {
                            if(i>=rows) {
                                src_temp=fillvalue;
                            }
                            k_buf[k].shift_down(j);
                            k_buf[k](0,j)=src_temp;
                            col_buf[k](0,0)=src_temp;
                        }
                    }

                    if(j==0) {
                        for(int buf_row=K_HEIGHT-1; buf_row>=0; buf_row--) {
                            for(int buf_col=K_WIDTH-1; buf_col>0; buf_col--) {
                                src_kernel_win[k](buf_row,buf_col)=fillvalue;
                            }
                        }
                    } else {
		                src_kernel_win[k].shift_right();
                    }
		            src_kernel_win[k].insert_right(col_buf[k].val[0]);

                    // figure out the output image pixel value
                    if(i >= start_row && j >= start_col) {
        		        HLS_TNAME(DST_T) temp;
                        fk_opr.apply(kernel,src_kernel_win[k],temp);
                        _dst.data_stream[k]<<temp;
                    }
                }
            }
        }
    }
};

template<typename OPR_KERNEL, typename BORDERMODE>
class sepfilter_opr {
private:

template<int CH, int IMG_HEIGHT, int IMG_WIDTH, int SRC_T, typename DST_T, 
         typename KNY_T, typename POINT_T, int K_HEIGHT>
static void column_filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        stream<DST_T> 				(&_dst)[CH],        
        Window<K_HEIGHT,1,KNY_T>		&kernelY,
        Point_<POINT_T>				_anchor,
        int 					rows,
        int 					cols)
{
    OPR_KERNEL						            fk_opr;
    // The main processing window
    Window<K_HEIGHT,1,HLS_TNAME(SRC_T)>			src_kernel_winY[CH];
    // A window buffer for keeping pixels value from k_buf tempory
    Window<K_HEIGHT,1,HLS_TNAME(SRC_T)>			col_buf[CH];
    // The main line buffer
    // its size depend on anchor.y
    LineBuffer<2*K_HEIGHT,IMG_WIDTH,HLS_TNAME(SRC_T)>	k_buf[CH];

    assert(rows >= 8);
    assert(cols >= 8);
    assert(rows <= IMG_HEIGHT);
    assert(cols <= IMG_WIDTH);

#pragma HLS ARRAY_PARTITION variable=col_buf complete dim=0
#pragma HLS ARRAY_PARTITION variable=&kernelY complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_winY complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1

    Point_<POINT_T> anchor;
    anchor.x=_anchor.x;
    anchor.y=_anchor.y;
    Size_<int> kernel_size;
    kernel_size.width=1;
    kernel_size.height=K_HEIGHT;
    normalizeAnchor(anchor,kernel_size);

    int start_row;
    int row_index;
    int stop_row;

    switch(BORDERMODE::value) {
        case BORDER_REPLICATE::value:
            start_row=K_HEIGHT-1-anchor.y;
            row_index=K_HEIGHT;
            stop_row=rows;
            break;
        case BORDER_REFLECT::value:
            start_row = (anchor.y<=K_HEIGHT/2) ? K_HEIGHT-1-anchor.y : anchor.y-1;
            row_index = (anchor.y<=K_HEIGHT/2) ? K_HEIGHT : 2*K_HEIGHT-2*anchor.y;
            stop_row = (anchor.y<=K_HEIGHT/2) ? rows : rows+K_HEIGHT-row_index;
            break;
        case BORDER_REFLECT_101::value:
            start_row = (anchor.y<=K_HEIGHT/2) ? K_HEIGHT-1-anchor.y : anchor.y;
            row_index = (anchor.y<=K_HEIGHT/2) ? K_HEIGHT : 2*K_HEIGHT-2*anchor.y-1;
            stop_row = (anchor.y<=K_HEIGHT/2) ? rows : rows+K_HEIGHT-row_index;
            break;
        default: //BORDER_DEFAULT::value:
            start_row = (anchor.y<=K_HEIGHT/2) ? K_HEIGHT-1-anchor.y : anchor.y;
            row_index = (anchor.y<=K_HEIGHT/2) ? K_HEIGHT : 2*K_HEIGHT-2*anchor.y-1;
            stop_row = (anchor.y<=K_HEIGHT/2) ? rows : rows+K_HEIGHT-row_index;
            break;
    }

    int heightloop = rows+start_row+1; // 1: inorder to update col_buf
    int widthloop = cols;

    loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
        loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
    // This DEPENDENCE pragma is necessary because the border mode handling is not affine.
    #pragma HLS DEPENDENCE array inter false
    #pragma HLS LOOP_FLATTEN OFF
    #pragma HLS PIPELINE
            channelloop: for(HLS_CHANNEL_T k= 0; k < CH; k++)
            {

                // Fetch the column from the line buffer to shift into the window.
                for(int buf_row=0; buf_row<K_HEIGHT; buf_row++) {
                    col_buf[k](buf_row,0)=k_buf[k](2*K_HEIGHT-1-buf_row,j);
                }

                HLS_TNAME(SRC_T) src_temp;
                if (i<rows) {
                    _src.data_stream[k] >> src_temp;
                }

                if(i<=start_row) { // top border
                    for(int buf_row=2*K_HEIGHT-1; buf_row>=0; buf_row--) {
                        int y = borderInterpolate(2*K_HEIGHT-1-anchor.y-buf_row, rows, BORDERMODE::value);
                        if(y==i) {
                            k_buf[k](buf_row,j)=src_temp;
                        }
                    }
                } else if(i<stop_row) {
                    for(int buf_row=2*K_HEIGHT-1; buf_row>0; buf_row--) {
                        if(buf_row>row_index) {
                            k_buf[k](buf_row,j)=k_buf[k](buf_row-1,j);
                        }
                    }
                    if(i<rows) {
                        k_buf[k](row_index,j) = src_temp;
                    }            
                }

                for(int buf_row=0; buf_row<K_HEIGHT; buf_row++){
                    if(i<=stop_row) {
                        src_kernel_winY[k](buf_row,0)=col_buf[k](K_HEIGHT-1-buf_row,0);
                    }
                    else { // bottom border
                        int y = borderInterpolate(i-(stop_row-rows+1)-buf_row, rows, BORDERMODE::value);
                        //src_kernel_winY[k](buf_row,0)=col_buf[k](K_HEIGHT-1-(rows-1-y),0);
                        // get the pixel index at col_buf[k](0,0)
                        int index = (rows-K_HEIGHT>0) ? rows-K_HEIGHT : 0;
                        src_kernel_winY[k](buf_row,0)=col_buf[k](y-index,0);
                    }
                }
              
                if(i > start_row) { 
                    DST_T tempY;					    
                    fk_opr.apply(kernelY,src_kernel_winY[k],tempY,false);
                    _dst[k]<<tempY;
                }
            }
        }
    }
}

template<int CH, int IMG_HEIGHT, int IMG_WIDTH, typename SRC_T, int DST_T, 
         typename KNX_T, typename POINT_T, int K_WIDTH, typename SCALE_T>
static void row_filter(
        stream<SRC_T> 				        (&_src)[CH],       
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<1,K_WIDTH,KNX_T>			    &kernelX,
        Point_<POINT_T>				        _anchor,
        int 					            rows,
        int 					            cols,
        SCALE_T                             scale)
{
    OPR_KERNEL			fk_opr;
    // The main processing window
    Window<1,K_WIDTH,SRC_T>	src_kernel_winX[CH];
    // a arrayFlatten bug with src_kernel_winX use Window<>
    SRC_T src_kernel_winX_temp[CH][K_WIDTH];
    // temporary buffer for row pixels
    // its size depend on anchor.x
    Window<1,2*K_WIDTH,SRC_T>	row_buf[CH];
    // small buffer keeping border pixels
    // it should be LUT-RAM, its size depend on anchor.x
    // specific left and right border buffer, for 
    // special case when a window size bigger than total imgage
    LineBuffer<1,K_WIDTH,SRC_T>	l_border_buf[CH];
    LineBuffer<1,K_WIDTH,SRC_T>	r_border_buf[CH];

    assert(rows >= 8);
    assert(cols >= 8);
    assert(rows <= IMG_HEIGHT);
    assert(cols <= IMG_WIDTH);

#pragma HLS ARRAY_PARTITION variable=&kernelX complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_winX complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_winX_temp complete dim=0
#pragma HLS ARRAY_PARTITION variable=row_buf complete dim=0
#pragma HLS ARRAY_PARTITION variable=l_border_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=r_border_buf complete dim=1

    Point_<POINT_T> anchor;
    anchor.x=_anchor.x;
    anchor.y=_anchor.y;
    Size_<int> kernel_size;
    kernel_size.width=K_WIDTH;
    kernel_size.height=1;
    normalizeAnchor(anchor,kernel_size);

    // parameters for different border modes and anchor position
    // buffer left border pixels to l_border_buf, then start read l_border_buf to create left border
    int left_border;
    // buffer right border pixels to r_border_buf, then start read r_border_buf to create right border
    int right_border;
    // index for writing src_temp to row_buf, then shift row_buf pixel left
    int index;
    // start loop trip to real processing
    int start;
    // stop shift pixel from row_buf, start read l_border_buf to create right border
    int stop;

    switch(BORDERMODE::value) {
        case BORDER_REPLICATE::value:
            left_border = 0;
            right_border = cols-1;
            index = anchor.x;
            break;
        case BORDER_REFLECT::value:
            left_border = (anchor.x-1>0) ? anchor.x-1: 0;
            right_border = cols-(K_WIDTH-anchor.x)+1;
            index = (2*anchor.x-1>0) ? 2*anchor.x-1: 0;
            break;
        case BORDER_REFLECT_101::value:
            left_border = anchor.x;
            right_border = cols-(K_WIDTH-anchor.x);
            index = 2*anchor.x;
            break;
        default: //BORDER_DEFAULT::value:
            left_border = anchor.x;
            right_border = cols-(K_WIDTH-anchor.x);
            index = 2*anchor.x;
            break;
    }

    // for special case when a window size bigger than total imgage
    right_border = right_border<0 ? 0 : right_border;
    start = K_WIDTH+left_border;
    stop = cols+index;
    int heightloop= rows;
    int widthloop = cols+start;
    SRC_T src_temp;

    loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
        loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
    // This DEPENDENCE pragma is necessary because the border mode handling is not affine.
    #pragma HLS DEPENDENCE array inter false
    #pragma HLS LOOP_FLATTEN OFF
    #pragma HLS PIPELINE
            channelloop: for(HLS_CHANNEL_T k=0; k<CH; k++)
            {
                int x = borderInterpolate(j-index-1, cols, BORDERMODE::value);

                if (j<cols)
                    _src[k] >> src_temp;

		            //src_kernel_winX[k].shift_right();
                for (int buf_col=K_WIDTH-1; buf_col>0; buf_col--) {
                    src_kernel_winX_temp[k][buf_col] = src_kernel_winX_temp[k][buf_col-1];
                }
                  
                if (j<=left_border) {
                    l_border_buf[k](0,j) = src_temp;
                }
                if ((j>=right_border) && (j<cols)) {
                    r_border_buf[k](0,cols-1-j) = src_temp;
                }

                if ((j>left_border) && (j<=index)) { // create left border
                    //src_kernel_winX[k](0,0) = l_border_buf[k](0,x);
                    src_kernel_winX_temp[k][0] = l_border_buf[k](0,x);
                } else if (j<=stop) { // j==index+1, row_buf[k](0,0) is the first pixel c0
                    //src_kernel_winX[k][0](0,0) = row_buf[k](0,0);
                    src_kernel_winX_temp[k][0] = row_buf[k](0,0);
                } else { // create right border
                    //]src_kernel_winX[k][0](0,0) = r_border_buf[k](0,cols-1-x);
                    src_kernel_winX_temp[k][0] = r_border_buf[k](0,cols-1-x);
                }

                for (int buf_col=0; buf_col<2*K_WIDTH; buf_col++) {
                    if (buf_col<index)
                        row_buf[k](0,buf_col) = row_buf[k](0,buf_col+1);
                }
                row_buf[k](0,index) = src_temp;

                for (int buf_col=0; buf_col<K_WIDTH; buf_col++) {
                    src_kernel_winX[k](0,buf_col) = src_kernel_winX_temp[k][buf_col];
                }

                if(j >= start) { 
                    typedef typename filter2d_traits<SRC_T, KNX_T, K_WIDTH*1-1>::TEMP_T tempX_T;
                    tempX_T tempX;
                    fk_opr.apply(kernelX,src_kernel_winX[k],tempX,false);
                    _dst.data_stream[k]<<sr_cast<HLS_TNAME(DST_T)>(tempX*scale);
                }
            }
        }
    }
}

public:
template<int SRC_T, int DST_T, typename KNX_T, typename KNY_T, typename POINT_T,
         int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH, typename SCALE_T>
static void filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<1,K_WIDTH,KNX_T>			&kernelX,
        Window<K_HEIGHT,1,KNY_T>		&kernelY,
        Point_<POINT_T>				    _anchor,
        int 					        rows,
	    int 					        cols,
	    SCALE_T                         scale)

{
#pragma HLS inline
#pragma HLS DATAFLOW 

    typedef typename filter2d_traits<HLS_TNAME(SRC_T), KNY_T, K_HEIGHT*1-1>::TEMP_T tempY_T;
    stream<tempY_T> 			tempY_stream[HLS_MAT_CN(SRC_T)];        

    column_filter<HLS_MAT_CN(SRC_T)>(_src, tempY_stream, kernelY, Point_<POINT_T>(0,_anchor.y), rows, cols); 

    row_filter<HLS_MAT_CN(SRC_T)>(tempY_stream, _dst, kernelX, Point_<POINT_T>(_anchor.x,0), rows, cols, scale);
}

template<int SRC_T, int DST_T, typename KNX_T, typename KNY_T, typename POINT_T,
         int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
static void filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<1,K_WIDTH,KNX_T>			&kernelX,
        Window<K_HEIGHT,1,KNY_T>		&kernelY,
        Point_<POINT_T>				    _anchor,
        int 					        rows,
	    int 					        cols)

{
#pragma HLS inline
    filter(_src, _dst, kernelX, kernelY, _anchor, rows, cols, 1);
}

};

template<typename OPR_KERNEL>
class sepfilter_opr<OPR_KERNEL, BORDER_CONSTANT> {
public:
template<int SRC_T, int DST_T, typename KNX_T, typename KNY_T, typename POINT_T,
         int IMG_HEIGHT, int IMG_WIDTH, int K_HEIGHT, int K_WIDTH, typename SCALE_T>
    static void filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<1,K_WIDTH,KNX_T>			    &kernelX,
        Window<K_HEIGHT,1,KNY_T>		    &kernelY,
        Point_<POINT_T>				        _anchor,
        int 					            rows,
	    int 					            cols,
	    SCALE_T                             scale)
{
#pragma HLS inline
    HLS_TNAME(SRC_T) fillvalue = 0;
    OPR_KERNEL                                              fk_opr;
    typedef typename filter2d_traits<HLS_TNAME(SRC_T), KNY_T, K_HEIGHT*1-1>::TEMP_T tempY_T;
    // The two main processing window
    Window<K_HEIGHT,1,HLS_TNAME(SRC_T)>		    src_kernel_winY[HLS_MAT_CN(SRC_T)];
    Window<1,K_WIDTH,tempY_T>		              src_kernel_winX[HLS_MAT_CN(SRC_T)];
    tempY_T                                   src_kernel_winX_temp[HLS_MAT_CN(SRC_T)][K_WIDTH];
    // The main line buffer
    LineBuffer<K_HEIGHT,IMG_WIDTH,HLS_TNAME(SRC_T)>	    k_buf[HLS_MAT_CN(SRC_T)];

#pragma HLS ARRAY_PARTITION variable=&kernelX complete dim=0
#pragma HLS ARRAY_PARTITION variable=&kernelY complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_winX complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_winX_temp complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_winY complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1

    assert(rows <= IMG_HEIGHT);
    assert(cols <= IMG_WIDTH);

    // used for buffer image pixel
    Point_<POINT_T> anchor;
    anchor.x=_anchor.x;
    anchor.y=_anchor.y;
    Size_<int> kernel_size;
    kernel_size.width=K_WIDTH;
    kernel_size.height=K_HEIGHT;
    normalizeAnchor(anchor,kernel_size);

    int heightloop= rows+K_HEIGHT-1;
    int widthloop = cols+K_WIDTH-1;
    Point_<int> ImagLoc;
    int channels=HLS_MAT_CN(SRC_T);

    loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
        loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
    #pragma HLS LOOP_FLATTEN OFF
    #pragma HLS PIPELINE
            channelloop: for(HLS_CHANNEL_T k= 0; k < channels; k++) {
                //fill data x,y are the coordinate in the image, it could be negative.    
                //For example (-1,-1) represents the interpolation pixel.
                ImagLoc.x=j-anchor.x;
                ImagLoc.y=i-anchor.y;
                
                //row filter window shift right  
                //src_kernel_winX[k].shift_right(); 
                for(int buf_col=K_WIDTH-1; buf_col>=1; buf_col--) {
                    src_kernel_winX_temp[k][buf_col] = src_kernel_winX_temp[k][buf_col-1];
                }
                
                if(ImagLoc.x >= 0 && ImagLoc.x < cols) {
                    for(int buf_row= K_HEIGHT-1;buf_row >= 1;buf_row--) {
                        src_kernel_winY[k](buf_row,0)=k_buf[k](buf_row-1,ImagLoc.x);
                        k_buf[k](buf_row,ImagLoc.x)=k_buf[k](buf_row-1,ImagLoc.x);
                    }
                    //k_buf[k].shift_down(ImagLoc.x);
                    HLS_TNAME(SRC_T) temp=fillvalue;
                    if(ImagLoc.y >= 0 && ImagLoc.y < rows) {
                        _src.data_stream[k] >> temp;
                    }
                    src_kernel_winY[k](0,0)=temp;
                    k_buf[k](0,ImagLoc.x)=temp;
                } 
                
                // figure out the output image pixel value
                //column filter processing between interval of 
                //j in [anchor.x, cols_anchor.x] when i>=K_HEIGHT-1
                //accordingly,ImagLoc.x in [0,cols-1]
                if(ImagLoc.x < 0 || ImagLoc.x >= cols) {
                    //src_kernel_winX[k](0,0) = fillvalue;
                    src_kernel_winX_temp[k][0] = fillvalue;
                } else if(i >= (K_HEIGHT-1)) {
                    tempY_T tempY;
                    fk_opr.apply(kernelY,src_kernel_winY[k],tempY,false);
                    //src_kernel_winX[k](0,0) = tempY;
                    src_kernel_winX_temp[k][0] = tempY;
                }

                for (int buf_col=0; buf_col<K_WIDTH; buf_col++) {
                    src_kernel_winX[k](0,buf_col) = src_kernel_winX_temp[k][buf_col];
                }

                //row filter processing between interval of j in[K_WIDTH-1, cols+K_WIDTH-1] when i>=K_HEIGHT
                //wait K_WIDTH-anchor.x clocks after the column filter
                if(i >= (K_HEIGHT-1) && j >= (K_WIDTH-1)) { 
                    typename filter2d_traits<tempY_T, KNX_T, 1*K_WIDTH>::ACCUM_T tempX;
                    fk_opr.apply(kernelX,src_kernel_winX[k],tempX,false);
                    _dst.data_stream[k]<<sr_cast<HLS_TNAME(DST_T)>(tempX*scale);
                }
            }
        }
    }
}

template<int SRC_T, int DST_T, typename KNX_T, typename KNY_T, typename POINT_T,
         int IMG_HEIGHT, int IMG_WIDTH, int K_HEIGHT, int K_WIDTH>
    static void filter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<1,K_WIDTH,KNX_T>			    &kernelX,
        Window<K_HEIGHT,1,KNY_T>		    &kernelY,
        Point_<POINT_T>				        _anchor,
        int 					            rows,
	    int 					            cols)
{
#pragma HLS inline
    filter(_src, _dst, kernelX, kernelY, _anchor, rows, cols, 1);
}

};

template<typename OPR_KERNEL, typename BORDERMODE, int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void filter_opr_flow_more(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>		&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>		&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>		&_kernel,
        Point_<POINT_T>				&anchor,
        int rows, int cols
        )
{
    filter_opr<OPR_KERNEL, BORDERMODE>::filter(_src,_dst,_kernel,anchor,rows,cols);
}
template<typename OPR_KERNEL, typename BORDERMODE, int ITERATIONS,int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void filter_opr_iter(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>		&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>		&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>		&_kernel,
        Point_<POINT_T>				&anchor,
        int rows,int cols )
{
#pragma HLS DATAFLOW
    Mat<IMG_HEIGHT,IMG_WIDTH,DST_T> internal[ITERATIONS-1];
#pragma HLS array partition variable=internal dim=1
    filter_opr_flow_more<OPR_KERNEL,BORDERMODE,SRC_T,DST_T,KN_T,POINT_T>(_src,internal[0],_kernel,anchor,rows,cols);
loop_iterations: for(int iter=0;iter<ITERATIONS-2;iter++)
    {
#pragma HLS UNROLL
        filter_opr_flow_more<OPR_KERNEL,BORDERMODE,SRC_T,DST_T,KN_T,POINT_T>(internal[iter],internal[iter+1],_kernel,anchor,rows, cols);
    }
    filter_opr_flow_more<OPR_KERNEL,BORDERMODE,SRC_T,DST_T,KN_T,POINT_T>(internal[ITERATIONS-2],_dst,_kernel,anchor,rows,cols);
}
template<typename OPR_KERNEL, typename BORDERMODE, int SRC_T, int DST_T,
    int IMG_HEIGHT,int IMG_WIDTH>
void morp_opr(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	        &_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>  	&_dst)
{
#pragma HLS inline
    int rows=_src.rows;
    int cols=_src.cols;
    Size_<int> size;
    Point_<int>	anchor;
    anchor.x=1;
    anchor.y=1;
    size.width=2*1+1;
    size.height=2*1+1;
    Window<2*1+1,2*1+1,unsigned char>  temp_kernel;
    getStructuringElement<unsigned char,int,int,2*1+1,2*1+1>(MORPH_RECT, size, anchor,temp_kernel);
    //iterations=1;
    filter_opr<OPR_KERNEL, BORDERMODE>::filter(_src,_dst,temp_kernel,anchor,rows,cols);
}
template<typename OPR_KERNEL, typename BORDERMODE, int Shape_type, int ITERATIONS, int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void morp_opr(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel,
        Point_<POINT_T>			anchor)
{
#pragma HLS INLINE
    int rows=_src.rows;
    int cols=_src.cols;
    if((Shape_type==MORPH_RECT)&&ITERATIONS>1)
    {
        Size_<int> size;
        anchor.x=ITERATIONS*(anchor.x);
        anchor.y=ITERATIONS*(anchor.y);
        size.width=K_WIDTH+ITERATIONS*(K_WIDTH-1);
        size.height=K_HEIGHT+ITERATIONS*(K_HEIGHT-1);
        Window<K_WIDTH+ITERATIONS*(K_WIDTH-1),K_HEIGHT+ITERATIONS*(K_HEIGHT-1),KN_T>  temp_kernel;
        getStructuringElement<KN_T,int,int,K_WIDTH+ITERATIONS*(K_WIDTH-1),K_HEIGHT+ITERATIONS*(K_HEIGHT-1)>(MORPH_RECT, size, anchor,temp_kernel);
        //iterations=1;
        filter_opr<OPR_KERNEL, BORDERMODE>::filter(_src,_dst,temp_kernel,anchor,rows,cols);
    }
    else if(ITERATIONS==1)
    {
        filter_opr<OPR_KERNEL, BORDERMODE>::filter(_src,_dst,_kernel,anchor,rows,cols);
    }
    else if(ITERATIONS>=2)
    {
        filter_opr_iter<OPR_KERNEL, BORDERMODE, ITERATIONS,SRC_T,DST_T,KN_T,POINT_T>(_src,_dst,_kernel,anchor,rows,cols);
    }       
}
//void cvErode(
//		const CvArr* src,
//		CvArr* dst,
//		IplConvKernel* element=NULL,
//		int iterations=1 );---OPencv
template<int SRC_T, int DST_T, int IMG_HEIGHT,int IMG_WIDTH>
void Erode(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    morp_opr<erode_kernel,BORDER_REPLICATE>(_src,_dst);
}
template<int Shape_type,int ITERATIONS,int SRC_T, int DST_T, typename KN_T,int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void Erode(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel)
{
#pragma HLS INLINE
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    Point_<int> anchor(K_WIDTH/2,K_HEIGHT/2);
    morp_opr<erode_kernel,BORDER_REPLICATE,Shape_type,ITERATIONS>(_src,_dst,_kernel,anchor);
}
//void cvDilate(
//	const CvArr* src,
//	CvArr* dst, IplConvKernel* element=NULL,
//	int iterations=1 );----OPencv
template<int SRC_T, int DST_T, int IMG_HEIGHT,int IMG_WIDTH>
void Dilate(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    morp_opr<dilate_kernel,BORDER_REPLICATE>(_src,_dst);
}
template<int Shape_type,int ITERATIONS,int SRC_T, int DST_T, typename KN_T,int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void Dilate(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>	&_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel)
{
#pragma HLS INLINE
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    Point_<int> anchor(K_WIDTH/2,K_HEIGHT/2);
    morp_opr<dilate_kernel,BORDER_REPLICATE,Shape_type,ITERATIONS>(_src,_dst,_kernel,anchor);
}
//void cvFilter2D(const CvArr* src, CvArr* dst,
//		const CvMat* kernel,
//		CvPoint anchor=cvPoint(-1,-1));------oPencv
template<typename BORDERMODE, int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void Filter2D(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>  &_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel,
        Point_<POINT_T>			anchor)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    int rows=_src.rows;
    int cols=_src.cols;
    filter_opr<filter2d_kernel, BORDERMODE>::filter(_src,_dst,_kernel,anchor,rows,cols);
}

template<int SRC_T, int DST_T, typename KN_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void Filter2D(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>  &_dst,
        Window<K_HEIGHT,K_WIDTH,KN_T>	&_kernel,
        Point_<POINT_T>			anchor)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    int rows=_src.rows;
    int cols=_src.cols;
    filter_opr<filter2d_kernel, BORDER_DEFAULT>::filter(_src,_dst,_kernel,anchor,rows,cols);
}

template<typename BORDERMODE, int SRC_T, int DST_T, typename KNX_T, typename KNY_T,
typename POINT_T, int IMG_HEIGHT, int IMG_WIDTH, int K_HEIGHT, int K_WIDTH, typename SCALE_T>
void SepFilter2D(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T> &_dst,
        Window<1,K_WIDTH,KNX_T>			&_kernelX,
        Window<K_HEIGHT,1,KNY_T>		&_kernelY,
        Point_<POINT_T>				    anchor,
        SCALE_T                         scale)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    int rows=_src.rows;
    int cols=_src.cols;
    sepfilter_opr<filter2d_kernel, BORDERMODE>::filter(_src,_dst,_kernelX,_kernelY,anchor,rows,cols,scale);
}

template<typename BORDERMODE, int SRC_T, int DST_T, typename KNX_T, typename KNY_T,
typename POINT_T, int IMG_HEIGHT, int IMG_WIDTH, int K_HEIGHT, int K_WIDTH>
void SepFilter2D(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T> &_dst,
        Window<1,K_WIDTH,KNX_T>			&_kernelX,
        Window<K_HEIGHT,1,KNY_T>		&_kernelY,
        Point_<POINT_T>				    anchor)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    int rows=_src.rows;
    int cols=_src.cols;
    sepfilter_opr<filter2d_kernel, BORDERMODE>::filter(_src,_dst,_kernelX,_kernelY,anchor,rows,cols);
}

template<int SRC_T, int DST_T, typename KNX_T, typename KNY_T, typename POINT_T,
int IMG_HEIGHT,int IMG_WIDTH,int K_HEIGHT,int K_WIDTH>
void SepFilter2D(
        Mat<IMG_HEIGHT, IMG_WIDTH, SRC_T>	&_src,
        Mat<IMG_HEIGHT, IMG_WIDTH, DST_T>  	&_dst,
        Window<1,K_WIDTH,KNX_T>			&_kernelX,
        Window<K_HEIGHT,1,KNY_T>		&_kernelY,
        Point_<POINT_T>				    anchor)
{
#ifndef __SYNTHESIS__
    assert_cn(SRC_T,DST_T);
#endif
    int rows=_src.rows;
    int cols=_src.cols;
    sepfilter_opr<filter2d_kernel, BORDER_DEFAULT>::filter(_src,_dst,_kernelX,_kernelY,anchor,rows,cols);
}

//----------------------CvtColor---------------

class CvtColor_mode {
public:
    enum values {RGB2GRAY,BGR2GRAY,GRAY2RGB,GRAY2BGR,
                 BGR2XYZ, RGB2XYZ, XYZ2BGR, XYZ2RGB,
                 BGR2YCrCb, RGB2YCrCb, YCrCb2BGR, YCrCb2RGB,
                 BGR2HSV, RGB2HSV, HSV2BGR, HSV2RGB,
                 BGR2HLS, RGB2HLS, HLS2BGR, HLS2RGB};
    typedef void isCvtColorMode;
};

}

class HLS_RGB2GRAY : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::RGB2GRAY; };
class HLS_BGR2GRAY : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::BGR2GRAY; };
class HLS_GRAY2RGB : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::GRAY2RGB; };
class HLS_GRAY2BGR : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::GRAY2BGR; };

class HLS_RGB2XYZ : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::RGB2XYZ; };
class HLS_BGR2XYZ : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::BGR2XYZ; };
class HLS_XYZ2RGB : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::XYZ2RGB; };
class HLS_XYZ2BGR : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::XYZ2BGR; };

class HLS_RGB2YCrCb : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::RGB2YCrCb; };
class HLS_BGR2YCrCb : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::BGR2YCrCb; };
class HLS_YCrCb2RGB : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::YCrCb2RGB; };
class HLS_YCrCb2BGR : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::YCrCb2BGR; };

class HLS_RGB2HSV : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::RGB2HSV; };
class HLS_BGR2HSV : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::BGR2HSV; };
class HLS_HSV2RGB : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::HSV2RGB; };
class HLS_HSV2BGR : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::HSV2BGR; };

class HLS_RGB2HLS : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::RGB2HLS; };
class HLS_BGR2HLS : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::BGR2HLS; };
class HLS_HLS2RGB : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::HLS2RGB; };
class HLS_HLS2BGR : public hls::CvtColor_mode { public: static const values value = hls::CvtColor_mode::HLS2BGR; };

namespace hls {

    template <typename T> class CvtColor_traits {
        // Only supported explicitly for types below.
    };

    template <> class CvtColor_traits<unsigned char> {
    public:
        typedef unsigned char T;
        typedef ap_fixed<25,3> COEFF_T; // Integer versions in OpenCV are computed with 14 fractional bits.
    	typedef ap_ufixed<20,1,AP_RND> INV_T;
        //const unsigned int delta;
        const int delta;// modify unsigned int to int, for YCrCb2rgb, there is (Cr-CvtColor_traits<SRC_T>().delta) return a unsigned int, which should be int.
        const unsigned int hrange;
        const unsigned int scale;//255
        const INV_T inv_scale;//1/255
        const INV_T inv_hrange;// 6/180
        typedef typename x_traits<COEFF_T,  typename fixed_type<T>::T >::MULT_T MULT_T;
        typedef typename x_traits_d<MULT_T, 3>::ACCUM_T ACCUM_T;
        typedef typename x_traits<typename fixed_type<T>::T, typename fixed_type<T>::T>::SUB_T SUB_T;
        typedef typename x_traits<typename fixed_type<T>::T, typename fixed_type<T>::T>::ADD_T ADD_T;
        typedef typename x_traits<ap_fixed<17,17>, INV_T>::MULT_T H_MULT_T;
        typedef typename x_traits<typename fixed_type<T>::T, INV_T>::MULT_T S_MULT_T;
        CvtColor_traits() : delta(128), hrange(180), scale(255),
			    inv_scale(0.0039215686274509804), inv_hrange(0.0333333333333333333) {};//fixme, need values for 10, 12 etc bits.
    };
    template <int N> class CvtColor_traits< ap_uint<N> > {
    public:
        typedef ap_uint<N> T;
        typedef ap_fixed<25,3> COEFF_T; // Integer versions in OpenCV are computed with 14 fractional bits.
    	typedef ap_ufixed<20,1,AP_RND> INV_T;
        //const unsigned int delta;
        const int delta;
        const unsigned int hrange;
        const unsigned int scale;//255
        const INV_T inv_scale;//1/255
        const INV_T inv_hrange;// 6/180
        typedef typename x_traits<COEFF_T,  typename fixed_type<T>::T >::MULT_T MULT_T;
        typedef typename x_traits_d<MULT_T, 3>::ACCUM_T ACCUM_T;
        typedef typename x_traits<typename fixed_type<T>::T, typename fixed_type<T>::T>::SUB_T SUB_T;
        typedef typename x_traits<typename fixed_type<T>::T, typename fixed_type<T>::T>::ADD_T ADD_T;
        typedef typename x_traits<ap_fixed<17,17>, INV_T>::MULT_T H_MULT_T;
        typedef typename x_traits<typename fixed_type<T>::T, INV_T>::MULT_T S_MULT_T;
        CvtColor_traits() : delta(1 << (N-1)), hrange(180),
                            scale(::hls::numeric_limits<T>::max()),
                            inv_scale(1.0/::hls::numeric_limits<T>::max()),
                            inv_hrange(0.0333333333333333333) {};//fixme, need values for 10, 12 etc bits.
    };
    template <> class CvtColor_traits<float> {
    public:
        typedef float T;
        typedef float COEFF_T;
        typedef float INV_T;
        const float delta;
        const float hrange;
        const float scale;
        const INV_T inv_scale;
        const INV_T inv_hrange;// 6/360
        typedef float MULT_T;
        typedef float ACCUM_T;
        typedef float SUB_T;
        typedef float ADD_T;
        typedef float H_MULT_T;
        typedef float S_MULT_T;
        CvtColor_traits() : delta(0.5f), hrange(360), scale(1), inv_scale(1),
			    inv_hrange(0.0166666666666666667) {};
    };
    template <> class CvtColor_traits<double> {
    public:
        typedef double T;
        typedef double COEFF_T;
        typedef double INV_T;
        const double delta;
        const double hrange;
        const double scale;
        const INV_T inv_scale;
        const INV_T inv_hrange;// 6/360
        typedef double MULT_T;
        typedef double ACCUM_T;
        typedef double SUB_T;
        typedef double ADD_T;
        typedef double H_MULT_T;
        typedef double S_MULT_T;
        CvtColor_traits() : delta(0.5f), hrange(360), scale(1), inv_scale(1), 
			    inv_hrange(0.0166666666666666667) {};
};

template <typename SRC_T, typename DST_T>
class ColorConverter {
public:
    template <int CN1>
    static void convert(Scalar<CN1,SRC_T>& _src, DST_T& result, Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > par) {
#pragma HLS inline
        typename CvtColor_traits<SRC_T>::MULT_T b,g,r;
        r=par.val[0]*_src.val[0];
        b=par.val[1]*_src.val[1];
        g=par.val[2]*_src.val[2];
        typename CvtColor_traits<SRC_T>::ACCUM_T c;
        c=r+g+b;
        result=sr_cast<DST_T> (c);
    }
};

template <typename SRC_T, typename DST_T>
class MaxMin{
public:
    template <int CN1>
    static void max(Scalar<CN1,SRC_T>& _src, DST_T& result) {
#pragma HLS inline
	//result=HLS_8U_MIN;
	for(int i=0; i<CN1; i++) {
	  result=(_src.val[i]>result) ? _src.val[i] : result;
	}
    }

    template <int CN1>
    static void min(Scalar<CN1,SRC_T>& _src, DST_T& result) {
#pragma HLS inline
	//result=HLS_8U_MAX;
	for(int i=0; i<CN1; i++) {
	  result=(_src.val[i]<result) ? _src.val[i] : result;
	}
    }
};

template <typename CONVERSION, typename SRC_T, typename DST_T>
class kernel_CvtColor {};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_BGR2GRAY, SRC_T, DST_T> {
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > par;
public:
    kernel_CvtColor() : par(0.114,0.587,0.299) {};
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[0], par);
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_RGB2GRAY, SRC_T, DST_T> {
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > par;
public:
    kernel_CvtColor() : par(0.299,0.587,0.114) {};
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[0], par);
    }
};
template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_GRAY2RGB, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
#pragma HLS inline
        _dst.val[0] = sr_cast<DST_T> (_src.val[0]);
        _dst.val[1] = sr_cast<DST_T> (_src.val[0]);
        _dst.val[2] = sr_cast<DST_T> (_src.val[0]);
    }
};
template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_GRAY2BGR, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
#pragma HLS inline
        _dst.val[0] = sr_cast<DST_T> (_src.val[0]);
        _dst.val[1] = sr_cast<DST_T> (_src.val[0]);
        _dst.val[2] = sr_cast<DST_T> (_src.val[0]);
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_RGB2XYZ, SRC_T, DST_T> {
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > parX;
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > parY;
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > parZ;
public:
    kernel_CvtColor() : parX(0.412453, 0.357580, 0.180423),
                        parY(0.212671, 0.715160, 0.072169),
                        parZ(0.019334, 0.119193, 0.950227) {};
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[0], parX);
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[1], parY);
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[2], parZ);
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_BGR2XYZ, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> src2;
        src2.val[0] = _src.val[2];
        src2.val[1] = _src.val[1];
        src2.val[2] = _src.val[0];
        kernel_CvtColor<HLS_RGB2XYZ,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(src2, _dst);
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_XYZ2RGB, SRC_T, DST_T> {
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > parR;
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > parG;
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > parB;
public:
    kernel_CvtColor() : parR( 3.240479, -1.53715, -0.498535),
                        parG(-0.969256,  1.875991, 0.041556),
                        parB( 0.055648, -0.204043, 1.057311) {};
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[0], parR);
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[1], parG);
        ColorConverter<SRC_T,DST_T>::convert(_src, _dst.val[2], parB);
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_XYZ2BGR, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> dst2;
        kernel_CvtColor<HLS_XYZ2RGB,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(_src, dst2);
        _dst.val[0] = dst2.val[2];
        _dst.val[1] = dst2.val[1];
        _dst.val[2] = dst2.val[0];
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_RGB2YCrCb, SRC_T, DST_T> {
    Scalar<3, typename CvtColor_traits<SRC_T>::COEFF_T > par;
    typename CvtColor_traits<SRC_T>::COEFF_T Cr;
    typename CvtColor_traits<SRC_T>::COEFF_T Cb;
public:
    kernel_CvtColor() : par(0.299,0.587,0.114),Cr(.713),Cb(.564) {};
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        SRC_T R = _src.val[0];
        SRC_T G = _src.val[1];
        SRC_T B = _src.val[2];
        DST_T Y;
        ColorConverter<SRC_T,DST_T>::convert(_src, Y, par);
        _dst.val[0] = Y;
        _dst.val[1] = sr_cast<DST_T>((R-Y)*Cr+ CvtColor_traits<SRC_T>().delta);
        _dst.val[2] = sr_cast<DST_T>((B-Y)*Cb+ CvtColor_traits<SRC_T>().delta);
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_BGR2YCrCb, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> src2;
        src2.val[0] = _src.val[2];
        src2.val[1] = _src.val[1];
        src2.val[2] = _src.val[0];
        kernel_CvtColor<HLS_RGB2YCrCb,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(src2, _dst);
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_YCrCb2RGB, SRC_T, DST_T> {
    typename CvtColor_traits<SRC_T>::COEFF_T parGr;
    typename CvtColor_traits<SRC_T>::COEFF_T parGb;
    typename CvtColor_traits<SRC_T>::COEFF_T parCr;
    typename CvtColor_traits<SRC_T>::COEFF_T parCb;
public:
    kernel_CvtColor() : parGr(-0.714), parGb(-0.344), parCr(1.403), parCb(1.773) {};
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        SRC_T Y = _src.val[0];
        SRC_T Cr = _src.val[1];
        SRC_T Cb = _src.val[2];
        _dst.val[0] = sr_cast<DST_T> (Y + parCr * (Cr-CvtColor_traits<SRC_T>().delta));
        _dst.val[1] = sr_cast<DST_T> (Y + parGr * (Cr-CvtColor_traits<SRC_T>().delta) + parGb * (Cb-CvtColor_traits<SRC_T>().delta));
        _dst.val[2] = sr_cast<DST_T> (Y + parCb * (Cb-CvtColor_traits<SRC_T>().delta));
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_YCrCb2BGR, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> dst2;
        kernel_CvtColor<HLS_YCrCb2RGB,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(_src, dst2);
        _dst.val[0] = dst2.val[2];
        _dst.val[1] = dst2.val[1];
        _dst.val[2] = dst2.val[0];
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_RGB2HSV, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
	#pragma HLS inline
        SRC_T R = _src.val[0];
        SRC_T G = _src.val[1];
        SRC_T B = _src.val[2];
	typename CvtColor_traits<DST_T>::H_MULT_T H;
	typename CvtColor_traits<DST_T>::S_MULT_T S;
	typename CvtColor_traits<DST_T>::SUB_T sub;
	typename CvtColor_traits<DST_T>::INV_T inv_max = 0;
	typename CvtColor_traits<DST_T>::INV_T inv_diff = 0;
	DST_T max = B;
	DST_T min = B;
	DST_T diff;

	MaxMin<SRC_T,DST_T>::max(_src, max);
	MaxMin<SRC_T,DST_T>::min(_src, min);
	diff=max-min;

	if(max!=0)
	  inv_max = (typename CvtColor_traits<DST_T>::INV_T) 1/max;
	else
	  inv_max = 0;

	S=inv_max*diff;	

	if(diff!=0)
	  inv_diff = (typename CvtColor_traits<DST_T>::INV_T) 1/diff;
	else
	  inv_diff = 0;
	
	sub=(max==R) ? (G-B) : (max==G) ? (B-R) : (R-G); 
	H=(max==R) ? 0 : (max==G) ? 120 : 240; 
	H+=60*sub*inv_diff;	

	if(180==CvtColor_traits<SRC_T>().hrange) {
	  H=H/2;
	  H+=((H+(typename CvtColor_traits<DST_T>::H_MULT_T)0.5)<0) ? 180 : 0;
	}
	else
	  H+=(H<0) ? 360 :0;

        _dst.val[0] = sr_cast<DST_T>(H);
        _dst.val[1] = (180==CvtColor_traits<SRC_T>().hrange) ? sr_cast<DST_T>(255*S) : sr_cast<DST_T>(S);
        _dst.val[2] = max;
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_BGR2HSV, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> src2;
        src2.val[0] = _src.val[2];
        src2.val[1] = _src.val[1];
        src2.val[2] = _src.val[0];
        kernel_CvtColor<HLS_RGB2HSV,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(src2, _dst);
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_HSV2RGB, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
	#pragma HLS inline
        SRC_T H = _src.val[0];
        SRC_T S = _src.val[1];
        SRC_T V = _src.val[2];
	typename CvtColor_traits<DST_T>::S_MULT_T tab[4];
	typename CvtColor_traits<DST_T>::S_MULT_T f;
	static const int sector_data[][3]=
		{{0,3,1}, {2,0,1}, {1,0,3}, {1,2,0}, {3,1,0},{0,1,2}};
	typename CvtColor_traits<DST_T>::S_MULT_T S_scale;
	unsigned char h_i;

	#pragma HLS array_partition variable=tab dim=0 complete

	h_i = (unsigned char)(H*(CvtColor_traits<SRC_T>().inv_hrange)); 
	f = H*(CvtColor_traits<SRC_T>().inv_hrange) - h_i;
	if(h_i >= 6)//for hrange=180, 0<H<255, then 0<h_i<8 
	  h_i -= 6;

	S_scale=S*(CvtColor_traits<SRC_T>().inv_scale);

	tab[0] = V;
	tab[1] = V*(1-S_scale);
	tab[2] = V*(1-f*S_scale);
	tab[3] = V*(1-S_scale+f*S_scale);

        _dst.val[0] = sr_cast<DST_T>(tab[sector_data[h_i][0]]);
        _dst.val[1] = sr_cast<DST_T>(tab[sector_data[h_i][1]]);
        _dst.val[2] = sr_cast<DST_T>(tab[sector_data[h_i][2]]);
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_HSV2BGR, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> dst2;
        kernel_CvtColor<HLS_HSV2RGB,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(_src, dst2);
        _dst.val[0] = dst2.val[2];
        _dst.val[1] = dst2.val[1];
        _dst.val[2] = dst2.val[0];
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_RGB2HLS, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
	#pragma HLS inline
        SRC_T R = _src.val[0];
        SRC_T G = _src.val[1];
        SRC_T B = _src.val[2];
	typename CvtColor_traits<DST_T>::H_MULT_T H;
	typename CvtColor_traits<DST_T>::S_MULT_T S;
	DST_T L;
	typename CvtColor_traits<DST_T>::SUB_T sub;
	typename CvtColor_traits<DST_T>::ADD_T add;
	typename CvtColor_traits<DST_T>::INV_T inv_add = 0;
	typename CvtColor_traits<DST_T>::INV_T inv_add_comp= 0;
	typename CvtColor_traits<DST_T>::INV_T inv_diff = 0;
	DST_T max = B;
	DST_T min = B;
	DST_T diff;

	MaxMin<SRC_T,DST_T>::max(_src, max);
	MaxMin<SRC_T,DST_T>::min(_src, min);
	diff=max-min;
	add=max+min;
	L=sr_cast<DST_T>((max+min)/2);

	if(add!=0)
	  inv_add = (typename CvtColor_traits<DST_T>::INV_T) 1/add;
	else
	  inv_add = 0;

    //assert(add < 2*CvtColor_traits<SRC_T>().scale);

	if((2*(CvtColor_traits<SRC_T>().scale)-add)!=0)
	  inv_add_comp = (typename CvtColor_traits<DST_T>::INV_T) 1/(2*(CvtColor_traits<SRC_T>().scale)-add);
	else
	  inv_add_comp = 0;

	if(L<=CvtColor_traits<SRC_T>().delta)//0.5*(CvtColor_traits<SRC_T>().scale))
	  S=inv_add*diff;	
	else
	  S=inv_add_comp*diff;

	if(diff!=0)
	  inv_diff = (typename CvtColor_traits<DST_T>::INV_T) 1/diff;
	else
	  inv_diff = 0;
	
	sub=(max==R) ? (G-B) : (max==G) ? (B-R) : (R-G); 
	H=(max==R) ? 0 : (max==G) ? 120 : 240; 
	H+=60*sub*inv_diff;	

	if(180==CvtColor_traits<SRC_T>().hrange) {
	  H=H/2;
	  H+=((H+(typename CvtColor_traits<DST_T>::H_MULT_T)0.5)<0) ? 180 : 0;
	}
	else
	  H+=(H<0) ? 360 :0;

        _dst.val[0] = sr_cast<DST_T>(H);
        _dst.val[1] = L;
        _dst.val[2] = (180==CvtColor_traits<SRC_T>().hrange) ?
            sr_cast<DST_T>(CvtColor_traits<SRC_T>().scale*S) :
            sr_cast<DST_T>(S);

    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_BGR2HLS, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> src2;
        src2.val[0] = _src.val[2];
        src2.val[1] = _src.val[1];
        src2.val[2] = _src.val[0];
        kernel_CvtColor<HLS_RGB2HLS,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(src2, _dst);
    }
};

template<typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_HLS2RGB, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
	#pragma HLS inline
        SRC_T H = _src.val[0];
        SRC_T L = _src.val[1];
        SRC_T S = _src.val[2];
	typename CvtColor_traits<DST_T>::S_MULT_T tab[4];
	typename CvtColor_traits<DST_T>::S_MULT_T f;
	static const int sector_data[][3]=
		{{0,3,1}, {2,0,1}, {1,0,3}, {1,2,0}, {3,1,0},{0,1,2}};
	typename CvtColor_traits<DST_T>::S_MULT_T S_scale;
	unsigned char h_i;

	#pragma HLS array_partition variable=tab dim=0 complete

	h_i = (unsigned char)(H*(CvtColor_traits<SRC_T>().inv_hrange)); 
	f = H*(CvtColor_traits<SRC_T>().inv_hrange) - h_i;
	if(h_i >= 6)//for hrange=180, 0<H<255, then 0<h_i<8 
	  h_i -= 6;

	S_scale=S*(CvtColor_traits<SRC_T>().inv_scale);

	if(L<=0.5*CvtColor_traits<SRC_T>().scale)
	  tab[0] = L+L*S_scale;
	  //tab[0] = L*(1+S_scale);
	else
 	  tab[0] = L+S-L*S_scale;

	tab[1] = 2*L-tab[0];
	tab[2] = tab[0]-(tab[0]-tab[1])*f;
	tab[3] = tab[1]+(tab[0]-tab[1])*f;

        _dst.val[0] = sr_cast<DST_T>(tab[sector_data[h_i][0]]);
        _dst.val[1] = sr_cast<DST_T>(tab[sector_data[h_i][1]]);
        _dst.val[2] = sr_cast<DST_T>(tab[sector_data[h_i][2]]);
    }
};

template <typename SRC_T, typename DST_T>
class kernel_CvtColor<HLS_HLS2BGR, SRC_T, DST_T> {
public:
    template<int CN1,int CN2>
    void apply(Scalar<CN1,SRC_T>& _src, Scalar<CN2,DST_T>& _dst) {
        Scalar<CN1,SRC_T> dst2;
        kernel_CvtColor<HLS_HLS2RGB,SRC_T,DST_T> kernel_opr;
        kernel_opr.apply(_src, dst2);
        _dst.val[0] = dst2.val[2];
        _dst.val[1] = dst2.val[1];
        _dst.val[2] = dst2.val[0];
    }
};

template<typename CONVERSION,int SRC_T, int DST_T,
    int ROWS,int COLS>
void CvtColor(
        Mat<ROWS, COLS, SRC_T>	&_src,
        Mat<ROWS, COLS, DST_T>	&_dst
        )
{
    kernel_CvtColor<CONVERSION,HLS_TNAME(SRC_T),HLS_TNAME(DST_T)> kernel_opr;
    int     cols=_src.cols;
    int     rows=_src.rows;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> _s;
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> _d;
 loop_height: for(int i= 0; i < rows; i++) {
    loop_width: for (int j= 0; j < cols; j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
            _src >> _s;
            kernel_opr.apply(_s,_d);
            _dst << _d;
        }
    }
}
//---------------------integral---------------------
template<int SRC_T, int DST_T,
int ROWS,int COLS>
void Integral(
        Mat<ROWS, COLS, SRC_T>		&_src,
        Mat<ROWS+1, COLS+1, DST_T>	&_sum
        )
{
    LineBuffer<1,COLS+1,HLS_TNAME(DST_T)>    k_buf;
    int cols=_src.cols;
    int rows=_src.rows;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<1,HLS_TNAME(SRC_T)> _s;
    Scalar<1,HLS_TNAME(DST_T)> _d;
    int sqsum, sum=0;

 loop_height: for(int i= 0;i<rows+1;i++) {
    loop_width: for (int j= 0;j<cols+1;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
            if(i==0||(j==0&&i!=0))
            {
                k_buf.val[0][j]=0;
                _d.val[0]=0;
                sum=0;
            }
            else
            {
                _src>>_s;
                sum=sum+_s.val[0];
                _d.val[0]=sum+k_buf.val[0][j];
                k_buf.val[0][j]=_d.val[0];
            }
            _sum<<_d;   
        }
    }
}
template<int SRC_T, int DST_T,int DSTSQ_T,
    int ROWS,int COLS>
void Integral(
		Mat<ROWS, COLS, SRC_T>		&_src,
		Mat<ROWS+1, COLS+1, DST_T>	&_sum,
		Mat<ROWS+1, COLS+1, DSTSQ_T>	&_sqsum
                )
{
    LineBuffer<1,COLS+1,HLS_TNAME(DST_T)>	    k_buf;
    LineBuffer<1,COLS+1,HLS_TNAME(DSTSQ_T)>	    k_sqbuf;
    int cols=_src.cols;
    int rows=_src.rows;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<1,HLS_TNAME(SRC_T)> _s;       
    Scalar<1,HLS_TNAME(DST_T)> _d;       
    Scalar<1,HLS_TNAME(DSTSQ_T)> _sqd;       
    int sqsum, sum=0;       

 loop_height: for(int i= 0;i<rows+1;i++) {
    loop_width: for (int j= 0;j<cols+1;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
            if(i==0||(j==0&&i!=0))
            {
                k_buf.val[0][j]=0;
                k_sqbuf.val[0][j]=0;
                _d.val[0]=0;
                _sqd.val[0]=0;
                sum=0;
                sqsum=0;
            }
            else
            {
                _src>>_s;
                sum=sum+_s.val[0];
                sqsum=sqsum+_s.val[0]*_s.val[0];

                _d.val[0]=sum+k_buf.val[0][j];
                _sqd.val[0]=sqsum+k_sqbuf.val[0][j];
                k_buf.val[0][j]=_d.val[0];
                k_sqbuf.val[0][j]=_sqd.val[0];
            }
            _sum<<_d;   
            _sqsum<<_sqd;   
        }
    }
}
//----------------Resize----------
// Bicubic Coefficient Generation:
// For a particular scaling ratio, the coefficients will be pre-calculated and stored to memory.
// This coefficient generation algorithm implements the Catmull-Rom spline:
// http://en.wikipedia.org/wiki/Cubic_interpolation
// This whole function is expected to be absorbed into initialization content for
// dual port distributed memory:
template <int NTAPS, int NPHASES, int COEFF_BITS,typename HT, typename VT>
void init_scale_coefficients(HT *Hcoefs, VT *Vcoefs) {

#pragma HLS inline off
    static const float A = -0.5;					// Coefficients will be stored in inverse 16.15 notation,
    double t;										// Because Fixed point N.N-1 notation can not represent 1.0
    double coeff[NPHASES][NTAPS];
    int i,j;

    for (i=0; i<NPHASES; i++) {
        t=((double) i)/NPHASES;
        coeff[i][0] = A*(t*((2-t)*t-1));			//coeff[0] = A*(-  t*t*t + 2*t*t -  t    );
        coeff[i][1] = A*(t*t*(3*t-5)+2);  			//coeff[1] = A*( 3*t*t*t - 5*t*t      + 2);
        coeff[i][2] = A*(t*((4-3*t)*t+1));			//coeff[2] = A*(-3*t*t*t + 4*t*t  + t    );
        coeff[i][3] = A*((t-1)*t*t);				//coeff[3] = A*(   t*t*t -   t*t         );
    }
    for (i = 0; i < NPHASES; i++) {
        for (j = 0; j < NTAPS; j++) {
            Hcoefs[(NTAPS*i)+j] = (HT)coeff[i][j];
            Vcoefs[(NTAPS*i)+j] = (VT)coeff[i][j];
        }
    }
}

template<int NTAPS, int COEFF_BITS, int T,typename FILTER_T>
hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> scale_operator(hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)>  *samples, FILTER_T *coeffs) {

    typedef typename filter2d_traits<HLS_TNAME(T), FILTER_T, NTAPS>::SRC_CAST_T SRCT;
    int i,j,ch;
    hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)>  pix_out;
CHAN_LOOP : for (ch =0; ch<HLS_MAT_CN(T); ch++)	{				// Iterate over all color channels in parallel:
        typename filter2d_traits<HLS_TNAME(T), FILTER_T, NTAPS>::ACCUM_T t=0;
        typename filter2d_traits<HLS_TNAME(T), FILTER_T, NTAPS>::MULT_T temp;
    TAPS_LOOP : for (i = 0,t=0.5; i < NTAPS; i++) {
#pragma HLS inline
            HLS_TNAME(T) sample = samples[i].val[ch]; 	// Pull out the color channel value
            temp = ((SRCT)sample)*coeffs[NTAPS-1-i];
            t = t - temp ;
        }
        pix_out.val[ch] = sr_cast<HLS_TNAME(T)> (t);
    }
    return pix_out;
}

// Linear Scaler Function
template <int COLS, int ROWS,int DCOLS, int DROWS, int T>
void Resize_opr_bicubic(hls::Mat<ROWS, COLS, T>&  _src, hls::Mat<DROWS, DCOLS, T>& _dst) {
    typedef ap_fixed<32, 16, AP_RND> FT;
    typedef short N16;
    const int NTAPS = 4;
    const int NPHASES = 16;
    const int COEFF_BITS = 16; // Maximum is 16 for S16 format
    const short ES = 2; // Edge size. 2 Rows and 2 columns are needed before processing can start.
    typename filter2d_traits<HLS_TNAME(T), ap_fixed<25, 5, AP_RND>, NTAPS>::FILTER_CAST_T hcoeffs[NPHASES][NTAPS];
    typename filter2d_traits<HLS_TNAME(T), ap_fixed<25, 5, AP_RND>, NTAPS>::FILTER_CAST_T vcoeffs[NPHASES][NTAPS];
    N16 row, col;
    N16 rows, cols;
    ap_uint<32> v_phase_acc; // Vertical Phase accumulator. This accumulator is at least 16bit fractional + 13 bit integer = 29 bits wide
    ap_uint<32> h_phase_acc; // Horizontal Phase accumulator

    hls::LineBuffer<NTAPS, (COLS>DCOLS?COLS:DCOLS) + 3, hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> > linebuf;
    hls::Window<1, NTAPS, hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> > h_shreg;
    hls::Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> pix_in, h_fir_out, pix_out, h_fir[NTAPS], v_fir[NTAPS];
    hls::Scalar < HLS_MAT_CN(T), HLS_TNAME(T) > temp_in[NTAPS];
    hls::Scalar < HLS_MAT_CN(T), HLS_TNAME(T) > temp_out[NTAPS];
    HLS_TNAME(T)	pix_in_ch, pix_out_ch;

    N16 rows_rw = -1, cols_rw = -1;
    bool col_rd_en;
    bool row_rd_en;
    bool col_wr_en;
    bool row_wr_en;
    ap_uint<4> v_phase;
    ap_uint<4> h_phase;
#pragma HLS array_partition variable= temp_in dim=0 complete
#pragma HLS array_partition variable= temp_out dim=0 complete
#pragma HLS array_partition variable= h_fir dim=0 complete
#pragma HLS array_partition variable= v_fir dim=0 complete
#pragma HLS array_partition variable=hcoeffs dim=2 complete
#pragma HLS array_partition variable=vcoeffs dim=2 complete

    // Initialization:
    init_scale_coefficients<NTAPS, NPHASES, COEFF_BITS>(hcoeffs[0], vcoeffs[0]); // 4 Taps, 16 phases, 16 bit signed coefficients

    N16 rows_in = _src.rows;
    N16 cols_in = _src.cols;
    N16 rows_out = _dst.rows;
    N16 cols_out = _dst.cols;
    FT row_ratio = (FT(rows_out)) / (FT)rows_in;
    FT col_ratio = (FT(cols_out)) / (FT)cols_in;

    int row_rate = (ap_fixed<4, 2, AP_RND>(0.5) + row_ratio * 65536);
    int col_rate = (ap_fixed<4, 2, AP_RND>(0.5) + col_ratio * 65536);
    rows = (rows_in > rows_out) ? rows_in : rows_out;
    cols = (cols_in > cols_out) ? cols_in : cols_out;
    assert(rows<=ROWS || rows<=DROWS);
    assert(cols<=COLS || cols<=DCOLS);
    v_phase_acc = 0;

 ROW_LOOP: for (row = 0; row < rows + 3; row++) {
    COL_LOOP: for (col = 0; col < cols + 3; col++) {
#pragma HLS PIPELINE
#pragma HLS loop_flatten off
#pragma HLS DEPENDENCE array inter false
              if (col == 0) {
                  v_phase = (v_phase_acc >> 12) & 15; // Slice out bits 15..12 from the phase accumulator (Consider rounding here!!!)
                  if (row_rate < 65536) { // Down scaling, writes are less frequent than reads
                      row_rd_en = true;
                      N16 drow = row * row_ratio;
                      if (rows_rw != drow) {
                          row_wr_en = true;
                          rows_rw = drow;
                      } else
                          row_wr_en = false;
                  } else { // Up scaling, reads are less frequent than writes
                      row_wr_en = true;
                      N16 drow = row / row_ratio;
                      if (rows_rw != drow) {
                          row_rd_en = true;
                          rows_rw = drow;
                      } else
                          row_rd_en = false;
                  }
                  if (row_rate < 65536 || row_rd_en)
                      v_phase_acc = v_phase_acc + row_rate;
              }

              h_phase = (h_phase_acc >> 12) & 15; // Slice out bits 15..12 from the phase accumulator
              if (col_rate < 65536) { // Down scaling, writes are less frequent than reads
                  col_rd_en = true;
                  N16 dcol = col * col_ratio;
                  if (col == 0 || (col > 0 && cols_rw != dcol)) {
                      col_wr_en = true;
                      cols_rw = dcol;
                  } else
                      col_wr_en = false;
              } else { // Up scaling, reads are less frequent than writes
                  col_wr_en = true;
                  N16 dcol = col / col_ratio;
                  if (col == 0 || (col > 0 && cols_rw != dcol)) {
                      col_rd_en = true;
                      cols_rw = dcol;
                  } else
                      col_rd_en = false;
              }

              if (col > 0 && (col_rate < 65536 || col_rd_en))
                  h_phase_acc = h_phase_acc + col_rate;
              else
                  h_phase_acc = col_rate;

              if (col_rd_en) {
                  h_shreg.shift_left();
                  if (row_rd_en) {
                      if (row < rows && col < cols) {
                          pix_in = _src.read();
                          h_shreg.insert(pix_in, 0, NTAPS - 1);
                      } else if (col >= cols && row < rows) {
                          h_shreg.insert(pix_in, 0, NTAPS - 1);
                      } else if (row >= rows) {
                          for (int m = 0; m < HLS_MAT_CN(T); m++)
                              pix_in.val[0] = 0;
                          h_shreg.insert(pix_in, 0, NTAPS - 1);
                      }
                  }
              }

              for (int i = 0; i < NTAPS; i++) {
                  temp_in[i] = linebuf.val[i][col];
              }

              if (col_wr_en) {
                  if ((row_rd_en) || (row < ES) || (row >= rows - ES)) {
                      for (int i = NTAPS - 1; i > 0; i--) {
                          temp_out[i] = temp_in[i - 1];
                      }
H_LOOP: for (int i = 0; i < NTAPS; i++) {
            h_fir[i] =
                (col > i) ?
                h_shreg.getval(0, i) :
                h_shreg.getval(0, NTAPS - 1 - col);
        }
        h_fir_out = scale_operator<NTAPS, COEFF_BITS, T>(h_fir,
                hcoeffs[h_phase]);
        temp_out[0] = h_fir_out;
        v_fir[NTAPS - 1] = h_fir_out;
                  } else {
                      for (int i = NTAPS - 1; i >= 0; i--) {
                          temp_out[i] = temp_in[i];
                      }
                      v_fir[NTAPS - 1] = temp_in[0];
                  }
V_LOOP: for (int i = 0; i < NTAPS - 1; i++) {
            v_fir[i] =
                (row > 0) ? temp_out[NTAPS - 1 - i] : temp_out[0];
        }
              }

              for (int i = 0; i < NTAPS; i++) {
                  linebuf.val[i][col] = (row > 0) ? temp_out[i] : temp_out[0];
              }

              if (row >= 3 && col >= 3 && row_wr_en && col_wr_en) {
                  pix_out = scale_operator<NTAPS, COEFF_BITS, T>(v_fir, vcoeffs[v_phase]);
                  _dst.write(pix_out);
              }
          }
          }
}

template<int SRC_T, int ROWS,int COLS,int DROWS,int DCOLS>
void Resize_opr_linear (
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<DROWS, DCOLS, SRC_T>	&_dst )
{
    LineBuffer<2,(COLS>DCOLS?COLS:DCOLS)+1,Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> >	    k_buf;
    Window<2,2,Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> >   win;
    short     dcols=_dst.cols;
    short     drows=_dst.rows;
    short     srows=_src.rows;
    short     scols=_src.cols;
    ap_fixed<32,16,AP_RND>    	row_rate=((ap_fixed<32,16,AP_RND> )srows)/(ap_fixed<32,16,AP_RND>)drows;
    ap_fixed<32,16,AP_RND>    	col_rate=((ap_fixed<32,16,AP_RND> )scols)/(ap_fixed<32,16,AP_RND>)dcols;
    typename filter2d_traits<HLS_TNAME(SRC_T), ap_fixed<20,2,AP_RND> ,4>::FILTER_CAST_T	u,v, u1,v1;

    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> s, temp, d;
    ap_fixed<4,2,AP_RND> 		par=0.5;
    ap_fixed<20,10,AP_RND> 		offset_row=row_rate/2-par;
    ap_fixed<20,10,AP_RND> 		offset_col=col_rate/2-par;
    ap_fixed<32,16,AP_RND> 		fx=0;
    ap_fixed<32,16,AP_RND> 		fy=0;

    short     rows=srows > drows ? srows : (drows+1);
    short     cols=scols > dcols ? scols : (dcols+1);
    assert(rows<=ROWS || rows<=DROWS+1);
    assert(cols<=COLS || cols<=DCOLS+1);

    short x=0;
    short pre_fy=-10;
    short pre_fx=-10;
    bool row_rd=false;
    bool row_wr=false;

    for(short i= 0;i<rows;i++) {
        for(short j= 0;j<cols;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE

            bool col_rd=false;
            bool col_wr=false;

            short dy = row_rate>1 ? (short)(i/row_rate):(i-1);
            short dx = col_rate>1 ? (short)(j/col_rate):(j-1);

            fy =(dy)*row_rate+offset_row;
            fx =(dx)*col_rate+offset_col;

            short sx=(short)fx;
            short sy=(short)fy;
            if(fx-sx>0)
                u=fx-sx;
            else
                u=0;
            if(fy-sy>0)
                v=fy-sy;
            else
                v=0;
            u1=1-u;
            v1=1-v;
            if(sx>scols-1)
            {
                sx=scols-1;
                u=0;
            }
            if(sy>srows-1)
            {
                sy=srows-1;
                v=0;
            }
            if(j==0)
            {
                x=0;
                pre_fx=-10;
                if(row_rate>1)
                {
                    row_rd=true;
                    row_wr= (sy==(i-1)) ? true : false;
                }
                else
                {
                    if(i==0){
                        row_rd=true;
                    }
                    else if(sy!=pre_fy)
                    {
                        row_rd=true;
                        pre_fy=sy;
                    }
                    else {
                        row_rd=false;
                    }
                    row_wr= i>0? true: false;
                }
            }
            if(col_rate>1)
            {
                col_rd=true;
                col_wr= (sx==(j-1)) ? true : false;
            }else{
                if(j==0){
                    col_rd=true;
                }
                else if(sx!=pre_fx)
                {
                    col_rd=true;
                    pre_fx=sx;
                }
                else
                    col_rd=false;
                col_wr= j>0? true: false;
            }
            if(col_rd)
            {
                for(int r= 0;r<2;r++)
                {
                    win.val[r][1]=win.val[r][0];
                }
                if(row_rd)
                {

                    k_buf.val[1][x]=k_buf.val[0][x];
                    win.val[1][0]=k_buf.val[0][x];
                    if(sy<srows-1&&sx<scols-1)
                    {
                        _src >> s;
                        k_buf.val[0][x]=s;
                        win.val[0][0]=k_buf.val[0][x];
                    }
                    else if(sx>=scols-1&&sy<srows-1){
                        k_buf.val[0][x]=s;//border
                    }
                    else if(sy>=srows-1){
                        win.val[0][0]=k_buf.val[0][x];
                    }
                }
                else
                {
                    for(int r= 0;r<2;r++)
                    {
                        win.val[r][0]=k_buf.val[r][x];
                    }
                }

                x++;
            }
            if(row_wr && col_wr)
            {
                for(int k=0;k<HLS_MAT_CN(SRC_T);k++)
                {
                    typename filter2d_traits<HLS_TNAME(SRC_T), ap_fixed<15,1,AP_RND> ,4>::ACCUM_T t=0;
                    typedef typename fixed_type<HLS_TNAME(SRC_T)>::T SRCT;

                    t=((SRCT)win.val[1][1].val[k])*u1*v1+
                        ((SRCT)win.val[1][0].val[k])*v1*u+
                        ((SRCT)win.val[0][1].val[k])*u1*v+
                        ((SRCT)win.val[0][0].val[k])*u*v;
                    d.val[k]=sr_cast<HLS_TNAME(SRC_T)>(t);
                }
                _dst << d;
            }
        }
    }
}
#define HLS_INTER_NN 0
#define HLS_INTER_NEAREST 0
#define HLS_INTER_LINEAR 1
#define HLS_INTER_AREA 2
#define HLS_INTER_CUBIC 3
#define HLS_INTER_LANCZOS4 4

template<int SRC_T, int ROWS,int COLS,int DROWS,int DCOLS>
void Resize (
        Mat<ROWS, COLS, SRC_T>	        &_src,
        Mat<DROWS, DCOLS, SRC_T>	&_dst,
        int interpolation=HLS_INTER_LINEAR )
{
    switch(interpolation) {
    case HLS_INTER_NEAREST:
#ifndef __SYNTHESIS__
        assert("hls::Resize: HLS_INTER_NEAREST not supported");
#endif
        break;
    case HLS_INTER_AREA:
#ifndef __SYNTHESIS__
        assert("hls::Resize: HLS_INTER_AREA not supported");
#endif
        break;
    case HLS_INTER_LANCZOS4:
#ifndef __SYNTHESIS__
        assert("hls::Resize: HLS_INTER_LANCZOS4 not supported");
#endif
        break;
    case HLS_INTER_CUBIC:
        Resize_opr_bicubic(_src,_dst);
        break;
    case HLS_INTER_LINEAR:
        Resize_opr_linear(_src,_dst);
        break;
    }
}
//------------------
template<int SRC_T, int DST_T,
    int ROWS,int COLS>
void ConvertScale (
        Mat<ROWS, COLS, SRC_T>	&_src,
        Mat<ROWS, COLS, DST_T>	&_dst,
        double alpha=1,
        double beta=0
        )
{
    int     cols=_src.cols;
    int     rows=_src.rows;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> _s;
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> _d;
loop_height: for(int i= 0;i<rows;i++) {
    loop_width: for (int j= 0;j<cols;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
            _src>>_s;
        loop_channels: for(int k=0;k<HLS_MAT_CN(SRC_T);k++)
            {
                _d.val[k]=sr_cast<HLS_TNAME(DST_T)>(_s.val[k]*alpha+beta);
            }
            _dst<<_d;
        }
    }
}

template<int SRC_T, int DST_T,
    int ROWS,int COLS>
void ConvertScaleAbs (
        Mat<ROWS, COLS, SRC_T>	&_src,
        Mat<ROWS, COLS, DST_T>	&_dst,
        double alpha=1,
        double beta=0
        )
{
    int     cols=_src.cols;
    int     rows=_src.rows;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> _s;
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> _d;
loop_height: for(int i= 0;i<rows;i++) {
    loop_width: for (int j= 0;j<cols;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE
            _src>>_s;
        loop_channels: for(int k=0;k<HLS_MAT_CN(SRC_T);k++)
            {
                _d.val[k]=sr_cast<HLS_TNAME(DST_T)>(abs(_s.val[k]*alpha+beta));
            }
            _dst<<_d;
        }
    }
}
template<int XORDER, int YORDER, int SIZE, typename T>
void Sobel_kernel(Window<SIZE,SIZE,T > &kernel)
{
    int kerI[SIZE+1];
    Window<1,SIZE,T> kernelX;
    Window<SIZE,1,T> kernelY;

    for(int k=0; k<2; k++)
    {
        int order = k == 0 ? XORDER : YORDER;

        if( SIZE==3 )
        {
            if(order==0 )
                kerI[0]=1, kerI[1]=2, kerI[2]=1;
            else if( order == 1 )
                kerI[0]=-1, kerI[1]=0, kerI[2]=1;
            else
                kerI[0]=1, kerI[1]=-2, kerI[2]=1;
        }
        else
        {
            int oldval, newval;
            kerI[0] = 1;
            for(int i=0; i<SIZE; i++)
                kerI[i+1] = 0;

            for(int i=0; i<SIZE-order-1; i++)
            {
                oldval = kerI[0];
                for(int j = 1; j <= SIZE; j++ )
                {
                    newval = kerI[j]+kerI[j-1];
                    kerI[j-1] = oldval;
                    oldval = newval;
                }
            }

            for(int i = 0; i < order; i++ )
            {
                oldval = -kerI[0];
                for(int j = 1; j <= SIZE; j++ )
                {
                    newval = kerI[j-1] - kerI[j];
                    kerI[j-1] = oldval;
                    oldval = newval;
                }
            }
        }

   	    for(int i=0; i<SIZE; i++) {
	        if(k==0)
	            kernelX(0,i)=kerI[i];
	        else
	            kernelY(i,0)=kerI[i]; 
	    }
    }
    
   	for(int i=0; i<SIZE; i++) {
   	    for(int j=0; j<SIZE; j++) {
            kernel(i,j)=kernelY(i,0)*kernelX(0,j);
	    }
    }

}
//---------------sobel-------------------
template<int XORDER, int YORDER, int SIZE, typename BORDERMODE, int SRC_T, int DST_T, int ROWS,int COLS,int DROWS,int DCOLS>
void Sobel (
        Mat<ROWS, COLS, SRC_T>	        &_src,
        Mat<DROWS, DCOLS, DST_T>	&_dst)
{
#ifndef __SYNTHESIS__
   assert((SIZE==3 || SIZE==5||SIZE==7) );
#endif

    Window<SIZE,SIZE,ap_int<8> >	kernel;
    Sobel_kernel<XORDER, YORDER >(kernel);
    Point_<int> c_point;
    c_point.x=-1;
    c_point.y=-1;
    Filter2D<BORDERMODE>(_src,_dst,kernel, c_point);
}

template<int XORDER, int YORDER, int SIZE, int SRC_T, int DST_T, int ROWS,int COLS,int DROWS,int DCOLS>
void Sobel (
        Mat<ROWS, COLS, SRC_T>	        &_src,
        Mat<DROWS, DCOLS, DST_T>	&_dst)
{
    Sobel<XORDER,YORDER,SIZE,BORDER_DEFAULT>(_src, _dst);
}

template<int KH,int KW,typename K_T,typename XT, typename YT>
void getGaussianKernel(
        Window<KH,KW,K_T> &kernel,
        XT sigmaX=0,
        YT sigmaY=0
        )
{
#pragma HLS INLINE off
     float kval[KH][KW];
     float sum=0;
     if(sigmaX <= 0)
     {
         sigmaX=0.3*((KW-1)*0.5-1)+0.8;
     }
     if(sigmaY <= 0)
     {
         sigmaY=0.3*((KH-1)*0.5-1)+0.8;
     }
     for (int i = 0; i < KH; i++) {
            for (int j = 0; j < KW; j++) {
                float index1=powf( (i-(KH-1)/2),2)*1.0/(2*sigmaY*sigmaY);
                float index2=powf( (j-(KW-1)/2),2)*1.0/(2*sigmaX*sigmaX);
                float index= -index1-index2;
                kval[i][j] = expf(index);
                sum+=kval[i][j];
            }
        }
     for (int i = 0; i < KH; i++) {
         for (int j = 0; j < KW; j++) {
             kval[i][j]=kval[i][j]/sum;
             kernel.val[i][j]=kval[i][j];
         }
     }

}

template<int KH,int KW,typename BORDERMODE,int SRC_T,int DST_T,int ROWS,int COLS>
void GaussianBlur(
		Mat<ROWS, COLS, SRC_T>	    &_src,
                Mat<ROWS, COLS, DST_T>	    &_dst,
                double sigmaX=0,
                double sigmaY=0)
{
    Point_<int> anchor;
    anchor.x = -1;
    anchor.y = -1;
    Window<KW, KH, ap_fixed<16,2,AP_RND>  > kernel;
    getGaussianKernel(kernel,sigmaX,sigmaY);
    Filter2D<BORDERMODE>(_src, _dst, kernel, anchor);
}

template<int KH,int KW,int SRC_T,int DST_T,int ROWS,int COLS>
void GaussianBlur(
		Mat<ROWS, COLS, SRC_T>	    &_src,
                Mat<ROWS, COLS, DST_T>	    &_dst,
                double sigmaX=0,
                double sigmaY=0)
{
    GaussianBlur<KH,KW,BORDER_DEFAULT>(_src, _dst, sigmaX, sigmaY);
}

//---------------BoxFilter-------------------
template <int KH,int KW>
void Box_kernel_1d(
        Window<1, KW, ap_ufixed<1,1> >& kernelX,
        Window<KH, 1, ap_ufixed<1,1> >& kernelY
        )
{
    float kval[KW];

    for (int i = 0; i < KW; i++) {
        kernelX.val[0][i] = 1;
    }

    for (int i = 0; i < KH; i++) {
        kernelY.val[i][0] = 1;
    }
}

template<int KH,int KW, bool NORMALIZE,typename BORDERMODE,
	 int SRC_T,int DST_T,int ROWS,int COLS>
void BoxFilter(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        Point_<int> anchor=Point_<int>(-1,-1))
{
    Window<1, KW, ap_ufixed<1,1> > kernelX;
    Window<KH, 1, ap_ufixed<1,1> > kernelY;
    Box_kernel_1d(kernelX,kernelY);

    if( NORMALIZE)
    {
        const int BW = BitWidth<KH*KW>::Value+HLS_TBITDEPTH(SRC_T);
        const ap_ufixed<BW+1,1> scale = 1.0/(KH*KW);
        SepFilter2D<BORDERMODE>(_src, _dst, kernelX, kernelY, anchor, scale);
    }
    else
    {
        SepFilter2D<BORDERMODE>(_src, _dst, kernelX, kernelY, anchor);
    }
}

template<int KH,int KW, typename BORDERMODE, int SRC_T,int DST_T,int ROWS,int COLS>
void BoxFilter(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        Point_<int> anchor=Point_<int>(-1,-1))
{
    BoxFilter<KH,KW,true,BORDERMODE>(_src, _dst, anchor);
}

template<int KH,int KW, int SRC_T,int DST_T,int ROWS,int COLS>
void BoxFilter(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_dst,
        Point_<int> anchor=Point_<int>(-1,-1))
{
    BoxFilter<KH,KW,true,BORDER_DEFAULT>(_src, _dst, anchor);
}

//-------------------------PyrUp---
template <typename T,typename FT, int KH,int KW>
void Pyr_kernel(
        Window<KH, KW, T>& kernel,
        FT factor
        )
{
    int k[25]={1, 4, 6, 4, 1, 4, 16, 24, 16, 4, 6, 24, 36, 24, 6, 4, 16, 24, 16, 4, 1, 4, 6, 4, 1};
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            kernel.val[i][j]=k[i*5+j]*(1.0/256)*factor;
}
template<int K_HEIGHT,int K_WIDTH,int SRC_T, int DST_T, typename KN_T,
         int ROWS,int COLS,int DROWS, int DCOLS>
void PyrUp_opr(
        Mat<ROWS, COLS, SRC_T>	&_src,
        Mat<DROWS, DCOLS, DST_T>	&_dst,
        Window<5,5,KN_T>		&kernel
        )
{
    HLS_SIZE_T rows=_src.rows;
    HLS_SIZE_T cols=_src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
#pragma HLS inline
    // The main processing window
    Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>		src_kernel_win[HLS_MAT_CN(SRC_T)];
    // The main line buffer, case the border have to pre load so the row is K_HEIGHT rather than K_HEIGHT-1
    LineBuffer<K_HEIGHT,COLS,HLS_TNAME(SRC_T)>	    k_buf[HLS_MAT_CN(SRC_T)];
    // A small buffer keeping a few pixels from the line
    // buffer, so that we can complete right borders correctly.and avoid read k_buf port limit
    Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>		right_border_buf[HLS_MAT_CN(SRC_T)];
    // Temporary storage for reading from the line buffers.
    Window<K_HEIGHT,1,HLS_TNAME(SRC_T)>		        col_buf[HLS_MAT_CN(SRC_T)];

#pragma HLS ARRAY_PARTITION variable=col_buf complete dim=0
#pragma HLS ARRAY_PARTITION variable=&kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_win complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=right_border_buf complete dim=0

    Point_< ap_int<14> > anchor;
    anchor.x=K_HEIGHT/2;
    anchor.y=K_WIDTH/2;
    ap_int<14> heightloop= 2*(rows+K_HEIGHT-1+K_HEIGHT);
    ap_int<14> widthloop = 2*(cols+K_WIDTH-1);//one pixel overlap, so it should minus one
    Point_<ap_int<14> > ImagLoc;

    int channels=HLS_MAT_CN(SRC_T);
 loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
    loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
                bool oddrow=(i+1)&1;
                bool oddcol=(j+1)&1;
                ImagLoc.x=j/2-anchor.x;    //fill data x,y are the coordinate in the image, it could be negative. For example (-1,-1) represents the interpolation pixel.
                ImagLoc.y=i/2-K_HEIGHT-anchor.y;
                int x= ImagLoc.x;
                if(ImagLoc.x<0)
                    x= -ImagLoc.x;
                else if( ImagLoc.x >= cols )
                    x=2*cols- ImagLoc.x-1; // right and bottom insert 0 ,so border type is not 101
channelloop:	for(HLS_CHANNEL_T k= 0; k < channels; k++)
                {
                    if(oddcol)
                    {
                        // column left shift
                        for(int row= 0;row<K_HEIGHT;row++)
                            for(int col= K_WIDTH-1;col>=1;col--)
                                src_kernel_win[k](row,col) = src_kernel_win[k](row,col-1);

                        for(int buf_row = 0; buf_row < K_HEIGHT; buf_row++)
                        {
                            // Fetch the column from the line buffer to shift into the window.
                            col_buf[k](buf_row,0) =  k_buf[k](buf_row,x);
                        }

                        if((ImagLoc.y < (-anchor.y)) ||
                                (ImagLoc.y >= K_HEIGHT-1 && ImagLoc.y < rows-1 )) {
                            //Advance load and body process
                            if(ImagLoc.x >= 0 && ImagLoc.x < cols) {
                                HLS_TNAME(SRC_T) Toppixel = col_buf[k](K_HEIGHT-1,0); //k_buf[k](K_HEIGHT-1,ImagLoc.x);
                                src_kernel_win[k](K_HEIGHT-1,0) = Toppixel;
                                for(int buf_row= K_HEIGHT-1;buf_row >= 1;buf_row--)
                                {
                                    HLS_TNAME(SRC_T) temp = col_buf[k](buf_row-1,0); //k_buf[k](buf_row-1,ImagLoc.x);
                                    src_kernel_win[k](buf_row-1,0)=temp;
                                }
                                if(oddrow)
                                {
                                    if(ImagLoc.x >= cols - K_WIDTH)
                                    {
                                        right_border_buf[k](0,ImagLoc.x-(cols-K_WIDTH)) = Toppixel;
                                    }
                                    for(int buf_row= K_HEIGHT-1;buf_row >= 1;buf_row--)
                                    {
                                        HLS_TNAME(SRC_T) temp = col_buf[k](buf_row-1,0); //k_buf[k](buf_row-1,ImagLoc.x);
                                        k_buf[k](buf_row,x)=temp;
                                        if(ImagLoc.x >= cols - K_WIDTH)
                                        {
                                            right_border_buf[k](K_HEIGHT-buf_row,ImagLoc.x-(cols-K_WIDTH)) = temp;
                                        }
                                    }
                                    HLS_TNAME(SRC_T) temp=0;
                                    _src.data_stream[k] >> temp;
                                    k_buf[k](0,x)=temp;
                                }
                            }
                            else if(ImagLoc.x < 0)
                            {
                                for(int buf_row = 0;buf_row < K_HEIGHT; buf_row++)
                                {
                                    src_kernel_win[k](buf_row,0) = col_buf[k](buf_row,0); //k_buf[k](buf_row,x);
                                }
                            }
                            else if (ImagLoc.x >= cols) {
                                for(int buf_row= 0;buf_row < K_HEIGHT; buf_row++)
                                {
                                    src_kernel_win[k](buf_row,0) = right_border_buf[k](K_HEIGHT-1-buf_row,x-(cols-K_WIDTH)); //col_buf[k](buf_row+1,0); //k_buf[k](buf_row+1,x);
                                }
                                if(!oddrow)
                                {
                                	  for(int buf_row= K_HEIGHT-1;buf_row>0;buf_row--)
                                	    src_kernel_win[k](buf_row,0)= src_kernel_win[k](buf_row-1,0);
                                }

                            }
                        }
                        else if(ImagLoc.y > 0 )
                        {
                            ap_int<14> ref = K_HEIGHT-1;
                            if(ImagLoc.y >= rows-1)
                                ref = rows-1;
                            ap_int<14> y = ImagLoc.y;
                            for(int buf_row= 0;buf_row<K_HEIGHT;buf_row++)
                            {
                                ap_int<14>  t=y;
                                if(y<0)
                                    t=-y;
                                else if( y >= rows )
                                    t=2*rows-y-1;

                                ap_int<14> locy = ref - t;
                                src_kernel_win[k](buf_row,0) = col_buf[k](locy,0);
                                y--;
                            }
                           if(ImagLoc.y >= rows-1&&!oddrow)
                            {
                                for(int buf_row= K_HEIGHT-1;buf_row>0;buf_row--)
                                   src_kernel_win[k](buf_row,0)= src_kernel_win[k](buf_row-1,0);
                            }
                        }

                    }
                    // figure out the output image pixel value
                    if(i >= 2*(K_HEIGHT + K_HEIGHT - 1) && j >= 2*(K_WIDTH-1))
                    {
                        typename filter2d_traits<HLS_TNAME(SRC_T), KN_T, K_HEIGHT*K_WIDTH>::ACCUM_T result=0;
                        if(oddrow)
                        {
                            if(oddcol)
                            {
                                for(int m=0; m<K_HEIGHT; m++)
                                    for(int n=0; n<K_WIDTH; n++)
                                        result= result + src_kernel_win[k](K_HEIGHT-1-m, K_WIDTH-1-n)* kernel.val[2*m][2*n];
                            }
                            else
                            {
                                for(int m=0; m<K_HEIGHT; m++)
                                    for(int n=1; n<K_WIDTH; n++)
                                        result= result + src_kernel_win[k](K_HEIGHT-1-m, K_WIDTH-1-n)* kernel.val[2*m][2*n-1];
                            }
                        }else
                        {
                            if(oddcol)
                            {
                                for(int m=0; m<K_HEIGHT-1; m++)
                                    for(int n=0; n<K_WIDTH; n++)
                                        result= result + src_kernel_win[k](K_HEIGHT-1-m, K_WIDTH-1-n)* kernel.val[2*m+1][2*n];
                            }
                            else
                            {

                                for(int m=0; m<K_HEIGHT-1; m++)
                                    for(int n=1; n<K_WIDTH; n++)
                                        result= result + src_kernel_win[k](K_HEIGHT-1-m, K_WIDTH-1-n)* kernel.val[2*m+1][2*n-1];
                            }
                        }
                        _dst.data_stream[k]<< sr_cast<HLS_TNAME(DST_T)>(result);
                    }
                }
            }
             }
}
template<int SRC_T,int DST_T,int ROWS,int COLS, int DROWS, int DCOLS>
void PyrUp(
		Mat<ROWS, COLS, SRC_T>	    &_src,
                Mat<DROWS, DCOLS, DST_T>    &_dst
                )
{
#ifndef __SYNTHESIS__
    assert(_src.rows*2== _dst.rows && _src.cols*2==_dst.cols);
#endif
    Window<5, 5, ap_fixed<16,2,AP_RND>  > kernel;
    Pyr_kernel(kernel, 4);
    PyrUp_opr<3,3>(_src, _dst, kernel);
}
//-----------------------PyrDown----------
template<int K_HEIGHT,int K_WIDTH,int SRC_T, int DST_T, typename KN_T,
         int ROWS,int COLS,int DROWS, int DCOLS>
void PyrDown_opr(
        Mat<ROWS, COLS, SRC_T>	        &_src,
        Mat<DROWS, DCOLS, DST_T>	&_dst,
        Window<5,5,KN_T>		&kernel
        )
{
#ifndef __SYNTHESIS__
      assert((_src.rows+1)/2== _dst.rows && (_src.cols+1)/2==_dst.cols);
#endif
#pragma HLS inline
    // The main processing window
    Window<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>       src_kernel_win[HLS_MAT_CN(SRC_T)];
    // The main line buffer
    LineBuffer<K_HEIGHT,COLS,HLS_TNAME(SRC_T)>	    k_buf[HLS_MAT_CN(SRC_T)];
    // A small buffer keeping a few pixels from the line
    // buffer, so that we can complete right borders correctly.
    LineBuffer<K_HEIGHT,K_WIDTH,HLS_TNAME(SRC_T)>   right_border_buf[HLS_MAT_CN(SRC_T)];
    // Temporary storage for reading from the line buffers.
    LineBuffer<K_HEIGHT,1,HLS_TNAME(SRC_T)>	    col_buf[HLS_MAT_CN(SRC_T)];

#pragma HLS ARRAY_PARTITION variable=col_buf complete dim=0
#pragma HLS ARRAY_PARTITION variable=&kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=src_kernel_win complete dim=0
#pragma HLS ARRAY_PARTITION variable=k_buf complete dim=1
#pragma HLS ARRAY_PARTITION variable=right_border_buf complete dim=0

    Point_< ap_int<4> > anchor;// -1 ~3
    anchor.x=K_WIDTH/2;
    anchor.y=K_HEIGHT/2;

    HLS_SIZE_T  rows=_src.rows;
    HLS_SIZE_T  cols=_src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    HLS_SIZE_T  heightloop= rows+K_HEIGHT-1+K_HEIGHT;
    HLS_SIZE_T  widthloop = cols+K_WIDTH-1;//one pixel overlap, so it should minus one
    Point_< ap_int<14> > ImagLoc;

    int channels=HLS_MAT_CN(SRC_T);
 loop_height: for(HLS_SIZE_T i= 0;i< heightloop;i++) {
    loop_width: for (HLS_SIZE_T j= 0;j< widthloop;j++) {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
                bool oddrow=(i+1)&1;
                bool oddcol=(j+1)&1;
                //fill data x,y are the coordinate in the image, it could be negative. For example (-1,-1) represents the interpolation pixel.
                ImagLoc.x=j-anchor.x;
                ImagLoc.y=i-K_HEIGHT-anchor.y;
                int x=ImagLoc.x ;
                if(ImagLoc.x<0)
                    x=-ImagLoc.x;
                else if( ImagLoc.x >= cols )
                    x=2*cols-ImagLoc.x-2;
channelloop:	for(HLS_CHANNEL_T k= 0; k < channels; k++)
                {
                    // column left shift
                    for(int row= 0;row<K_HEIGHT;row++)
                        for(int col= K_WIDTH-1;col>=1;col--)
                            src_kernel_win[k](row,col) = src_kernel_win[k](row,col-1);

                    for(int buf_row = 0; buf_row < K_HEIGHT; buf_row++)
                    {
                        // Fetch the column from the line buffer to shift into the window.
                        col_buf[k](buf_row,0) = k_buf[k](buf_row,x);
                    }
                    if((ImagLoc.y < (-anchor.y)) ||
                            (ImagLoc.y >= K_HEIGHT-1 && ImagLoc.y < rows-1 ))
                    {
                        //Advance load and body process
                        if(ImagLoc.x >= 0 && ImagLoc.x < cols)
                        {
                            HLS_TNAME(SRC_T) Toppixel = col_buf[k](K_HEIGHT-1,0); //k_buf[k](K_HEIGHT-1,ImagLoc.x);
                            src_kernel_win[k](K_HEIGHT-1,0) = Toppixel;
                            if(ImagLoc.x >= cols - K_WIDTH)
                            {
                                right_border_buf[k](0,ImagLoc.x-(cols-K_WIDTH)) = Toppixel;
                            }
                            for(int buf_row= K_HEIGHT-1;buf_row >= 1;buf_row--)
                            {
                                HLS_TNAME(SRC_T) temp = col_buf[k](buf_row-1,0); //k_buf[k](buf_row-1,ImagLoc.x);
                                src_kernel_win[k](buf_row-1,0)=temp;
                                k_buf[k](buf_row,x)=temp;
                                if(ImagLoc.x >= cols - K_WIDTH)
                                {
                                    right_border_buf[k](K_HEIGHT-buf_row,ImagLoc.x-(cols-K_WIDTH)) = temp;
                                }
                            }
                            HLS_TNAME(SRC_T) temp=0;
                            _src.data_stream[k] >> temp;
                            k_buf[k](0,x)=temp;
                        }
                        else if(ImagLoc.x < 0)
                        {
                            for(int buf_row = 0;buf_row < K_HEIGHT; buf_row++)
                            {
                                src_kernel_win[k](buf_row,0) = col_buf[k](buf_row,0); //k_buf[k](buf_row,x);
                            }
                        }
                        else if (ImagLoc.x >= cols)
                        {
                            for(int buf_row= 0;buf_row < K_HEIGHT; buf_row++)
                            {
                                src_kernel_win[k](buf_row,0) = right_border_buf[k](K_HEIGHT-1-buf_row,x-(cols-K_WIDTH)); //col_buf[k](buf_row+1,0); //k_buf[k](buf_row+1,x);
                            }
                        }
                    }
                    else if(ImagLoc.y >= 0) {

                        ap_int<14> ref = K_HEIGHT-1;
                        if(ImagLoc.y >= rows-1)
                            ref = rows-1;
                        ap_int<14> y = ImagLoc.y;
                        for(int buf_row= 0;buf_row<K_HEIGHT;buf_row++)
                        {
                            ap_int<14>  t = y;
                            if(y<0)
                                t=-y;
                            else if( y>= rows )
                                t=2*rows-y-2;
                            ap_int<14>  locy = ref - t;
                            assert(t < 0 || (locy >= 0 && locy < K_HEIGHT));
                            src_kernel_win[k](buf_row,0) = col_buf[k](locy,0);
                            y--;
                        }
                    }
                    // figure out the output image pixel value
                    if(i >= (K_HEIGHT + K_HEIGHT - 1) && j >= (K_WIDTH-1)) // && i < heightloop && j < widthloop)
                    {
                        typename filter2d_traits<HLS_TNAME(SRC_T), KN_T, 5*5>::ACCUM_T result=0;
                        if(!oddrow&&oddcol)
                        {
                            for(int m=0; m<K_HEIGHT; m++)
                                for(int n=0; n<K_WIDTH; n++)
                                    result= result + src_kernel_win[k](K_HEIGHT-1-m, K_WIDTH-1-n)* kernel.val[m][n];
                            _dst.data_stream[k]<< sr_cast<HLS_TNAME(DST_T)>(result);
                        }
                    }
                }
            }
             }
}
template<int SRC_T,int DST_T,int ROWS,int COLS, int DROWS, int DCOLS>
void PyrDown(
		Mat<ROWS, COLS, SRC_T>	    &_src,
                Mat<DROWS, DCOLS, DST_T>    &_dst
                )
{
    Window<5, 5, ap_fixed<16,2,AP_RND>  > kernel;
    Pyr_kernel(kernel, 1);
    PyrDown_opr<5,5>(_src, _dst, kernel);
}

}
#endif


