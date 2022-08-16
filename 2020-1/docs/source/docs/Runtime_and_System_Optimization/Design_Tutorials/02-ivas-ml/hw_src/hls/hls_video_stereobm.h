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
// opencv/modules/calib3d/src/stereobm.cpp

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
 * HLS Video Stereo Block Matching Header File
 */

#ifndef ___HLS__VIDEO__
#error hls_video_stereobm.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef __HLS_VIDEO_STEREOBM__
#define __HLS_VIDEO_STEREOBM__


namespace hls {

#define HLS_STEREO_BM_MAX_WIN_SIZE         21

#define HLS_STEREO_BM_NORMALIZED_RESPONSE  0
#define HLS_STEREO_BM_XSOBEL               1


template<int WSIZE, int NDISP, int NDISP_UNIT>
class StereoBMState {
public:
    // pre-filtering (normalization of input images)
    int preFilterType; // =HLS_STEREO_BM_XSOBEL
    int preFilterSize; // averaging window size: ~5x5..21x21
    int preFilterCap; // the output of pre-filtering is clipped by [-preFilterCap,preFilterCap]

    // correspondence using Sum of Absolute Difference (SAD)
    int SADWindowSize; // ~5x5..21x21 // defined in macro
    int minDisparity;  // minimum disparity (can be negative)
    int numberOfDisparities; // maximum disparity - minimum disparity (> 0)

    // post-filtering
    int textureThreshold;  // the disparity is only computed for pixels
                           // with textured enough neighborhood
    int uniquenessRatio;   // accept the computed disparity d* only if
                           // SAD(d) >= SAD(d*)*(1 + uniquenessRatio/100.)
                           // for any d != d*+/-1 within the search range.
    //int speckleWindowSize; // disparity variation window
    //int speckleRange; // acceptable range of variation in window

    int ndisp_unit;
    int sweepFactor;
    int remainder;

    StereoBMState() {
        preFilterType = HLS_STEREO_BM_XSOBEL; //HLS_STEREO_BM_NORMALIZED_RESPONSE;
        preFilterSize = WSIZE;
        preFilterCap = 32; // prevent mismatch of CV_SSE2 result
        SADWindowSize = WSIZE;
        minDisparity = 0;
        numberOfDisparities = NDISP;
        textureThreshold = 10;
        uniquenessRatio = 15;
        sweepFactor = (NDISP/NDISP_UNIT) + ((NDISP%NDISP_UNIT) != 0);
        ndisp_unit = NDISP_UNIT;
        remainder = NDISP_UNIT*sweepFactor - NDISP;
    }
};

template<int PWSIZE, int SRC_T, int DST_T, int ROWS, int COLS>
void PreFilterNorm(
            Mat<ROWS, COLS, SRC_T>& src,
            Mat<ROWS, COLS, DST_T>& dst,
            int cap)
{
    int rows = src.rows;
    int cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);

    LineBuffer<PWSIZE, COLS+PWSIZE-1, Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> > buff;
    Window<PWSIZE, PWSIZE, Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> > win;

    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> tmp[PWSIZE];
#pragma HLS array_partition variable=tmp complete dim=0
    int sum_cols[PWSIZE];
#pragma HLS array_partition variable=tmp complete dim=0
    int sum;

loop_sum_row:
    for (int row = 0; row < rows+PWSIZE-1; row++) {
loop_sum_init:
        for (int i = 0; i < PWSIZE; i++) {
            sum_cols[i] = 0;
        }
        sum = 0;
loop_sum_col:
        for (int col = 0; col < cols+PWSIZE-1; col++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
            Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> pix_in(0);
            buff.shift_down(col);
            if (!(row < (PWSIZE-1)/2 || row >= rows+(PWSIZE-1)/2 || col < (PWSIZE-1)/2 || col >= cols+(PWSIZE-1)/2)) {
                src >> pix_in;
            }
            buff.insert_bottom(pix_in, col);
            for (int i = 0; i< PWSIZE; i++) {
                tmp[i] = buff.val[i][col];
            }

            // compute sum incrementally;
            int a_sum = 0, b_sum = 0;
            for (int i = 0; i < PWSIZE; i++) {
                b_sum += tmp[i].val[0];
            }
            a_sum = sum_cols[PWSIZE-1];
            for (int j = PWSIZE-1; j > 0; j--) {
                sum_cols[j] = sum_cols[j-1];
            }
            sum_cols[0] = b_sum;
            sum = sum - a_sum + b_sum;
            win.shift_right();
            win.insert_right(tmp);

            if (row >= PWSIZE-1 && col >= PWSIZE-1) {
                int avg = sum / (PWSIZE*PWSIZE);
                Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> pix_out;
                int center = win.val[PWSIZE/2][PWSIZE/2].val[0];
                pix_out.val[0] = hls::min<int>(hls::max<int>(center-avg, -cap), cap) + cap;
                dst << pix_out;
            }
        }
    }
}

template<int ROWS, int COLS, int SRC_T, int DST_T>
void Clip(
            Mat<ROWS, COLS, SRC_T>& src,
            Mat<ROWS, COLS, DST_T>& dst,
            int cap)
{
    HLS_SIZE_T rows = src.rows;
    HLS_SIZE_T cols = src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)> s;
    Scalar<HLS_MAT_CN(DST_T), HLS_TNAME(DST_T)> d;
    for (HLS_SIZE_T i = 0; i < rows; i++) {
        for (HLS_SIZE_T j = 0; j < cols; j++) {
#pragma HLS pipeline II=1
            src >> s;
            for (HLS_SIZE_T k = 0; k < HLS_MAT_CN(SRC_T); k++) {
                s.val[k] = s.val[k] < -cap ? 0 : s.val[k] > cap ? cap*2 : s.val[k] + cap;
                d.val[k] = s.val[k];
            }
            dst << d;
        }
    }
}

template<typename T>
T absdiff(T a, T b) {
#pragma HLS inline
    long long x = a-b;
    long long y = b-a;
    T r;
    if(x < 0) { // signbit check.
        r = y; 
        assert(r == y);
    } else {
        r = x; 
        assert(r == x);
    }
    return r;
}

template<int WSIZE, class T>
void UpdateTexture(
            Window<WSIZE, WSIZE, T>& window,
            T* col_tbi,
            int& text_sum,
            int row,
            int col,
            T cap)
{
#pragma HLS inline
    int tmp = 0;
    for (int i = 0; i < WSIZE; i++) {
        //text_sum += (i > row? 0 : absdiff(col_tbi[i], cap)) - ((col < WSIZE || i > row) ? 0 : absdiff(window.val[i][WSIZE-1], cap));
        tmp += (i > row? 0 : absdiff(col_tbi[i], cap)) - ((col < WSIZE || i > row) ? 0 : absdiff(window.val[i][WSIZE-1], cap));
    }
    text_sum += tmp;
}

template<int WSIZE, int L_WIN_COLS, int R_WIN_COLS, typename T>
int SAD_compute_inc(
            Window<WSIZE, L_WIN_COLS, T>& l_win,
            Window<WSIZE, R_WIN_COLS, T>& r_win_s,
            int d,
            int col,
            int sad_cols_d[WSIZE])
{
#pragma HLS inline
    int a_sum = 0, b_sum = 0;
    // compute new column sads;
    for (int i = 0; i < WSIZE; i++) {
        b_sum += absdiff(l_win.val[i][0], r_win_s.val[i][d]);
    }
    // valid guard;
    if (col < d) b_sum = 0;
    // get previous sad_cols value;
    a_sum = sad_cols_d[WSIZE-1];
    // shift sad_cols[d];
    for (int j = WSIZE-1; j > 0; j--) {
        sad_cols_d[j] = sad_cols_d[j-1];
    }
    // fill in sad_cols with newly computed values;
    sad_cols_d[0] = b_sum;

    return (-a_sum+b_sum);
}

template<int SIZE>
class MinSAD {
public:
    template <typename T, typename T_idx>
    static void find(T a[SIZE], T_idx &loc, T &val) {
#pragma HLS inline
#pragma HLS array_partition variable=a complete dim=0
        T a1[SIZE/2];
        T a2[SIZE-SIZE/2];
        for(int i = 0; i < SIZE/2; i++) {
            a1[i] = a[i];
        }
        for(int i = 0; i < SIZE-SIZE/2; i++) {
            a2[i] = a[i+SIZE/2];
        }
        T_idx l1,l2;
        T v1,v2;
        MinSAD<SIZE/2>::find(a1,l1,v1);
        MinSAD<SIZE-SIZE/2>::find(a2,l2,v2);
        if(v1 >= v2) {
            val = v2;
            loc = l2+SIZE/2;
        } else {
            val = v1;
            loc = l1;
        }
    }
};

template<>
class MinSAD<1> {
public:
    template <typename T, typename T_idx>
    static void find(T a[1], T_idx &loc, T &val) {
#pragma HLS inline
#pragma HLS array_partition variable=a complete dim=0
        loc = 0;
        val = a[0];
    }
};

template<>
class MinSAD<2> {
public:
    template <typename T, typename T_idx>
    static void find(T a[2], T_idx &loc, T &val) {
#pragma HLS inline
#pragma HLS array_partition variable=a complete dim=0
        T_idx l1=0, l2=1;
        T v1=a[0], v2=a[1];
        if(v1 >= v2) {
            val = v2;
            loc = l2;
        } else {
            val = v1;
            loc = l1;
        }
    }
};

template<int WSIZE, int NDISP, int NDISP_UNIT, int ROWS, int COLS, int SRC_T, int DST_T>
void SADBlockMatching(
            Mat<ROWS, COLS, SRC_T>& left,
            Mat<ROWS, COLS, SRC_T>& right,
            Mat<ROWS, COLS, DST_T>& disp,
            StereoBMState<WSIZE, NDISP, NDISP_UNIT>& state)
//just for clang compiler
#if defined(__clang__) && !defined(__CLANG_3_1__)
            __attribute__((no_sanitize("undefined")))
#endif
{
    int rows = left.rows;
    int cols = left.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    LineBuffer<WSIZE, COLS+WSIZE-1, HLS_TNAME(SRC_T)> l_buff;
    LineBuffer<WSIZE, COLS+WSIZE-1, HLS_TNAME(SRC_T)> r_buff;
    Window<WSIZE, WSIZE, HLS_TNAME(SRC_T)> l_window;
    Window<WSIZE, WSIZE+NDISP_UNIT-1, HLS_TNAME(SRC_T)> r_window_search;
    int ndisp = state.numberOfDisparities;
    int mindisp = 0; // cameras parellel setting
    
    HLS_TNAME(DST_T) FILTERED = (HLS_TNAME(DST_T))((mindisp - 1) << 4);
    HLS_TNAME(SRC_T) cap = state.preFilterCap;
    HLS_TNAME(SRC_T) l_tmp[WSIZE];
#pragma HLS array_partition variable=l_tmp complete dim=0
    HLS_TNAME(SRC_T) r_tmp[WSIZE];
#pragma HLS array_partition variable=r_tmp complete dim=0
    int text_sum = 0;
    int sad[NDISP_UNIT];
#pragma HLS array_partition variable=sad complete dim=0
    int sad_cols[NDISP_UNIT][WSIZE];
#pragma HLS array_partition variable=sad_cols complete dim=0
    int minsad[COLS+WSIZE-1];
    int mind[COLS+WSIZE-1];
    int skip[COLS+WSIZE-1];
    int skip_val[COLS+WSIZE-1];
    int edge_neighbor[COLS+WSIZE-1];
    int edge[COLS+WSIZE-1];
    int minsad_p[COLS+WSIZE-1];
    int minsad_n[COLS+WSIZE-1];

    int d = 0;
loop_row:
    for (int row = 0; row < rows+WSIZE-1; row++) {
loop_mux:
        for (int sweep = 0; sweep < state.sweepFactor; sweep++) {
loop_col:
            for (int col = 0; col < cols+WSIZE-1; col++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1
                if (col == 0) {
loop_sad_init:
                    for (d = 0; d < NDISP_UNIT; d++) {
#pragma HLS unroll
                        sad[d] = 0;
                        for (int i = 0; i < WSIZE; i++) {
#pragma HLS unroll
                            sad_cols[d][i] = 0;
                        }
                    }
                }

                Scalar<1, HLS_TNAME(SRC_T)> l_in(cap), r_in(cap);
                if (sweep == 0) {
                    // load and shifting buffs
                    l_buff.shift_down(col);
                    r_buff.shift_down(col);
                    if (!(row < (WSIZE-1)/2 || row >= rows+(WSIZE-1)/2 || col < (WSIZE-1)/2 || col >= cols+(WSIZE-1)/2)) {
                        left >> l_in;
                        right >> r_in;
                    }
                    l_buff.insert_bottom(l_in.val[0], col);
                    r_buff.insert_bottom(r_in.val[0], col);
loop_get_data_from_linebuff:
                    for (int i = 0; i < WSIZE; i++) {
                        l_tmp[i] = l_buff.val[i][col];
                        r_tmp[i] = r_buff.val[i][col];
                    }
                } else {
                    int offset = sweep * NDISP_UNIT;
loop_get_data_from_linebuff_with_offset:
                    for (int i = 0; i < WSIZE; i++) {
                        l_tmp[i] = l_buff.val[i][col];
                        r_tmp[i] = r_buff.val[i][col-offset < 0 ? 0 : col-offset];
                    }
                }
    
                UpdateTexture<WSIZE, HLS_TNAME(SRC_T)>(l_window, l_tmp, text_sum, row, col, cap);
    
                l_window.shift_right();
                r_window_search.shift_right();
                l_window.insert_right(l_tmp);
                r_window_search.insert_right(r_tmp);
    
loop_sad_compute:
                for (d = 0; d < NDISP_UNIT; d++) {
                    sad[d] += SAD_compute_inc<WSIZE, WSIZE, WSIZE+NDISP_UNIT-1, HLS_TNAME(SRC_T)>(l_window, r_window_search, d, col, sad_cols[d]);
                    if (sweep == state.sweepFactor-1 && d >= (NDISP_UNIT-state.remainder)) sad[d] = HLS_32S_MAX;
                }
    
                // SAD computing and store output
                if (row >= WSIZE-1 && col >= WSIZE-1) {
                    int skip_flag = 0;
                    Scalar<1, HLS_TNAME(DST_T)> out_pix;
                    if (text_sum < state.textureThreshold) skip_flag = 1;
                    if ((row - WSIZE+1) < (WSIZE-1)/2 || (row - WSIZE+1) >= rows - (WSIZE-1)/2) skip_flag = 1;
                    if ((col - WSIZE+1) < NDISP-1 + (WSIZE-1)/2 || (col - WSIZE+1) >= cols - (WSIZE-1)/2) skip_flag = 1;
                    int gminsad = ((sweep > 0) ? minsad[col] : HLS_32S_MAX);
                    int gmind   = ((sweep > 0) ? mind[col] : 0);
                    int gskip = ((sweep > 0) ? skip[col] : 0);
                    int gskip_val = ((sweep > 0) ? skip_val[col] : HLS_32S_MAX);
                    int gedge_neighbor = ((sweep > 0) ? edge_neighbor[col] : HLS_32S_MAX);  // for uniqueness check
                    int gedge = ((sweep > 0) ? edge[col] : sad[1]); // for subpixel interpolation
                    int lminsad = HLS_32S_MAX;
                    int lmind = 0;
                    int gminsad_p = ((sweep > 0) ? minsad_p[col] : HLS_32S_MAX); 
                    int gminsad_n = ((sweep > 0) ? (gmind == sweep*NDISP_UNIT-1 ? sad[0] : minsad_n[col]) : HLS_32S_MAX); 
                    MinSAD<NDISP_UNIT>::find(sad, lmind, lminsad);
    
                    if (lminsad <= gminsad) {
                        gskip = 0;
                        if (state.uniquenessRatio > 0) {
                            int thresh = lminsad + (lminsad * state.uniquenessRatio / 100);
                            if (gminsad <= thresh && lmind+sweep*NDISP_UNIT > gmind+1) {
                                gskip = 1;
                                gskip_val = gminsad;
                            } else if (gminsad <= thresh && lmind+sweep*NDISP_UNIT == gmind+1 && gskip_val <= thresh) {
                                gskip = 1;
                                // gskip_val unchanged;
                            } else if (gminsad <= thresh && lmind+sweep*NDISP_UNIT == gmind+1 && gedge_neighbor <= thresh) {
                                gskip = 1;
                                gskip_val = gedge_neighbor;
                            }
loop_unique_search_0:
                            for (d = 0; d < NDISP_UNIT; d++) {
                                if (sad[d] <= thresh && sad[d] < gskip_val && (d < lmind-1 || d > lmind+1)) {
                                    gskip = 1;
                                    gskip_val = sad[d];
                                }
                            }
                        }
                        // update global values;
                        gminsad_p = (lmind == 0 ? gedge : sad[lmind-1]);
                        gminsad_n = sad[lmind == NDISP_UNIT-1 ? lmind-1 : lmind+1];
                        gminsad = lminsad;
                        gmind = lmind + sweep*NDISP_UNIT;
                    } else {
                        if (state.uniquenessRatio > 0) {
                            int thresh = gminsad + (gminsad * state.uniquenessRatio / 100);
loop_unique_search_1:
                            for (d = 0; d < NDISP_UNIT; d++) {
                                if (sad[d] <= thresh && sad[d] < gskip_val && ((gmind == (sweep*NDISP_UNIT-1)) ? (d > 0) : 1)) {
                                    gskip = 1;
                                    gskip_val = sad[d];
                                }
                            }
                        }
                    }
                    minsad[col] = gminsad;
                    mind[col] = gmind;
                    skip[col] = gskip;
                    skip_val[col] = gskip_val;
                    edge_neighbor[col] = sad[NDISP_UNIT-2];
                    edge[col] = sad[NDISP_UNIT-1];
                    minsad_p[col] = gminsad_p;
                    minsad_n[col] = gminsad_n;
                    
                    if (sweep == state.sweepFactor-1) {
                        ap_int<BitWidth<255*WSIZE*WSIZE>::Value> p = gminsad_p;
                        ap_int<BitWidth<255*WSIZE*WSIZE>::Value> n = gminsad_n;
                        ap_int<BitWidth<255*WSIZE*WSIZE>::Value> k = p + n - 2*gminsad + absdiff(p, n);
                        ap_int<BitWidth<255*WSIZE*WSIZE>::Value+8> num = p - n;
                        num = num << 8;
                        ap_int<10> delta = 0;
                        if (k != 0) delta = num/k;
                        out_pix.val[0] = ((gmind*256 + delta + 15) >> 4);
                        //out_pix.val[0] = ((gmind*256 + (k != 0 ? (p-n)*256/k : 0) + 15) >> 4);
                        skip_flag |= gskip;
                        if (skip_flag) out_pix.val[0] = FILTERED;
                        disp << out_pix;
                    }
                }
            }
        }
    }
}


template<int WSIZE, int NDISP, int NDISP_UNIT, int ROWS, int COLS, int SRC_T, int DST_T>
void FindStereoCorrespondenceBM(
            Mat<ROWS, COLS, SRC_T>& left,
            Mat<ROWS, COLS, SRC_T>& right,
            Mat<ROWS, COLS, DST_T>& disp,
            StereoBMState<WSIZE, NDISP, NDISP_UNIT>& state)
{
    assert(left.rows == right.rows && left.cols == right.cols && "All the images must have the same size");
    assert(SRC_T == HLS_8UC1 && "Both input images must have HLS_8UC1 format");
    assert(DST_T == HLS_16SC1 && "Disparity image must have HLS_16SC1 format");
    assert(state.preFilterType == HLS_STEREO_BM_NORMALIZED_RESPONSE || state.preFilterType == HLS_STEREO_BM_XSOBEL);
    assert(state.preFilterSize >= 5 && state.preFilterSize <= HLS_STEREO_BM_MAX_WIN_SIZE && (state.preFilterSize & 1) && "preFilterSize must be odd and be within 5..HLS_STEREO_BM_MAX_WIN_SIZE");
    assert(state.preFilterCap >=1 && state.preFilterCap <= 63 && "preFilterCap must be within 1..63");
    assert(state.SADWindowSize >= 5 && state.SADWindowSize <= HLS_STEREO_BM_MAX_WIN_SIZE && (state.SADWindowSize & 1) && state.SADWindowSize < hls::min<int>(left.rows, left.cols) && "SADWindowSize must be odd, be within 5..HLS_STEREO_BM_MAX_WIN_SIZE and be lesser than image width or height");
    assert(NDISP > 1 && "numberOfDisparities must be greater than 1");
    assert(NDISP >= NDISP_UNIT && "numberOfDisparities must be not less than ndisp_unit");
    assert(state.textureThreshold >= 0 && "texture threshold must be non-negative");
    assert(state.uniquenessRatio >= 0 && "uniqueness ratio must be non-negative");
    int rows = left.rows;
    int cols = left.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Mat<ROWS, COLS, SRC_T> _left_0(rows, cols);
    Mat<ROWS, COLS, SRC_T> _right_0(rows, cols);
#pragma HLS dataflow
    if (state.preFilterType == HLS_STEREO_BM_NORMALIZED_RESPONSE) {
        PreFilterNorm<WSIZE>(left, _left_0, state.preFilterCap);
        PreFilterNorm<WSIZE>(right, _right_0, state.preFilterCap);
    } else {
        Mat<ROWS, COLS, HLS_16SC1> left_sobel(rows, cols);
        Mat<ROWS, COLS, HLS_16SC1> right_sobel(rows, cols);
        Sobel<1, 0, 3>(left, left_sobel);
        Sobel<1, 0, 3>(right, right_sobel);
        Clip(left_sobel, _left_0, state.preFilterCap);
        Clip(right_sobel, _right_0, state.preFilterCap);
    }
    SADBlockMatching(_left_0, _right_0, disp, state);
}

} // namespace hls

#endif


