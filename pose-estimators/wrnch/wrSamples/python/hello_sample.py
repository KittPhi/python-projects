# Copyright (c) 2019 wrnch Inc.
# All rights reserved

from __future__ import print_function, division

import argparse

import cv2
import wrnchAI


def main(args):
    code = wrnchAI.license_check_string(args.license_key) if args.license_key \
        else wrnchAI.license_check()

    if code != 0:
        raise RuntimeError(wrnchAI.returncode_describe(code))

    frame = cv2.imread(args.image)

    if frame is None:
        raise RuntimeError('could not read image at {}'.format(args.image))

    print('Initializing networks...')
    estimator = wrnchAI.PoseEstimator(models_path=args.models_dir,
                                      license_string=args.license_key)
    print('Initialization done')

    options = wrnchAI.PoseEstimatorOptions()

    print('Inferring ...')

    estimator.process_frame(frame, options)

    num_persons = len(estimator.humans_2d())

    print('Inference done! Found ', num_persons, ' humans')


def create_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument('image', type=str)
    parser.add_argument('--models-dir', '-m', type=str, required=True)
    parser.add_argument('--license-key', '-k', type=str, default=None)

    return parser


if __name__ == '__main__':
    main(create_parser().parse_args())
