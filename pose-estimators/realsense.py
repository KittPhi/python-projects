from visualizer import Visualizer
from usb.core import find as finddev
import argparse
import cv2
import pyrealsense2 as rs
from traceback import print_exc
from time import sleep, strftime

def reset_camera(): # reset realsense camera
    dev = finddev(idVendor=0x8086)
    if dev:
        dev.reset()
        print('camera has been reset...')
    else:
        sys_exit("Error: No USB devices detected")

def input_handler():
    # reset realsense camera
    reset_camera()

    image_width = 1280  # 1280 - 640
    image_height = 720  # 720 - 480

    try:
        # initialize realsense
        pipe = rs.pipeline()
        config = rs.config()

        # disable any active streams
        config.disable_all_streams()

        # ensure it is safe to enable new stream
        sleep(0.5)

        # enable new color and depth streams
        config.enable_stream(rs.stream.depth, image_width,
                                image_height, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, image_width,
                                image_height, rs.format.bgr8, 30)

        pipe.start(config)

        try:
            frame = pipe.try_wait_for_frames()
        except:
            pipe.stop()
            print_exc()
    except:
        print_exc()
