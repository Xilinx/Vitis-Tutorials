<table style="width:100%">
  <tr>
    <th width="100%" colspan="6"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>How to freeze a Keras model for use with Vitis AI</h2>
</th>
  </tr>

</table>

### Current Status

+ Tested with Vitis AI 1.2.1, TensorFlow 1.15.2, and Keras 2.3.1.

# 1. Introduction

When using TensorFlow and Vitis-AI, we are required to supply a 'frozen graph' in binary protobuf format (usually has a '.pb' file extension). When we are working directly in TensorFlow, generating a .pb file is relatively easy - just save an inference graph and a TensorFlow checkpoint, then run the freeze_graph script that comes with TensorFlow.

Things are not so straightforward when using the Keras standalone library, as its native format for saving trained models is either HDF5 or a mix of JSON and HDF5. Therefore, we need a way of translating those into binary protobuf format.

``
There are three ways to save a Keras model:

+ Method #1: Using HDF5 checkpoints during training.
+ Method #2: By saving the complete model in HDF5 format.
+ Method #3: By saving the network architecture in JSON format and the weights, biases and other parameters in HDF5 format.


:warning: Do not confuse TensorFlow checkpoints and Keras checkpoints, they are **_not_** the same!


Methods #1 and #2 will save a complete training model, including the network architecture, its current weights/biases, training operations such as loss and optimizer functions and their current state. This will allow training to be resumed from its current point if required.

Method #3 creates a JSON file which describes the network architecture. The weights are saved into a separate HDF5 file. The training information is not saved, so this format cannot be used for resuming training, but it normally produces much smaller files and so is favored when going to deployment.

# 2. Method #1: Keras Checkpoint to Vitis AI

If we are starting from a Keras checkpoint, then we first access the underlying TensorFlow back-end using the `backend` API. We set the learning phase to '0' to indicate to layers such as dropout or batch normalization that we are no longer training. We then load the checkpoint into a new model using `load_model`.

```python
# set learning phase for no training
keras.backend.set_learning_phase(0)

# load weights & architecture into new model
loaded_model = load_model('keras_chkpt.h5')
```

Now we can write out the TensorFlow compatible checkpoint and inference graph which will be used later with the freeze_graph.py script to create the frozen model:

```python
# fetch the tensorflow session using the Keras backend
tf_session = keras.backend.get_session()

# write out tensorflow checkpoint & meta graph
saver = tf.compat.v1.train.Saver()
save_path = saver.save(tf_session,tf_ckpt)
```

# 3. Method #2: Keras model to Vitis AI

If the Keras model has been saved as a complete model using `save_model` like this:

```python
# save weights, model architecture & optimizer to an HDF5 format file
model.save('k_complete_model.h5')
```

..then it will be an HDF5 file that is identical to the one produced when we create a Keras checkpoint so just follow the same procedure as Method #1 above.


# 4. Method #3: JSON and HDF5 to Vitis AI

If the Keras model has been saved as a JSON file for the architecture and an HDF5 file for the weights and biases using `save_model` like this:

```python
# save just the weights (no architecture) to an HDF5 format file
model.save_weights('k_model_weights.h5')

# save just the architecture (no weights) to a JSON file
with open('k_model_architecture.json', 'w') as f:
    f.write(model.to_json())
```

..then the model needs to recreated by first reading the JSON file and then loading the model with the weights stored in the HDF5 file:

```python
# set learning phase for no training
keras.backend.set_learning_phase(0)

# load json and create model
json_file = open('k_model_architecture.json', 'r')
loaded_model_json = json_file.read()
json_file.close()
loaded_model = model_from_json(loaded_model_json)

# load weights into new model
loaded_model.load_weights('k_model_weights.h5')
```

Now we can write out the TensorFlow compatible checkpoint and inference graph which will be used later with the freeze_graph.py script to create the frozen model:

```python
# fetch the tensorflow session using the Keras backend
tf_session = keras.backend.get_session()

# write out tensorflow checkpoint & meta graph
saver = tf.compat.v1.train.Saver()
save_path = saver.save(tf_session,tf_ckpt)
```


# 5. Prerequisites to Run this Tutorial

 The host machine has several requirements that need to be met before we begin. You will need:

  + An x86 host machine with that meets the [sytem requirements](https://github.com/Xilinx/Vitis-AI/blob/master/doc/system_requirements.md) and internet access to download files.

  + A GPU card suitable for training is highly recommended.


For more details, refer to the latest version of the *Vitis AI User Guide* ([UG1414](https://www.xilinx.com/html_docs/vitis_ai/1_2/zkj1576857115470.html)).

  + This tutorial assumes the user is familiar with Vitis-AI, Python3, TensorFlow and has some knowledge of machine learning principles.



# 6. Files in this Repository

This repository contains examples for all three methods.

+ `train_save.py` will train a simple network to classify the CIFAR-10 dataset. It saves the trained model using the three methods outlined in this app note. Note that it only runs for a few epochs and the accuracy will be quite low.
+ `keras_2_tf.py` will create TensorFlow checkpoints and inference graphs from the saved Keras models.
+ `image_input_fn.py` contains the pre-processing function for the images - used during the calibration phase of quantization.
+ `generate_images.py` creates image files from the CIFAR10 dataset.
+ `run_all.sh` will run the complete flow.
+ `start_cpu_docker.sh` starts the Vitis-AI CPU Docker container.
+ `start_gpu_docker.sh` starts the Vitis-AI GPU Docker container.


# 7. Instructions

1. This repository should be downloaded to the host machine as a zip file and then unzipped to a folder, or cloned using the ``git clone`` command from a terminal. Note that each Vitis-AI tutorial is provided as a separate git branch, so the clone command should specify the correct branch and the target folder you want to clone to:

```shell
 git clone -b Moving-Edge-Cloud https://github.com/Xilinx/Vitis-AI-Tutorials.git <target_folder_path>
```

2. Open a linux terminal, cd into the repository folder then into the 'files' folder. Start the Vitis AI docker - if you have a GPU in the host system, it is recommended that you use the GPU version of the docker container.


```shell
# navigate to densenet tutorial folder
cd <path_to_densenet_design>/files

# to start GPU docker
source ./start_gpu_docker.sh

# ..or to start CPU docker
source ./start_cpu_docker.sh
```


The docker container will start and you should see something like this in the terminal:


```shell
==========================================
__      ___ _   _                   _____
\ \    / (_) | (_)            /\   |_   _|
 \ \  / / _| |_ _ ___ ______ /  \    | |  
  \ \/ / | | __| / __|______/ /\ \   | |  
   \  /  | | |_| \__ \     / ____ \ _| |_
    \/   |_|\__|_|___/    /_/    \_\_____|

==========================================

Docker Image Version: latest
Build Date: Wed Apr 15 11:01:32 CEST 2020
VAI_ROOT=/opt/vitis_ai
For TensorFlow Workflows do:
  conda activate vitis-ai-tensorflow
For Caffe Workflows do:
  conda activate vitis-ai-caffe
For Neptune Workflows do:
  conda activate vitis-ai-neptune
mharvey@XITMHARVEY33:/workspace$
```

>:bulb: If you get a "Permission Denied" error when running the start_gpu_docker.sh or start_cpu_docker.sh scripts, it is almost certainly because the docker_run.sh script is not set to be executable. You can fix this by running the following command:
>
>```shell
> chmod +x ./docker_run.sh
>```

3. Run all steps using the command ``source ./run_all.sh``



When the flow is finished, you should find three folders; method1, method2 and method3 which contain all the Keras models, TensorFlow checkpoints and graphs and the .elf file.


# 8. References

+ CIFAR-10 dataset developed by Alex Krizhevsky for <a href="https://www.cs.toronto.edu/~kriz/learning-features-2009-TR.pdf">Learning Multiple Layers of Features from Tiny Images</a>.
