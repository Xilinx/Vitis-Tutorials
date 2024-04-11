# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

exec rm -rf linux_debug_ws 
exec mkdir linux_debug_ws
setws linux_debug_ws
platform create -name "zcu104_test" -hw zcu104.xsa -proc psu_cortexa53 -os linux
platform generate
app create -name linux_test_app -sysproj linux_test_app_system -platform zcu104_test -domain linux_domain -template "Linux Empty Application"
importsources -name linux_test_app -path src
