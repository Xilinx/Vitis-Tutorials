# To disable any model add -D<model> during meson build;
# <model> are SSD, REID, CLASSIFICATION, YOLOV3, FACEDETECT

```
meson -DSSD=1 -DREID=0 --libdir=/usr/local/lib/ivas/ build;
cd build;
ninja;
sudo ninja install;
```
# Test Setup
```
cd ../ivas_xdpuinfer/
Copy resnet model to /usr/share/vitis_ai_library/models/
./cmd_resnet50.sh
