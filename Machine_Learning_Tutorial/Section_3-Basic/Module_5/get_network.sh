

NETWORK_NAME=cf_refinedet_coco_480_360_0.96_1.1.zip

wget -O $NETWORK_NAME  https://www.xilinx.com/bin/public/openDownload?filename=$NETWORK_NAME

unzip $NETWORK_NAME 

rm $NETWORK_NAME

mv cf_refinedet_coco_480_360_0.96_5.08G/code cf_refinedet_coco_480_360_0.96_5.08G/code_bak
cp -r reference_scripts/code cf_refinedet_coco_480_360_0.96_5.08G/
