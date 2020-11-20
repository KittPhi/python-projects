import pyrealsense2 as rs
import sys, os
from usb.core import find as finddev

# DIDN'T WORK
# 1 allows it to be searched before others, including built-ins
# sys.path.append('/Repos/dev-env-py/pose-estimators')
# import realsense
# realsense.reset_camera()

# allow local files containing functions to be accessible
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
from realsense import reset_camera

reset_camera()

pipeline = rs.pipeline()
pipeline.start()

try:
    while True:
         frames = pipeline.wait_for_frames()
        depth = frames.get_depth_frame()
        if not depth: continue

        # Print a simple text-based representation of the image, by breaking it into 10x20 pixel regions and approximating the coverage of pixels within one meter
        coverage = [0]*64
        for y in range(480):
            for x in range(640):
                dist = depth.get_distance(x, y)
                if 0 < dist and dist < 1:
                    coverage[x//10] += 1
            
            if y%20 is 19:
                line = ""
                for c in coverage:
                    line += " .:nhBXWW"[c//25]
                coverage = [0]*64
                print(line)
except:
    pass
finally:
    pass

