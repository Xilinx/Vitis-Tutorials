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

import tensorflow as tf
import os
import cv2
import random
import colorsys
from tensorflow.python.platform import gfile
import colorsys
import numpy as np
from PIL import Image
from tqdm import tqdm
import argparse
import json
from pycocotools.coco import COCO
from pycocotools.cocoeval import COCOeval

#configuration settings for evaluation:
quantized=0 #set to a 1 to evaluate the quantized model
input_height = 512
input_width = 512
bbox_output = 'bbox_out.json'
display = 0 #set to a 1 to display images as they are processed
classes_path = "./coco_classes.txt" 
anchors_path = "./yolo4_anchors.txt"
score_thresh = 0.25
nms_thresh = 0.45

# set keras_model to 0 for darknet -> tf conversion, 
# set keras_model to a 1 for a darknet -> keras -> tf conversion, 
# set keras_model to 2 for keras model using qqwweee repo for convert, 
# set keras_model to 3 for keras model with no SPP module

keras_model = 1 

if quantized==1:
    import tensorflow.contrib.decent_q

images = []

class yolo_predictor:
    def __init__(self, obj_threshold, nms_threshold, classes_file, anchors_file):
        """
        Introduction
        ------------
            初始化函数
        Parameters
        ----------
            obj_threshold: 目标检测为物体的阈值
            nms_threshold: nms阈值
        """
        self.obj_threshold = obj_threshold
        self.nms_threshold = nms_threshold
        self.classes_path = classes_file
        self.anchors_path = anchors_file
        self.class_names = self._get_class()
        self.anchors = self._get_anchors()
        hsv_tuples = [(x / len(self.class_names), 1., 1.)for x in range(len(self.class_names))]
        self.colors = list(map(lambda x: colorsys.hsv_to_rgb(*x), hsv_tuples))
        self.colors = list(map(lambda x: (int(x[0] * 255), int(x[1] * 255), int(x[2] * 255)), self.colors))
        random.seed(10101)
        random.shuffle(self.colors)
        random.seed(None)


    def _get_class(self):
        """
        Introduction
        ------------
            读取类别名称
        """
        classes_path = os.path.expanduser(self.classes_path)
        with open(classes_path) as f:
            class_names = f.readlines()
        class_names = [c.strip() for c in class_names]
        return class_names

    def _get_anchors(self):
        """
        Introduction
        ------------
            读取anchors数据
        """
        anchors_path = os.path.expanduser(self.anchors_path)
        with open(anchors_path) as f:
            anchors = f.readline()
            anchors = [float(x) for x in anchors.split(',')]
            anchors = np.array(anchors).reshape(-1, 2)
        return anchors

    def _iou(box1, box2):
        b1_x0, b1_y0, b1_x1, b1_y1 = box1
        b2_x0, b2_y0, b2_x1, b2_y1 = box2

        int_x0 = max(b1_x0, b2_x0)
        int_y0 = max(b1_y0, b2_y0)
        int_x1 = min(b1_x1, b2_x1)
        int_y1 = min(b1_y1, b2_y1)

        int_area = (int_x1 - int_x0) * (int_y1 - int_y0)

        b1_area = (b1_x1 - b1_x0) * (b1_y1 - b1_y0)
        b2_area = (b2_x1 - b2_x0) * (b2_y1 - b2_y0)

        iou = int_area / (b1_area + b2_area - int_area + 1e-05)
        return iou

    def non_max_suppression(predictions_with_boxes, confidence_threshold, iou_threshold=0.4):
        """
        Applies Non-max suppression to prediction boxes.

        :param predictions_with_boxes: 3D numpy array, first 4 values in 3rd dimension are bbox attrs, 5th is confidence
        :param confidence_threshold: the threshold for deciding if prediction is valid
        :param iou_threshold: the threshold for deciding if two boxes overlap
        :return: dict: class -> [(box, score)]
        """
        conf_mask = np.expand_dims((predictions_with_boxes[:, :, 4] > confidence_threshold), -1)
        predictions = predictions_with_boxes * conf_mask

        result = {}
        items = []
        for i, image_pred in enumerate(predictions):
            shape = image_pred.shape
            non_zero_idxs = np.nonzero(image_pred)
            image_pred = image_pred[non_zero_idxs]
            image_pred = image_pred.reshape(-1, shape[-1])

            bbox_attrs = image_pred[:, :5]
            classes = image_pred[:, 5:]
            classes = np.argmax(classes, axis=-1)

            unique_classes = list(set(classes.reshape(-1)))

            for cls in unique_classes:
                cls_mask = classes == cls
                cls_boxes = bbox_attrs[np.nonzero(cls_mask)]
                cls_boxes = cls_boxes[cls_boxes[:, -1].argsort()[::-1]]
                cls_scores = cls_boxes[:, -1]
                cls_boxes = cls_boxes[:, :-1]

                while len(cls_boxes) > 0:
                    box = cls_boxes[0]
                    score = cls_scores[0]
                    if not cls in result:
                        result[cls] = []
                    result[cls].append((box, score))
                    item  = [box, score, cls]
                    cls_boxes = cls_boxes[1:]
                    ious = np.array([_iou(box, x) for x in cls_boxes])
                    iou_mask = ious < iou_threshold
                    cls_boxes = cls_boxes[np.nonzero(iou_mask)]
                    cls_scores = cls_scores[np.nonzero(iou_mask)]
                    item.append(item)
        return items

    def eval(self, yolo_outputs, image_shape, keras_model, max_boxes = 1000):
        """
        Introduction
        ------------
            根据Yolo模型的输出进行非极大值抑制，获取最后的物体检测框和物体检测类别
        Parameters
        ----------
            yolo_outputs: yolo模型输出
            image_shape: 图片的大小
            max_boxes:  最大box数量
        Returns
        -------
            boxes_: 物体框的位置
            scores_: 物体类别的概率
            classes_: 物体类别
        """
        anchor_mask = [[6, 7, 8], [3, 4, 5], [0, 1, 2]]
        boxes = []
        box_scores = []
        input_shape = tf.shape(yolo_outputs[0])[1 : 3] * 32
        # 对三个尺度的输出获取每个预测box坐标和box的分数，score计算为置信度x类别概率
        for i in range(len(yolo_outputs)):
            _boxes, _box_scores = self.boxes_and_scores(yolo_outputs[i], self.anchors[anchor_mask[i]], len(self.class_names), input_shape, image_shape)
            boxes.append(_boxes)
            box_scores.append(_box_scores)
        boxes = tf.concat(boxes, axis = 0)
        box_scores = tf.concat(box_scores, axis = 0)

        mask = box_scores >= self.obj_threshold
        max_boxes_tensor = tf.constant(max_boxes, dtype = tf.int32)
        boxes_ = []
        scores_ = []
        classes_ = []
        items_ = []
        for c in range(len(self.class_names)):
            class_boxes = tf.boolean_mask(boxes, mask[:, c])
            class_box_scores = tf.boolean_mask(box_scores[:, c], mask[:, c])
            item = [class_boxes, class_box_scores]
            #filtered_boxes = non_max_suppression(output_y,
            #                            confidence_threshold=0.25,
            #                            iou_threshold=0.45)
    
            nms_index = tf.image.non_max_suppression(class_boxes, class_box_scores, max_boxes_tensor, iou_threshold = self.nms_threshold)
            class_boxes = tf.gather(class_boxes, nms_index)
            class_box_scores = tf.gather(class_box_scores, nms_index)
            classes = tf.ones_like(class_box_scores, 'int32') * c
            boxes_.append(class_boxes)
            scores_.append(class_box_scores)
            classes_.append(classes)
        boxes_ = tf.concat(boxes_, axis = 0)
        scores_ = tf.concat(scores_, axis = 0)
        classes_ = tf.concat(classes_, axis = 0)
        return boxes_, scores_, classes_


    def boxes_and_scores(self, feats, anchors, classes_num, input_shape, image_shape):
        """
        Introduction
        ------------
            将预测出的box坐标转换为对应原图的坐标，然后计算每个box的分数
        Parameters
        ----------
            feats: yolo输出的feature map
            anchors: anchor的位置
            class_num: 类别数目
            input_shape: 输入大小
            image_shape: 图片大小
        Returns
        -------
            boxes: 物体框的位置
            boxes_scores: 物体框的分数，为置信度和类别概率的乘积
        """
        box_xy, box_wh, box_confidence, box_class_probs = self._get_feats(feats, anchors, classes_num, input_shape)
        boxes = self.correct_boxes(box_xy, box_wh, input_shape, image_shape)
        boxes = tf.reshape(boxes, [-1, 4])
        box_scores = box_confidence * box_class_probs
        box_scores = tf.reshape(box_scores, [-1, classes_num])
        return boxes, box_scores


    def correct_boxes(self, box_xy, box_wh, input_shape, image_shape):
        """
        Introduction
        ------------
            计算物体框预测坐标在原图中的位置坐标
        Parameters
        ----------
            box_xy: 物体框左上角坐标
            box_wh: 物体框的宽高
            input_shape: 输入的大小
            image_shape: 图片的大小
        Returns
        -------
            boxes: 物体框的位置
        """
        box_yx = box_xy[..., ::-1]
        box_hw = box_wh[..., ::-1]
        input_shape = tf.cast(input_shape, dtype = tf.float32)
        image_shape = tf.cast(image_shape, dtype = tf.float32)
        new_shape = tf.round(image_shape * tf.reduce_min(input_shape / image_shape))
        offset = (input_shape - new_shape) / 2. / input_shape
        scale = input_shape / new_shape
        #box_yx = (box_yx - offset) * scale
        #box_hw = box_hw*scale

        box_mins = box_yx - (box_hw / 2.)
        box_maxes = box_yx + (box_hw / 2.)
        boxes = tf.concat([
            box_mins[..., 0:1],
            box_mins[..., 1:2],
            box_maxes[..., 0:1],
            box_maxes[..., 1:2]
        ], axis = -1)
        boxes *= tf.concat([image_shape, image_shape], axis = -1)
        return boxes



    def _get_feats(self, feats, anchors, num_classes, input_shape):
        """
        Introduction
        ------------
            根据yolo最后一层的输出确定bounding box
        Parameters
        ----------
            feats: yolo模型最后一层输出
            anchors: anchors的位置
            num_classes: 类别数量
            input_shape: 输入大小
        Returns
        -------
            box_xy, box_wh, box_confidence, box_class_probs
        """
        num_anchors = len(anchors)
        anchors_tensor = tf.reshape(tf.constant(anchors, dtype=tf.float32), [1, 1, 1, num_anchors, 2])
        grid_size = tf.shape(feats)[1:3]
        predictions = tf.reshape(feats, [-1, grid_size[0], grid_size[1], num_anchors, num_classes + 5])
        # 这里构建13*13*1*2的矩阵，对应每个格子加上对应的坐标
        grid_y = tf.tile(tf.reshape(tf.range(grid_size[0]), [-1, 1, 1, 1]), [1, grid_size[1], 1, 1])
        grid_x = tf.tile(tf.reshape(tf.range(grid_size[1]), [1, -1, 1, 1]), [grid_size[0], 1, 1, 1])
        grid = tf.concat([grid_x, grid_y], axis = -1)
        grid = tf.cast(grid, tf.float32)
        # 将x,y坐标归一化为占416的比例
        #box_xy = (tf.sigmoid(predictions[..., :2]) + grid) / tf.cast(grid_size[::-1], tf.float32)
        box_xy = (tf.sigmoid(predictions[..., :2]) + grid) / tf.cast(grid_size[...,::-1], tf.float32)
        # 将w,h也归一化为占416的比例
        #box_wh = tf.exp(predictions[..., 2:4]) * anchors_tensor / tf.cast(input_shape[::-1], tf.float32)
        box_wh = tf.exp(predictions[..., 2:4]) * anchors_tensor / tf.cast(input_shape[...,::-1], tf.float32)
        box_confidence = tf.sigmoid(predictions[..., 4:5])
        box_class_probs = tf.sigmoid(predictions[..., 5:])
        return box_xy, box_wh, box_confidence, box_class_probs


    def predict(self, output, image_shape, keras_model):
        """
        Introduction
        ------------
            构建预测模型
        Parameters
        ----------
            inputs: 处理之后的输入图片
            image_shape: 图像原始大小
        Returns
        -------
            boxes: 物体框坐标
            scores: 物体概率值
            classes: 物体类别
        """
        boxes, scores, classes = self.eval(output, image_shape, keras_model, max_boxes = 1000)
        return boxes, scores, classes



def letterbox_image(image, size):
    '''resize image with unchanged aspect ratio using padding'''
    ih, iw, _ = image.shape
    w, h = size
    scale = min(w/iw, h/ih)
    nw = int(iw*scale)
    nh = int(ih*scale)

    image = cv2.resize(image, (nw,nh), interpolation=cv2.INTER_LINEAR)
    new_image = np.ones((h,w,3), np.uint8) * 128
    h_start = (h-nh)//2
    w_start = (w-nw)//2
    new_image[h_start:h_start+nh, w_start:w_start+nw, :] = image
    return new_image


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


def write_items_to_file(image_id, items, fw):
    for item in items:
        fw.write(image_id + " " + " ".join([str(comp) for comp in item]) + "\n")


def pred_img(img_path, model_image_size):
    image = cv2.imread(img_path)
    images.append(image)
    image = image[...,::-1]
    image_h, image_w, _ = image.shape
    
    # image preprocessing
    #if model_image_size != (None, None):
    #    assert model_image_size[0]%32 == 0, 'Multiples of 32 required'
    #    assert model_image_size[1]%32 == 0, 'Multiples of 32 required'
    #    boxed_image = letterbox_image(image, tuple(reversed(model_image_size)))
    #else:
    #    new_image_size = (image_w - (image_w % 32), image_h - (image_h % 32))
    #    boxed_image = letterbox_image(image, new_image_size)
    #boxed_image = cv2.resize(image, (416,416), interpolation=cv2.INTER_LINEAR)
    #custom_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    custom_image = cv2.resize(image, model_image_size, interpolation=cv2.INTER_NEAREST)
    image_data = np.array(custom_image, dtype='float32')
    #image_data = image_data.transpose(2,0,1)
    image_data /= 255.
    image_data = np.expand_dims(image_data, 0)  # Add batch dimension.

    out_boxes, out_scores, out_classes, out_y = sess.run(
        [pred_boxes, pred_scores, pred_classes, output_y],
        feed_dict={input_x: image_data, input_image_shape: (image_h, image_w)})
    #return filtered_boxes
    # convert the result to label format
    items = []
    for i, c in reversed(list(enumerate(out_classes))):
        predicted_class = class_names[c]
        box = out_boxes[i]
        score = out_scores[i]

        top, left, bottom, right = box
        top = max(0, np.floor(top + 0.5).astype('int32'))
        left = max(0, np.floor(left + 0.5).astype('int32'))
        bottom = min(image_h, np.floor(bottom + 0.5).astype('int32'))
        right = min(image_w, np.floor(right + 0.5).astype('int32'))
        box = [left, top, right, bottom]
        item  = [box, score, predicted_class]
        items.append(item)

    return items

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
            category_id = convert_coco_category(class_names.index(class_id))
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
    if keras_model==1:
        if quantized==0:
            parser.add_argument('--pb_file', type=str, default="../tf_model/tf_model.pb" , help='path of frozen pb file')
        else:   
            parser.add_argument('--pb_file', type=str, default="../yolov4_quantized/quantize_eval_model.pb" , help='path of frozen pb file')

    parser.add_argument('--data_dir', type=str, default='./val2017/', required=False, help='path to the COCO validation images')
    parser.add_argument('--annotations', type=str, default='./instances_val2017.json',
                    required=False, help='path to the validation annotations')
    FLAGS = parser.parse_args()

    pb_file_path = FLAGS.pb_file


    class_names = get_class(classes_path)
    predictor = yolo_predictor(score_thresh, nms_thresh, classes_path, anchors_path)

    sess = tf.Session()
    with gfile.FastGFile(pb_file_path, 'rb') as f: # file I/O
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read()) # get graph_def from file
        sess.graph.as_default()
        tf.import_graph_def(graph_def, name='')  # import graph
    sess.run(tf.global_variables_initializer())

    input_x = sess.graph.get_tensor_by_name('image_input:0')
    output_y1 = sess.graph.get_tensor_by_name('conv2d_109/BiasAdd:0')
    output_y2 = sess.graph.get_tensor_by_name('conv2d_101/BiasAdd:0')
    output_y3 = sess.graph.get_tensor_by_name('conv2d_93/BiasAdd:0')
    
    output_y = [output_y1, output_y2, output_y3]
    input_image_shape = tf.placeholder(tf.int32, shape=(2))
    pred_boxes, pred_scores, pred_classes = predictor.predict(output_y, input_image_shape, keras_model)

    bboxes = []
    cnt = 0
    with open(FLAGS.annotations) as json_file:
        data = json.load(json_file)
        for i in data['images']:
            cnt=cnt+1
            print("processing: ", cnt, "of 5000 images")
            img_file_name = i['file_name']
            metadata = {'id': i['id'], 'height': i['height'], 'width': i['width']}
            img_path=FLAGS.data_dir + img_file_name
            items = pred_img(img_path, (input_width, input_height))
            #eval_ops = runDPU()
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