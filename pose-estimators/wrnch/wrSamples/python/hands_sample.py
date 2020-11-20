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

    print('Initializing networks...')
    estimator = wrnchAI.PoseEstimator(models_path=args.models_dir,
                                      license_string=args.license_key)
    estimator.initialize_hands2d(args.models_dir)
    print('Initialization done!')

    options = wrnchAI.PoseEstimatorOptions()
    options.estimate_all_handboxes = True
    options.estimate_hands = True

    print('Opening webcam...')
    with videocapture_context(args.webcam_index) as cap:
        visualizer = Visualizer()

        joint_definition = estimator.hand_output_format()
        bone_pairs = joint_definition.bone_pairs()

        def draw_hands(hands):
            for hand in hands:
                joints = hand.joints()

                visualizer.draw_points(joints, joint_size=6)
                visualizer.draw_lines(joints, bone_pairs, bone_width=2)

        while True:
            _, frame = cap.read()

            if frame is not None:
                estimator.process_frame(frame, options)

                visualizer.draw_image(frame)

                draw_hands(estimator.left_hands())
                draw_hands(estimator.right_hands())
                hand_box_pairs = estimator.hand_boxes()

                for hand_box_pair in hand_box_pairs:
                    left_box = hand_box_pair.left
                    right_box = hand_box_pair.right

                    visualizer.draw_box(left_box.min_x, left_box.min_y,
                                        left_box.width, left_box.height)
                    visualizer.draw_box(right_box.min_x, right_box.min_y,
                                        right_box.width, right_box.height)

                visualizer.show()

            key = cv2.waitKey(1)

            if key & 255 == 27:  # escape key
                break


def create_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument('--models-dir', '-m', type=str, required=True)
    parser.add_argument('--webcam-index', '-i', type=int, default=0)
    parser.add_argument('--license-key', '-k', type=str, default=None)

    return parser


if __name__ == '__main__':
    main(create_parser().parse_args())
