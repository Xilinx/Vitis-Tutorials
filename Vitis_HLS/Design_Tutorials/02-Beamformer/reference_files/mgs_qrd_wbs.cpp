/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "mgs_qrd_wbs.h"

void mgs_qrd(float Qr_i[MAX_ROW*MAX_COL], float Qi_i[MAX_ROW*MAX_COL], int col, int row, float br_i[MAX_ROW], float bi_i[MAX_ROW], float xr_i[MAX_COL],float xi_i[MAX_COL])
{

#pragma HLS ARRAY_RESHAPE variable=Qr_i complete dim=1
#pragma HLS ARRAY_RESHAPE variable=Qi_i complete dim=1
#pragma HLS ARRAY_RESHAPE variable=br_i complete dim=1
#pragma HLS ARRAY_RESHAPE variable=bi_i complete dim=1
#pragma HLS ARRAY_RESHAPE variable=xr_i complete dim=1
#pragma HLS ARRAY_RESHAPE variable=xi_i complete dim=1

	float Qr[MAX_ROW][MAX_COL],  Qi[MAX_ROW][MAX_COL],  br[MAX_ROW], bi[MAX_ROW],  xr[MAX_COL], xi[MAX_COL];

#pragma HLS ARRAY_PARTITION variable=Qr complete dim=1
#pragma HLS ARRAY_PARTITION variable=Qi complete dim=1
#pragma HLS ARRAY_PARTITION variable=br complete dim=1
#pragma HLS ARRAY_PARTITION variable=bi complete dim=1
#pragma HLS ARRAY_PARTITION variable=xr complete dim=1
#pragma HLS ARRAY_PARTITION variable=xi complete dim=1

	int i, j, k,ii,kk,lcnts[UNFACT],cnt;
	float  Rr[MAX_ROW][MAX_ROW], Ri[MAX_ROW][MAX_ROW],Qrtb[MAX_ROW],Qitb[MAX_ROW],Qcr[MAX_ROW],Qci[MAX_ROW];
	float rtr[UNFACT],rti[UNFACT],rtr2,rti2,qrt[UNFACT],qti[UNFACT],qti2,qtr[UNFACT],qtr2,qqr[UNFACT],qqi[UNFACT],tr[UNFACT],ti[UNFACT],nrmr[UNFACT],nrmi[UNFACT],acr,aci,rrr,rii;

#pragma HLS ARRAY_PARTITION variable=Rr complete dim=1  
#pragma HLS ARRAY_PARTITION variable=Ri complete dim=1  
#pragma HLS ARRAY_PARTITION variable=Qrtb complete dim=1 
#pragma HLS ARRAY_PARTITION variable=Qitb complete dim=1 
#pragma HLS ARRAY_PARTITION variable=Qcr complete dim=1 
#pragma HLS ARRAY_PARTITION variable=Qci complete dim=1 
#pragma HLS ARRAY_PARTITION variable=rtr complete dim=1 
#pragma HLS ARRAY_PARTITION variable=rti complete dim=1 
#pragma HLS ARRAY_PARTITION variable=qrt complete dim=1 
#pragma HLS ARRAY_PARTITION variable=qti complete dim=1 
#pragma HLS ARRAY_PARTITION variable=qtr complete dim=1 
#pragma HLS ARRAY_PARTITION variable=qqr complete dim=1 
#pragma HLS ARRAY_PARTITION variable=qqi complete dim=1 
#pragma HLS ARRAY_PARTITION variable=tr complete dim=1 
#pragma HLS ARRAY_PARTITION variable=ti complete dim=1 
#pragma HLS ARRAY_PARTITION variable=nrmr complete dim=1 
#pragma HLS ARRAY_PARTITION variable=nrmi complete dim=1   

	cnt=0;

//Read in Matrix X, Matrix B to be solved  //Solve the QRD, Decompose to Q, & R
	l3:for (ii=0; ii<col; ii++) {
#pragma HLS loop_tripcount min=32 max=32

		l3a:for (kk=0; kk<row; kk++) {
#pragma HLS loop_tripcount min=32 max=32
#pragma HLS PIPELINE II=1 
DO_PRAGMA(HLS UNROLL factor=UNFACT)
			Qcr[kk] = Qr[kk][ii];
			Qci[kk] = Qi[kk][ii];
		}

		l4:for (j=0; j<ii; j++) {

#pragma HLS loop_tripcount min=0 max=31 avg=15

			l4a:for (kk=0; kk<UNFACT; kk++) {

#pragma HLS loop_tripcount min=32 max=32
#pragma HLS UNROLL 

				rtr[kk] = 0;
				rti[kk] = 0;
			}

			l5:for (kk=0; kk<row; kk++) {

#pragma HLS loop_tripcount min=32 max=32
DO_PRAGMA(HLS UNROLL factor=UNFACT)
#pragma HLS PIPELINE II=1 

				cmult(Qr[kk][j],-Qi[kk][j],Qcr[kk],Qci[kk],&tr[kk%UNFACT],&ti[kk%UNFACT]);

				rtr[kk%UNFACT] = rtr[kk%UNFACT] + tr[kk%UNFACT];
				rti[kk%UNFACT] = rti[kk%UNFACT] + ti[kk%UNFACT];
			} 

			l6a:for (kk=0; kk<UNFACT-1; kk++) {

#pragma HLS loop_tripcount min=31 max=31
#pragma HLS UNROLL

				rtr[0]= rtr[0]+ rtr[kk+1];
				rti[0]= rti[0]+ rti[kk+1];
			}

			Rr[j][ii] = rtr[0];
			Ri[j][ii] = rti[0];

			l6:for (kk=0; kk<row; kk++) {

#pragma HLS loop_tripcount min=32 max=32
#pragma HLS PIPELINE
DO_PRAGMA(HLS UNROLL factor=UNFACT)

				qtr[kk%UNFACT] = Qr[kk][j];//past Q upto current
				qti[kk%UNFACT] = Qi[kk][j];

				cmult(qtr[kk%UNFACT],qti[kk%UNFACT],rtr[0],rti[0],&tr[kk%UNFACT],&ti[kk%UNFACT]);

				Qcr[kk]=Qcr[kk]-tr[kk%UNFACT];	//Replace current Q
				Qci[kk]=Qci[kk]-ti[kk%UNFACT];
				nrmr[kk%UNFACT] = 0;
				nrmi[kk%UNFACT] = 0;
			} 	
		}

		l7:for (kk=0; kk<row; kk++) {

#pragma HLS loop_tripcount min=32 max=32
DO_PRAGMA(HLS UNROLL factor=UNFACT)
#pragma HLS PIPELINE II=1 

			nrmr[kk%UNFACT]  = nrmr[kk%UNFACT]  + Qcr[kk]* Qcr[kk];
			nrmi[kk%UNFACT]  = nrmi[kk%UNFACT]  + Qci[kk]* Qci[kk];
		}	

		l7a:for (kk=0; kk<UNFACT-1; kk++) {

#pragma HLS loop_tripcount min=31 max=31
#pragma HLS UNROLL

			nrmr[0]= nrmr[0]+ nrmr[kk+1];
			nrmi[0]= nrmi[0]+ nrmi[kk+1];
		}

		Rr[ii][ii]=sqrt(nrmr[0]+nrmi[0]);
		Ri[ii][ii]=0.0;


		l8:for (kk=0; kk<row; kk++) {

#pragma HLS loop_tripcount min=32 max=32
DO_PRAGMA(HLS UNROLL factor=UNFACT)
#pragma HLS PIPELINE II=1 

			Qcr[kk]=Qcr[kk]/Rr[ii][ii];
			Qci[kk]=Qci[kk]/Rr[ii][ii];
		}	

		l8a:for (kk=0; kk<row; kk++) {

#pragma HLS loop_tripcount min=32 max=32
#pragma HLS PIPELINE II=1 
DO_PRAGMA(HLS UNROLL factor=UNFACT)

			Qr[kk][ii]= Qcr[kk];
			Qi[kk][ii]= Qci[kk];
			rtr[kk%UNFACT] = 0;
			rti[kk%UNFACT] = 0;
		}

	} 

	//Back Sub and solve l0:for X
	l9:for (ii=0; ii<col; ii++) {

#pragma HLS loop_tripcount min=32 max=32

		l9a:for (kk=0; kk<UNFACT; kk++) {

#pragma HLS loop_tripcount min=32 max=32
#pragma HLS UNROLL

			rtr[kk]= 0;
			rti[kk]= 0;
		}

		loop10:for (kk=0; kk<row; kk++) {

#pragma HLS loop_tripcount min=32 max=32
DO_PRAGMA(HLS UNROLL factor=UNFACT)
#pragma HLS PIPELINE II=1 

			qtr[kk%UNFACT] = Qr[kk][ii];
			qti[kk%UNFACT] = Qi[kk][ii];

			cmult(qtr[kk%UNFACT],-qti[kk%UNFACT],br[kk],bi[kk],&tr[kk%UNFACT],&ti[kk%UNFACT]);

			rtr[kk%UNFACT] = rtr[kk%UNFACT] + tr[kk%UNFACT];
			rti[kk%UNFACT] = rti[kk%UNFACT] + ti[kk%UNFACT];
		} 

		l10a:for (kk=0; kk<UNFACT-1; kk++) {

#pragma HLS loop_tripcount min=31 max=31
#pragma HLS UNROLL

			rtr[0]= rtr[0]+ rtr[kk+1];
			rti[0]= rti[0]+ rti[kk+1];
		}

		Qrtb[ii]=rtr[0];
		Qitb[ii]=rti[0];
	}


	l11:for (j=col-1; j >= 0; j--) {

#pragma HLS loop_tripcount min=32 max=32

		tr[0]= 0;
		ti[0] =0;

		l12:for (k=j+1; k<col; k++) {

#pragma HLS loop_tripcount min=0 max=31 avg=15
#pragma HLS PIPELINE II=1 

			cmult(Rr[j][k],Ri[j][k],xr[k],xi[k],&acr,&aci);
			tr[0] = tr[0] + acr;
			ti[0] = ti[0] + aci;
		}
		cdiv((Qrtb[j] - tr[0]),(Qitb[j] - ti[0]),Rr[j][j],Ri[j][j],&xr[j],&xi[j]);
	}

	//Stream out the answer, back sub x
	l13:for (ii=0; ii<col; ii++) {

#pragma HLS loop_tripcount min=32 max=32
#pragma HLS PIPELINE II=1 

		xr_i[ii] = xr[ii];
		xi_i[ii] = xi[ii];
	}
}

void cdiv(float a, float b, float c, float d, float *R, float *I)
{
	*R = ((a*c) + (b*d))/(c*c + d*d);
	*I = ((b*c)-(a*d))/(c*c + d*d);
}

void cmult(float a, float b, float c, float d, float *R, float *I)
{
	*R = (a*c) - (b*d);
	*I = (a+b)*(c+d) -(a*c) - (b*d);
}

