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
    estimator.initialize_head(args.models_dir)
    print('Initialization done!')

    options = wrnchAI.PoseEstimatorOptions()
    options.estimate_heads = True
    options.estimate_face_poses = True

    print('Opening webcam...')
    with videocapture_context(args.webcam_index) as cap:
        visualizer = Visualizer()

        while True:
            _, frame = cap.read()

            if frame is not None:

                estimator.process_frame(frame, options)
                heads = estimator.heads()
                faces = estimator.faces()

                visualizer.draw_image(frame)
                for head in heads:
                    bounding_box = head.bounding_box
                    visualizer.draw_box(bounding_box.min_x, bounding_box.min_y,
                                        bounding_box.width, bounding_box.height)

                for face in faces:
                    landmarks = face.landmarks()
                    arrow = face.arrow()

                    visualizer.draw_points(landmarks, joint_size=2)
                    visualizer.draw_arrow(arrow)

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
