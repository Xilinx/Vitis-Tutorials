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
#error hls_video_haar.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef _HLS_VIDEO_HAAR_
#define _HLS_VIDEO_HAAR_

#define HLS_HAAR_FEATURE_MAX 3

namespace hls {

typedef     ap_fixed<12,6,AP_RND>     VAR_T;
typedef     ap_uint<3>                WEIGHT_T;
typedef     unsigned int              STHRESHOLD_T;
typedef     short                     CTHRESHOLD_T;
typedef     unsigned short            ALPHA_T;
//------------------haar--
struct HaarRect
{
    Rect_< ap_uint<6> >    r;// max size 64x64
    WEIGHT_T                weight;
};
struct HaarFeature
{
    public:
    HaarFeature()
    {
#pragma HLS ARRAY_PARTITION variable=rect complete dim=1
    };
    HaarRect rect[HLS_HAAR_FEATURE_MAX];
    //int     tilted;
};
template <int FEATURE_MAX>        
struct HaarClassifier
{
    public:
    HaarClassifier()
    {
#pragma HLS ARRAY_PARTITION variable=haar_feature complete dim=1
        };
        unsigned short     count;
        HaarFeature     haar_feature[FEATURE_MAX];
        CTHRESHOLD_T    threshold[FEATURE_MAX];
        ap_int<5>       left[FEATURE_MAX];
        ap_int<5>       right[FEATURE_MAX];
        ALPHA_T         alpha[FEATURE_MAX+1];
        };
template <int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX>
struct HaarStageClassifier
{
    public:
    HaarStageClassifier()
    {
#pragma HLS ARRAY_PARTITION variable=classifier complete dim=1
        };
        unsigned char               count[STAGE];// max calssifer each stage is 255
        STHRESHOLD_T                threshold[STAGE];
        HaarClassifier<FEATURE_MAX> classifier[CLASSIFIER_ALL];
       // int     next[STAGE];
       // int     child[STAGE];
       // int     parent[STAGE];

        };
template <int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX>
struct HaarClassifierCascade
{
    public:
        HaarClassifierCascade()
        {
        };
        unsigned short        count;
        Size_<unsigned char>  real_window_size;
        HaarStageClassifier<STAGE,CLASSIFIER_ALL,FEATURE_MAX>   stage_classifier;
};
//---------------load--------------------------------------
class HaarLoadData
{
        public:
template <int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX>
HaarLoadData(HaarClassifierCascade<STAGE,CLASSIFIER_ALL,FEATURE_MAX> &cascade,
        //const  int ca_flags,
        const  int ca_count,
        //const  float ca_scale,
        const  Size_<unsigned char>   ca_real_window_size,
        const  int          s_count[],
        const  float        s_threshold[],
        const  int          c_left[][FEATURE_MAX],
        const  int          c_right[][FEATURE_MAX],
        const  float        c_threshold[][FEATURE_MAX],
        const  float        c_alpha[][FEATURE_MAX+1],
        const  unsigned int haar_feature[][FEATURE_MAX][4*HLS_HAAR_FEATURE_MAX],
        const  float        haar_feature_weight[][FEATURE_MAX][HLS_HAAR_FEATURE_MAX]
        //const  int s_next[],
        //const  int s_child[],
        //const  int s_parent[],
        //const  int c_count[]
        )
{
       // cascade.flags=ca_flags;
        cascade.count=ca_count;
       // cascade.scale=ca_scale;
        cascade.real_window_size=ca_real_window_size;
        for(int i=0;i<STAGE;i++)
        {
                cascade.stage_classifier.count[i]=s_count[i];
                cascade.stage_classifier.threshold[i]=s_threshold[i];
               // cascade.stage_classifier.next[i]=s_next[i];
               // cascade.stage_classifier.child[i]=s_child[i];
               // cascade.stage_classifier.parent[i]=s_parent[i];
        }
        for(int i=0;i<CLASSIFIER_ALL;i++)
        {
              //        cascade.stage_classifier.classifier[i].count=c_count[i];
               for(int j=0;j<FEATURE_MAX;j++)
               {
                      cascade.stage_classifier.classifier[i].left[j]=c_left[i][j];
                      cascade.stage_classifier.classifier[i].right[j]=c_right[i][j];
                      cascade.stage_classifier.classifier[i].threshold[j]=c_threshold[i][j];
                      cascade.stage_classifier.classifier[i].alpha[j]=c_alpha[i][j];
                      cascade.stage_classifier.classifier[i].alpha[FEATURE_MAX]=c_alpha[i][FEATURE_MAX];
                      //cascade.stage_classifier.classifier[i].haar_feature[j].tilted=haar_feature[i][j][0];
                      for(int n=0;n<HLS_HAAR_FEATURE_MAX;n++)
                      {
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[n].r.x=haar_feature[i][j][n*4+0];
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[n].r.y=haar_feature[i][j][n*4+1];
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[n].r.width=haar_feature[i][j][n*4+2];
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[n].r.height=haar_feature[i][j][n*4+3];
                       }
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[0].weight=-1;
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[1].weight=haar_feature_weight[i][j][1];
                            cascade.stage_classifier.classifier[i].haar_feature[j].rect[2].weight=haar_feature_weight[i][j][2];
                 }
        }
}
};
//---------------calculate window integral -------------
template<typename SUMT, typename VART,int ROWS,int COLS>
void intergral_win(
        Window<ROWS,COLS,VART>          &win_var,
        SUMT                             &var
        )
{
#pragma HLS INLINE
    var=0;  
    for(int j=0;j<COLS;j++)
    {
        SUMT t = 0;
        for(int i=0;i<ROWS;i++)
        {
            var=var+win_var.val[i][j];
            t += win_var.val[i][j];
        }
        //        std::cout << "col " << j << " " << t << "\n";
    }
}
//---------------------------calculate classifier---------------
template< typename SRC_T,int ROWS,int COLS,int FEATURE_MAX>
ALPHA_T icvEvalHidHaarClassifier(HaarClassifier<FEATURE_MAX> & classifier,Window<ROWS,COLS,SRC_T>& win,VAR_T &variance )
{
#pragma HLS inline off
#pragma HLS function_instantiate variable=&classifier

    // double factor=1.0/(20*20);//num form 18 to 20 , the face num form 103 to 108
    int m=0,i=0;
    for(m=0;m<FEATURE_MAX;m++)// classifier is tree  ,haar_feature[]  is parallel arry 
    {
        Rect_<ap_uint<6> > rr=classifier.haar_feature[i].rect[0].r;
        if(i>0||(i==0&&m==0))
        {
            // FIXME: sum is ap_uint<8+BitWidth<WEIGHT_T>+BitWidth<ROWS*COLS*HLS_HAAR_FEATURE_MAX>::val >
            CTHRESHOLD_T      sum=-(win.val[rr.height][rr.width]-win.val[rr.height][rr.x]-win.val[rr.y][rr.width]+win.val[rr.y][rr.x]);//?
            for(int j=1;j<HLS_HAAR_FEATURE_MAX;j++)
            {
                WEIGHT_T weight=classifier.haar_feature[i].rect[j].weight;
                Rect_< ap_uint<6> > rj=classifier.haar_feature[i].rect[j].r;
                sum+=(win.val[rj.height][rj.width]-win.val[rj.height][rj.x]-win.val[rj.y][rj.width]+win.val[rj.y][rj.x])*weight;//?
            }
            if(sum<classifier.threshold[i]*variance)
            {
                i=classifier.left[i];
            }
            else
            {
                i=classifier.right[i];
            }
        }
        // if (i<=0)
        //         break;
    }      
    return classifier.alpha[-i] ;
}
//---------------------------calculate stage_classifier--------------
template< typename SRC_T,int ROWS,int COLS,int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX>
bool RunHaarClassifierCascade(HaarClassifierCascade<STAGE,CLASSIFIER_ALL,FEATURE_MAX> &cascade,Window<ROWS,COLS,SRC_T>&win,VAR_T &variance )
{
#pragma HLS inline
    ALPHA_T sum=0;
    bool result=true;
    unsigned char stage_index=0;//max classifier of each stage is 255
    unsigned char stage_count=0;
    for (int i=0;i<CLASSIFIER_ALL;i++)//in the face trning data there is no ralition between diffierent trees(stage_classifier),is an line no next,just child and parent. stage_classifier is  trees(classifier) group.
    {
        sum+=icvEvalHidHaarClassifier(cascade.stage_classifier.classifier[i],win,variance);
        stage_count++;
        if(cascade.stage_classifier.count[stage_index]==stage_count)
        {
            if(sum < cascade.stage_classifier.threshold[stage_index])
            {
                result =false;  
            }
            sum=0;
            stage_count=0;
            stage_index++;
        }
    }        
    return result;
}
template < typename T>
T TaylorExp(T &t)
{
   return (1+((T)0.5)*(t)-((T)0.125)*(t)*(t));
}
//------------------------haar scan the whole imge - --------------
template< int K_H,int K_W,int S_T,int ROWS,int COLS,int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX, int DST_T>
void HaarClassifierObject_opr(
                HaarClassifierCascade<STAGE,CLASSIFIER_ALL,FEATURE_MAX>  &cascade,
		Mat<ROWS, COLS, S_T>	&_src,
		Mat<ROWS, COLS, DST_T>	&_mask
		)
{
    int storage_count=0;

#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
    Window<K_H,K_W,ap_uint<16> >	    qwin; // 5 = ceil(log2(COLS))
    LineBuffer<K_H-1,COLS,ap_uint<16> >	    qbuf;
#endif

    Window<1,K_W+1,ap_uint<16+16> >	    sqwin; // 5 = ceil(log2(COLS))
    Window<K_H+1,K_W+1,ap_uint<8+16> >	    swin; // 16 = ceil(log2(K_H*COLS))
    LineBuffer<K_H,COLS,HLS_TNAME(S_T)>	    sbuf;

    int rows=_src.rows;
    int cols=_src.cols;
    assert(rows <= ROWS);
    assert(cols <= COLS);
    Scalar<1,HLS_TNAME(DST_T)>                      m;
    Scalar<HLS_MAT_CN(S_T),HLS_TNAME(S_T)>          ss;

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
#pragma HLS loop_flatten off
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false

            {
#pragma HLS expression_balance off
            sqwin.shift_left();
            swin.shift_left();//column left shift
            swin.val[0][K_W]=(j == 0 ? ap_uint<24>(0) :swin.val[0][K_W-1])+sbuf.val[0][j];
            ap_uint<16> col_sum=sbuf.val[0][j];
            ap_uint<32> col_sum_sq=0;
            for(int buf_row= 1;buf_row<K_H;buf_row++)
            {
                ss.val[0]=sbuf.val[buf_row][j];
                sbuf.val[buf_row-1][j]=ss.val[0];
                col_sum += ss.val[0];
                col_sum_sq += ss.val[0] * ss.val[0];
                swin.val[buf_row][K_W] = (j == 0 ? ap_uint<24>(0) :swin.val[buf_row][K_W-1]) + col_sum;
            }
            _src >> ss;
            sbuf.val[K_H-1][j]=ss.val[0];
            col_sum += ss.val[0];
            col_sum_sq += ss.val[0] * ss.val[0];
            swin.val[K_H][K_W] = (j == 0 ? ap_uint<24>(0) : swin.val[K_H][K_W-1]) + col_sum;
            sqwin.val[0][K_W] = (j == 0 ? ap_uint<32>(0) : sqwin.val[0][K_W-1]) + col_sum_sq;

#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
            qwin.shift_left();//column left shift
            qwin.val[0][K_W-1]=qbuf.val[0][j];
            for(int buf_row= 1;buf_row<K_H-1;buf_row++)
            {
                qwin.val[buf_row][K_W-1]=qbuf.val[buf_row][j];
                qbuf.val[buf_row-1][j]=qbuf.val[buf_row][j];
            }
            qbuf.val[K_H-2][j]=ss.val[0] * ss.val[0];
            qwin.val[K_H-1][K_W-1]=ss.val[0] * ss.val[0];
#endif
            //            std::cout << "var = " << col_sum_sq << " " << sqwin.val[0][0] << " " << sqwin.val[0][K_W] << "\n";
            }

            m.val[0]=0;
            if(i>=K_H && j>=K_W &&( (j & 1)==0 ))//?
            {
                int b=K_H,s=0;
                // FIXME: perhaps ap_ufixed<16,0> ?
                float inv_window_area=1.0/(K_H*K_W);
                ap_uint<26> varsum=sqwin.val[0][K_W] - sqwin.val[0][0];
#if !defined(__SYNTHESIS__) && defined(_HLSCLIB_DEBUG_)
                ap_uint<26> varsum2;
                intergral_win(qwin,varsum2);
                if(varsum != varsum2)
                    std::cout << i << " " << j << " " << varsum << " " << varsum2 << "\n";
#endif
                ap_uint<17> meansum=swin.val[b][b]-swin.val[b][s]-swin.val[s][b]+swin.val[s][s];
                ap_fixed<32,18,AP_RND> mean=(meansum)*inv_window_area;
                ap_fixed<32,18,AP_RND> meansq=mean*mean;
                ap_fixed<32,18,AP_RND> var_a=varsum*inv_window_area;
                ap_fixed<32,18,AP_RND> variance=var_a-meansq;
                if(variance>0)
                {
                    //variance = ::hls::sqrtf(variance);
                    variance = (variance/100);
                    ap_fixed<32,18,AP_RND> t=variance-1;
                    if(variance<=4)
                        variance= TaylorExp(t)+1;
                    else if(variance >4 && variance <=16)
                    {
                        ap_fixed<32,18,AP_RND> tt=t/4;
                        variance= 2*TaylorExp(tt)+1;
                    }
                    else if(variance >16 && variance <=64)
                    {
                        ap_fixed<32,18,AP_RND> tt=t/16;
                        variance= 4*TaylorExp(tt)+1;
                    }
                    else if(variance >64 && variance <=256)
                    {
                        ap_fixed<32,18,AP_RND> tt=t/64;
                        variance= 8*TaylorExp(tt)+1;
                    }
                    else
                        variance=16;
                }
                else
                {
                    variance = 1;
                }
                //variance=variance/10; // FIXME: Why?  At least use a power of 2.
                ap_fixed<12,6,AP_RND> var=variance;
                bool  Isface=RunHaarClassifierCascade(cascade,swin,var);
                if(Isface)
                {
                    if(variance>65)
                    {
                        variance=65;
                    }
                    variance=variance*1000;  // FIXME: Why?  At least use a power of 2.
                    storage_count++;
                    //m.val[0]=(unsigned short)variance;
                    m.val[0]=(unsigned char)variance;
                }
            }
            _mask << m;
        }        
    }
    //printf("Face detected %d\n",storage_count);  
}
template<int K_H,int K_W,int SRC_T,int DST_T,int ROWS,int COLS,int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX>
void HaarClassifierObject(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_mask,
                HaarClassifierCascade<STAGE,CLASSIFIER_ALL,FEATURE_MAX>       &_cascade
                )
{       
#pragma HLS inline 
//         int rows =_src.rows;
//         int cols =_src.cols;
// 	Mat<ROWS, COLS, HLS_16UC1>	    sqsrc(rows,cols);
// 	Mat<ROWS, COLS, HLS_8UC1>	    gray0(rows,cols);
// 	Mat<ROWS, COLS, HLS_8UC1>	    gray1(rows,cols);
// 	Mat<ROWS, COLS, HLS_8UC1>	    gray2(rows,cols);
// 	Mat<ROWS, COLS, HLS_8UC1>	    gray3(rows,cols);
//         Duplicate(_src,gray0,gray1);
//         Duplicate(gray0,gray2,gray3);
//         Mul(gray1,gray2,sqsrc,1);
        HaarClassifierObject_opr<K_H,K_W>(_cascade,_src, _mask);
}
//-------------------------combin storage-----------------
template<typename RECT_T>
void CombineStorage(Rect_<RECT_T> _storage_group[][300],int m, Rect_<RECT_T> _storage[],int num)
{
        int count=0;
        for(int i=0;i<m;i++)
        {
                for(int j=0;j<num;j++)
                {
                        if(count<num&&_storage_group[i][j].width!=0&&_storage_group[i][j].height!=0)
                        {
                            _storage[count++]=_storage_group[i][j];
                        }
                }
        }
        for(int i=0;i<num;i++)
        {
         if(i>=count)
            { 
               _storage[count].x=0;
               _storage[count].y=0;
               _storage[count].width=0;
               _storage[count].height=0;
         
            }
         }
}
template<int SRC_T, int DST_T,int ROWS,int COLS>
void Combin_mask(
		Mat<ROWS, COLS, SRC_T>	    &mask0,
		Mat<ROWS/2, COLS/2, SRC_T>  &mask1,
		Mat<ROWS/4, COLS/4, SRC_T>  &mask2,
		Mat<ROWS/8, COLS/8, SRC_T>  &mask3,
		Mat<ROWS, COLS, DST_T>	    &dst
                )
{
    int rows =mask0.rows;
    int cols =mask0.cols;
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>  s;
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>  m;

    int i1=0;
    int i2=0;
    int i3=0;
    for(int i=0;i<rows;i++)
    {
        int j1=0;
        int j2=0;
        int j3=0;
        for(int j=0;j<cols;j++)
        {
#pragma HLS PIPELINE
            s.val[0]=0;
            mask0>> m;
            if(m.val[0]!=0)
                s.val[0]=255;
            if(i==i1*2&&j==j1*2)
            {
                mask1>>m;
                s.val[0]=s.val[0]|m.val[0];
                j1++;
            }
            if(i==i2*4&&j==j2*4)
            {
                mask2>>m;
                s.val[0]=s.val[0]|m.val[0];
                j2++;
            }
            if(i==i3*8&&j==j3*8)
            {
                mask3>>m;
                s.val[0]=s.val[0]|m.val[0];
                j3++;
            }
            dst<<s;
        }
        if(i==i1*2)
            i1++;
        if(i==i2*4)
            i2++;
        if(i==i3*8)
            i3++;
    }
}
//-------------------------haar detect- with different size of image--------------
template<int K_H,int K_W,int SRC_T,int DST_T,int ROWS,int COLS,int STAGE,int CLASSIFIER_ALL,int FEATURE_MAX>
void HaarClassifierObject_group(
		Mat<ROWS, COLS, SRC_T>	    &_src,
		Mat<ROWS, COLS, DST_T>	    &_mask,
                HaarClassifierCascade<STAGE,CLASSIFIER_ALL,FEATURE_MAX>       &_cascade
                )
{       
#pragma HLS inline
        int rows =_src.rows;
        int cols =_src.cols;
	Mat<ROWS, COLS, HLS_8UC1>	    gray1(rows,cols);
	Mat<ROWS, COLS, HLS_8UC1>	    gray2(rows,cols);
	Mat<ROWS, COLS, HLS_8UC1>	    gray3(rows,cols);
	Mat<ROWS, COLS, HLS_8UC1>	    gray4(rows,cols);
	Mat<ROWS, COLS, HLS_8UC1>	    gray5(rows,cols);
	Mat<ROWS, COLS, HLS_8UC1>	    gray6(rows,cols);
        Duplicate(_src, gray1, gray2 );
        Duplicate(gray1, gray3, gray4 );
        Duplicate(gray2, gray5, gray6 );
	Mat<ROWS/2, COLS/2, HLS_8UC1>	    internal_1(rows/2,cols/2);
	Mat<ROWS/4, COLS/4, HLS_8UC1>	    internal_2(rows/4,cols/4);
	Mat<ROWS/8, COLS/8, HLS_8UC1>	    internal_3(rows/8,cols/8);
	Mat<ROWS,   COLS, HLS_8UC1>	    mask_0(rows,cols);
	Mat<ROWS/2, COLS/2, HLS_8UC1>	    mask_1(rows/2,cols/2);
	Mat<ROWS/4, COLS/4, HLS_8UC1>	    mask_2(rows/4,cols/4);
	Mat<ROWS/8, COLS/8, HLS_8UC1>	    mask_3(rows/8,cols/8);
        Resize(gray4,internal_1);
        Resize(gray5,internal_2);
        Resize(gray6,internal_3);
        HaarClassifierObject<K_H, K_W > ( gray3, mask_0, _cascade);
        HaarClassifierObject<K_H, K_W > ( internal_1, mask_1, _cascade);
        HaarClassifierObject<K_H, K_W > ( internal_2, mask_2, _cascade);
        HaarClassifierObject<K_H, K_W > ( internal_3, mask_3, _cascade);
        Combin_mask<DST_T,DST_T,ROWS,COLS>(mask_0,mask_1,mask_2,mask_3,_mask);
}

}
#endif




