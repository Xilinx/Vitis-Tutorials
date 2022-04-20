/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
****************************************************************/

#ifndef __C1_E_H__
  #define __C1_E_H__

  // SIMD matrix of coefficients for IIR filter stage 1 (even columns)
  alignas(16) const float C1_e[48] = {

    -2.73070750703956477423162e-01,	// Ky0_ym2
    -1.67428166649410931077213e-01,	// Ky1_ym2
    -2.80877791933900021470460e-02,	// Ky2_ym2
    +2.84982445221789899847398e-02,	// Ky3_ym2
    +2.51431106375899354743453e-02,	// Ky4_ym2
    +7.63398577110352676705629e-03,	// Ky5_ym2
    -2.18521409049264592194817e-03,	// Ky6_ym2
    -3.42444091955018898101870e-03,	// Ky7_ym2

    +1.63143503853171056849902e-01,	// Ky0_xm2
    +1.00028354118783982529273e-01,	// Ky1_xm2
    +1.67807745840625743838359e-02,	// Ky2_xm2
    -1.70260031622835976417019e-02,	// Ky3_xm2
    -1.50215105668031843194621e-02,	// Ky4_xm2
    -4.56085166152889014135052e-03,	// Ky5_xm2
    +1.30553522291659103091166e-03,	// Ky6_xm2
    +2.04589941952174969932998e-03,	// Ky7_xm2

    +1.63143503853171029094327e-01,	// Ky0_x0
    +4.26126741652342932997755e-01,	// Ky1_x0
    +3.79865337782927359189244e-01,	// Ky2_x0
    +1.16544498845832775191234e-01,	// Ky3_x0
    -3.22730575047724399517790e-02,	// Ky4_x0
    -5.16125086712465250271542e-02,	// Ky5_x0
    -2.28324055876729485603516e-02,	// Ky6_x0
    +9.46087947873411918816092e-05,	// Ky7_x0

    +0.00000000000000000000000e+00,	// Ky0_x2
    +0.00000000000000000000000e+00,	// Ky1_x2
    +1.63143503853171029094327e-01,	// Ky2_x2
    +4.26126741652342932997755e-01,	// Ky3_x2
    +3.79865337782927359189244e-01,	// Ky4_x2
    +1.16544498845832775191234e-01,	// Ky5_x2
    -3.22730575047724399517790e-02,	// Ky6_x2
    -5.16125086712465250271542e-02,	// Ky7_x2

    +0.00000000000000000000000e+00,	// Ky0_x4
    +0.00000000000000000000000e+00,	// Ky1_x4
    +0.00000000000000000000000e+00,	// Ky2_x4
    +0.00000000000000000000000e+00,	// Ky3_x4
    +1.63143503853171029094327e-01,	// Ky4_x4
    +4.26126741652342932997755e-01,	// Ky5_x4
    +3.79865337782927359189244e-01,	// Ky6_x4
    +1.16544498845832775191234e-01,	// Ky7_x4

    +0.00000000000000000000000e+00,	// Ky0_x6
    +0.00000000000000000000000e+00,	// Ky1_x6
    +0.00000000000000000000000e+00,	// Ky2_x6
    +0.00000000000000000000000e+00,	// Ky3_x6
    +0.00000000000000000000000e+00,	// Ky4_x6
    +0.00000000000000000000000e+00,	// Ky5_x6
    +1.63143503853171029094327e-01,	// Ky6_x6
    +4.26126741652342932997755e-01,	// Ky7_x6

  };

#endif // __C1_E_H__
