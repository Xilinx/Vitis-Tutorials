@Library('pipeline-library')_

VitisLibPipeline (branch: 'master', libname: 'Vitis-Tutorials', TARGETS: 'hls_csim:hls_csynth:hls_cosim:vitis_sw_emu:vitis_hw_emu:vitis_hw_build:vitis_aie_sim',
                  forcerdi:'true', devtest: 'RunDevTest.sh', 
                  additional_json:'add_tutorials.json',
                  NEW_GENERATE:"true",
                  disable_standardization: "true",
                  allowed_modes:"sw_emu,hw_emu,hw",
                  yml:'2022.1/tutorial.yml', tql:'2022.1/tutorial.tql',
                  email: 'randyh@xilinx.com', select_case: '/group/xcoswmktg/randyh/sprite-test-list.txt', TOOLVERSION: '2022.1_daily_latest')
