import pyrealsense2 as rs
import numpy as np
import cv2

import sys, os
from usb.core import find as finddev
from traceback import print_exc
from time import sleep, strftime

# allow local files containing functions to be accessible
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
from realsense import reset_camera

reset_camera()

try:
    # Configure depth and color streams
    pipeline = rs.pipeline()
    config = rs.config()
    width, height = 640, 480
    config.enable_stream(rs.stream.depth, width, height, rs.format.z16, 30)
    config.enable_stream(rs.stream.color, width, height, rs.format.bgr8, 30)
    # config.enable_stream(rs.stream.confidence, width, height, rs.format.bgr8, 30)
    # ERROR: pipeline.stop() RuntimeError: stop() cannot be called before start()

    # Start streaming
    pipeline.start(config)

    while True:

        # Wait for a coherent pair of frames: depth and color
        frames = pipeline.wait_for_frames()
        depth_frame = frames.get_depth_frame()
        color_frame = frames.get_color_frame()
        if not depth_frame or not color_frame:
            continue

        # Convert images to numpy arrays
        depth_image = np.asanyarray(depth_frame.get_data())
        color_image = np.asanyarray(color_frame.get_data())

        # Apply colormap on depth image (image must be converted to 8-bit per pixel first)
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)

        # Stack both images horizontally
        images = np.hstack((color_image, depth_colormap))

        # Show images
        cv2.namedWindow('RealSense', cv2.WINDOW_AUTOSIZE)
        cv2.imshow('RealSense', images)

        # waitKey(0) is for one frame, waitKey(1) is for continuous frames
        ch = cv2.waitKey(1)
        if ch == 27 or ch == ord('q') or ch == ord('Q'):
            break

finally:

    # Stop streaming
    pipeline.stop()