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
// opencv/modules/core/include/opencv2/core/core.hpp

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
 * HLS Video Core Structures Header File
 */

#ifndef ___HLS__VIDEO_CORE__
#define ___HLS__VIDEO_CORE__

namespace hls {

/* Template class of Point_ */
template<typename T>
class Point_ {
public:
    Point_();
    Point_(T _x, T _y);
    Point_(const Point_& pt);
    ~Point_();
    
    T x, y;
};

/* Member functions of Point_ class */
template<typename T> inline Point_<T>::Point_() {}
template<typename T> inline Point_<T>::Point_(T _x, T _y) : x(_x), y(_y) {}
template<typename T> inline Point_<T>::Point_(const Point_<T>& pt) : x(pt.x), y(pt.y) {}
template<typename T> inline Point_<T>::~Point_() {}

typedef Point_<int> Point;


/* Template class of Size_ */
template<typename T>
class Size_ {
public:
    Size_();
    Size_(T _width, T _height);
    Size_(const Size_<T>& sz);
    Size_(const Point_<T>& pt);
    T area();
    ~Size_();

    T width, height;
};

/* Member functions of Size_ class */
template<typename T> inline Size_<T>::Size_() {}
template<typename T> inline Size_<T>::Size_(T _width, T _height) : width(_width), height(_height) {}
template<typename T> inline Size_<T>::Size_(const Size_<T>& sz) : width(sz.width), height(sz.height) {}
template<typename T> inline Size_<T>::Size_(const Point_<T>& pt) : width(pt.x), height(pt.y) {}
template<typename T> inline T Size_<T>::area() { return width*height; }
template<typename T> inline Size_<T>::~Size_() {}

typedef Size_<int> Size;


/* Template class of Rect_ */
template<typename T>
class Rect_ {
public:
    Rect_();
    Rect_(T _x, T _y, T _width, T _height);
    Rect_(const Rect_& rect);
    Rect_(const Point_<T>& pt, const Size_<T>& sz);
    T area();
    Size_<T> size();
    Point_<T> tl(); // top-left point(inside);
    Point_<T> tr(); // top-right point(outside);
    Point_<T> bl(); // bottom-left point(outside);
    Point_<T> br(); // bottom-right point(outside);
    bool bContains(const Point_<T>& pt);
    ~Rect_();

    T x, y, width, height;
};

/* Member functions of Rect_ class */
template<typename T> inline Rect_<T>::Rect_() {}
template<typename T> inline Rect_<T>::Rect_(T _x, T _y, T _width, T _height) : x(_x), y(_y), width(_width), height(_height) {}
template<typename T> inline Rect_<T>::Rect_(const Rect_<T>& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}
template<typename T> inline Rect_<T>::Rect_(const Point_<T>& pt, const Size_<T>& sz) : x(pt.x), y(pt.y), width(sz.width), height(sz.height) {}
template<typename T> inline T Rect_<T>::area() { return width*height; }
template<typename T> inline Size_<T> Rect_<T>::size() { return Size_<T>(width, height); }
template<typename T> inline Point_<T> Rect_<T>::tl() { return Point_<T>(x, y); }
template<typename T> inline Point_<T> Rect_<T>::tr() { return Point_<T>(x+width, y); }
template<typename T> inline Point_<T> Rect_<T>::bl() { return Point_<T>(x, y+height); }
template<typename T> inline Point_<T> Rect_<T>::br() { return Point_<T>(x+width, y+height); }
template<typename T> inline bool Rect_<T>::bContains(const Point_<T>& pt) { return (pt.x >= x && pt.x < x+width && pt.y >= y && pt.y < y+height); }
template<typename T> inline Rect_<T>::~Rect_() {}

typedef Rect_<int> Rect;

//data types with same width are compared and equally traited.
template<bool c>
struct eqb {};

template<>
struct eqb <true> { typedef void type; };

template<typename T1, typename T2>
struct eqwt { typedef typename eqb<Type_BitWidth<T1>::Value == Type_BitWidth<T2>::Value>::type type; };

/* Template class of Scalar */
template<int N, typename T>
class Scalar {
public:
    Scalar() {
#pragma HLS ARRAY_PARTITION variable=val dim=1 complete
#pragma HLS INLINE
        assert(N > 0);
    }
    Scalar(T v0) { 
#pragma HLS ARRAY_PARTITION variable=val dim=1 complete
#pragma HLS INLINE
        assert(N >= 1 && "Scalar must have enough channels for constructor.");
        val[0] = v0;
    }
    Scalar(T v0, T v1) { 
#pragma HLS ARRAY_PARTITION variable=val dim=1 complete
#pragma HLS INLINE
        assert(N >= 2 && "Scalar must have enough channels for constructor.");
        val[0] = v0; val[1] = v1;
    }
    Scalar(T v0, T v1, T v2) { 
#pragma HLS ARRAY_PARTITION variable=val dim=1 complete
#pragma HLS INLINE
        assert(N >= 3 && "Scalar must have enough channels for constructor.");
        val[0] = v0; val[1] = v1; val[2] = v2;
    }
    Scalar(T v0, T v1, T v2, T v3) { 
#pragma HLS ARRAY_PARTITION variable=val dim=1 complete
#pragma HLS INLINE
        assert(N >= 4 && "Scalar must have enough channels for constructor.");
        val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    }

    void operator = (T value);

    //overload operator =, to make sure hls::scalar supports  ap_int<8>=int8_t=singed char=char,likewise.  
    template<typename T2>
    typename eqwt<T,T2>::type operator = (const Scalar<N, T2> &value) {
#pragma HLS inline
        for (int k = 0; k < N; k++) {
#pragma HLS unroll
            val[k] = (T)value.val[k];
        }
    }

    Scalar<N, T> operator + (T value);
    Scalar<N, T> operator + (Scalar<N, T> s);
    Scalar<N, T> operator - (T value);
    Scalar<N, T> operator - (Scalar<N, T> s);
    Scalar<N, T> operator * (T value);
    Scalar<N, T> operator * (Scalar<N, T> s);
    Scalar<N, T> operator / (T value);
    Scalar<N, T> operator / (Scalar<N, T> s);

    T val[N];
};

template<int N, typename T>
void Scalar<N, T>::operator = (T value) {
#pragma HLS inline
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        val[k] = value;
    }
}

template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator + (T value) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] + value;
    }
    return res;
}

template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator + (Scalar<N, T> s) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] + s.val[k];
    }
    return res;
}

template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator - (T value) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] - value;
    }
    return res;
}

template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator - (Scalar<N, T> s) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] - s.val[k];
    }
    return res;
}


template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator * (T value) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] * value;
    }
    return res;
}

template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator * (Scalar<N, T> s) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] * s.val[k];
    }
    return res;
}


template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator / (T value) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] / value;
    }
    return res;
}

template<int N, typename T>
Scalar<N, T> Scalar<N, T>::operator / (Scalar<N, T> s) {
#pragma HLS inline
    Scalar<N, T> res;
    for (int k = 0; k < N; k++) {
#pragma HLS unroll
        res.val[k] = val[k] / s.val[k];
    }
    return res;
}


/* convertors for different pixel types*/

/* yuv422_8 and yuv420_8 */
template<typename T>
yuv422_8 Scalar_to_yuv422_8(Scalar<2, T> scl) {
#pragma HLS inline
    yuv422_8 pix;
    pix.Y = (unsigned char)scl.val[0];
    pix.UV = (char)scl.val[1];
    return pix;
}
template<typename T>
Scalar<2, T> yuv422_8_to_Scalar(yuv422_8 pix) {
#pragma HLS inline
    Scalar<2, T> scl;
    scl.val[0] = (T)pix.Y;
    scl.val[1] = (T)pix.UV;
    return scl;
}

/* yuv444_8 */
template<typename T>
yuv444_8 Scalar_to_yuv444_8(Scalar<3, T> scl) {
#pragma HLS inline
    yuv444_8 pix;
    pix.Y = (unsigned char)scl.val[0];
    pix.U = (char)scl.val[1];
    pix.V = (char)scl.val[2];
    return pix;
}
template<typename T>
Scalar<3, T> yuv444_8_to_Scalar(yuv444_8 pix) {
#pragma HLS inline
    Scalar<3, T> scl;
    scl.val[0] = (T)pix.Y;
    scl.val[1] = (T)pix.U;
    scl.val[2] = (T)pix.V;
    return scl;
}

/* rgb_8 */
template<typename T>
rgb_8 Scalar_to_rgb_8(Scalar<3, T> scl) {
#pragma HLS inline
    rgb_8 pix;
    pix.R = (unsigned char)scl.val[0];
    pix.G = (unsigned char)scl.val[1];
    pix.B = (unsigned char)scl.val[2];
    return pix;
}
template<typename T>
Scalar<3, T> rgb_8_to_Scalar(rgb_8 pix) {
#pragma HLS inline
    Scalar<3, T> scl;
    scl.val[0] = (T)pix.R;
    scl.val[1] = (T)pix.G;
    scl.val[2] = (T)pix.B;
    return scl;
}

/* yuva422_8 */
template<typename T>
yuva422_8 Scalar_to_yuva422_8(Scalar<3, T> scl) {
#pragma HLS inline
    yuva422_8 pix;
    pix.A = (char)scl.val[0];
    pix.Y = (unsigned char)scl.val[1];
    pix.UV = (char)scl.val[2];
    return pix;
}
template<typename T>
Scalar<3, T> yuva422_8_to_Scalar(yuva422_8 pix) {
#pragma HLS inline
    Scalar<3, T> scl;
    scl.val[0] = (T)pix.A;
    scl.val[1] = (T)pix.Y;
    scl.val[2] = (T)pix.UV;
    return scl;
}

/* yuva444_8 */
template<typename T>
yuva444_8 Scalar_to_yuva444_8(Scalar<4, T> scl) {
#pragma HLS inline
    yuva444_8 pix;
    pix.A = (char)scl.val[0];
    pix.Y = (unsigned char)scl.val[1];
    pix.U = (char)scl.val[2];
    pix.V = (char)scl.val[3];
    return pix;
}
template<typename T>
Scalar<4, T> yuva444_8_to_Scalar(yuva444_8 pix) {
#pragma HLS inline
    Scalar<4, T> scl;
    scl.val[0] = (T)pix.A;
    scl.val[1] = (T)pix.Y;
    scl.val[2] = (T)pix.U;
    scl.val[3] = (T)pix.V;
    return scl;
}

/* rgba_8 */
template<typename T>
rgba_8 Scalar_to_rgba_8(Scalar<4, T> scl) {
#pragma HLS inline
    rgba_8 pix;
    pix.A = (char)scl.val[0];
    pix.R = (unsigned char)scl.val[1];
    pix.G = (unsigned char)scl.val[2];
    pix.B = (unsigned char)scl.val[3];
    return pix;
}
template<typename T>
Scalar<4, T> rgba_8_to_Scalar(rgba_8 pix) {
#pragma HLS inline
    Scalar<4, T> scl;
    scl.val[0] = (T)pix.A;
    scl.val[1] = (T)pix.R;
    scl.val[2] = (T)pix.G;
    scl.val[3] = (T)pix.B;
    return scl;
}

/* yuva420_8 */
template<typename T>
yuva420_8 Scalar_to_yuva420_8(Scalar<2, T> scl) {
#pragma HLS inline
    yuva420_8 pix;
    pix.AUV = (char)scl.val[0];
    pix.Y = (unsigned char)scl.val[1];
    return pix;
}
template<typename T>
Scalar<2, T> yuva420_8_to_Scalar(yuva420_8 pix) {
#pragma HLS inline
    Scalar<2, T> scl;
    scl.val[0] = (T)pix.AUV;
    scl.val[1] = (T)pix.Y;
    return scl;
}

/* yuvd422_8 and yuvd420_8*/
template<typename T>
yuvd422_8 Scalar_to_yuvd422_8(Scalar<3, T> scl) {
#pragma HLS inline
    yuvd422_8 pix;
    pix.D = (unsigned char)scl.val[0];
    pix.Y = (unsigned char)scl.val[1];
    pix.UV = (char)scl.val[2];
    return pix;
}
template<typename T>
Scalar<3, T> yuvd422_8_to_Scalar(yuvd422_8 pix) {
#pragma HLS inline
    Scalar<3, T> scl;
    scl.val[0] = (T)pix.D;
    scl.val[1] = (T)pix.Y;
    scl.val[2] = (T)pix.UV;
    return scl;
}

/* yuvd444_8 */
template<typename T>
yuvd444_8 Scalar_to_yuvd444_8(Scalar<4, T> scl) {
#pragma HLS inline
    yuvd444_8 pix;
    pix.D = (unsigned char)scl.val[0];
    pix.Y = (unsigned char)scl.val[1];
    pix.U = (char)scl.val[2];
    pix.V = (char)scl.val[3];
    return pix;
}
template<typename T>
Scalar<4, T> yuvd444_8_to_Scalar(yuvd444_8 pix) {
#pragma HLS inline
    Scalar<4, T> scl;
    scl.val[0] = (T)pix.D;
    scl.val[1] = (T)pix.Y;
    scl.val[2] = (T)pix.U;
    scl.val[3] = (T)pix.V;
    return scl;
}

/* rgbd_8 */
template<typename T>
rgbd_8 Scalar_to_rgbd_8(Scalar<4, T> scl) {
#pragma HLS inline
    rgbd_8 pix;
    pix.D = (unsigned char)scl.val[0];
    pix.R = (unsigned char)scl.val[1];
    pix.G = (unsigned char)scl.val[2];
    pix.B = (unsigned char)scl.val[3];
    return pix;
}
template<typename T>
Scalar<4, T> rgbd_8_to_Scalar(rgbd_8 pix) {
#pragma HLS inline
    Scalar<4, T> scl;
    scl.val[0] = (T)pix.D;
    scl.val[1] = (T)pix.R;
    scl.val[2] = (T)pix.G;
    scl.val[3] = (T)pix.B;
    return scl;
}

/* bayer_8 */
template<typename T>
bayer_8 Scalar_to_bayer_8(Scalar<1, T> scl) {
#pragma HLS inline
    bayer_8 pix;
    pix.CMY = (unsigned char)scl.val[0];
    return pix;
}
template<typename T>
Scalar<1, T> bayer_8_to_Scalar(bayer_8 pix) {
#pragma HLS inline
    Scalar<1, T> scl;
    scl.val[0] = (T)pix.CMY;
    return scl;
}

/* luma_8 */
template<typename T>
luma_8 Scalar_to_luma_8(Scalar<1, T> scl) {
#pragma HLS inline
    luma_8 pix;
    pix.Y = (unsigned char)scl.val[0];
    return pix;
}
template<typename T>
Scalar<1, T> luma_8_to_Scalar(luma_8 pix) {
#pragma HLS inline
    Scalar<1, T> scl;
    scl.val[0] = (T)pix.Y;
    return scl;
}

/* Template class of Mat */
template<int ROWS, int COLS, int T>                               // T: type of Mat
class Mat {
public:
    Mat();                                                        // default constructor
    Mat(int _rows, int _cols);
    Mat(Size _sz);

    void init(int _rows, int _cols);
    void assignto(Mat<ROWS, COLS, T>& mat);                       // do not copy image data

    Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> read();                   // data_stream array readout a Scalar

    void read(Scalar<HLS_MAT_CN(T), HLS_TNAME(T)>& s);            // data_stream array readout a Scalar
    template <typename T2>
    typename eqwt<HLS_TNAME(T),T2>::type read(Scalar<HLS_MAT_CN(T), T2>& s){
#pragma HLS inline
        s = read();
    }

    void write(Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> s);            // data_stream array writein a Scalar
    template<typename T2>
    typename eqwt<HLS_TNAME(T),T2>::type write(Scalar<HLS_MAT_CN(T), T2> s) {
#pragma HLS inline
#pragma HLS data_pack variable=data_stream
    HLS_CHANNEL_T i;
        {
        // Force stream reads to be scheduled together.  This avoids
        // issues where scheduling forces them apart, which would  
        // require buffer
#pragma HLS protocol floating
            for (i = 0; i < HLS_MAT_CN(T); i++) {
#pragma HLS unroll
                data_stream[i] << s.val[i];
            }
        }
    }
	
   void operator >> (Scalar<HLS_MAT_CN(T), HLS_TNAME(T)>& s);    // data_stream array readout a Scalar
    template <typename T2>
    typename eqwt<HLS_TNAME(T),T2>::type operator >> (Scalar<HLS_MAT_CN(T), T2>& s) {
#pragma HLS inline
        s = read();
    }

    void operator << (Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> s);     // data_stream array writein a Scalar
    template<typename T2>
    typename eqwt<HLS_TNAME(T),T2>::type operator << (Scalar<HLS_MAT_CN(T), T2> s) {
#pragma HLS inline
        write(s);
    }

    bool empty();                                                 // check stream

    const int type() const;                                       // return matrix type
    const int depth() const;                                      // return matrix depth
    const int channels() const;                                   // return matrix channels

    HLS_SIZE_T rows, cols;                                        // actual image size

    hls::stream<HLS_TNAME(T)> data_stream[HLS_MAT_CN(T)];         // image data stream array
};

/* Member functions of Mat class */
template<int ROWS, int COLS, int T>
inline Mat<ROWS, COLS, T>::Mat() {
#pragma HLS inline
    init(ROWS, COLS);
}

template<int ROWS, int COLS, int T>
inline Mat<ROWS, COLS, T>::Mat(int _rows, int _cols) {
#pragma HLS inline
    init(_rows, _cols);
}

template<int ROWS, int COLS, int T>
inline Mat<ROWS, COLS, T>::Mat(Size _sz) {
#pragma HLS inline
    init(_sz.height, _sz.width);
}

template<int ROWS, int COLS, int T>
inline void Mat<ROWS, COLS, T>::init(int _rows, int _cols) {
#pragma HLS inline
    assert((_rows > 0) && (_rows <= ROWS) && (_cols > 0) && (_cols <= COLS) &&
           "The number of rows and columns must be less than the template arguments.");
    rows = _rows;
    cols = _cols;
}

template<int ROWS, int COLS, int T>
void Mat<ROWS, COLS, T>::assignto(Mat<ROWS, COLS, T>& mat) {
#pragma HLS inline
    mat.rows = rows;
    mat.cols = cols;
}

template<int ROWS, int COLS, int T>
Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> Mat<ROWS, COLS, T>::read() {
#pragma HLS inline
    Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> scl;
    HLS_CHANNEL_T i;
    {
        // Force stream reads to be scheduled together.  This avoids
        // issues where scheduling forces them apart, which would
        // require buffers
        #pragma HLS protocol floating
        for (i = 0; i < HLS_MAT_CN(T); i++) {
#pragma HLS unroll
            data_stream[i] >> scl.val[i];
        }
    }
    return scl;
}

template<int ROWS, int COLS, int T>
void Mat<ROWS, COLS, T>::read(Scalar<HLS_MAT_CN(T), HLS_TNAME(T)>& s) {
#pragma HLS inline
    s = read();
}

template<int ROWS, int COLS, int T>
void Mat<ROWS, COLS, T>::write(Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> s) {
#pragma HLS inline
#pragma HLS data_pack variable=data_stream
    HLS_CHANNEL_T i;
    {
        // Force stream reads to be scheduled together.  This avoids
        // issues where scheduling forces them apart, which would
        // require buffers
#pragma HLS protocol floating
        for (i = 0; i < HLS_MAT_CN(T); i++) {
#pragma HLS unroll
            data_stream[i] << s.val[i];
        }
    }
}

template<int ROWS, int COLS, int T>
void Mat<ROWS, COLS, T>::operator >> (Scalar<HLS_MAT_CN(T), HLS_TNAME(T)>& s) {
#pragma HLS inline
    s = read();
}

template<int ROWS, int COLS, int T>
void Mat<ROWS, COLS, T>::operator << (Scalar<HLS_MAT_CN(T), HLS_TNAME(T)> s) {
#pragma HLS inline
    write(s);
}

template<int ROWS, int COLS, int T>
bool Mat<ROWS, COLS, T>::empty() {
#pragma HLS inline
    unsigned char flag = 0;
    for (HLS_CHANNEL_T i = 0; i < HLS_MAT_CN(T); i++) {
#pragma HLS unroll
        flag += (data_stream[i].empty()) ? 1 : 0;
    }
#ifndef __SYNTHESIS__
    assert(flag == HLS_MAT_CN(T) || flag == 0 &&
           "Some channels are empty but others are not.");
#endif
    return ((flag == HLS_MAT_CN(T)) ? true : false);
}

template<int ROWS, int COLS, int T> 
const int Mat<ROWS, COLS, T>::type() const {
#pragma HLS inline
    return HLS_MAT_TYPE(T); 
}

template<int ROWS, int COLS, int T> 
const int Mat<ROWS, COLS, T>::depth() const {
#pragma HLS inline
    return HLS_MAT_DEPTH(T); 
}
template<int ROWS, int COLS, int T> 
const int Mat<ROWS, COLS, T>::channels() const {
#pragma HLS inline
    return HLS_MAT_CN(T); 
}

} // namespace hls


#endif


