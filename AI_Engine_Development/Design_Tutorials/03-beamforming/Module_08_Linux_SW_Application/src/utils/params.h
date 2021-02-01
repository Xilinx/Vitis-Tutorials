//  © Copyright 2020 Xilinx, Inc.
//  Licensed under the Apache License, Version 2.0 (the 'License');
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//       http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an 'AS IS' BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef __PARAMS_H__
#define __PARAMS_H__

#define NUM_OF_STAMPS			3
#define NUM_DLBF_PORTS			4
#define NUM_ULBF_PORTS			8
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 DLBF DATA IPs
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define DLBF_NITER	             		3

#define NUM_DLBF_DATA                   1*NUM_OF_STAMPS

#define DLBF_DATA_CSR_OFFSET  			0x00080000/4
#define DLBF_DATA_RAM0_OFFSET	  		0x00000000/4
#define DLBF_DATA_RAM1_OFFSET	  		0x00100000/4
#define DLBF_DATA_RAM2_OFFSET	  		0x00200000/4
#define DLBF_DATA_RAM3_OFFSET	  		0x00300000/4

#define DLBF_DATA_INPUT_BLOCK_SIZE      768/2 		// 64b 
#define DLBF_DATA_INPUT_ROLLOVER_ADDR   1152        //1536     	// 384*4	

#define DLBF_DATA_REG_OFFSET_ID	        0x0/4
#define DLBF_DATA_REG_OFFSET_RESET      0x4/4
#define DLBF_DATA_REG_OFFSET_START      0x4/4
#define DLBF_DATA_REG_OFFSET_BLOCK_SIZE	0x8/4
#define DLBF_DATA_REG_OFFSET_NITER	    0xC/4
#define DLBF_DATA_REG_OFFSET_ROLLOVER	0x10/4
#define DLBF_DATA_REG_OFFSET_DONE	    0x20/4
#define DLBF_DATA_REG_OFFSET_RAM_STATUS	0x24/4


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 DLBF COEFFS IPs
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NUM_DLBF_COEFFS	                8*NUM_OF_STAMPS

#define DLBF_COEFFS_CSR_OFFSET          0x00080000/4
#define DLBF_COEFFS_RAM0_OFFSET	        0x00000000/4
#define DLBF_COEFFS_RAM1_OFFSET	        0x00100000/4
#define DLBF_COEFFS_RAM2_OFFSET	        0x00200000/4
#define DLBF_COEFFS_RAM3_OFFSET	        0x00300000/4

#define DLBF_COEFFS_INPUT_BLOCK_SIZE    512/2 		// 64b 
#define DLBF_COEFFS_ROLLOVER_ADDR       768         //1024

#define DLBF_COEFFS_REG_OFFSET_ID	    0x0/4
#define DLBF_COEFFS_REG_OFFSET_RESET    0x4/4
#define DLBF_COEFFS_REG_OFFSET_START    0x4/4
#define DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE	0x8/4
#define DLBF_COEFFS_REG_OFFSET_NITER    0xC/4
#define DLBF_COEFFS_REG_OFFSET_ROLLOVER	0x10/4
#define DLBF_COEFFS_REG_OFFSET_DONE	    0x20/4
#define DLBF_COEFFS_REG_OFFSET_RAM_STATUS	0x24/4


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 DLBF SLAVE IPs
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NUM_DLBF_SLAVES	                8*NUM_OF_STAMPS

#define DLBF_OUTPUT_BLOCK_SIZE 	        768
#define DLBF_OUTPUT_DATA_WIDTH          64
#define DLBF_OUTPUT_BLOCK               DLBF_OUTPUT_BLOCK_SIZE/(DLBF_OUTPUT_DATA_WIDTH/32) 

#define DLBF_SLAVE_CSR_OFFSET           0x00080000/4
#define DLBF_SLAVE_REG_OFFSET_ID        0x0/4
#define DLBF_SLAVE_REG_OFFSET_RESET	    0x4/4
#define DLBF_SLAVE_REG_OFFSET_NITER	    0xC/4
#define DLBF_SLAVE_REG_OFFSET_DONE	    0x20/4
#define DLBF_SLAVE_REG_OFFSET_RAM_STATUS 	0x24/4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  ULBF DATA IPs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ULBF_NITER                              4

#define NUM_ULBF_DATA                           1*NUM_OF_STAMPS

#define ULBF_DATA_CSR_OFFSET                    0x00080000/4
#define ULBF_DATA_RAM0_OFFSET                   0x00000000/4
#define ULBF_DATA_RAM1_OFFSET                   0x00100000/4
#define ULBF_DATA_RAM2_OFFSET                   0x00200000/4
#define ULBF_DATA_RAM3_OFFSET                   0x00300000/4
#define ULBF_DATA_RAM4_OFFSET                   0x00400000/4
#define ULBF_DATA_RAM5_OFFSET                   0x00500000/4
#define ULBF_DATA_RAM6_OFFSET                   0x00600000/4
#define ULBF_DATA_RAM7_OFFSET                   0x00700000/4

#define ULBF_DATA_INPUT_BLOCK_SIZE              768/2           // 64b 
#define ULBF_DATA_INPUT_ROLLOVER_ADDR           1536     	// 768/2 * 4        

#define ULBF_DATA_REG_OFFSET_ID                 0x0/4
#define ULBF_DATA_REG_OFFSET_RESET              0x4/4
#define ULBF_DATA_REG_OFFSET_START              0x4/4
#define ULBF_DATA_REG_OFFSET_BLOCK_SIZE         0x8/4
#define ULBF_DATA_REG_OFFSET_NITER              0xC/4
#define ULBF_DATA_REG_OFFSET_ROLLOVER           0x10/4
#define ULBF_DATA_REG_OFFSET_DONE               0x20/4
#define ULBF_DATA_REG_OFFSET_RAM_STATUS         0x24/4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  ULBF COEFFS IPs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NUM_ULBF_COEFFS                         4*NUM_OF_STAMPS

#define ULBF_COEFFS_CSR_OFFSET                  0x00080000/4
#define ULBF_COEFFS_RAM0_OFFSET                 0x00000000/4
#define ULBF_COEFFS_RAM1_OFFSET                 0x00100000/4
#define ULBF_COEFFS_RAM2_OFFSET                 0x00200000/4
#define ULBF_COEFFS_RAM3_OFFSET                 0x00300000/4
#define ULBF_COEFFS_RAM4_OFFSET                 0x00400000/4
#define ULBF_COEFFS_RAM5_OFFSET                 0x00500000/4
#define ULBF_COEFFS_RAM6_OFFSET                 0x00600000/4
#define ULBF_COEFFS_RAM7_OFFSET                 0x00700000/4

#define ULBF_COEFFS_INPUT_BLOCK_SIZE            512/2           // 64b 
#define ULBF_COEFFS_ROLLOVER_ADDR               1024

#define ULBF_COEFFS_REG_OFFSET_ID               0x0/4
#define ULBF_COEFFS_REG_OFFSET_RESET            0x4/4
#define ULBF_COEFFS_REG_OFFSET_START            0x4/4
#define ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE       0x8/4
#define ULBF_COEFFS_REG_OFFSET_NITER            0xC/4
#define ULBF_COEFFS_REG_OFFSET_ROLLOVER         0x10/4
#define ULBF_COEFFS_REG_OFFSET_DONE             0x20/4
#define ULBF_COEFFS_REG_OFFSET_RAM_STATUS       0x24/4

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  ULBF SLAVE IPs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NUM_ULBF_SLAVES                         4*NUM_OF_STAMPS

#define ULBF_OUTPUT_BLOCK_SIZE                  768
#define ULBF_OUTPUT_DATA_WIDTH                  64
#define ULBF_OUTPUT_BLOCK                       ULBF_OUTPUT_BLOCK_SIZE/(ULBF_OUTPUT_DATA_WIDTH/32) 

#define ULBF_SLAVE_CSR_OFFSET                   0x00080000/4
#define ULBF_SLAVE_REG_OFFSET_ID                0x0/4
#define ULBF_SLAVE_REG_OFFSET_RESET             0x4/4
#define ULBF_SLAVE_REG_OFFSET_NITER             0xC/4
#define ULBF_SLAVE_REG_OFFSET_DONE              0x20/4
#define ULBF_SLAVE_REG_OFFSET_RAM_STATUS        0x24/4

#define NUM_STREAMS NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES+NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES

#define MAX_TP_MEASUREMENT_SAMPLES         10

#endif //__PARAMS_H__
