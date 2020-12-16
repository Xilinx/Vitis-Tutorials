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


#ifndef __TEST_BF_APP_H__
#define __TEST_BF_APP_H__


int test_dlbf(int iterFlag, int dataCheck);
int test_ulbf(int iterFlag, int dataCheck);

void enable_graphs(int niter, int debug);
void test_perf_all(int flag_all, int samples);
void stop_run();


#endif // __TEST_BF_APP_H_
