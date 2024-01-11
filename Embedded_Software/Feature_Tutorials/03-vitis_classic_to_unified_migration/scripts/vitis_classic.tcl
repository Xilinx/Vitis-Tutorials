# Create Workspace
setws ../classic_workspace

platform create -name zcu102_platform -hw zcu102

domain create -name a53_domain -os standalone -proc psu_cortexa53_0
domain active a53_domain
	
# Test some domain specific options
bsp setlib -name xilffs
bsp config -append extra_compiler_flags "-pg"

platform write
platform generate

# Create test applications
app create -name testapp_a53 -platform zcu102_platform -domain a53_domain -template "Hello World"
	
# Test some app compiler options
app config -name testapp_a53 define-compiler-symbols TEST_SYMBOL
app config -name testapp_a53 -set compiler-misc {-c -fmessage-length=0 -MT"$@"}