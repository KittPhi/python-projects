from realsense import input_func, reset_camera
import argparse

def d2_func(args):
    print("Inside d2_func...")
    print(args.pose_estimator, " = selected pose estimator")
    print(args.license_key, " = license key")

    reset_camera()
    input_handler()