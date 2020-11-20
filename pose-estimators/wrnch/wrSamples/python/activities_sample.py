# Copyright (c) 2019 wrnch Inc.
# All rights reserved

from __future__ import print_function, division

import argparse
import os.path

import cv2
import numpy as np
import wrnchAI

from visualizer import Visualizer
from utils import videocapture_context


LEFT = 'left'
RIGHT = 'right'


def annotate_predicted_classes(visualizer,
                               side,
                               class_probabilities,
                               class_names,
                               joints):
    predicted_class_name = class_names[np.argmax(class_probabilities)]
    text = side + ': ' + predicted_class_name

    (_, text_height), _ = visualizer.get_text_size(text)

    if side == LEFT:
        vertical_offset = 0
    else:
        vertical_offset = text_height + 5

    visualizer.draw_text_in_frame(text,
                                  x=int(joints[0] * visualizer.frame.shape[1]),
                                  y=int(
                                      joints[1] * visualizer.frame.shape[0]+vertical_offset))


def main(args):
    code = wrnchAI.license_check_string(args.license_key) if args.license_key \
        else wrnchAI.license_check()

    if code != 0:
        raise RuntimeError(wrnchAI.returncode_describe(code))

    #  the activity model wrnch_activity_v1.0.enc predicts 7 classes:
    #
    #  Gesture Name            | Type | Explaination
    #  ==========================================================================================
    #  None                   | No gesture is predicted.
    #  -----------------------------------------------------------------------------------------
    #  (left) Fist Pump       | Left hand is in a fist and left arm is repeatedly extending
    #                         | up and down overhead.
    #  -----------------------------------------------------------------------------------------
    #  (left) Wave            | Left hand is waving.
    #  -----------------------------------------------------------------------------------------
    #  (left) Come here       | Left hand makes a come here gestures, wrist and/or finger tips
    #                         | are moving cyclically (in a scooping motion) towards the body
    #  -----------------------------------------------------------------------------------------
    #  (left) Stop            | Static gesture. Arm is fully or nearly extended outwards (not
    #                         | upwards or downwards). Fingers may be together or somewhat apart
    #                         | but not fully splayed.
    #  -----------------------------------------------------------------------------------------
    #  Clap                   | Both hands come together, forming a clap. Note that unlike the
    #                         | previous 4 gestures, this is not predicting something about just
    #                         | the left arm of the body: it's predicting something about both
    #                         | arms together.
    #  -----------------------------------------------------------------------------------------
    #  Summon                 | Arms are straight, starting at the sides of the body, then are
    #                         | raised in tandem forming a V in front of the body. Like the Clap
    #                         | gesture,  this gestures is not just predicting something about
    #                         | the left arm of the body: it's predicting something about both
    #                         | arms together.
    #  -----------------------------------------------------------------------------------------
    activity_model_name = 'wrnch_activity_v1.0.enc'
    activity_model_path = os.path.join(args.models_dir, activity_model_name)

    # Builders are how we create `ActivityModel` instances
    builder = wrnchAI.ActivityModel.Builder(activity_model_path)

    print('Initializing networks... (this could take some time)')
    print('Building activity model ...')
    activity_models = {LEFT: builder.build(), RIGHT: builder.build_reflected()}
    print('Building pose estimator ... ')
    requirements = builder.pose_estimator_requirements()
    pose_estimator = requirements.build_compatible_estimator(
        args.models_dir,
        device_id=0,
        desired_net_width=328,
        desired_net_height=184,
        license_string=args.license_key)
    options = requirements.build_compatible_options()
    print('Initialization done!')

    print('Opening webcam...')
    with videocapture_context(args.webcam_index) as cap:
        visualizer = Visualizer()

        bone_pairs = pose_estimator.human_2d_output_format().bone_pairs()

        class_names = activity_models[LEFT].class_names

        while True:
            _, frame = cap.read()

            if frame is None:
                break

            pose_estimator.process_frame(frame, options)

            for model in activity_models.values():
                model.process_poses(pose_estimator=pose_estimator,
                                    image_width=frame.shape[1],
                                    image_height=frame.shape[0])

            humans2d = pose_estimator.humans_2d()

            visualizer.draw_image(frame)

            for human in humans2d:
                person_id = human.id

                joints = human.joints()

                visualizer.draw_points(joints)
                visualizer.draw_lines(joints, bone_pairs)

                for (model_name, model) in activity_models.items():
                    # an individual model for a person holds probabilities of gestures for that person
                    individual_model = model.individual_model(person_id)

                    class_probabilities = individual_model.probabilities()

                    annotate_predicted_classes(
                        visualizer, model_name, class_probabilities, class_names, joints)

            visualizer.show()

            key = cv2.waitKey(1)

            if key & 255 == 27:
                break


def create_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument('--models-dir', '-m', type=str, required=True)
    parser.add_argument('--webcam-index', '-i', type=int, default=0)
    parser.add_argument('--license-key', '-k', type=str, default=None)

    return parser


if __name__ == '__main__':
    main(create_parser().parse_args())
