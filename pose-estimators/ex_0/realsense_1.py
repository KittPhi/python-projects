# modified pyrealsense example
# need to waite 3 seconds to rerun, otherwise "ERROR: Frame didn't arrive within 5000"

from usb.core import find as finddev
from time import sleep
import pyrealsense2 as rs
import realsense as ps

# reset realsense camera
ps.reset()

pipe = rs.pipeline()
profile = pipe.start()

try:
  for i in range(0, 5):
    frames = pipe.wait_for_frames()
    for f in frames:
      print(f.profile)
finally:
    pipe.stop()