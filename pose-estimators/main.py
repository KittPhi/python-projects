# RUNNING PROGRAM ------------------------------------------------
# python3.7 main.py -p wrnch -i 3

import argparse
from cubemos import cubemos_func as cubemos
from d2 import d2_func as d2
from wrnch import wrnch_func as wrnch

def main(args):
    print("Inside main...")
    if (args.pose_estimator == "d2"):
        d2(args)
    if (args.pose_estimator == "wrnch"):
        wrnch(args)
    if (args.pose_estimator == "cubemos"):
        cubemos(args)

def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', '-i', type=int, required=True)
    parser.add_argument('--models-dir', '-m', type=str, default="/usr/bin/wrModels")
    parser.add_argument('--pose-estimator', '-p', type=str, required=True)
    parser.add_argument('--webcam-index', '-i', type=int, default=0)
    parser.add_argument('--license-key', '-k', type=str, default="C53B58-66623B-4E7AAA-05BD50-028B95-093938")
    return parser

if __name__ == "__main__":
    print('1=camera frame, 2=camera video, 3=jpg/png')
    print('E.g. Python3.7 main.py -i 3 -p wrnch')
    main(create_parser().parse_args())