from usb.core import find as finddev
from time import sleep
import pyrealsense2 as rs
from traceback import print_exc

import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
# import pose_estimators as ps
from realsense import reset_camera

# reset realsense camera
reset_camera()

# initialize realsense
pipe = rs.pipeline()
config = rs.config()

# disable any active streams
config.disable_all_streams()

try:
    sleep(0.5)  # allow wait before starting new stream
    pipeline.start(config)
except:
    print_exc()

finally:
    pipe.stop()
