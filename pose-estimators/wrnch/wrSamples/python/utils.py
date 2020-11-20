# Copyright (c) 2019 wrnch Inc.
# All rights reserved

from __future__ import print_function, division

from contextlib import contextmanager
import sys

import cv2


@contextmanager
def videocapture_context(video_source):
    cap = cv2.VideoCapture(video_source)
    if not cap.isOpened():
        sys.exit('Cannot open webcam.')

    try:
        yield cap
    finally:
        cap.release()
        cv2.destroyAllWindows()
