# © Copyright 2020 Xilinx, Inc.
# 
# Licensed under the Apache License, Version 2.0 (the 'License');
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an 'AS IS' BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

create_clock -period 2.222 -name m_axis_clk -waveform {0.000 1.111} [get_ports s_axis_clk]
create_clock -period 10.000 -name s_axi_aclk -waveform {0.000 5.000} [get_ports s_axi_aclk]
