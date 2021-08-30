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
#error hls_video_fast.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef _HLS_VIDEO_FAST_H_
#define _HLS_VIDEO_FAST_H_

namespace hls {

    template<int PSize,int KERNEL_SIZE,int NUM,typename SRC_T, typename DIFF_T>
int fast_pixel_diff(
        Window<KERNEL_SIZE,KERNEL_SIZE,SRC_T>  &win,
        SRC_T  _threshold,
        ap_uint<2> (&flag_val)[NUM], // Either 0,1 or 2 depending on the threshold.
        DIFF_T (&flag_d)[NUM], // The difference between the center pixel and the pixel
        int (&flag)[PSize][2]) // The coordinate of the pixel
{
#pragma HLS INLINE
    DIFF_T d1,d2;
    int vt1,vt2;
    int x1,x2,y1,y2;  // These are coordinates
    int result=3;
    int K=PSize/2;
    for(int index=0;index<K;index++) {
        int kernel_half=KERNEL_SIZE/2;
        x1=flag[index][0];
        y1=flag[index][1];
        x2=flag[index+K][0];
        y2=flag[index+K][1];
        d1=win.val[kernel_half][kernel_half]-win.val[ y1 ][ x1 ];
        d2=win.val[kernel_half][kernel_half]-win.val[ y2 ][ x2 ];
        flag_d[index]=d1;
        flag_d[index+K]=d2;
        flag_d[index+PSize]=d1;
        if (d1 > _threshold)
            vt1=1;
        else if(d1 < -_threshold)
            vt1=2;
        else
            vt1=0;
        if (d2 > _threshold)
            vt2=1;
        else if(d2 < -_threshold)
            vt2=2;
        else
            vt2=0;
        result &= vt1 | vt2;
        flag_val[index] = vt1;
        flag_val[index+K] = vt2;
        flag_val[index+PSize] = vt1;
    }
    flag_val[PSize/2+PSize] = flag_val[PSize/2];
    flag_d[PSize/2+PSize] = flag_d[PSize/2];
    return result;
}

    template<int PSize,int NUM, typename DIFF_T, typename SRC_T>
int coreScore(DIFF_T (&flag_d)[NUM], SRC_T _threshold)
{
#pragma HLS INLINE

    DIFF_T flag_d_min2[NUM-1];
    DIFF_T flag_d_max2[NUM-1];
    DIFF_T flag_d_min4[NUM-3];
    DIFF_T flag_d_max4[NUM-3];
    DIFF_T flag_d_min8[NUM-7];
    DIFF_T flag_d_max8[NUM-7];
#pragma HLS ARRAY_PARTITION variable=flag_d_min2 dim=0
#pragma HLS ARRAY_PARTITION variable=flag_d_max2 dim=0
#pragma HLS ARRAY_PARTITION variable=flag_d_min4 dim=0
#pragma HLS ARRAY_PARTITION variable=flag_d_max4 dim=0
    if(PSize == 16) {
        for(int i=0;i<NUM-1;i++) {
            flag_d_min2[i] = reg(hls::min<int>(flag_d[i],flag_d[i+1]));
            flag_d_max2[i] = reg(hls::max<int>(flag_d[i],flag_d[i+1]));
        }
        for(int i=0;i<NUM-3;i++) {
            flag_d_min4[i] = reg(hls::min<int>(flag_d_min2[i],flag_d_min2[i+2]));
            flag_d_max4[i] = reg(hls::max<int>(flag_d_max2[i],flag_d_max2[i+2]));
        }

        for(int i=0;i<NUM-7;i++) {
            flag_d_min8[i] = reg(hls::min<int>(flag_d_min4[i],flag_d_min4[i+4]));
            flag_d_max8[i] = reg(hls::max<int>(flag_d_max4[i],flag_d_max4[i+4]));
        }
    } else {
#ifndef __SYNTHESIS__
        assert(false);
#endif
    }

    DIFF_T a0 = _threshold;
    for(int i=0;i<PSize;i+=2) {
        DIFF_T a = hls::numeric_limits<DIFF_T>::max();
        if(PSize == 16) {
            a = flag_d_min8[i+1];
        } else {
            for(int j=1;j<PSize/2+1;j++) {
                a=hls::min<int>(a,flag_d[i+j]);
            }
        }
        a0 = hls::max<int>(a0,hls::min<int>(a,flag_d[i]));
        a0 = hls::max<int>(a0,hls::min<int>(a,flag_d[i+PSize/2+1]));
    }
    DIFF_T b0 = -_threshold;
    for(int i=0;i<PSize;i+=2) {
        DIFF_T b = hls::numeric_limits<DIFF_T>::min();
        if(PSize == 16) {
            b = flag_d_max8[i+1];
        } else {
            for(int j=1;j<PSize/2+1;j++) {
                b=hls::max<int>(b,flag_d[i+j]);
            }
        }
        b0 = hls::min<int>(b0,hls::max<int>(b,flag_d[i]));
        b0 = hls::min<int>(b0,hls::max<int>(b,flag_d[i+PSize/2+1]));
    }
    return hls::max<int>(a0,(DIFF_T)-b0)-1;
}

template<int PSize,int KERNEL_SIZE,typename SRC_T, typename DIFF_T>
bool fast_judge(Window<KERNEL_SIZE,KERNEL_SIZE,SRC_T>  & win,
                SRC_T _threshold,
                ap_uint<2> (&flag_val)[PSize+PSize/2+1],
                DIFF_T  (&flag_d)[PSize+PSize/2+1],
                int (&flag)[PSize][2], // Coordinates to use in comparison
                int &core,
                bool _nonmax_supression) {
#pragma HLS ARRAY_PARTITION variable=flag_val dim=0
#pragma HLS ARRAY_PARTITION variable=flag_d dim=0

#pragma HLS INLINE
    bool iscorner=false;
    int kernel_half=KERNEL_SIZE/2;
    int result=fast_pixel_diff<PSize>(win,_threshold,flag_val,flag_d,flag);
    int count=1;
    for(int c=1;c<PSize+PSize/2+1;c++)
    {
        if(flag_val[c-1]==flag_val[c] && flag_val[c]>0)
        {
            count++;
            if(count>PSize/2)
            {
                iscorner=true;
            }
        }
        else
        {
            count=1;
        }
    }
    if(_nonmax_supression && iscorner) {
        core=coreScore<PSize>(flag_d,_threshold);
    }
    return iscorner;
}
template<int KERNEL_SIZE,typename SRC_T>
bool fast_nonmax(
        Window<KERNEL_SIZE,KERNEL_SIZE,SRC_T>  & core_win
        )
{
#pragma HLS INLINE
    bool iscorner=true;
    for(int k=0;k<3;k++) {
        if(core_win.val[1][1]>core_win.val[0][k])
            iscorner=iscorner && true;
        else
            iscorner=iscorner && false;
    }
    for(int k=0;k<3;k++) {
        if(core_win.val[1][1]>core_win.val[2][k])
            iscorner=iscorner && true;
        else
            iscorner=iscorner && false;
    }
    if(core_win.val[1][1]>core_win.val[1][0]&&core_win.val[1][1]>core_win.val[1][2])
        iscorner=iscorner && true;
    else
        iscorner=iscorner && false;
    return iscorner;
}
//generate mask
template<int PSize,int KERNEL_SIZE,int SRC_T,int ROWS,int COLS>
void FAST_t_opr(
        Mat<ROWS,COLS,SRC_T>    &_src,
        Mat<ROWS,COLS,HLS_8UC1>    &_mask,
        HLS_TNAME(SRC_T)        _threshold,
        bool                    _nonmax_supression,
        int                     (&flag)[PSize][2]) {
    typedef typename pixel_op_type<HLS_TNAME(SRC_T)>::T INPUT_T;
    LineBuffer<KERNEL_SIZE-1,COLS,INPUT_T>    k_buf;
    LineBuffer<2,COLS+KERNEL_SIZE,ap_int<16> >         core_buf;
    Window<3,3,ap_int<16> >                            core_win;
    Window<KERNEL_SIZE,KERNEL_SIZE,INPUT_T>       win;
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)>             s;
    Scalar<HLS_MAT_CN(HLS_8UC1),HLS_TNAME(HLS_8UC1)>             d;
    int rows= _src.rows;
    int cols= _src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    int kernel_half=KERNEL_SIZE/2;
    ap_uint<2> flag_val[PSize+PSize/2+1];
    int flag_d[PSize+PSize/2+1];
#pragma HLS ARRAY_PARTITION variable=flag_val dim=0
#pragma HLS ARRAY_PARTITION variable=flag_d dim=0

    int index=0;
    int offset=KERNEL_SIZE/2;
    if(_nonmax_supression)
    {
        offset=offset+1;
    }
 loop_height: for(HLS_SIZE_T i=0;i<rows+offset;i++) {
    loop_width: for(HLS_SIZE_T j=0;j<cols+offset;j++) {
#pragma HLS LOOP_FLATTEN off
#pragma HLS PIPELINE II=1
            d.val[0]=0;
            if((i<rows&&j<cols)) {
                for(int r= 0;r<KERNEL_SIZE;r++) {
                    for(int c=0;c<KERNEL_SIZE-1;c++) {
                        win.val[r][c]=win.val[r][c+1];//column left shift
                    }
                }
                win.val[0][KERNEL_SIZE-1]=k_buf.val[0][j];
                for(int buf_row= 1;buf_row< KERNEL_SIZE-1;buf_row++) {
                    win.val[buf_row][KERNEL_SIZE-1]=k_buf.val[buf_row][j];
                    k_buf.val[buf_row-1][j]=k_buf.val[buf_row][j];
                }
                //-------
                _src>>s;
                win.val[KERNEL_SIZE-1][KERNEL_SIZE-1]=s.val[0];
                k_buf.val[KERNEL_SIZE-2][j]=s.val[0];
            }
            //------core
            for(int r= 0;r<3;r++)
            {
                for(int c=0;c<3-1;c++)
                {
                    core_win.val[r][c]=core_win.val[r][c+1];//column left shift
                }
            }
            core_win.val[0][3-1]=core_buf.val[0][j];
            for(int buf_row= 1;buf_row< 3-1;buf_row++)
            {
                core_win.val[buf_row][3-1]=core_buf.val[buf_row][j];
                core_buf.val[buf_row-1][j]=core_buf.val[buf_row][j];
            }
            int core=0;
            //output
            //if(i>=KERNEL_SIZE-1&&j>=KERNEL_SIZE-1)
            if(i>=KERNEL_SIZE-1 && i<rows && j>=KERNEL_SIZE-1 & j<cols)
            {
                //process
                bool iscorner=fast_judge<PSize>(win,(INPUT_T)_threshold,flag_val,flag_d,flag,core,_nonmax_supression);
                if(iscorner&&!_nonmax_supression)
                {
                    d.val[0]=255;
                }
            }
            if(i>=rows||j>=cols)
            {
                core=0;
            }
            if(_nonmax_supression)
            {
                core_win.val[3-1][3-1]=core;
                core_buf.val[3-2][j]=core;
                if(core_win.val[1][1]!=0&&i>=KERNEL_SIZE&&j>=KERNEL_SIZE)
                {
                    bool iscorner=fast_nonmax(core_win);
                    if(iscorner)
                    {
                        d.val[0]=255;
                    }
                }
            }

            if(i>=offset&&j>=offset)
            {
                _mask<<d;
            }
        }
    }
}
template<int SRC_T,int ROWS,int COLS>
void FASTX(
        Mat<ROWS,COLS,SRC_T>    &_src,
        Mat<ROWS,COLS,HLS_8UC1> &_mask,
        HLS_TNAME(SRC_T)    _threshold,
        bool   _nomax_supression
        )
{
#pragma HLS INLINE
    int flag[16][2]={{3,0},{4,0},{5,1},{6,2},{6,3},{6,4},{5,5},{4,6},
        {3,6},{2,6},{1,5},{0,4},{0,3},{0,2},{1,1},{2,0}};
    FAST_t_opr<16,7>(_src,_mask,_threshold,_nomax_supression,flag);
}
//generate array 
template<int PSize,int KERNEL_SIZE,typename T, int N, int SRC_T,int ROWS,int COLS>
void FAST_t_opr(
        Mat<ROWS,COLS,SRC_T>    &_src,
        Point_<T>                    (&_keypoints)[N],
        HLS_TNAME(SRC_T)                    _threshold,
        bool                    _nonmax_supression,
        int                     (&flag)[PSize][2]
        )
{
    typedef typename pixel_op_type<HLS_TNAME(SRC_T)>::T INPUT_T;
    LineBuffer<KERNEL_SIZE-1,COLS,INPUT_T>    k_buf;
    LineBuffer<2,COLS+KERNEL_SIZE,ap_int<16> >         core_buf;
    Window<3,3,ap_int<16> >                            core_win;
    Window<KERNEL_SIZE,KERNEL_SIZE,INPUT_T>       win;
    Scalar<HLS_MAT_CN(SRC_T), HLS_TNAME(SRC_T)>             s;
    int rows= _src.rows;
    int cols= _src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    int kernel_half=KERNEL_SIZE/2;
    ap_uint<2> flag_val[PSize+PSize/2+1];
    int  flag_d[PSize+PSize/2+1];
#pragma HLS ARRAY_PARTITION variable=flag_val dim=0
#pragma HLS ARRAY_PARTITION variable=flag_d dim=0
    int index=0;
    int offset=KERNEL_SIZE/2;

    if(_nonmax_supression)
    {
        offset=offset+1;
    }
 loop_height: for(HLS_SIZE_T i=0;i<rows+offset;i++) {
    loop_width: for(HLS_SIZE_T j=0;j<cols+offset;j++) {
#pragma HLS LOOP_FLATTEN off
#pragma HLS PIPELINE II=1
            if(i<rows&&j<cols) {
                for(int r= 0;r<KERNEL_SIZE;r++) {
                    for(int c=0;c<KERNEL_SIZE-1;c++) {
                        win.val[r][c]=win.val[r][c+1];//column left shift
                    }
                }
                win.val[0][KERNEL_SIZE-1]=k_buf.val[0][j];
                for(int buf_row= 1;buf_row< KERNEL_SIZE-1;buf_row++) {
                    win.val[buf_row][KERNEL_SIZE-1]=k_buf.val[buf_row][j];
                    k_buf.val[buf_row-1][j]=k_buf.val[buf_row][j];
                }
                //-------
                _src>>s;
                win.val[KERNEL_SIZE-1][KERNEL_SIZE-1]=s.val[0];
                k_buf.val[KERNEL_SIZE-2][j]=s.val[0];
            }
            //------core
            for(int r= 0;r<3;r++)
            {
                for(int c=0;c<3-1;c++)
                {
                    core_win.val[r][c]=core_win.val[r][c+1];//column left shift
                }
            }
            core_win.val[0][3-1]=core_buf.val[0][j];
            for(int buf_row= 1;buf_row< 3-1;buf_row++)
            {
                core_win.val[buf_row][3-1]=core_buf.val[buf_row][j];
                core_buf.val[buf_row-1][j]=core_buf.val[buf_row][j];
            }
            int core=0;
            //output
            //if(i>=KERNEL_SIZE-1&&j>=KERNEL_SIZE-1)
            if(i>=KERNEL_SIZE-1 && i<rows && j>=KERNEL_SIZE-1 & j<cols)
            {
                //process
                bool iscorner=fast_judge<PSize>(win,(INPUT_T)_threshold,flag_val,flag_d,flag,core,_nonmax_supression);
                if(iscorner&&!_nonmax_supression)
                {
                    if(index<N)
                    {
                    _keypoints[index].x=j-offset;
                    _keypoints[index].y=i-offset;
                    index++;
                    }
                }
            }
            if(i>=rows||j>=cols)
            {
                core=0;
            }
            if(_nonmax_supression)
            {
                core_win.val[3-1][3-1]=core;
                core_buf.val[3-2][j]=core;
                if(i>=KERNEL_SIZE&&j>=KERNEL_SIZE&&core_win.val[1][1]!=0)
                {
                    bool iscorner=fast_nonmax(core_win);
                    if(iscorner)
                    {
                    if(index<N)
                    {
                        _keypoints[index].x=j-offset;
                        _keypoints[index].y=i-offset;
                        index++;
                    }
                    }
                }
            }

        }
    }
}
template<typename T, int N, int SRC_T,int ROWS,int COLS>
void FASTX(
        Mat<ROWS,COLS,SRC_T>    &_src,
        Point_<T> (&_keypoints)[N],
        HLS_TNAME(SRC_T)    _threshold,
        bool   _nomax_supression
        )
{
#pragma HLS INLINE
    int flag[16][2]={{3,0},{4,0},{5,1},{6,2},{6,3},{6,4},{5,5},{4,6},
        {3,6},{2,6},{1,5},{0,4},{0,3},{0,2},{1,1},{2,0}};
    FAST_t_opr<16,7>(_src,_keypoints,_threshold,_nomax_supression,flag);
}
template<int SRC_T,int MASK_T,int ROWS,int COLS>
void PaintMask(
        Mat<ROWS,COLS,SRC_T>       &_src,
        Mat<ROWS,COLS,MASK_T>      &_mask,
        Mat<ROWS,COLS,SRC_T>       &_dst,
        Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)> _color
        )
{
#ifndef __SYNTHESIS__
    assert(_src.rows == _mask.rows);
    assert(_src.rows == _dst.rows);
    assert(_src.cols == _mask.cols);
    assert(_src.cols == _dst.cols);
#endif
    int rows= _src.rows;
    int cols= _src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>      s;
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>      d;
    Scalar<HLS_MAT_CN(MASK_T),HLS_TNAME(MASK_T)>    m;
 loop_height: for(HLS_SIZE_T i=0;i<rows;i++) {
    loop_width: for(HLS_SIZE_T j=0;j<cols;j++) {
#pragma HLS LOOP_FLATTEN off
#pragma HLS PIPELINE II=1
            _src >> s;
            _mask >> m;
            if(m.val[0]!=0)
            {
                d = _color;
            }
            else
            {
                d = s;
            }
            _dst << d;
        }
    }
}
}
#endif


