/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
****************************************************************/

#ifndef __C1_O_H__
  #define __C1_O_H__

  // SIMD matrix of coefficients for IIR filter stage 1 (odd columns)
  alignas(16) const float C1_o[48] = {

    +6.13131088620049280457636e-01,	// Ky0_ym1
    +1.02858981128450252540141e-01,	// Ky1_ym1
    -1.04362127575775121002266e-01,	// Ky2_ym1
    -9.20754440846294625044877e-02,	// Ky3_ym1
    -2.79560727446043523480856e-02,	// Ky4_ym1
    +8.00237332214938194840936e-03,	// Ky5_ym1
    +1.25404896376570176158571e-02,	// Ky6_ym1
    +5.50374997287244888816149e-03,	// Ky7_ym1

    +3.26098387533558964346270e-01,	// Ky0_xm1
    +3.63084563198864784805409e-01,	// Ky1_xm1
    +1.33570502008116376302382e-01,	// Ky2_xm1
    -1.72515469379692608364874e-02,	// Ky3_xm1
    -4.70516570097176331510802e-02,	// Ky4_xm1
    -2.41379408105895400249441e-02,	// Ky5_xm1
    -1.95129062473440792468971e-03,	// Ky6_xm1
    +5.39496867263784991852749e-03,	// Ky7_xm1

    +0.00000000000000000000000e+00,	// Ky0_x1
    +1.63143503853171029094327e-01,	// Ky1_x1
    +4.26126741652342932997755e-01,	// Ky2_x1
    +3.79865337782927359189244e-01,	// Ky3_x1
    +1.16544498845832775191234e-01,	// Ky4_x1
    -3.22730575047724399517790e-02,	// Ky5_x1
    -5.16125086712465250271542e-02,	// Ky6_x1
    -2.28324055876729485603516e-02,	// Ky7_x1

    +0.00000000000000000000000e+00,	// Ky0_x3
    +0.00000000000000000000000e+00,	// Ky1_x3
    +0.00000000000000000000000e+00,	// Ky2_x3
    +1.63143503853171029094327e-01,	// Ky3_x3
    +4.26126741652342932997755e-01,	// Ky4_x3
    +3.79865337782927359189244e-01,	// Ky5_x3
    +1.16544498845832775191234e-01,	// Ky6_x3
    -3.22730575047724399517790e-02,	// Ky7_x3

    +0.00000000000000000000000e+00,	// Ky0_x5
    +0.00000000000000000000000e+00,	// Ky1_x5
    +0.00000000000000000000000e+00,	// Ky2_x5
    +0.00000000000000000000000e+00,	// Ky3_x5
    +0.00000000000000000000000e+00,	// Ky4_x5
    +1.63143503853171029094327e-01,	// Ky5_x5
    +4.26126741652342932997755e-01,	// Ky6_x5
    +3.79865337782927359189244e-01,	// Ky7_x5

    +0.00000000000000000000000e+00,	// Ky0_x7
    +0.00000000000000000000000e+00,	// Ky1_x7
    +0.00000000000000000000000e+00,	// Ky2_x7
    +0.00000000000000000000000e+00,	// Ky3_x7
    +0.00000000000000000000000e+00,	// Ky4_x7
    +0.00000000000000000000000e+00,	// Ky5_x7
    +0.00000000000000000000000e+00,	// Ky6_x7
    +1.63143503853171029094327e-01	// Ky7_x7

  };

#endif // __C1_O_H__
