from usb.core import find as finddev
from time import sleep
import pyrealsense2 as rs
import pose_estimators as ps
from traceback import print_exc

# reset realsense camera
ps.reset()

# initialize realsense
pipe = rs.pipeline()
config = rs.config()

# disable any active streams
config.disable_all_streams()

try:
  sleep(0.5) # allow wait before starting new stream
  pipeline.start(config)
except:
  print_exc()

finally:
  pipe.stop()