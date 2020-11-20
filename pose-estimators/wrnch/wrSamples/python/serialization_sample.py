# Copyright (c) 2019 wrnch Inc.
# All rights reserved

from __future__ import print_function, division

import argparse
import time

import wrnchAI


def main(args):
    code = wrnchAI.license_check_string(args.license_key) if args.license_key \
        else wrnchAI.license_check()

    if code != 0:
        raise RuntimeError(wrnchAI.returncode_describe(code))

    params = wrnchAI.PoseParams()

    output_format = wrnchAI.JointDefinitionRegistry.get('j23')

    print('Initializing networks... (slow, innit?)')
    t0 = time.time()
    estimator = wrnchAI.PoseEstimator(models_path=args.models_dir,
                                      license_string=args.license_key,
                                      params=params,
                                      gpu_id=0,
                                      output_format=output_format)
    print('Initialization done. Took', time.time() - t0, 'seconds')

    print('serializing pose estimator to memory ...')
    serialized_estimator = estimator.serialize()
    print('done serializing. Deserializing ...')
    t0 = time.time()
    wrnchAI.PoseEstimator.deserialize(
        serialized_estimator)
    print('deserializing (from memory) took', time.time() - t0,
          'seconds. Compare to initializing the other way!')


def create_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument('--models-dir', '-m', type=str, required=True)
    parser.add_argument('--license-key', '-k', type=str, default=None)

    return parser


if __name__ == '__main__':
    main(create_parser().parse_args())
