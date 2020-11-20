# Copyright (c) 2019 wrnch Inc.
# All rights reserved

from __future__ import print_function, division

import argparse

import cv2
import wrnchAI
from visualizer import Visualizer
from utils import videocapture_context


def main(args):
    code = wrnchAI.license_check_string(args.license_key) if args.license_key \
        else wrnchAI.license_check()

    if code != 0:
        raise RuntimeError(wrnchAI.returncode_describe(code))

    params = wrnchAI.PoseParams()
    params.bone_sensitivity = wrnchAI.Sensitivity.high
    params.joint_sensitivity = wrnchAI.Sensitivity.high
    params.enable_tracking = True

    # Default Model resolution
    params.preferred_net_width = 328
    params.preferred_net_height = 184

    output_format = wrnchAI.JointDefinitionRegistry.get('j23')

    print('Initializing networks...')
    estimator = wrnchAI.PoseEstimator(models_path=args.models_dir,
                                      license_string=args.license_key,
                                      params=params,
                                      gpu_id=0,
                                      output_format=output_format)
    print('Initialization done!')

    options = wrnchAI.PoseEstimatorOptions()

    print('Opening webcam...')
    with videocapture_context(args.webcam_index) as cap:
        visualizer = Visualizer()

        joint_definition = estimator.human_2d_output_format()
        bone_pairs = joint_definition.bone_pairs()

        while True:
            _, frame = cap.read()

            if frame is not None:
                estimator.process_frame(frame, options) ### passing in frame
                humans2d = estimator.humans_2d()
                ### visualizer is using opencv: using frame and draws over it
                visualizer.draw_image(frame) ### overlaying skeleton
                for human in humans2d:
                    joints = human.joints()

                    visualizer.draw_points(joints)
                    visualizer.draw_lines(joints, bone_pairs)

                visualizer.show()

            key = cv2.waitKey(1)

            if key & 255 == 27:
                break


def create_parser():
    parser = argparse.ArgumentParser()
    # -m /user/bin/wrModels -i 0 -k 
    parser.add_argument('--models-dir', '-m', type=str, required=True)
    parser.add_argument('--webcam-index', '-i', type=int, default=0)
    parser.add_argument('--license-key', '-k', type=str, default=None)

    return parser


if __name__ == '__main__':
    main(create_parser().parse_args())
