/**********
© Copyright 2020-2022 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include "stdio.h"

int main(){
	//input data for float
	for(int i=0;i<16;i++){
		float tmp=i;
		printf("%d\n",*(int*)&tmp);
	}
	printf("\n\n");
	//input data for cint16
	for(int i=0;i<16;i++){
		int tmp=i*4;
		tmp=tmp<<16;
		tmp+=i*4;
		printf("%d\n",tmp);
	}
	printf("\n\n");
	//output data for float
	int out[16]={1077936128,1082130432,1084227584,1086324736,1088421888,1090519040,1091567616,1092616192,1093664768,1094713344,1095761920,1096810496,1097859072,1098907648,1099431936,1099956224};
	for(int i=0;i<16;i++){
		float tmp=*(float*)&out[i];
		printf("%f\n",tmp);
	}
}
