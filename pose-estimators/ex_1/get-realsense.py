import pyrealsense2 as rs
import numpy as np
import cv2

DS5_product_ids = ["0AD1", "0AD2", "0AD3", "0AD4", "0AD5", "0AF6", "0AFE", "0AFF", "0B00", "0B01", "0B03", "0B07","0B3A"]
def find_device_that_supports_advanced_mode() :
    ctx = rs.context()
    ds5_dev = rs.device()
    devices = ctx.query_devices()
    print("D: ", devices)
    devs = []
    for dev in devices:
        if dev.supports(rs.camera_info.product_id) and str(dev.get_info(rs.camera_info.product_id)) in DS5_product_ids:
            if dev.supports(rs.camera_info.name):
                print("Found device that supports advanced mode:", dev.get_info(rs.camera_info.name), " -> ", dev)
                devs.append(dev)
    #raise Exception("No device that supports advanced mode was found")
    return devs

devs = find_device_that_supports_advanced_mode()