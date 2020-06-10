/*
 * Copyright 2019 Xilinx, Inc.
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


#include "cholesky_kernel.hpp"	

extern "C" void cholesky_kernel(int diagSize, double* matrixA){
#pragma HLS INTERFACE m_axi offset = slave bundle = gmem0 port = matrixA latency = 64 \
  num_read_outstanding = 16 num_write_outstanding = 16 \
  max_read_burst_length = 64 max_write_burst_length = 64 depth = 256

#pragma HLS INTERFACE s_axilite port = diagSize bundle = control
#pragma HLS INTERFACE s_axilite port = matrixA bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control
	
	dataType dataA[MAXN][MAXN];

    for(int i = 0; i < diagSize; i++){
			 
		for (int j = 0; j < diagSize; j++) {
			dataA[i][j] = matrixA[i*diagSize + j];
		}
				  
	}
		  
	dataType tmp[8],tmpij,tmpjj,tmpcol[MAXN];
    Loop_col:
    for (int j = 0; j < diagSize; ++j){
	Loop_Read:
	for(int num = j;num < diagSize; num++){
#pragma HLS pipeline
		tmpcol[num]=dataA[num][j];
	}
	Loop_row:
        for(int i = j; i < diagSize; ++i){
#pragma HLS pipeline
    Loop_vec_mul:
	    for(int k = 0; k <= j; k++){	  
#pragma HLS dependence variable=dataA inter false
#pragma HLS dependence variable=tmp inter false
#pragma HLS dependence variable=tmpcol inter false

		if(k==0){
		tmp[i%8] = tmpcol[i];
		}
		if (k < j)
             	   tmp[i%8] -= dataA[i][k]*dataA[j][k];   
	    if (k==j)
	   {
    	    if (i==j){
	    	   tmpjj = sqrt(tmp[i%8]);
	   	   tmpcol[i] = tmpjj;
 	    }
	    else if(i>j){
	    	   tmpcol[i] = tmp[i%8]/tmpjj;
	    }
	    }

	    }
	}
	Loop_Write:
	for (int num = j; num < diagSize; num++){
#pragma HLS pipeline
		dataA[num][j] = tmpcol[num];
	}       
    }
	
    for (int i = 0; i < diagSize; i++) {
        for (int j = 0; j < diagSize; j++) {
            matrixA[i * diagSize + j] = dataA[i][j];
							  
		}
    }										
}

