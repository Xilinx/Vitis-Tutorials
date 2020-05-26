wget -O vitis_ai_model_ZCU104_2019.2-r1.1.0.deb https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_model_ZCU104_2019.2-r1.1.0.deb
wget -O vitis-ai-runtime-1.1.2.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai-runtime-1.1.2.tar.gz
tar -xzvf vitis-ai-runtime-1.1.2.tar.gz
mv vitis-ai-runtime-1.1.2/unilog/aarch64/* .
mv vitis-ai-runtime-1.1.2/VART/aarch64/* .
mv vitis-ai-runtime-1.1.2/Vitis-AI-Library/aarch64/* .
mv vitis-ai-runtime-1.1.2/XIR/aarch64/* .
rm -r vitis-ai-runtime-1.1.*
wget -O vitis_ai_library_r1.1_video.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r1.1_video.tar.gz
wget -O vitis_ai_library_r1.1_images.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r1.1_images.tar.gz
