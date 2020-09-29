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
// opencv/modules/imgproc/src/filter.cpp

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
 * HLS Video Image Base Functions Header File
 */
#ifndef _HLS_VIDEO_IMGBASE_H_
#define _HLS_VIDEO_IMGBASE_H_

//#include <limits.h>
//#include <float.h>

namespace hls {

enum {MORPH_RECT,MORPH_CROSS,MORPH_ELLIPSE};

class border_mode {
public:
    enum values {BORDER_CONSTANT,BORDER_REPLICATE,BORDER_REFLECT,BORDER_WRAP,BORDER_REFLECT_101};
    typedef void isBorderMode;
};

class BORDER_CONSTANT : public border_mode { public: static const values value = border_mode::BORDER_CONSTANT; };
class BORDER_REPLICATE : public border_mode { public: static const values value = border_mode::BORDER_REPLICATE; };
class BORDER_REFLECT : public border_mode { public: static const values value = border_mode::BORDER_REFLECT; };
class BORDER_WRAP : public border_mode { public: static const values value = border_mode::BORDER_WRAP; };
class BORDER_REFLECT_101 : public border_mode { public: static const values value = border_mode::BORDER_REFLECT_101; };
class BORDER_DEFAULT : public border_mode { public: static const values value = border_mode::BORDER_REFLECT_101; };

//Parameters: 0---based coordinate of the extrapolated pixel along one of the axes, likely <0 or >= len
//len - length of the array along the corresponding axis
 /*
 Various border types, image boundaries are denoted with '|'
 * BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
 * BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
 * BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
 * BORDER_WRAP:          cdefgh|abcdefgh|abcdefg//no support
 * BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
 */
static int borderInterpolate( int p, int len, int borderType )
{
    #pragma HLS INLINE
    assert(borderType != BORDER_WRAP::value && "BORDER_WRAP is not supported.");

    if( p >= 0 && p < len )
        return p;
    else if( borderType == BORDER_REPLICATE::value )
        p = p < 0 ? 0 : len - 1;
    else if( borderType == BORDER_REFLECT::value)
    {
        if( len == 1 )
            return 0;
        if(p<0)
        {
            p=-p-1;
        }
        if( p >= len )
        {
            p=2*len-p-1;
        }
    }
    else if(borderType == BORDER_REFLECT_101::value )
    {
        if( len == 1 )
            return 0;
        if(p<0)
        {

            p=-p;
        }
        if( p >= len )
        {

            p=2*len-p-2;
        }
    }

    else if( borderType == BORDER_CONSTANT::value )
        p = -1;
    return p;
}
// Make sure the anchor in the right location
template <typename anchor_T,typename kernel_T>
inline void normalizeAnchor(
        hls::Point_<anchor_T> &anchor,
        hls::Size_<kernel_T>  kernel_size)
{
#pragma HLS INLINE
    if(anchor.x==-1)
        anchor.x=kernel_size.width/2;
    if(anchor.y==-1)
        anchor.y=kernel_size.height/2;

    assert(anchor.x>=0);
    assert(anchor.x<=kernel_size.width-1);
    assert(anchor.y>=0);
    assert(anchor.y<=kernel_size.height-1);
}
template<int SRC_T,int ROWS,int COLS>
void DuplicateImageN(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, SRC_T>	    _dst[],
		int             	    num
                )
{
    int cols=_src.cols;
    int rows=_src.rows;
 loop_size: for(int k=0;k<num;k++) {
        _dst[k].rows=rows;
        _dst[k].cols=cols;
    }
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> s;
 loop_height: for(int i= 0;i<rows;i++)
	{
#pragma HLS LOOP_TRIPCOUNT min=20 max=2000
    loop_width: for (int j= 0;j<cols;j++)
#pragma HLS LOOP_TRIPCOUNT min=20 max=2000
		{
#pragma HLS PIPELINE
                        _src >>  s;
                        for(int k=0;k<num;k++)
                        _dst[k] << s;
                }
        }
}
}
#endif


