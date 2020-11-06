'''
Import a protobuf graph (.pb or .pbtxt) into Tensorboard
Example usage:
  python import_pb.py --graph=./freeze/frozen_graph.pb  --log_dir=./tb_logs
  python import_pb.py --graph=infer_graph.pbtxt  --log_dir=dummy
  python import_pb.py --graph=infer_graph.pbtxt
'''
import os
import sys
import argparse
from google.protobuf import text_format
import tensorflow as tf

from tensorflow.core.framework import graph_pb2
from tensorflow.python.client import session
from tensorflow.python.framework import importer
from tensorflow.python.framework import ops
from tensorflow.python.platform import app
from tensorflow.python.platform import gfile
from tensorflow.python.summary import summary

# Silence TensorFlow messages
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '1'



def import_to_tensorboard(model_dir, log_dir):
  """View an imported protobuf model (`.pb` file) as a graph in Tensorboard.
  Args:
    model_dir: The location of the protobuf (`pb`) model to visualize
    log_dir: The location for the Tensorboard log to begin visualization from.
  Usage:
    Call this function with your model location and desired log directory.
    Launch Tensorboard by pointing it to the log directory.
    View your imported `.pb` model as a graph.
  """
  with session.Session(graph=ops.Graph()) as sess:
    with gfile.GFile(model_dir, "rb") as f:
      graph_def = graph_pb2.GraphDef()
      graph_def.ParseFromString(f.read())
      importer.import_graph_def(graph_def)

    pb_visual_writer = summary.FileWriter(log_dir)
    pb_visual_writer.add_graph(sess.graph)
    print("Model Imported. Visualize by running: "
          "tensorboard --logdir={}".format(log_dir))
    print('..or try: `tensorboard --logdir={} --port 6006 --host localhost`'.format(log_dir))

def run_main():

  # command line argument parsing
  parser = argparse.ArgumentParser(description='Script to import frozen graph into TensorBoard')
  parser.add_argument(
      '-g', '--graph',
      type=str,
      default='',
      help='Protobuf graph file (.pb) to import into TensorBoard.',
      required='True')
  parser.add_argument(
      '-l', '--log_dir',
      type=str,
      default='tb_log',
      help='TensorBoard log directory.')

  flags = parser.parse_args()


  # get filename, file extension and full path of input graph
  pathname, filename_with_ext = os.path.split(flags.graph)
  filename_no_ext, ext = os.path.splitext(filename_with_ext)


  # assume a .pbtxt file is a text protobuf format - need to convert it to binary .pb
  # before it can be imported in TensorBoard

  print('-------------------------------------')
  print('Input graph:', flags.graph)

  if (ext == '.pbtxt'):
    f = open(flags.graph, 'r')
    text_graph = f.read()
    graph_def = text_format.Parse(text_graph, tf.GraphDef())
    tf.train.write_graph(graph_def, pathname, filename_no_ext + '.pb', as_text=False)
    temp_bin_graph = os.path.join(pathname, filename_no_ext + '.pb')
    print('Input graph is pbtxt. Created .pb file at', temp_bin_graph)
  elif (ext == '.pb'):
    temp_bin_graph = flags.graph
  else:
    print('Graph is not a .pb or .pbtxt. Exiting')
    sys.exit(1)


  print('-------------------------------------\n')


  import_to_tensorboard(model_dir=temp_bin_graph, log_dir=flags.log_dir)


if __name__ == '__main__':
    run_main()

