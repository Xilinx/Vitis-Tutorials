#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

import glob
from hsi import *
import vitis
import os
import subprocess
import re



def make_platform(xsa):
    extension = os.path.splitext(xsa)[1]
    if extension != ".xsa":
        print("Error: file type passed is not an XSA file")
        xsa = glob.glob("*.xsa")
        if len(xsa) == 0:
            print("Error: No XSA files found in [pwd]. Right click in Explorer view, and import -> files and select the XSA ")
            return ""
        if len(xsa) > 1:
            print("Error: More than 1 XSA files found in [pwd].")
            return ""
        xsa = xsa[0]
    print("Info: Using " + xsa + " file")

    client = vitis.create_client()
    pwd = os.getcwd()
    print("Info: Creating workspace at " + pwd + "/tmp")
    client.set_workspace(pwd + "/tmp")
    

    HwDesign = HwManager.open_hw_design(xsa)
    arch = HwDesign.FAMILY
    print("Arch detected in XSA is " + arch)
    if arch == "zynquplus":
        print("Info: Searching for processors....")
        a53_0 = HwDesign.get_cells(hierarchical='true',filter='IP_TYPE==PROCESSOR&&IP_NAME == "psu_cortexa53"')[0]
        pmu_0 = HwDesign.get_cells(hierarchical='true',filter='IP_TYPE==PROCESSOR&&IP_NAME == "psu_pmu"')[0]
        print("Info: Using " + a53_0.NAME + " for FSBL and " + pmu_0.NAME + " for PMUFW")
        
        print("Info: Creating Platform with build artifacts diasabled")
        platform = client.create_platform_component(name = "base_platform",hw_design = xsa,os = "standalone",cpu = a53_0.NAME,no_boot_bsp = True)
        print("Info: Creating FSBL Domain")
        fsbl_domain = platform.add_domain(cpu = a53_0.NAME,os = "standalone",name = "zynqmp_fsbl_domain",display_name = "zynqmp_fsbl_domain",support_app = "zynqmp_fsbl")
        print("Info: Creating PMUFW Domain")
        pmufw_domain = platform.add_domain(cpu = pmu_0.NAME,os = "standalone",name = "psu_pmu_domain",display_name = "psu_pmu_domain",support_app = "zynqmp_pmufw")
        print("Info: Building Platform")
        platform = client.get_component(name="base_platform")
        status = platform.build()
    
        print("Info: Creating FSBL Application")
        fsbl = client.create_app_component(name="zynqmp_fsbl",platform = "tmp/base_platform/export/base_platform/base_platform.xpfm",domain = "zynqmp_fsbl_domain",template = "zynqmp_fsbl")
        print("Info: Compiling FSBL Application")
        comp = client.get_component(name="zynqmp_fsbl")
        comp.build()
             
        print("Info: Creating PMUFW Application")
        pmufw = client.create_app_component(name="zynqmp_pmufw",platform = "tmp/base_platform/export/base_platform/base_platform.xpfm",domain = "psu_pmu_domain",template = "zynqmp_pmufw")
        print("Info: Compiling PMUFW Application")
        comp = client.get_component(name="zynqmp_pmufw")
        comp.build()
    if arch == "versal":
        print("Info: Nothing to do here, as the PDI contains the boot artifacts (PLM)")
    HwDesign.close()

def get_arch(xsa):
    HwDesign = HwManager.open_hw_design(xsa)
    arch = HwDesign.FAMILY
    #HwDesign.close()
    return(arch)

def get_board(xsa):
    HwDesign = HwManager.open_hw_design(xsa)
    board = HwDesign.HwDesign.BOARD
    #HwDesign.close()
    return (board)

def build_uboot(xsa):
    extension = os.path.splitext(xsa)[1]
    if extension != ".xsa":
        print("Error: file type passed is not an XSA file")
        xsa = glob.glob("*.xsa")
        if len(xsa) == 0:
            print("Error: No XSA files found in [pwd]. Right click in Explorer view, and import -> files and select the XSA ")
            return ""
        if len(xsa) > 1:
            print("Error: More than 1 XSA files found in [pwd].")
            return ""
        xsa = xsa[0]
    print("Info: Using " + xsa + " file. Searching for Arch....")
    arch = get_arch(xsa)
    print("Arch detected in XSA is " + arch)
    if arch == "zynquplus":
        print("Using xilinx_zynqmp_virt_defconfig")
        defconfig = "xilinx_zynqmp_virt_defconfig"
    if arch == "versal":
        print("Using xilinx_versal_virt_defconfig")
        defconfig = "xilinx_versal_virt_defconfig"
    result = subprocess.run(["cd u-boot-xlnx && make "+defconfig+" && make -f Makefile all -j 32"], shell=True, capture_output=True, text=True)
    print(result.stdout)

def build_atf(xsa):
    extension = os.path.splitext(xsa)[1]
    if extension != ".xsa":
        print("Error: file type passed is not an XSA file")
        xsa = glob.glob("*.xsa")
        if len(xsa) == 0:
            print("Error: No XSA files found in [pwd]. Right click in Explorer view, and import -> files and select the XSA ")
            return ""
        if len(xsa) > 1:
            print("Error: More than 1 XSA files found in [pwd].")
            return ""
        xsa = xsa[0]
    print("Info: Using " + xsa + " file. Searching for Arch....")
    arch = get_arch(xsa)
    print("Arch detected in XSA is " + arch)
    if arch == "zynquplus":
        print("Using PLAT zynqmp")
        plat = "zynqmp"
        extra = ""
    if arch == "versal":
        print("Using PLAT versal")
        plat = "versal"
        extra = " VERSAL_PLATFORM=silicon BUILD_BASE=build"
    print("Building ATF")
    result = subprocess.run(["cd arm-trusted-firmware && make -f Makefile DEBUG=0 PLAT="+ plat +" bl31"+ extra], shell=True, capture_output=True, text=True)
    print(result.stdout)


def read_target_connection():
    if not os.path.isfile(".peers.ini"):
        print("Info: No Target Connections found. Please run the Vitis -> Target Connections GUI")
        return("")
    file1 = open('.peers.ini', 'r')
    Lines = file1.readlines()
    ret_data = {}
        
    targets = ""
    for line in Lines:
        targets = targets+line
        
    # greedily match 2 or more new-lines
    blank_line_regex = r"(?:\r?\n){2,}"
        
    for target_connection in re.split(blank_line_regex, targets.strip()):
        target_info = target_connection.split('\n')
        target_info.reverse()
        for target_param in target_info:
            param_data = target_param.split('=')
            if param_data[0] == 'ID':
                target_id = param_data[1]
                ret_data[target_id] = dict.fromkeys(['Host','Port','PeerTypeId','DefaultTarget'])
            if param_data[0] == 'PeerTypeId':
                ret_data[target_id]['PeerTypeId'] = param_data[1]
            if param_data[0] == 'Host':
                ret_data[target_id]['Host'] = param_data[1]
            if param_data[0] == 'Port':
                ret_data[target_id]['Port'] = param_data[1]
            if param_data[0] == 'DefaultTarget':
                ret_data[target_id]['DefaultTarget'] = param_data[1]

    return(ret_data)


def generate_boot_script(xsa):
    extension = os.path.splitext(xsa)[1]
    if extension != ".xsa":
        print("Error: file type passed is not an XSA file")
        xsa = glob.glob("*.xsa")
        if len(xsa) == 0:
            print("Error: No XSA files found in [pwd]. Right click in Explorer view, and import -> files and select the XSA ")
            return ""
        if len(xsa) > 1:
            print("Error: More than 1 XSA files found in [pwd].")
            return ""
        xsa = xsa[0]

    print("Info: Using " + xsa + " file. Searching for Arch....")
    arch = get_arch(xsa)
    print("Arch detected in XSA is " + arch)
    f = open("boot_script.py", "w")
    print("Getting Hardware Server Target Connections")
    targets = read_target_connection()
    if targets != "":
        for d in targets.values():
            if d['PeerTypeId'] == 'HARDWARE_SERVER' and d['DefaultTarget'] == 'true':
                Host = d['Host']
                Port = d['Port']
                print("The script will connect to Host: " + Host + " at Port: " + Port)
    else:
        Host = "127.0.0.0"
        Port = "3121"
        print("Info: Defaulting to Host: " + Host + " at Port: " + Port)
    f.write("from xsdb import *\n")
    f.write("import time \n")
    f.write("session = start_debug_session()\n")
    f.write("session.connect(url=\"TCP:"+Host+":"+Port+"\")\n")
    if arch == "zynquplus":
        pmufw = glob.glob("tmp/zynqmp_pmufw/build/*.elf")
        if len(pmufw) != 0:
            f.write("\n# Add the Microblaze PMU to target\n")
            f.write("psu = session.targets(\"--set\", filter=\"name =~ *PSU*\")\n")
            f.write("psu.mwr(address=0xFFCA0038, words=0x1FF)\n")
            f.write("time.sleep(5)\n")

            f.write("\n# Download PMUFW to PMU\n")
            f.write("pmu = session.targets(\"--set\", filter=\"name =~ *MicroBlaze PMU*\")\n")
            f.write("pmu.dow(\""+ pmufw[0] + "\")\n")
            f.write("pmu.con()\n")
        else:
            print("Error: No PMUFW detected. Run the make platform")
        fsbl = glob.glob("tmp/zynqmp_fsbl/build/*.elf")
        if len(fsbl) != 0:
            f.write("\n# Download FSBL to A53 #0, block untill FSBL completes\n")
            f.write("a53 = session.targets(\"--set\", filter=\"name =~ *Cortex-A53 #0*\")\n")
            f.write("session.rst(type='cores')\n")
            f.write("a53.dow(\""+ fsbl[0] + "\")\n")
            # There is a bug in xsdb at the time of writing this where the symbol server wasnt working
            #f.write("a53.bpadd(addr=\"XFsbl_Exit\")\n")
            #f.write("a53.con(\"--block\", timeout = 60)\n")
            # I just added a WA here to sleep for 10 seconds
            f.write("a53.con()\n")
            f.write("time.sleep(10)\n")
        else:
            print("Error: No Fsbl detected. Run the make platform")
        dtb = glob.glob("system.dtb")
        if len(dtb) != 0:
            f.write("\n# Download DTB to A53 #0 at loacation 0x00100000\n")
            f.write("a53 = session.targets(\"--set\", filter=\"name =~ *Cortex-A53 #0*\")\n")
            f.write("a53.dow(\""+ dtb[0] + "\", \"-d\", addr=0x00100000)\n")
        else:
            print("Error: No DTB detected. Run the make create_dt and compile_dt")             
        f.write("\n# Download u-boot and atf to A53 #0\n")
        f.write("a53 = session.targets(\"--set\", filter=\"name =~ *Cortex-A53 #0*\")\n")
        uboot = glob.glob("u-boot-xlnx/*.elf")
        if len(uboot) != 0:
            f.write("a53.dow(\""+ uboot[0] + "\")\n")
        else:
            print("Error: No u-boot detected. Run the make uboot")
        atf = glob.glob("arm-trusted-firmware/build/zynqmp/release/bl31/*.elf")
        if len(atf) != 0:
            f.write("a53.dow(\""+ atf[0] + "\")\n")
        else:
            print("Error: No ATF detected. Run the make atf")
        f.write("a53.con()\n")
    if arch == "versal":
        pdi = glob.glob("*.pdi")
        if len(pdi) != 0:
            f.write("\n# Program the Versal device with the PDI\n")
            f.write("versal = session.targets(\"--set\", filter=\"name =~ *Versal*\")\n")
            f.write("versal.device_program(\""+ pdi[0] + "\")\n")
        else:
            print("Error: No PDI detected.")
        dtb = glob.glob("system.dtb")
        if len(dtb) != 0:
            f.write("\n# Download DTB to A72 #0 at location 0x1000\n")
            f.write("a72 = session.targets(\"--set\", filter=\"name =~ *Cortex-A72 #0*\")\n")
            f.write("session.rst(type='cores')\n")
            f.write("a72.dow(\""+ dtb[0] + "\", \"-d\", addr=0x1000)\n")
        else:
            print("Error: No DTB detected. Run the make create_dt and compile_dt") 
        uboot = glob.glob("u-boot-xlnx/*.elf")
        if len(uboot) != 0:
            f.write("\n# Download u-boot and atf to A72 #0\n")
            f.write("a72 = session.targets(\"--set\", filter=\"name =~ *Cortex-A72 #0*\")\n")
            f.write("a72.dow(\""+ uboot[0] + "\")\n")
        else:
            print("Error: No u-boot detected. Run the make uboot")
        atf = glob.glob("arm-trusted-firmware/build/versal/release/bl31/*.elf")
        if len(atf) != 0:
            f.write("a72.dow(\""+ atf[0] + "\")\n")
        else:
            print("Error: No ATF detected. Run the make atf")
        f.write("a72.con()\n")            
    f.close()

if sys.argv[1] == 'make_platform':
    print("Info: Creating Platform")
    make_platform(sys.argv[2])

if sys.argv[1] == 'generate_boot_script':
    print("Info: Generate boot script")
    generate_boot_script(sys.argv[2])

if sys.argv[1] == 'build_uboot':
    print("Info: Generate U-boot")
    build_uboot(sys.argv[2])

if sys.argv[1] == 'build_atf':
    print("Info: Generate Arm Trusted Firmware")
    build_atf(sys.argv[2]) 
