# Copyright 2020 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#import tensorflow as tf
import os
import cv2
from tensorflow.python.platform import gfile
import numpy as np
from PIL import Image
from tqdm import tqdm
import argparse
import json
from pycocotools.coco import COCO
from pycocotools.cocoeval import COCOeval
from ctypes import *
import math

display = 0
score_thresh = 0.25
nms_thresh = 0.45
bbox_output = 'bbox_out.json'
debug =1
images = []
class BOX(Structure):
    _fields_ = [("x", c_float),
                ("y", c_float),
                ("w", c_float),
                ("h", c_float)]


class DETECTION(Structure):
    _fields_ = [("bbox", BOX),
                ("classes", c_int),
                ("prob", POINTER(c_float)),
                ("mask", POINTER(c_float)),
                ("objectness", c_float),
                ("sort_class", c_int),
                ("uc", POINTER(c_float)),
                ("points", c_int),
                ("embeddings", POINTER(c_float)),
                ("embedding_size", c_int),
                ("sim", c_float),
                ("track_id", c_int)]

class DETNUMPAIR(Structure):
    _fields_ = [("num", c_int),
                ("dets", POINTER(DETECTION))]


class IMAGE(Structure):
    _fields_ = [("w", c_int),
                ("h", c_int),
                ("c", c_int),
                ("data", POINTER(c_float))]


class METADATA(Structure):
    _fields_ = [("classes", c_int),
                ("names", POINTER(c_char_p))]


def network_width(net):
    return lib.network_width(net)


def network_height(net):
    return lib.network_height(net)


def bbox2points(bbox):
    """
    From bounding box yolo format
    to corner points cv2 rectangle
    """
    x, y, w, h = bbox
    xmin = int(round(x - (w / 2)))
    xmax = int(round(x + (w / 2)))
    ymin = int(round(y - (h / 2)))
    ymax = int(round(y + (h / 2)))
    return xmin, ymin, xmax, ymax


def class_colors(names):
    """
    Create a dict with one random BGR color for each
    class name
    """
    return {name: (
        random.randint(0, 255),
        random.randint(0, 255),
        random.randint(0, 255)) for name in names}


def load_network(config_file, data_file, weights, batch_size=1):
    """
    load model description and weights from config files
    args:
        config_file (str): path to .cfg model file
        data_file (str): path to .data model file
        weights (str): path to weights
    returns:
        network: trained model
        class_names
        class_colors
    """
    network = load_net_custom(
        config_file.encode("ascii"),
        weights.encode("ascii"), 0, batch_size)
    metadata = load_meta(data_file.encode("ascii"))
    class_names = [metadata.names[i].decode("ascii") for i in range(metadata.classes)]
    colors = class_colors(class_names)
    return network, class_names, colors


def print_detections(detections, coordinates=False):
    print("\nObjects:")
    for label, confidence, bbox in detections:
        x, y, w, h = bbox
        if coordinates:
            print("{}: {}%    (left_x: {:.0f}   top_y:  {:.0f}   width:   {:.0f}   height:  {:.0f})".format(label, confidence, x, y, w, h))
        else:
            print("{}: {}%".format(label, confidence))


def draw_boxes(detections, image, colors):
    import cv2
    for label, confidence, bbox in detections:
        left, top, right, bottom = bbox2points(bbox)
        cv2.rectangle(image, (left, top), (right, bottom), colors[label], 1)
        cv2.putText(image, "{} [{:.2f}]".format(label, float(confidence)),
                    (left, top - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5,
                    colors[label], 2)
    return image


def decode_detection(detections):
    decoded = []
    for label, confidence, bbox in detections:
        confidence = str(round(confidence * 100, 2))
        decoded.append((str(label), confidence, bbox))
    return decoded


def remove_negatives(detections, class_names, num):
    """
    Remove all classes with 0% confidence within the detection
    """
    predictions = []
    for j in range(num):
        for idx, name in enumerate(class_names):
            if detections[j].prob[idx] > 0:
                bbox = detections[j].bbox
                bbox = (bbox.x, bbox.y, bbox.w, bbox.h)
                predictions.append((name, detections[j].prob[idx], (bbox)))
    return predictions


def detect_image(network, class_names, image, thresh=.5, hier_thresh=.5, nms=.45):
    """
        Returns a list with highest confidence class and their bbox
    """
    pnum = pointer(c_int(0))
    predict_image(network, image)
    detections = get_network_boxes(network, image.w, image.h,
                                   thresh, hier_thresh, None, 0, pnum, 0)
    num = pnum[0]
    if nms:
        do_nms_sort(detections, num, len(class_names), nms)
    predictions = remove_negatives(detections, class_names, num)
    predictions = decode_detection(predictions)
    free_detections(detections, num)
    return sorted(predictions, key=lambda x: x[1])

hasGPU = True
if os.name == "nt":
    cwd = os.path.dirname(__file__)
    os.environ['PATH'] = cwd + ';' + os.environ['PATH']
    winGPUdll = os.path.join(cwd, "yolo_cpp_dll.dll")
    winNoGPUdll = os.path.join(cwd, "yolo_cpp_dll_nogpu.dll")
    envKeys = list()
    for k, v in os.environ.items():
        envKeys.append(k)
    try:
        try:
            tmp = os.environ["FORCE_CPU"].lower()
            if tmp in ["1", "true", "yes", "on"]:
                raise ValueError("ForceCPU")
            else:
                print("Flag value {} not forcing CPU mode".format(tmp))
        except KeyError:
            # We never set the flag
            if 'CUDA_VISIBLE_DEVICES' in envKeys:
                if int(os.environ['CUDA_VISIBLE_DEVICES']) < 0:
                    raise ValueError("ForceCPU")
            try:
                global DARKNET_FORCE_CPU
                if DARKNET_FORCE_CPU:
                    raise ValueError("ForceCPU")
            except NameError as cpu_error:
                print(cpu_error)
        if not os.path.exists(winGPUdll):
            raise ValueError("NoDLL")
        lib = CDLL(winGPUdll, RTLD_GLOBAL)
    except (KeyError, ValueError):
        hasGPU = False
        if os.path.exists(winNoGPUdll):
            lib = CDLL(winNoGPUdll, RTLD_GLOBAL)
            print("Notice: CPU-only mode")
        else:
            # Try the other way, in case no_gpu was compile but not renamed
            lib = CDLL(winGPUdll, RTLD_GLOBAL)
            print("Environment variables indicated a CPU run, but we didn't find {}. Trying a GPU run anyway.".format(winNoGPUdll))
else:
    lib = CDLL("../darknet/libdarknet.so", RTLD_GLOBAL)
lib.network_width.argtypes = [c_void_p]
lib.network_width.restype = c_int
lib.network_height.argtypes = [c_void_p]
lib.network_height.restype = c_int

copy_image_from_bytes = lib.copy_image_from_bytes
copy_image_from_bytes.argtypes = [IMAGE,c_char_p]

predict = lib.network_predict_ptr
predict.argtypes = [c_void_p, POINTER(c_float)]
predict.restype = POINTER(c_float)

if hasGPU:
    set_gpu = lib.cuda_set_device
    set_gpu.argtypes = [c_int]

init_cpu = lib.init_cpu

make_image = lib.make_image
make_image.argtypes = [c_int, c_int, c_int]
make_image.restype = IMAGE

get_network_boxes = lib.get_network_boxes
get_network_boxes.argtypes = [c_void_p, c_int, c_int, c_float, c_float, POINTER(c_int), c_int, POINTER(c_int), c_int]
get_network_boxes.restype = POINTER(DETECTION)

make_network_boxes = lib.make_network_boxes
make_network_boxes.argtypes = [c_void_p]
make_network_boxes.restype = POINTER(DETECTION)

free_detections = lib.free_detections
free_detections.argtypes = [POINTER(DETECTION), c_int]

free_batch_detections = lib.free_batch_detections
free_batch_detections.argtypes = [POINTER(DETNUMPAIR), c_int]

free_ptrs = lib.free_ptrs
free_ptrs.argtypes = [POINTER(c_void_p), c_int]

network_predict = lib.network_predict_ptr
network_predict.argtypes = [c_void_p, POINTER(c_float)]

reset_rnn = lib.reset_rnn
reset_rnn.argtypes = [c_void_p]

load_net = lib.load_network
load_net.argtypes = [c_char_p, c_char_p, c_int]
load_net.restype = c_void_p

load_net_custom = lib.load_network_custom
load_net_custom.argtypes = [c_char_p, c_char_p, c_int, c_int]
load_net_custom.restype = c_void_p

do_nms_obj = lib.do_nms_obj
do_nms_obj.argtypes = [POINTER(DETECTION), c_int, c_int, c_float]

do_nms_sort = lib.do_nms_sort
do_nms_sort.argtypes = [POINTER(DETECTION), c_int, c_int, c_float]

free_image = lib.free_image
free_image.argtypes = [IMAGE]

letterbox_image = lib.letterbox_image
letterbox_image.argtypes = [IMAGE, c_int, c_int]
letterbox_image.restype = IMAGE

load_meta = lib.get_metadata
lib.get_metadata.argtypes = [c_char_p]
lib.get_metadata.restype = METADATA

load_image = lib.load_image_color
load_image.argtypes = [c_char_p, c_int, c_int]
load_image.restype = IMAGE

rgbgr_image = lib.rgbgr_image
rgbgr_image.argtypes = [IMAGE]

predict_image = lib.network_predict_image
predict_image.argtypes = [c_void_p, IMAGE]
predict_image.restype = POINTER(c_float)

predict_image_letterbox = lib.network_predict_image_letterbox
predict_image_letterbox.argtypes = [c_void_p, IMAGE]
predict_image_letterbox.restype = POINTER(c_float)

network_predict_batch = lib.network_predict_batch
network_predict_batch.argtypes = [c_void_p, IMAGE, c_int, c_int, c_int,
                                   c_float, c_float, POINTER(c_int), c_int, c_int]
network_predict_batch.restype = POINTER(DETNUMPAIR)


def array_to_image(arr):
    import numpy as np
    # need to return old values to avoid python freeing memory
    arr = arr.transpose(2,0,1)
    c = arr.shape[0]
    h = arr.shape[1]
    w = arr.shape[2]
    arr = np.ascontiguousarray(arr.flat, dtype=np.float32) / 255.0
    data = arr.ctypes.data_as(POINTER(c_float))
    im = IMAGE(w,h,c,data)
    return im, arr


def convert_coco_category(category_id):
    '''
    convert continuous coco class id (0~79) to discontinuous coco category id
    '''
    if category_id >= 0 and category_id <= 10:
        category_id = category_id + 1
    elif category_id >= 11 and category_id <= 23:
        category_id = category_id + 2
    elif category_id >= 24 and category_id <= 25:
        category_id = category_id + 3
    elif category_id >= 26 and category_id <= 39:
        category_id = category_id + 5
    elif category_id >= 40 and category_id <= 59:
        category_id = category_id + 6
    elif category_id == 60:
        category_id = category_id + 7
    elif category_id == 61:
        category_id = category_id + 9
    elif category_id >= 62 and category_id <= 72:
        category_id = category_id + 10
    elif category_id >= 73 and category_id <= 79:
        category_id = category_id + 11
    else:
        raise ValueError('Invalid category id')
    return category_id


def get_class(classes_path):
    with open(classes_path) as f:
        class_names = f.readlines()
    class_names = [c.strip() for c in class_names]
    return class_names


def get_anchors(anchors_path):
    with open(anchors_path) as f:
        anchors = f.readline()
    anchors = [float(x) for x in anchors.split(',')]
    return np.array(anchors).reshape(-1, 2)


def detect_image(net, imagepath, thresh=.5, hier_thresh=.5, nms=.45, debug= False):
    image_in = cv2.imread(imagepath)
    pred_height, pred_width, c = image_in.shape
    net_width, net_height = (network_width(net), network_height(net)) 
    custom_image = cv2.cvtColor(image_in, cv2.COLOR_BGR2RGB)
    custom_image = cv2.resize(custom_image, (net_width, net_height), interpolation=cv2.INTER_NEAREST)
    im, arr = array_to_image(custom_image)	
    num = c_int(0)
    if debug: print("Assigned num")
    pnum = pointer(num)
    if debug: print("Assigned pnum")
    predict_image(net, im)
    letter_box = 0
    #predict_image_letterbox(net, im)
    #letter_box = 1
    if debug: print("did prediction")
    #dets = get_network_boxes(net, custom_image_bgr.shape[1], custom_image_bgr.shape[0], thresh, hier_thresh, None, 0, pnum, letter_box) # OpenCV
    dets = get_network_boxes(net, 1, 1, 0.005, hier_thresh, None, 0, pnum, letter_box)
    if debug: print("Got dets")
    num = pnum[0]
    if debug: print("got zeroth index of pnum")
    if nms:
        do_nms_sort(dets, num, 80, nms)
    if debug: print("did sort")
    res = []
    if debug: print("about to range")
    items = []
    for i in range(num):
        det = dets[i]
        score = -1
        label = None
        for c in range(det.classes):
            p = det.prob[c]
            if p > score:
                score = p
                label = c
        if score > thresh:
            box = det.bbox
            left, top, right, bottom = map(int,((box.x - box.w / 2)*pred_width, (box.y - box.h / 2)*pred_height, (box.x + box.w / 2)*pred_width, (box.y + box.h / 2)*pred_height))
            box = [left, top, right, bottom]
            item  = [box, score, label]
            items.append(item)
    if debug: print("did sort")
    free_detections(dets, num)
    if debug: print("freed detections")
    return items

def singledetect(imagepath, net, thresh= 0.25, hier_thresh=.5, nms=.45, batch_size=1):
    import cv2
    import numpy as np
    # NB! Image sizes should be the same
    # You can change the images, yet, be sure that they have the same width and height
    #img_samples = ['data/person.jpg', 'data/person.jpg', 'data/person.jpg']
    #image_list = [cv2.imread(k) for k in img_samples]
    image_in = cv2.imread(imagepath)
    #meta = load_meta(metaPath.encode('utf-8'))
    pred_height, pred_width, c = image_in.shape
    net_width, net_height = (network_width(net), network_height(net))
    img_list = []
    #for custom_image_bgr in image_list:
    custom_image = cv2.cvtColor(image_in, cv2.COLOR_BGR2RGB)
    custom_image = cv2.resize(
        custom_image, (net_width, net_height), interpolation=cv2.INTER_NEAREST)   #items = singledetect(img_path, net, thresh=score_thresh, hier_thresh=0, nms=nms_thresh, batch_size=1)

    arr = np.concatenate(img_list, axis=0)
    arr = np.ascontiguousarray(arr.flat, dtype=np.float32) / 255.0
    data = arr.ctypes.data_as(POINTER(c_float))
    im = IMAGE(net_width, net_height, c, data)

    batch_dets = network_predict_batch(net, im, batch_size, pred_width,
                                                pred_height, .005, hier_thresh, None, 0, 0)
    batch_boxes = []
    batch_scores = []
    batch_classes = []
    for b in range(batch_size):
        num = batch_dets[b].num
        dets = batch_dets[b].dets
        if nms:
            do_nms_obj(dets, num, 80, nms)
        items = []
        for i in range(num):
            det = dets[i]
            score = -1
            label = None
            for c in range(det.classes):
                p = det.prob[c]
                if p > score:
                    score = p
                    label = c
            if score > thresh:
                box = det.bbox
                left, top, right, bottom = map(int,(box.x - box.w / 2, box.y - box.h / 2,
                                            box.x + box.w / 2, box.y + box.h / 2))
                #boxes.append((left, top, right, bottom))
                #scores.append(score)
                #classes.append(label)
                box = [left, top, right, bottom]
                item  = [box, score, label]
                items.append(item)
                #boxColor = (int(255 * (1 - (score ** 2))), int(255 * (score ** 2)), 0)
                #cv2.rectangle(image_list[b], (left, top),
                #          (right, bottom), boxColor, 2)
        #cv2.imwrite(os.path.basename(img_samples[b]),image_list[b])
    free_batch_detections(batch_dets, batch_size)
    #return batch_boxes, batch_scores, batch_classes   
    return items


def generate_bboxes(bbox_out, eval_ops, metadata, class_names, truncate=True):
    single_bboxes_id = []
    image = images.pop()
    image_id = metadata['id']
    image_h = metadata['height']
    image_w = metadata['width']

    try:
        for box, score, class_id in eval_ops:
            left, top, right, bottom = box
            bbox = [left,top,(right - left),(bottom - top)]
            category_id = convert_coco_category(class_id)
            if (truncate):
                bbox = [np.round(b * 100) / 100 for b in bbox]
                score = np.round(score * 10000) / 10000
            bbox = [b.item() for b in bbox]
            score = score.item()
            bbox_out.append({'image_id': int(image_id), 'category_id': int(category_id), 'bbox': bbox, 'score': score})
            #for showing images set display=1
            if display ==1:
                image = cv2.rectangle(image,(left,top),(right,bottom),(0,255,0),3)
                image = cv2.putText(image,str(category_id) + " [" + str(round(score * 100, 2)) + "]",(left+5, top + 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5,[255, 255, 0], 2)
            if len(box) == 1:
                # doubling with bg class of 0 and score of 0
                single_bboxes_id.append(image_id)
                bbox_out.append({'image_id': int(image_id), 'category_id': int(0), 'bbox': bbox, 'score': 0})
    except:
        raise ValueError("Error on boxes: {}".format(eval_ops))


     #for showing images set display=1
    if display ==1:
        cv2.imshow(str(image_id), image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    if len(single_bboxes_id) > 0:
        print("WARNING: Only one bounding box found for {} images. We will append an additional"
        "dummy box with score 0 and category id 0.".format(len(single_bboxes_id)))


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('--data_dir', type=str, default='./val2017/', required=False, help='path to the COCO validation images')
    parser.add_argument('--annotations', type=str, default='./instances_val2017.json',
                    required=False, help='path to the validation annotations')
    FLAGS = parser.parse_args()

    classes_path = "../scripts/coco_classes.txt"
    anchors_path = "../scripts/yolo4_anchors.txt"

    class_names = get_class(classes_path)

    bboxes = []
    cnt = 0
    configPath = "../dk_model/yolov4-leaky.cfg"
    weightPath = "../dk_model/yolov4-leaky_best.weights"

    net = load_net_custom(configPath.encode('utf-8'), weightPath.encode('utf-8'), 0, 1)
    with open(FLAGS.annotations) as json_file:
        data = json.load(json_file)
        for i in data['images']:
            cnt=cnt+1
            print("processing: ", cnt, "of 5000 images")
            img_file_name = i['file_name']
            metadata = {'id': i['id'], 'height': i['height'], 'width': i['width']}
            images.append(cv2.imread((FLAGS.data_dir + img_file_name),cv2.IMREAD_COLOR))
            img_path=FLAGS.data_dir + img_file_name
            items = detect_image(net, img_path, thresh=score_thresh, hier_thresh=0, nms=.45, debug=False)
            #items = singledetect(img_path, net, thresh=score_thresh, hier_thresh=0, nms=nms_thresh, batch_size=1)
            generate_bboxes(bboxes, items, metadata, class_names)
            del(metadata)
        with open(bbox_output, 'w') as json_file:
            json.dump(bboxes, json_file)
        
    print("Generating COCO mAP score using ground truth values in {}".format(FLAGS.annotations))
    cocoGt = COCO(FLAGS.annotations)
    cocoDt = cocoGt.loadRes(bbox_output)
    cocoEval = COCOeval(cocoGt, cocoDt, 'bbox')  # running bbox evaluation
    cocoEval.evaluate()
    cocoEval.accumulate()
    cocoEval.summarize()
    print(cocoEval.stats)
    print("done")