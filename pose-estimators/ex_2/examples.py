import pyrealsense2 as rs
import sys, os
from usb.core import find as finddev

# DIDN'T WORK
# 1 allows it to be searched before others, including built-ins
# sys.path.append('/Repos/dev-env-py/pose-estimators')
# import realsense
# realsense.reset_camera()

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
from realsense import reset_camera

reset_camera()

pipeline = rs.pipeline()
pipeline.start()

try:
    while True:
        frames = pipeline.wait_for_frames()
        depth = frames.get_depth_frame()
except:
    pass
finally:
    pass

