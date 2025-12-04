import vitis
import sys
import os

board = sys.argv[1]  # index vck190_custom
xsa = sys.argv[2]    # index for xsa path
#sdt = sys.argv[3]    # index for sdt path

print("board:", board)
print("xsa:", xsa)
#print("sdt:", sdt)
workspace = "./build/vitis_workspace"
sdt_repo = os.path.join(os.environ["XILINX_VITIS"], "data", "system-device-tree-xlnx")
client = vitis.create_client()
client.update_workspace(path=workspace)

advanced_options = client.create_advanced_options_dict(sdt_repo=sdt_repo)

platform = client.create_platform_component(
    name="platform",
    hw_design=xsa,
    os="standalone",
    cpu="psv_cortexa72_0",
    domain_name="standalone_cpu",
    advanced_options=advanced_options,
    compiler="gcc"
)


aie_domain = platform.add_domain(
    name="aiengine",
    os="aie_runtime",
    cpu="ai_engine"
)

aie_domain.set_qemu_args(qemu_option="PMC", path="./src/qemu/aie/pmc_args.txt")
aie_domain.set_qemu_args(qemu_option="PS", path="./src/qemu/aie/qemu_args.txt")
aie_domain.set_qemu_data(path="./src/boot")

# Create Linux domain
linux_domain = platform.add_domain(
    name="xrt",
    cpu="psv_cortexa72",
    os="linux",
    architecture="64-bit",
    #runtime="ocl",
    sd_dir="./build/image",
    #bootmode="sd"
)
#linux_domain.set_active()
linux_domain.set_qemu_data(path="./src/boot")
linux_domain.set_boot_dir(path="./build/vck190_linux/images/linux")
linux_domain.set_bif(path="./build/vck190_linux/images/linux/linux.bif")

platform.build()














