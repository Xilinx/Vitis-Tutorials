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
// opencv/modules/core/src/arithm.cpp

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
 * HLS Video Arithmetic Functions Header File
 */

#ifndef ___HLS__VIDEO__
#error hls_video_arithm.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef ___HLS__VIDEO_ARITHM__
#define ___HLS__VIDEO_ARITHM__

namespace hls {

#define    HLS_CMP_EQ    0
#define    HLS_CMP_GT    1
#define    HLS_CMP_GE    2
#define    HLS_CMP_LT    3
#define    HLS_CMP_LE    4
#define    HLS_CMP_NE    5

#define    HLS_THRESH_BINARY      0
#define    HLS_THRESH_BINARY_INV  1
#define    HLS_THRESH_TRUNC       2
#define    HLS_THRESH_TOZERO      3
#define    HLS_THRESH_TOZERO_INV  4

#define    HLS_REDUCE_SUM  1
#define    HLS_REDUCE_AVG  2
#define    HLS_REDUCE_MAX  3
#define    HLS_REDUCE_MIN  4



typedef  ap_fixed<64,32,AP_RND>    _AP_T;
typedef  unsigned long long        _SUM_T;

class kernel_min {
public: 
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        //dst = __HLS_MIN(src1, src2);
        if(src1<src2) {
            dst=sr_cast<DST_T>(src1);
        } else {
            dst=sr_cast<DST_T>(src2);
        }
    }
};

class kernel_max {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        //dst = __HLS_MAX(src1, src2);
        if(src1>src2) {
            dst=sr_cast<DST_T>(src1);
        } else {
            dst=sr_cast<DST_T>(src2);
        }
    }
};

class kernel_absdiff {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typename pixel_op_type<SRC1_T>::T src1_cast = src1;
        typename pixel_op_type<SRC2_T>::T src2_cast = src2;
        typedef typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<SRC2_T>::T >::ADD_T TEMP_TYPE;
        TEMP_TYPE diff = src1_cast-src2_cast;
        TEMP_TYPE mdiff = src2_cast-src1_cast;
        dst = sr_cast<DST_T>((src1_cast > src2_cast) ? diff : mdiff);
    }
};

template<typename P_T, typename SRC1_T, typename SRC2_T, typename DST_T>
void kernel_and_apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst) {
#pragma HLS inline
    dst = (sr_cast<DST_T>(src1)) & (sr_cast<DST_T>(src2)); // not work with float & float
}

template<typename P_T>
void kernel_and_apply(float& src1, float& src2, float& dst) {
#pragma HLS inline
    fp_struct<float> src1_fp(src1);
    fp_struct<float> src2_fp(src2);
    unsigned int dst_tmp = src1_fp.to_int() & src2_fp.to_int();
    fp_struct<float> dst_fp(dst_tmp);
    dst = dst_fp.to_float();
}

class kernel_and {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        kernel_and_apply<P_T>(src1, src2, dst);
    }
};

class kernel_not {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src, SRC2_T& val, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = ~(sr_cast<DST_T>(src));
    }
};

class kernel_set {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src, SRC2_T& val, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        dst = sr_cast<DST_T>(val); 
    }
};

class kernel_scale {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src, SRC2_T& val, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typedef typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<P_T>::T>::MULT_T MULT_T;
        typedef typename x_traits<typename pixel_op_type<P_T>::T, MULT_T>::ADD_T ADD_T;
        MULT_T mult_t=(typename pixel_op_type<SRC1_T>::T)src * (typename pixel_op_type<P_T>::T)p0;
        ADD_T sum_t=mult_t + (typename pixel_op_type<P_T>::T)p1;
        //dst = sr_cast<DST_T>(p0*src+p1); 
        dst = sr_cast<DST_T>(sum_t);
    }
};

class kernel_add {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<SRC2_T>::T>::ADD_T t =
            (typename pixel_op_type<SRC1_T>::T)src1 + (typename pixel_op_type<SRC2_T>::T)src2;
        dst = sr_cast<DST_T>(t);
    }
};

class kernel_sub {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<SRC2_T>::T>::SUB_T t =
            (typename pixel_op_type<SRC1_T>::T)src1 - (typename pixel_op_type<SRC2_T>::T)src2;
        dst = sr_cast<DST_T>(t);
    }
};

class kernel_subR {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<SRC2_T>::T>::SUB_T t =
            (typename pixel_op_type<SRC2_T>::T)src2 - (typename pixel_op_type<SRC1_T>::T)src1;
        dst = sr_cast<DST_T>(t);
    }
};

class kernel_mul {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T, typename P2_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P2_T p1=0, P2_T p2=0) {
#pragma HLS inline
        typedef typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<SRC2_T>::T>::MULT_T MULT_T;
        MULT_T t1 = (typename pixel_op_type<SRC1_T>::T)src1 * (typename pixel_op_type<SRC2_T>::T)src2;
        typename x_traits<MULT_T, typename pixel_op_type<P_T>::T>::MULT_T t2 = t1 * (typename pixel_op_type<P_T>::T)p0;
        //dst = sr_cast<DST_T>(p0*src1*src2); 
        dst = sr_cast<DST_T>(t2); 
    }
};

class kernel_mul2 {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<SRC2_T>::T>::MULT_T t =
            (typename pixel_op_type<SRC1_T>::T)src1 * (typename pixel_op_type<SRC2_T>::T)src2;
        //dst = sr_cast<DST_T>(src1*src2); 
        dst = sr_cast<DST_T>(t); 
    }
};

class kernel_addWeighted {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        typedef typename x_traits<typename pixel_op_type<SRC1_T>::T, typename pixel_op_type<P_T>::T>::MULT_T MULT_T1;
        MULT_T1 t1 = (typename pixel_op_type<SRC1_T>::T)src1 * (typename pixel_op_type<P_T>::T)p0;
        typedef typename x_traits<typename pixel_op_type<SRC2_T>::T, typename pixel_op_type<P_T>::T>::MULT_T MULT_T2;
        MULT_T2 t2 = (typename pixel_op_type<SRC2_T>::T)src2 * (typename pixel_op_type<P_T>::T)p1;
        typedef typename x_traits<MULT_T1, MULT_T2>::ADD_T ADD_T;
        ADD_T sum = t1 + t2 + p2;
        //dst = sr_cast<DST_T>(p0*src1+p1*src2+p2); 
        dst = sr_cast<DST_T>(sum);
    }
};

class kernel_cmp {
public:
    template<typename SRC1_T, typename SRC2_T, typename DST_T, typename P_T>
    static void apply(SRC1_T& src1, SRC2_T& src2, DST_T& dst, P_T p0=0, P_T p1=0, P_T p2=0) {
#pragma HLS inline
        switch (p0) {
            case HLS_CMP_EQ:
            dst = (src1==src2 ? 255 : 0);
            break;
            case HLS_CMP_GT:
            dst = (src1 >src2 ? 255 : 0);
            break;
            case HLS_CMP_GE:
            dst = (src1>=src2 ? 255 : 0);
            break;
            case HLS_CMP_LT:
            dst = (src1 <src2 ? 255 : 0);
            break;
            case HLS_CMP_LE:
            dst = (src1<=src2 ? 255 : 0);
            break;
            case HLS_CMP_NE:
            dst = (src1!=src2 ? 255 : 0);
            break;
            default:
            break;
        }
    }
};

class kernel_sum {
public:
    template<typename SRC_T>
    static void apply(SRC_T& s, _SUM_T& sum, _SUM_T& sqsum) {
#pragma HLS inline
        sum = sum+(_SUM_T)s;
    }
};

class kernel_avgsdv {
public:
    template<typename SRC_T>
    static void apply(SRC_T& s, _SUM_T& sum, _SUM_T& sqsum) {
#pragma HLS inline
        sum=sum+ (_SUM_T)s;
        sqsum=sqsum+ ((_SUM_T)s*(_SUM_T)s);
    }
};


// Min, Max, And(no mask), AbsDiff, Mul, AddWeighted, Cmp
template<typename KERNEL, int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T, typename P0_T, typename P1_T, typename P2_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC1_T>& src1,
            Mat<ROWS, COLS, SRC2_T>& src2,
            Mat<ROWS, COLS, DST_T>& dst,
            P0_T   p0=0,
            P1_T   p1=0,
            P2_T   p2=0)
{
    KERNEL opr;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC1_T), HLS_TNAME(SRC1_T)> s1;
    Scalar<HLS_MAT_CN(SRC2_T), HLS_TNAME(SRC2_T)> s2;
    Scalar<HLS_MAT_CN(DST_T),  HLS_TNAME(DST_T) > d;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src1 >> s1;
            src2 >> s2;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
                opr.template apply(s1.val[k], s2.val[k], d.val[k], p0, p1, p2);
            }
            dst << d;
        }
    }
}

// And (mask)
template<typename KERNEL, int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC1_T>& src1,
            Mat<ROWS, COLS, SRC2_T>& src2,
            Mat<ROWS, COLS, DST_T>& dst,
            Mat<ROWS, COLS, HLS_8UC1>& mask,
            Mat<ROWS, COLS, DST_T>& dst_ref)
{
    KERNEL opr;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC1_T), HLS_TNAME(SRC1_T)> s1;
    Scalar<HLS_MAT_CN(SRC2_T), HLS_TNAME(SRC2_T)> s2;
    Scalar<HLS_MAT_CN(DST_T),  HLS_TNAME(DST_T)>  d_ref, d;
    Scalar<1, unsigned char> m;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src1 >> s1;
            src2 >> s2;
            mask >> m;
            dst_ref >> d_ref;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
                if (m.val[0] == 0) {
                    d.val[k] = d_ref.val[k];
                } else {
                    opr.template apply(s1.val[k], s2.val[k], d.val[k]);
                }
            }
            dst << d;
        }
    }
}

// Set, Not, MinS, MaxS, Scale, CmpS
// AddS, SubS, SubRS (no-mask)
template<typename KERNEL, int ROWS, int COLS, int SRC_T, typename _T, int DST_T, typename P0_T, typename P1_T, typename P2_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC_T>& src,
            Scalar<HLS_MAT_CN(SRC_T), _T> scl,
            Mat<ROWS, COLS, DST_T>& dst,
            P0_T p0=0,
            P1_T p1=0,
            P2_T p2=0)
{
    KERNEL opr;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(SRC_T), _T> _s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    _s = scl;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
                // std::cout << "in  @ " << i << "," << j << " " << s.val[k] << "\n";
                opr.template apply(s.val[k], _s.val[k], d.val[k], p0, p1, p2);
                // std::cout << "out @ " << i << "," << j << " " << d.val[k] << "\n";
            }
            dst << d;
        }
    }
}

// Set (no src)
template<typename KERNEL, int ROWS, int COLS, typename _T, int DST_T, typename P0_T, typename P1_T, typename P2_T>
void arithm_pro(
            Scalar<HLS_MAT_CN(DST_T), _T> scl,
            Mat<ROWS, COLS, DST_T>& dst,
            P0_T p0=0,
            P1_T p1=0,
            P2_T p2=0)
{
    KERNEL opr;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(DST_T), _T> _s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    _s = scl;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            _T s = 0;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
                opr.template apply(s, _s.val[k], d.val[k], p0, p1, p2);
            }
            dst << d;
        }
    }
}

// AddS, SubS, SubRS (mask)
template<typename KERNEL, int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC_T>& src,
            Scalar<HLS_MAT_CN(SRC_T), _T> scl,
            Mat<ROWS, COLS, DST_T>& dst,
            Mat<ROWS, COLS, HLS_8UC1>& mask,
            Mat<ROWS, COLS, DST_T>& dst_ref)
{
    KERNEL opr;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s; 
    Scalar<HLS_MAT_CN(SRC_T), _T> _s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d_ref, d;
    Scalar<1, unsigned char> m;
    _s = scl; 
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            mask >> m;
            dst_ref >> d_ref;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
                if (m.val[0] == 0) {
                    d.val[k] = d_ref.val[k];
                } else {
                    //std::cout << "in  @ " << i << "," << j << " " << s.val[k] << "\n";
                    opr.template apply(s.val[k], _s.val[k], d.val[k]);
                    //std::cout << "out @ " << i << "," << j << " " << d.val[k] << "\n";
                }
            }
            dst << d;
        }
    }
}

// Sum, Mean, Avg, AvgSdv (no mask)
template<typename KERNEL, int ROWS, int COLS, int SRC_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC_T>& src,
            Scalar<HLS_MAT_CN(SRC_T), _SUM_T>& sum,
            Scalar<HLS_MAT_CN(SRC_T), _SUM_T>& sqsum)
{
    KERNEL opr;
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_init: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        sum.val[k] = 0;
        sqsum.val[k] = 0;
    }
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
        loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
                opr.template apply(s.val[k], sum.val[k], sqsum.val[k]);
            }
        }
    }
}

// Sum, Mean, Avg, AvgSdv (mask)
template<typename KERNEL, int ROWS, int COLS, int SRC_T>
void arithm_pro(
            Mat<ROWS, COLS, SRC_T>& src,
            Scalar<HLS_MAT_CN(SRC_T), _SUM_T>& sum,
            Scalar<HLS_MAT_CN(SRC_T), _SUM_T>& sqsum,
            int& ncount,
            Mat<ROWS, COLS, HLS_8UC1>& mask)
{
    KERNEL opr;
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_init: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        sum.val[k] = 0;
        sqsum.val[k] = 0;
    }
    ncount = 0;
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<1, unsigned char> m;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            mask >> m;
            if (m.val[0]) {
            loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
                    opr.template apply(s.val[k], sum.val[k], sqsum.val[k]);
                }
                ncount++;
            }
        }
    }
}

// Reduce 
template<typename INTER_SUM_T, int ROWS, int COLS, int SRC_T, int DST_ROWS, int DST_COLS, int DST_T>
void reduce_opr(
        Mat<ROWS, COLS, SRC_T> &src,
        Mat<DST_ROWS, DST_COLS, DST_T> &dst,
        int dim,
        int op=HLS_REDUCE_SUM)
{
    LineBuffer<1,DST_COLS,INTER_SUM_T> dst_buffer[HLS_MAT_CN(DST_T)];
#pragma HLS ARRAY_PARTITION variable=dst_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=dst_buffer complete dim=2
    HLS_SIZE_T rows=src.rows;
    HLS_SIZE_T cols=src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T),INTER_SUM_T> internal;
 loop_init: for(HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        internal.val[k]=0;
    }
 loop_height: for(HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
        loop_channels: for(HLS_CHANNEL_T k= 0;k< HLS_MAT_CN(SRC_T);k++) {
                HLS_TNAME(SRC_T) src_val;
                src.data_stream[k]>>src_val;
                if(dim==0) {
                    if(i!=0) {
                        internal.val[k]=(INTER_SUM_T)dst_buffer[k].val[i][0];
                    } else {
                        if(op==HLS_REDUCE_SUM||op==HLS_REDUCE_AVG)
                            internal.val[k]=0;
                        else if(op==HLS_REDUCE_MAX||op==HLS_REDUCE_MIN)
                            internal.val[k]=src_val;
                    }
                }
                switch (op) {
                    case HLS_REDUCE_SUM:
                    internal.val[k]=internal.val[k]+src_val;
                    break;
                    case HLS_REDUCE_AVG:
                    internal.val[k]=internal.val[k]+src_val;
                    break;
                    case HLS_REDUCE_MAX:
                    internal.val[k]=(internal.val[k]>(INTER_SUM_T)src_val?internal.val[k]:src_val);
                    break;
                    case HLS_REDUCE_MIN:
                    internal.val[k]=(internal.val[k]<(INTER_SUM_T)src_val?internal.val[k]:src_val);
                    break;
		}
                if(dim==0) {
                    if(i==rows-1) {
                        if(op==HLS_REDUCE_AVG)
                            dst.data_stream[k] << ((HLS_TNAME(DST_T))internal.val[k])/rows;
                        else
                            dst.data_stream[k]<< (HLS_TNAME(DST_T))internal.val[k];
                    } else {
                        dst_buffer[k].val[i][0]=internal.val[k];
                    }
                }
            }
            if(dim==1&&j==cols-1) {
            loop_output: for(HLS_CHANNEL_T k=0;k<HLS_MAT_CN(SRC_T);k++) {
                    if(op==HLS_REDUCE_AVG) {
                        dst.data_stream[k] << ((HLS_TNAME(DST_T))internal.val[k])/cols;
                    } else {
                        dst.data_stream[k] << (HLS_TNAME(DST_T))internal.val[k];
                    }
                    internal.val[k]=0;
                }
            }
        }
    }
}


/* video arithmetic functions. */
template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Min(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_min>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Max(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_max>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void MinS(
        Mat<ROWS, COLS, SRC_T>& src,
        _T value,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), _T> scl;
 loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        scl.val[k] = (value);
    }
    arithm_pro<kernel_min>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void MaxS(
        Mat<ROWS, COLS, SRC_T>& src,
        _T value,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), _T> scl;
 loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        scl.val[k] = (value);
    }
    arithm_pro<kernel_max>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void AddS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_add>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void AddS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_add>(src, scl, dst, mask, dst_ref);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_sub>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_sub>(src, scl, dst, mask, dst_ref);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubRS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_subR>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void SubRS(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_subR>(src, scl, dst, mask, dst_ref);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T, typename P_T>
void Mul(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst,
        P_T scale)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_mul>(src1, src2, dst, scale, (HLS_TNAME(DST_T))0, (HLS_TNAME(DST_T))0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Mul(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_mul2>(src1, src2, dst, (HLS_TNAME(DST_T))0, (HLS_TNAME(DST_T))0, (HLS_TNAME(DST_T))0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T, typename P_T>
void AddWeighted(
        Mat<ROWS, COLS, SRC1_T>& src1,
        P_T alpha,
        Mat<ROWS, COLS, SRC2_T>& src2,
        P_T beta,
        P_T gamma,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_addWeighted>(src1, src2, dst, alpha, beta, gamma);
}

template<int ROWS, int COLS, int SRC_T, int DST_T, typename P_T>
void Scale(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst,
        P_T scale=1.0,
        P_T shift=0.0)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> scl;
    arithm_pro<kernel_scale>(src, scl, dst, scale, shift, (P_T)0);
}

template<int ROWS, int COLS, int SRC_T, typename _T, int DST_T>
void Set(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    arithm_pro<kernel_set>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, typename _T, int DST_T>
void Set(
        Scalar<HLS_MAT_CN(DST_T), _T> scl,
        Mat<ROWS, COLS, DST_T>& dst)
{
    arithm_pro<kernel_set>(scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Zero(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst)
{
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s0;
 loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        s0.val[k] = 0;
    }
    arithm_pro<kernel_set>(src, s0, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int DST_T>
void Zero(
        Mat<ROWS, COLS, DST_T>& dst)
{
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> s0;
 loop_channels: for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
#pragma HLS unroll
        s0.val[k] = 0;
    }
    arithm_pro<kernel_set>(s0, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void AbsDiff(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_absdiff>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Not(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> scl;
    arithm_pro<kernel_not>(src, scl, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void Cmp(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst,
        int cmp_op)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_cmp>(src1, src2, dst, cmp_op, 0, 0);
}

template<int ROWS, int COLS, int SRC_T, typename P_T, int DST_T>
void CmpS(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T value,
        Mat<ROWS, COLS, DST_T>& dst,
        int cmp_op)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), P_T> scl;
    for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        scl.val[k] = (value);
    }
    arithm_pro<kernel_cmp>(src, scl, dst, cmp_op, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void And(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>&  dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_and>(src1, src2, dst, 0, 0, 0);
}

template<int ROWS, int COLS, int SRC1_T, int SRC2_T, int DST_T>
void And(
        Mat<ROWS, COLS, SRC1_T>& src1,
        Mat<ROWS, COLS, SRC2_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst,
        Mat<ROWS, COLS, HLS_8UC1>& mask,
        Mat<ROWS, COLS, DST_T>& dst_ref)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC1_T) == HLS_MAT_CN(DST_T)
        && HLS_MAT_CN(SRC2_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    arithm_pro<kernel_and>(src1, src2, dst, mask, dst_ref);
}

template<typename INTER_SUM_T, int ROWS, int COLS, int SRC_T, int DST_ROWS, int DST_COLS, int DST_T>
void Reduce(
         Mat<ROWS, COLS, SRC_T> &src,
         Mat<DST_ROWS, DST_COLS, DST_T> &dst,
         int dim,
         int op=HLS_REDUCE_SUM)
{
    reduce_opr<INTER_SUM_T>(src, dst, dim, op);
}

template<int ROWS, int COLS, int SRC_T, int DST_T, typename P_T>
void Range(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst,
        P_T start,
        P_T end)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
    assert("Source must be single channel image"
        && HLS_MAT_CN(SRC_T) == 1);
#endif
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    _AP_T _dis = end-start;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(DST_T); k++) {
                d.val[k] = _dis*(i*cols+j)/(rows*cols);
            }
            dst << d;
        }
    }
}

template<typename DST_T, int ROWS, int COLS, int SRC_T>
Scalar<HLS_MAT_CN(SRC_T), DST_T> Sum(
        Mat<ROWS, COLS, SRC_T>& src)
{
    Scalar<HLS_MAT_CN(SRC_T), _SUM_T> sum, sqsum;
    int ncount = src.rows * src.cols;
    arithm_pro<kernel_sum>(src, sum, sqsum);
    Scalar<HLS_MAT_CN(SRC_T), DST_T> res;
    for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        res.val[k] = sr_cast<DST_T>(sum.val[k]);
    }
    return res;
}

template<typename DST_T, int ROWS, int COLS, int SRC_T>
Scalar<HLS_MAT_CN(SRC_T), DST_T> Avg(
        Mat<ROWS, COLS, SRC_T>& src)
{
    Scalar<HLS_MAT_CN(SRC_T), _SUM_T> sum, sqsum;
    int ncount = src.rows * src.cols;
    _AP_T _sum = 0;
    arithm_pro<kernel_sum>(src, sum, sqsum);
    Scalar<HLS_MAT_CN(SRC_T), DST_T> avg;
    for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        _sum = sum.val[k];
        avg.val[k] = _sum/ncount;
    }
    return avg;
}

template<typename DST_T, int ROWS, int COLS, int SRC_T>
Scalar<HLS_MAT_CN(SRC_T), DST_T> Avg(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, HLS_8UC1>& mask)
{
    Scalar<HLS_MAT_CN(SRC_T), _SUM_T> sum, sqsum;
    int ncount = 0;
    _AP_T _sum = 0;
    arithm_pro<kernel_sum>(src, sum, sqsum, ncount, mask);
    Scalar<HLS_MAT_CN(SRC_T), DST_T> avg;
    for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
#pragma HLS unroll
        _sum = sum.val[k];
        avg.val[k] = _sum/ncount; 
    }
    return avg;
}

template<typename DST_T, int ROWS, int COLS, int SRC_T>
DST_T Mean(
        Mat<ROWS, COLS, SRC_T>& src)
{
    Scalar<HLS_MAT_CN(SRC_T), DST_T> avg;
    avg = Avg<DST_T>(src);
    return avg.val[0];
}

template<typename DST_T, int ROWS, int COLS, int SRC_T>
DST_T Mean(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, HLS_8UC1>& mask)
{
    Scalar<HLS_MAT_CN(SRC_T), DST_T> avg;
    avg = Avg<DST_T>(src, mask);
    return avg.val[0];
}

template<int ROWS, int COLS, int SRC_T, typename _T>
void AvgSdv(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T>& avg,
        Scalar<HLS_MAT_CN(SRC_T), _T>& sdv)
{
    Scalar<HLS_MAT_CN(SRC_T), _SUM_T> sum, sqsum;
    int ncount = src.rows * src.cols;
    _AP_T _sum = 0;
    ap_fixed<64,40,AP_RND> _sqsum = 0;
    arithm_pro<kernel_avgsdv>(src, sum, sqsum);
    for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
        _sum = sum.val[k];
        _sqsum = sqsum.val[k];
        avg.val[k] = _sum/ncount;
        sdv.val[k] = sqrt((double)((_sqsum*ncount)-((_sum)*(_sum))))/ncount;
    }
}

template<int ROWS, int COLS, int SRC_T, typename _T>
void AvgSdv(
        Mat<ROWS, COLS, SRC_T>& src,
        Scalar<HLS_MAT_CN(SRC_T), _T>& avg,
        Scalar<HLS_MAT_CN(SRC_T), _T>& sdv,
        Mat<ROWS, COLS, HLS_8UC1>& mask)
{
    Scalar<HLS_MAT_CN(SRC_T), _SUM_T> sum, sqsum;
    int ncount = 0;
    _AP_T _sum = 0;
    ap_fixed<64,40,AP_RND> _sqsum = 0;
    arithm_pro<kernel_avgsdv>(src, sum, sqsum, ncount, mask);
    for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
        _sum = sum.val[k];
        _sqsum = sqsum.val[k];
        avg.val[k] = _sum/ncount;
        sdv.val[k] = sqrt((double)((_sqsum*ncount)-((_sum)*(_sum))))/ncount;
    }
}

template<typename S_T>
class MinMaxLoc_opr
{
public:
template<int ROWS, int COLS, int SRC_T, typename P_T>
static void get_max_min(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T* min_val,
        P_T* max_val,
        Point& min_loc,
        Point& max_loc)
{
#pragma HLS inline
#ifndef __SYNTHESIS__
    assert("Source must be single channel image"
        && HLS_MAT_CN(SRC_T) == 1);
#endif
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Name<HLS_TNAME(SRC_T)> type_val;
    HLS_TNAME(SRC_T) _min_val = (HLS_TNAME(SRC_T)) type_val._max;
    HLS_TNAME(SRC_T) _max_val = (HLS_TNAME(SRC_T)) type_val._min;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            if (s.val[0] < _min_val) {
                _min_val = s.val[0];
                min_loc.x = j;
                min_loc.y = i;
            }
            if (s.val[0] > _max_val) {
                _max_val = s.val[0];
                max_loc.x = j;
                max_loc.y = i;
            }
        }
    }
    *min_val = sr_cast<P_T>(_min_val);
    *max_val = sr_cast<P_T>(_max_val);
}

template<int ROWS, int COLS, int SRC_T, typename P_T>
static void get_max_min(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T* min_val,
        P_T* max_val,
        Point& min_loc,
        Point& max_loc,
        Mat<ROWS, COLS, HLS_8UC1>& mask)
{
#pragma HLS inline
#ifndef __SYNTHESIS__
    assert("Source must be single channel image"
        && HLS_MAT_CN(SRC_T) == 1);
#endif
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<1, unsigned char> m;
    Name<HLS_TNAME(SRC_T)> type_val;
    HLS_TNAME(SRC_T) _min_val = (HLS_TNAME(SRC_T))type_val._max;
    HLS_TNAME(SRC_T) _max_val = (HLS_TNAME(SRC_T))type_val._min;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            mask >> m;
            if (m.val[0]) {
                if (s.val[0] < (_min_val)) {
                    _min_val = s.val[0];
                    min_loc.x = j;
                    min_loc.y = i;
                }
                if (s.val[0] > (_max_val)) {
                    _max_val = s.val[0];
                    max_loc.x = j;
                    max_loc.y = i;
                }
            }
        }
    }
    *min_val = sr_cast<P_T>(_min_val);
    *max_val = sr_cast<P_T>(_max_val);
}

};

template< >
class MinMaxLoc_opr<float>
{
public:
template<int ROWS, int COLS, int SRC_T, typename P_T>
static void get_max_min(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T* min_val,
        P_T* max_val,
        Point& min_loc,
        Point& max_loc)
{
#pragma HLS inline
#ifndef __SYNTHESIS__
    assert("Source must be single channel image"
        && HLS_MAT_CN(SRC_T) == 1);
#endif
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Name<HLS_TNAME(SRC_T)> type_val;
    HLS_TNAME(SRC_T) _min_val = (HLS_TNAME(SRC_T)) type_val._max;
    HLS_TNAME(SRC_T) _max_val = (HLS_TNAME(SRC_T)) type_val._min;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            fp_struct<float> min_fp(_min_val);
	        fp_struct<float> max_fp(_max_val);
	        fp_struct<float> s_fp(s.val[0]);
            if (min_fp.sign==1 && s_fp.sign==1) {
                if(s_fp.to_int() > min_fp.to_int()) {
                    _min_val = s.val[0];
                    min_loc.x = j;
                    min_loc.y = i;
                }
            }
            else {
                if(s_fp.to_int() < min_fp.to_int()) {
                    _min_val = s.val[0];
                    min_loc.x = j;
                    min_loc.y = i;
                }
            }

            if (max_fp.sign==1 && s_fp.sign==1) {
                if(s_fp.to_int() < max_fp.to_int()) {
                    _max_val = s.val[0];
                    max_loc.x = j;
                    max_loc.y = i;
                }
            }
            else {
                if(s_fp.to_int() > max_fp.to_int()) {
                    _max_val = s.val[0];
                    max_loc.x = j;
                    max_loc.y = i;
                }
            }
        }
    }
    *min_val = sr_cast<P_T>(_min_val);
    *max_val = sr_cast<P_T>(_max_val);
}

template<int ROWS, int COLS, int SRC_T, typename P_T>
static void get_max_min(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T* min_val,
        P_T* max_val,
        Point& min_loc,
        Point& max_loc,
        Mat<ROWS, COLS, HLS_8UC1>& mask)
{
#pragma HLS inline
#ifndef __SYNTHESIS__
    assert("Source must be single channel image"
        && HLS_MAT_CN(SRC_T) == 1);
#endif
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<1, unsigned char> m;
    Name<HLS_TNAME(SRC_T)> type_val;
    HLS_TNAME(SRC_T) _min_val = (HLS_TNAME(SRC_T))type_val._max;
    HLS_TNAME(SRC_T) _max_val = (HLS_TNAME(SRC_T))type_val._min;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            mask >> m;
            fp_struct<float> min_fp(_min_val);
	        fp_struct<float> max_fp(_max_val);
	        fp_struct<float> s_fp(s.val[0]);
            if (m.val[0]) {
                if (min_fp.sign==1 && s_fp.sign==1) {
                    if(s_fp.to_int() > min_fp.to_int()) {
                        _min_val = s.val[0];
                        min_loc.x = j;
                        min_loc.y = i;
                    }
                }
                else {
                    if(s_fp.to_int() < min_fp.to_int()) {
                        _min_val = s.val[0];
                        min_loc.x = j;
                        min_loc.y = i;
                    }
                }

                if (max_fp.sign==1 && s_fp.sign==1) {
                    if(s_fp.to_int() < max_fp.to_int()) {
                        _max_val = s.val[0];
                        max_loc.x = j;
                        max_loc.y = i;
                    }
                }
                else {
                    if(s_fp.to_int() > max_fp.to_int()) {
                        _max_val = s.val[0];
                        max_loc.x = j;
                        max_loc.y = i;
                    }
                }
            }
        }
    }
    *min_val = sr_cast<P_T>(_min_val);
    *max_val = sr_cast<P_T>(_max_val);
}

};

template<int ROWS, int COLS, int SRC_T, typename P_T>
void MinMaxLoc(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T* min_val,
        P_T* max_val,
        Point& min_loc,
        Point& max_loc)
{
    MinMaxLoc_opr< HLS_TNAME(SRC_T) >::get_max_min(src, min_val, max_val, min_loc, max_loc);
}

template<int ROWS, int COLS, int SRC_T, typename P_T>
void MinMaxLoc(
        Mat<ROWS, COLS, SRC_T>& src,
        P_T* min_val,
        P_T* max_val,
        Point& min_loc,
        Point& max_loc,
        Mat<ROWS, COLS, HLS_8UC1>& mask)
{
    MinMaxLoc_opr< HLS_TNAME(SRC_T) >::get_max_min(src, min_val, max_val, min_loc, max_loc, mask);
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Threshold(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst,
        HLS_TNAME(SRC_T) thresh,
        HLS_TNAME(DST_T) maxval,
        int thresh_type)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst));
    assert("Source must be single channel image"
        && HLS_MAT_CN(SRC_T) == 1);
#endif
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_TNAME(SRC_T) _thresh = sr_cast<HLS_TNAME(SRC_T)>(thresh);
    HLS_TNAME(DST_T) _maxval = sr_cast<HLS_TNAME(DST_T)>(maxval);
    HLS_TNAME(DST_T) _zero = 0;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            for (HLS_CHANNEL_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
                switch (thresh_type) {
                    case HLS_THRESH_BINARY:
                    d.val[k] = (s.val[k] > _thresh) ? (_maxval) : _zero;
                    break;
                    case HLS_THRESH_BINARY_INV:
                    d.val[k] = (s.val[k] > _thresh) ? _zero : (_maxval);
                    break;
                    case HLS_THRESH_TRUNC:
                    d.val[k] = (s.val[k] > _thresh) ? (_thresh) : s.val[k];
                    break;
                    case HLS_THRESH_TOZERO:
                    d.val[k] = (s.val[k] > _thresh) ? (s.val[k]) : _zero;
                    break;
                    case HLS_THRESH_TOZERO_INV:
                    d.val[k] = (s.val[k] > _thresh) ? _zero : (s.val[k]);
                    break;
                    default:
                    d.val[k] = s.val[k];
                    break;
                }
            }
            dst << d;
        }
    }
}

template<int ROWS, int COLS, int SRC_T>
void Consume(
        Mat<ROWS, COLS, SRC_T>& src)
{
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Duplicate(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst1,
        Mat<ROWS, COLS, DST_T>& dst2)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have same number of channels" 
        && HLS_MAT_CN(SRC_T) == HLS_MAT_CN(DST_T));
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst1)
        && HLS_ARE_SIZES_EQ(src, dst2));
#endif
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            dst1 << s;
            dst2 << s;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Split(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst0,
        Mat<ROWS, COLS, DST_T>& dst1)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have required number of channels" 
        && HLS_MAT_CN(SRC_T) == 2
        && HLS_MAT_CN(DST_T) == 1);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst0)
        && HLS_ARE_SIZES_EQ(src, dst1));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            d.val[0] = s.val[0]; dst0 << d;
            d.val[0] = s.val[1]; dst1 << d;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Split(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst0,
        Mat<ROWS, COLS, DST_T>& dst1,
        Mat<ROWS, COLS, DST_T>& dst2)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have required number of channels" 
        && HLS_MAT_CN(SRC_T) == 3
        && HLS_MAT_CN(DST_T) == 1);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst0)
        && HLS_ARE_SIZES_EQ(src, dst1)
        && HLS_ARE_SIZES_EQ(src, dst2));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            d.val[0] = s.val[0]; dst0 << d;
            d.val[0] = s.val[1]; dst1 << d;
            d.val[0] = s.val[2]; dst2 << d;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Split(
        Mat<ROWS, COLS, SRC_T>& src,
        Mat<ROWS, COLS, DST_T>& dst0,
        Mat<ROWS, COLS, DST_T>& dst1,
        Mat<ROWS, COLS, DST_T>& dst2,
        Mat<ROWS, COLS, DST_T>& dst3)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have required number of channels" 
        && HLS_MAT_CN(SRC_T) == 4
        && HLS_MAT_CN(DST_T) == 1);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src, dst0)
        && HLS_ARE_SIZES_EQ(src, dst1)
        && HLS_ARE_SIZES_EQ(src, dst2)
        && HLS_ARE_SIZES_EQ(src, dst3));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src >> s;
            d.val[0] = s.val[0]; dst0 << d;
            d.val[0] = s.val[1]; dst1 << d;
            d.val[0] = s.val[2]; dst2 << d;
            d.val[0] = s.val[3]; dst3 << d;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Merge(
        Mat<ROWS, COLS, SRC_T>& src0,
        Mat<ROWS, COLS, SRC_T>& src1,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have required number of channels" 
        && HLS_MAT_CN(SRC_T) == 1
        && HLS_MAT_CN(DST_T) == 2);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src0, dst)
        && HLS_ARE_SIZES_EQ(src1, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src0 >> s; d.val[0] = s.val[0];
            src1 >> s; d.val[1] = s.val[0];
            dst << d;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Merge(
        Mat<ROWS, COLS, SRC_T>& src0,
        Mat<ROWS, COLS, SRC_T>& src1,
        Mat<ROWS, COLS, SRC_T>& src2,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have required number of channels" 
        && HLS_MAT_CN(SRC_T) == 1
        && HLS_MAT_CN(DST_T) == 3);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src0, dst)
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src0 >> s; d.val[0] = s.val[0];
            src1 >> s; d.val[1] = s.val[0];
            src2 >> s; d.val[2] = s.val[0];
            dst << d;
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Merge(
        Mat<ROWS, COLS, SRC_T>& src0,
        Mat<ROWS, COLS, SRC_T>& src1,
        Mat<ROWS, COLS, SRC_T>& src2,
        Mat<ROWS, COLS, SRC_T>& src3,
        Mat<ROWS, COLS, DST_T>& dst)
{
#ifndef __SYNTHESIS__
    assert("Source and destination images must have required number of channels" 
        && HLS_MAT_CN(SRC_T) == 1
        && HLS_MAT_CN(DST_T) == 4);
    assert("Source and destination images must have same sizes"
        && HLS_ARE_SIZES_EQ(src0, dst)
        && HLS_ARE_SIZES_EQ(src1, dst)
        && HLS_ARE_SIZES_EQ(src2, dst)
        && HLS_ARE_SIZES_EQ(src3, dst));
#endif
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    HLS_SIZE_T rows = dst.rows;
    HLS_SIZE_T cols = dst.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
 loop_height: for (HLS_SIZE_T i = 0; i < rows; i++) {
    loop_width: for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            src0 >> s; d.val[0] = s.val[0];
            src1 >> s; d.val[1] = s.val[0];
            src2 >> s; d.val[2] = s.val[0];
            src3 >> s; d.val[3] = s.val[0];
            dst << d;
        }
    }
}

} // namespace hls

#endif 



