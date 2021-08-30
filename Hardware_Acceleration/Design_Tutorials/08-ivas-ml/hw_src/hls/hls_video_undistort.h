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
#error hls_video_undistort.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef HLS_VIDEO_UNDISTORT_H_
#define HLS_VIDEO_UNDISTORT_H_

namespace hls {

template <int N, typename T,typename PT>
void LU( T A[N][N], PT pos[N])
{
    int  imax = 0;
    T lu[N] ;
    for (int i = 0; i < N; i++)
    {
        lu[i]=0;
    }
    for (int i = 0; i < N; i++) {
        T big = 0.0;
        for (int j = 0; j < N; j++) {
            T temp = ::hls::abs((double)A[i][j]);
            if (temp > big) big = temp;
        }
        lu[i] = ((T)1.0)/big;
    }
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < j; i++) {
            T sum = A[i][j];
            for (int k = 0; k < i; k++) sum -= A[i][k] * A[k][j];
            A[i][j] = sum;
        }
        T big = 0.0;
        for (int i = j; i < N; i++) {
            T sum = A[i][j];
            for (int k = 0; k < j; k++) sum -= A[i][k] * A[k][j];
            A[i][j] = sum;
            T dum = lu[i] *((T) ::hls::abs((double)sum));
            if (dum >= big) {
                big = dum;
                imax = i;
            }
        }
        if (j != imax&&imax<N&&imax>=0) {
            for (int i = 0; i < N; i++) {
                T mid = A[imax][i];
                A[imax][i] = A[j][i];
                A[j][i] = mid;
            }
            T mid = lu[j];
            lu[j] = lu[imax];
            lu[imax] = mid;
        }
        pos[j] = imax;
        if (j != N - 1) {
            T dum = ((T)1.0)/(A[j][j]);
            for (int i = j + 1; i < N; i++) A[i][j] *= dum;
        }
    }
}
template <int N, typename T, typename PT,typename BT>
void LUex(T A[N][N], PT pos[N],BT b[N])
{
    int ii = 0;
    for (int i = 0; i < N; i++) {
        int ip = pos[i];
        T sum=0;
        if(ip<N&& ip>=0)
        {
            sum = b[ip];
            b[ip] = b[i];
        }
        if (ii != 0)
            for (int j = ii - 1; j < i; j++) sum -= A[i][j] * b[j];
        else
            ii = i + 1;
        b[i] = sum;
    }
    for (int i = N - 1; i >= 0; i--) {
        T sum = b[i];
        for (int j = i + 1; j < N; j++) sum -= A[i][j]*b[j];
        b[i] = (sum)*((T) 1.0/A[i][i]);
    }
}


template <int N,typename T>
void Invert(T A[N][N],T Ar[N][N], int flags=0)
{
    T b[N];
    int pos[N];
    for (int i = 0; i < N; i++) {
        b[i]=0;
        pos[i]=0;
    }
    LU<N>(A,pos);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) b[j] = 0;
        b[i] = 1.0;
        LUex<N>(A,pos,b);
        for (int j = 0; j < N; j++) Ar[j][i] = b[j];
    }
}
template<int AROW, int ACOL, int BCOL, typename AT, typename BT, typename CT>
void mul(
               AT A [AROW][ ACOL],
               BT B [ACOL][ BCOL],
               CT C [AROW][ BCOL]
)
{
 loop_i: for(int i=0; i<AROW; i++)
    {
    loop_j: for(int j=0; j<BCOL; j++)
        {
            CT sum=0;
        loop_k: for(int k=0; k<ACOL; k++)
                {
                sum=sum+A[i][k]*B[k][j];

            }
            C[i][j]=sum;
        }
    }
}


#define HLS_INTER_TAB_SIZE 32
#define HLS_INTER_BITS     5

template <int WIN_ROW, int ROWS, int COLS, int SRC_T, int DST_T, int MAP1_T, int MAP2_T>
void Remap_nearest(
           Mat<ROWS, COLS, SRC_T>    &src,
           Mat<ROWS, COLS, DST_T>    &dst,
           Mat<ROWS, COLS, MAP1_T>   &map1,
           Mat<ROWS, COLS, MAP2_T>   &map2
        //int bordertype=BORDER_CONSTANT
        //const Scalar<HLS_MAT_CN(SRC_T),HLS_NAME_MAT(SRC_T)> borderValue
        )
{
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> buf[WIN_ROW][COLS];
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>      s;
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)>      d;
    Scalar<HLS_MAT_CN(MAP1_T),HLS_TNAME(MAP1_T)>    m1;
    Scalar<HLS_MAT_CN(MAP2_T),HLS_TNAME(MAP2_T)>    m2;
    int rows=src.rows;
    int cols=src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    int ishift=WIN_ROW/2;
    int r[WIN_ROW] = {};

 loop_height: for( int i=0; i< rows+ishift; i++)
    {
#pragma HLS LOOP_FLATTEN OFF
    loop_width: for( int j=0; j< cols; j++)
        {
#pragma HLS PIPELINE II=1
#pragma HLS dependence array inter false
            if(i<rows&& j<cols)
            {
                src >> s;
            }
            buf[i % WIN_ROW][j] = s;
            r[i % WIN_ROW] = i;

            if(i>=ishift)
            {
                map1 >> m1;
                map2 >> m2;
                int x=m1.val[0]+1; // OpenCV is off by one?
                int y=m1.val[1]+1;

                bool in_range = (y>=0 && y<rows && r[y%WIN_ROW] == y && x>=0 && x<cols);
                if(in_range) {
                    d = buf[y%WIN_ROW][x];
                } else {
                    for(int k=0;k<HLS_MAT_CN(SRC_T);k++) {
                        d.val[k]=0;
                    }
                }
                dst<< d;
            }
        }
    }
}

template <int WIN_ROW, int ROWS, int COLS, int SRC_T, int DST_T, int MAP1_T, int MAP2_T>
void Remap_linear(
           Mat<ROWS, COLS, SRC_T>    &src,
           Mat<ROWS, COLS, DST_T>    &dst,
           Mat<ROWS, COLS, MAP1_T>   &map1,
           Mat<ROWS, COLS, MAP2_T>   &map2
        //int bordertype=BORDER_CONSTANT
        //const Scalar<HLS_MAT_CN(SRC_T),HLS_NAME_MAT(SRC_T)> borderValue
        )
{
    // Add one to always get zero for boundary interpolation. Maybe need initialization here?
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> buf[WIN_ROW/2+1][2][COLS/2+1][2];
#pragma HLS array_partition complete variable=buf dim=2
#pragma HLS array_partition complete variable=buf dim=4
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>      s;
    Scalar<HLS_MAT_CN(MAP1_T),HLS_TNAME(MAP1_T)>    m1;
    Scalar<HLS_MAT_CN(MAP2_T),HLS_TNAME(MAP2_T)>    m2;
    int rows=src.rows;
    int cols=src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    int ishift=WIN_ROW/2;
    int r1[WIN_ROW] = {};
    int r2[WIN_ROW] = {};

 loop_height: for( int i=0; i< rows+ishift; i++)
    {
#pragma HLS LOOP_FLATTEN OFF
    loop_width: for( int j=0; j< cols; j++)
        {
#pragma HLS PIPELINE II=1
#pragma HLS dependence array inter false
            if(i<rows&& j<cols)
            {
                src >> s;
            }
            if((i % WIN_ROW) % 2) {
                buf[(i % WIN_ROW)/2][(i % WIN_ROW) % 2][j/2][j%2] = s;
            } else {
                buf[(i % WIN_ROW)/2][(i % WIN_ROW) % 2][j/2][j%2] = s;
            }
            r1[i % WIN_ROW] = i;
            r2[i % WIN_ROW] = i;

            if(i>=ishift)
            {
                map1 >> m1;
                map2 >> m2;
                int x=m1.val[0];
                int y=m1.val[1];

                int ynext = y+1;

                ap_ufixed<HLS_INTER_BITS, 0> iu, iv;
                iu(HLS_INTER_BITS-1, 0) = m2.val[0] & (HLS_INTER_TAB_SIZE-1);
                iv(HLS_INTER_BITS-1, 0) = (m2.val[0] >> HLS_INTER_BITS) & (HLS_INTER_TAB_SIZE-1);

                // Note that the range here is larger than expected by 1 horizontal and 1 vertical pixel, to allow
                // Interpolating at the edge of the image
                bool in_range = (y>=0 && y<rows && r1[y%WIN_ROW] == y && r2[ynext%WIN_ROW] == ynext && x>=0 && x<cols);

                int xa0, xa1, ya0, ya1;
                // The buffer is essentially cyclic partitioned, but we have
                // to do this manually because HLS can't figure it out.
                // The code below is wierd, but it is this code expanded.
               //  if ((y % WIN_ROW) % 2) {
//                     // Case 1, where y hits in bank 1 and ynext in bank 0
//                     ya0 = (ynext%WIN_ROW)/2;
//                     ya1 = (y%WIN_ROW)/2;
//                 } else {
//                     // The simpler case, where y hits in bank 0 and ynext hits in bank 1
//                     ya0 = (y%WIN_ROW)/2;
//                     ya1 = (ynext%WIN_ROW)/2;
//                 }
                // Both cases reduce to this, if WIN_ROW is a power of two.
                assert(((WIN_ROW & (WIN_ROW-1)) == 0) && "WIN_ROW must be a power of two");
                xa0 = x/2 + x%2;
                xa1 = x/2;
                ya0 = (y/2 + y%2)%(WIN_ROW/2);
                ya1 = (y/2)%(WIN_ROW/2);

                Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)>  d00,d01,d10,d11;
                d00=buf[ya0][0][xa0][0];
                d01=buf[ya0][0][xa1][1];
                d10=buf[ya1][1][xa0][0];
                d11=buf[ya1][1][xa1][1];

                Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)>  d_t00,d_t01,d_t10,d_t11;
                if(x%2) {
                    d_t00 = d00;
                    d_t01 = d01;
                    d00 = d_t01;
                    d01 = d_t00;

                    d_t10 = d10;
                    d_t11 = d11;
                    d10 = d_t11;
                    d11 = d_t10;
//                    std::swap(d00,d01);
//                    std::swap(d10,d11);
                }
                if(y%2) {
                    d_t00 = d00;
                    d_t10 = d10;
                    d00 = d_t10;
                    d10 = d_t00;

                    d_t01 = d01;
                    d_t11 = d11;
                    d01 = d_t11;
                    d11 = d_t01;
//                    std::swap(d00,d10);
//                    std::swap(d01,d11);
                }
                ap_ufixed<2*HLS_INTER_BITS + 1, 1> k01 = (1-iv)*(  iu); // iu-iu*iv
                ap_ufixed<2*HLS_INTER_BITS + 1, 1> k10 = (  iv)*(1-iu); // iv-iu*iv
                ap_ufixed<2*HLS_INTER_BITS + 1, 1> k11 = (  iv)*(  iu); // iu*iv
                ap_ufixed<2*HLS_INTER_BITS + 1, 1> k00 = 1-iv-k01; //(1-iv)*(1-iu) = 1-iu-iv+iu*iv = 1-iv-k01
                assert( k00 + k01 + k10 + k11 == 1);

                Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)> d;

                for(int k = 0; k < HLS_MAT_CN(DST_T); k++) {
                    if(in_range) {
                        d.val[k] =
                            d00.val[k] * k00 +
                            d01.val[k] * k01 +
                            d10.val[k] * k10 +
                            d11.val[k] * k11;
                    } else {
                        d.val[k] = 0;
                    }
                }
                dst<< d;
            }
        }
    }
}

template <int WIN_ROW, int ROWS, int COLS, int SRC_T, int DST_T, int MAP1_T, int MAP2_T>
void Remap(
           Mat<ROWS, COLS, SRC_T>    &src,
           Mat<ROWS, COLS, DST_T>    &dst,
           Mat<ROWS, COLS, MAP1_T>   &map1,
           Mat<ROWS, COLS, MAP2_T>   &map2,
           int interpolation=HLS_INTER_NEAREST
        //int bordertype=BORDER_CONSTANT
        //const Scalar<HLS_MAT_CN(SRC_T),HLS_NAME_MAT(SRC_T)> borderValue
        )
{
    if(interpolation==HLS_INTER_NEAREST) {
        Remap_nearest<WIN_ROW>(src, dst, map1, map2);
    } else if(interpolation==HLS_INTER_LINEAR) {
        Remap_linear<WIN_ROW>(src, dst, map1, map2);
    }
}

//#define _HLSCLIB_DEBUG_

template <typename T>
class RangeAnalyzer {
public:
#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
    T min,max;
    RangeAnalyzer() {
        min=hls::numeric_limits<T>::max();
        max=hls::numeric_limits<T>::min();
    }
    void operator ()(T x) {
        if(x < min) min = x;
        if(x > max) max = x;
    }
#else
    void operator ()(T x) {};
#endif

    //    friend std::ostream& operator<<(std::ostream& os, const RangeAnalyzer<T>& ra);
};
#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)

#include <iostream>
template <typename T>
std::ostream& operator<<(std::ostream& os, const RangeAnalyzer<T>& ra) {
    return os << "[" << ra.min << ", " << ra.max << "]";
}
#endif

#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
static RangeAnalyzer<float> rax, ray, raxerr;
static RangeAnalyzer<float> ra1, ra2, rawerr;
static RangeAnalyzer<float> rakr1, rakr2, rakrerr;
static RangeAnalyzer<float> rax2, ray2,rar2,ra2xy;
#endif

// Approximation of 1/x around x=1.
// In most cases (e.g. float), we just do computation in type T.
template<typename T>
T one_over_x_approx(T x) {
    return T(1.0)/x;
}

// if x is ~ 1+delta, then 1/x is 1-delta+..., or 2-x;
template <int W, int I, ap_q_mode Q, ap_o_mode O>
ap_fixed<W,I,Q,O> one_over_x_approx(ap_fixed<W,I,Q,O> x) {
    return 2-x;
}

template <int W, int I, ap_q_mode Q, ap_o_mode O>
ap_ufixed<W,I,Q,O> one_over_x_approx(ap_ufixed<W,I,Q,O> x) {
    return 2-x;
}

// Approximation of 1/(1+x) around x=0.
// In most cases (e.g. float), we just do computation in type T.
template<typename T>
T one_over_one_plus_x_approx(T x) {
    return T(1.0)/(T(1.0)+x);
}

// if x is ~ 1+delta, then 1/x is 1-delta+delta^2-...
template <int W, int I, ap_q_mode Q, ap_o_mode O>
ap_fixed<W,I,Q,O> one_over_one_plus_x_approx(ap_fixed<W,I,Q,O> x) {
    return 1-x;
}

template <int W, int I, ap_q_mode Q, ap_o_mode O>
ap_ufixed<W,I,Q,O> one_over_one_plus_x_approx(ap_ufixed<W,I,Q,O> x) {
    return 1-x;
}

template <typename FRAMET, typename FRAME2T, typename ROWT, typename COLT, typename ROWOUTT, typename COLOUTT, typename CMT, typename DT, typename ICMT, int N>
void computeUndistortCoordinates(Window<3,3, CMT> cameraMatrix,
                                 DT (&distCoeffs)[N],
                                 Window<3,3, ICMT> ir,
                                 int noRotation,
                                 ROWT i, COLT j,
                                 ROWOUTT &u, COLOUTT &v) {

    DT zo=0;
    DT k1=distCoeffs[0];
    DT k2=distCoeffs[1];
    DT p1=distCoeffs[2];
    DT p2=distCoeffs[3];
    DT k3= N>=5? distCoeffs[4] : zo;
    DT k4= N>=8? distCoeffs[5] : zo;
    DT k5= N>=8? distCoeffs[6] : zo;
    DT k6= N>=8? distCoeffs[7] : zo;
    CMT u0= cameraMatrix.val[0][2];
    CMT v0= cameraMatrix.val[1][2];
    CMT fx= cameraMatrix.val[0][0];
    CMT fy= cameraMatrix.val[1][1];


    // FRAMET is the type of normalized coordinates.
    // If IR is float, then FRAMET will also be float
    // If IR is ap_fixed, then FRAMET will be some ap_fixed type with more integer bits
    //             typedef typename x_traits<typename x_traits<ROWT,ICMT>::MULT_T,
    //                                       typename x_traits<COLT,ICMT>::MULT_T >::ADD_T FRAMET;
    //    typedef ap_fixed<18,2, AP_TRN, AP_SAT> FRAMET; // Assume frame coordinates are in [-1,1)
    //    typedef CMT FRAMET;
    //typedef float FRAMET;

    FRAMET _x, _y, x, y;
    _x=i*ir.val[0][1] + j * ir.val[0][0] + ir.val[0][2];
    _y=i*ir.val[1][1] + j * ir.val[1][0] + ir.val[1][2];

    float x_gold = ((float)i*(float)ir.val[0][1] + (float)j*(float)ir.val[0][0] + (float)ir.val[0][2]);
    if(noRotation) {
        // A special case if there is no rotation: equivalent to cv::initUndistortMap
        x=_x;
        y=_y;
    } else {
        FRAMET w=i*ir.val[2][1] + j * ir.val[2][0] + ir.val[2][2];
        FRAMET winv = one_over_x_approx(w);
        float winv_gold = 1.0f/((float)i*(float)ir.val[2][1] + (float)j*(float)ir.val[2][0] + (float)ir.val[2][2]);
#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
        ra1(w);
        ra2(winv);
        rawerr((float)winv-winv_gold);
#endif
        x = (FRAMET)(_x*winv);
        y = (FRAMET)(_y*winv);
        x_gold = x_gold*winv_gold;
    }

    typename x_traits<FRAMET,FRAMET>::MULT_T x2t = x*x, y2t = y*y;  // Full precision result here.
    FRAME2T _2xy = 2*x*y;
    FRAME2T r2 = x2t + y2t;
    FRAME2T x2 = x2t, y2 = y2t;

    FRAMET kr = (1 + FRAMET(FRAMET(k3*r2 + k2)*r2 + k1)*r2);
    FRAME2T krd = FRAMET(FRAMET(k6*r2 + k5)*r2 + k4)*r2;

    if(N >5) kr = kr*one_over_one_plus_x_approx(krd);

    float kr_gold = (1 + (((float)k3*(float)r2 + (float)k2)*(float)r2 + (float)k1)*(float)r2)/
                    (1 + (((float)k6*(float)r2 + (float)k5)*(float)r2 + (float)k4)*(float)r2);

    u = fx*(FRAMET(x*kr) + FRAMET(p1*_2xy) + FRAMET(p2*(2*x2 + r2))) + u0;
    v = fy*(FRAMET(y*kr) + FRAMET(p1*(r2 + 2*y2)) + FRAMET(p2*_2xy)) + v0;

#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
    rax(_x);
    ray(_y);
    raxerr((float)x-x_gold);
    rax2(x2);
    ray2(y2);
    ra2xy(_2xy);
    rar2(r2);
    rakr1(FRAMET(FRAMET(k3*r2 + k2)*r2 + k1)*r2);
    rakr2(FRAMET(FRAMET(k6*r2 + k5)*r2 + k4)*r2);
    rakrerr((float)kr-kr_gold);
#endif
}

template <typename ROWT, typename COLT, typename ROWOUTT, typename COLOUTT, int N>
void computeUndistortCoordinatesRef(Window<3,3, float> cameraMatrix,
                                 float (&distCoeffs)[N],
                                 Window<3,3, float> ir,
                                 int noRotation,
                                 ROWT i, COLT j,
                                 ROWOUTT &u, COLOUTT &v) {
    typedef float DT;
    typedef float CMT;
    typedef float FRAMET;
    typedef float FRAME2T;

    DT zo=0;
    DT k1=distCoeffs[0];
    DT k2=distCoeffs[1];
    DT p1=distCoeffs[2];
    DT p2=distCoeffs[3];
    DT k3= N>=5? distCoeffs[4] : zo;
    DT k4= N>=8? distCoeffs[5] : zo;
    DT k5= N>=8? distCoeffs[6] : zo;
    DT k6= N>=8? distCoeffs[7] : zo;
    CMT u0= cameraMatrix.val[0][2];
    CMT v0= cameraMatrix.val[1][2];
    CMT fx= cameraMatrix.val[0][0];
    CMT fy= cameraMatrix.val[1][1];


    FRAMET _x, _y, x, y;
    _x=i*ir.val[0][1] + j * ir.val[0][0] + ir.val[0][2];
    _y=i*ir.val[1][1] + j * ir.val[1][0] + ir.val[1][2];

    if(noRotation) {
        // A special case if there is no rotation: equivalent to cv::initUndistortMap
        assert(ir.val[2][0] == 0);
        assert(ir.val[2][1] == 0);
        assert(ir.val[2][1] == 1);
        x=_x;
        y=_y;
    } else {
        FRAMET w=i*ir.val[2][1] + j * ir.val[2][0] + ir.val[2][2];
        float winv = 1.0f/w;
#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
        ra1(w);
        ra2(winv);
#endif
        x = (FRAMET)(_x*winv);
        y = (FRAMET)(_y*winv);
    }

    typename x_traits<FRAMET,FRAMET>::MULT_T x2t = x*x, y2t = y*y;  // Full precision result here.

    FRAME2T _2xy = 2*x*y;
    FRAME2T r2 = x2t + y2t;
    FRAME2T x2 = x2t, y2 = y2t;

    FRAMET kr = (1 + FRAMET(FRAMET(k3*r2 + k2)*r2 + k1)*r2);
    if(N >5) kr = kr/
        (1 + FRAMET(FRAMET(k6*r2 + k5)*r2 + k4)*r2);

    u = fx*(FRAMET(x*kr) + FRAMET(p1*_2xy) + FRAMET(p2*(2*x2 + r2))) + u0;
    v = fy*(FRAMET(y*kr) + FRAMET(p1*(r2 + 2*y2)) + FRAMET(p2*_2xy)) + v0;

#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
    rax2(x2);
    ray2(y2);
    ra2xy(_2xy);
    rar2(r2);
    rakr1(FRAMET(FRAMET(k3*r2 + k2)*r2 + k1)*r2);
    rakr2(FRAMET(FRAMET(k6*r2 + k5)*r2 + k4)*r2);
    std::cout << "x:" << i << "," << j << " " << x << "\n";
    std::cout << "r2:" << i << "," << j << " " << r2 << "\n";
    std::cout << "kr:" << i << "," << j << " " << kr << "\n";
    std::cout << "u:" << i << "," << j << " " << u << "\n";
#endif
}

template<typename T>
struct InitUndistortRectifyMap_traits {
    typedef T FRAMET;
    typedef T FRAME2T;
};

template<int W, int I>
struct InitUndistortRectifyMap_traits<ap_fixed<W,I> > {
    typedef ap_fixed<18,2> FRAMET;
    typedef ap_fixed<23,4,AP_RND,AP_SAT> FRAME2T;
};
template<typename CMT, typename DT, typename ICMT, int ROWS, int COLS, int MAP1_T, int MAP2_T, int N>
void InitUndistortRectifyMapInverse (
                            Window<3,3, CMT> cameraMatrix,
                            DT (&distCoeffs)[N],
                            Window<3,3, ICMT> ir,
                            Mat<ROWS, COLS, MAP1_T>  &map1,
                            Mat<ROWS, COLS, MAP2_T>  &map2,
                            int noRotation=false)
{
#pragma HLS array_partition variable=distCoeffs complete
    int rows= map1.rows;
    int cols= map1.cols;
    Scalar<HLS_MAT_CN(MAP1_T), HLS_TNAME(MAP1_T)> m1;
    Scalar<HLS_MAT_CN(MAP2_T), HLS_TNAME(MAP2_T)> m2;

    assert(rows <= ROWS);
    assert(cols <= COLS);

    static RangeAnalyzer<float> rau, rav;
    static RangeAnalyzer<float> rauerr, raverr;

 loop_height: for(int i=0; i< rows; i++) {
    loop_width: for(int j=0; j< cols; j++) {
#pragma HLS PIPELINE II=1
            typedef ap_uint<BitWidth<ROWS>::Value> ROWT;
            typedef ap_uint<BitWidth<COLS>::Value> COLT;
            ROWT ifixed = i;
            COLT jfixed = j;

            ap_fixed<1+BitWidth<COLS>::Value+HLS_INTER_BITS, 1+BitWidth<COLS>::Value, AP_RND, AP_SAT> u;
            ap_fixed<1+BitWidth<ROWS>::Value+HLS_INTER_BITS, 1+BitWidth<ROWS>::Value, AP_RND, AP_SAT> v;
            computeUndistortCoordinates
                <typename InitUndistortRectifyMap_traits<CMT>::FRAMET, typename InitUndistortRectifyMap_traits<CMT>::FRAME2T>
                (cameraMatrix, distCoeffs, ir, noRotation,ifixed,jfixed,u,v);

#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
            float uFloat,vFloat;
            Window<3,3, float> cameraMatrixFloat;
            float distCoeffsFloat[N];
            Window<3,3, float> irFloat;
            for(int it=0; it<3; it++) {
                for(int jt=0; jt<3; jt++) {
                    irFloat.val[it][jt]=ir.val[it][jt];
                    cameraMatrixFloat.val[it][jt]=cameraMatrix.val[it][jt];
                }
            }
            for(int it=0; it<N; it++) {
                distCoeffsFloat[it] = distCoeffs[it];
            }
            computeUndistortCoordinatesRef(cameraMatrixFloat, distCoeffsFloat, irFloat, noRotation,ifixed,jfixed,uFloat,vFloat);
            rau(uFloat);
            rav(vFloat);
            rauerr((float)u-uFloat);
#endif

            assert(MAP1_T==HLS_16SC2);
            {
                int iu = sr_cast<int>(u*HLS_INTER_TAB_SIZE);
                int iv = sr_cast<int>(v*HLS_INTER_TAB_SIZE);
                m1.val[0]=(iu >> HLS_INTER_BITS);
                m1.val[1]=(iv >> HLS_INTER_BITS);
                m2.val[0]=(unsigned short)((iv &(HLS_INTER_TAB_SIZE-1))*HLS_INTER_TAB_SIZE + (iu & (HLS_INTER_TAB_SIZE-1)));
            }
//             else
//             {
//                 m1.val[0]=sr_cast<HLS_TNAME(MAP1_T)>(u);
//                 m1.val[1]=sr_cast<HLS_TNAME(MAP1_T)>(v);

//             }
            map1<< m1;
            map2<< m2;
        }
    }
#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
    std::cout << "x = " << rax << " y = " << ray << " err = " << raxerr << "\n";
    std::cout << "w = " << ra1 << " 1/w = " << ra2 << " err = " << rawerr << "\n";
    std::cout << "kr = (1+" << rakr1 << ")/(1+" << rakr2 << ") err = " << rakrerr << "\n";
    std::cout << "x2 = " << rax2 << " y2 = " << ray2 << " r2 = " << rar2 << "\n";
    std::cout << "u = " << rau << " v = " << rav << " err = " << rauerr << "\n";
#endif
}
template< typename CMT, typename RT, typename DT, int ROWS, int COL, int MAP1_T, int MAP2_T, int N>
void InitUndistortRectifyMap(
                            Window<3,3, CMT> cameraMatrix,
                            DT (&distCoeffs)[N],
                            Window<3,3, RT> R,
                            Window<3,3, CMT> newcameraMatrix,
                            Mat<ROWS, COL, MAP1_T>  &map1,
                            Mat<ROWS, COL, MAP2_T>  &map2
                            )
{
    Window<3,3, RT> iR;
    Window<3,3, RT> ir;
    mul<3, 3, 3>(newcameraMatrix.val, R.val, iR.val);
    Invert<3>(iR.val, ir.val);
    InitUndistortRectifyMapInverse(cameraMatrix, distCoeffs,ir,map1,map2);
}
}

#endif


