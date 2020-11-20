from realsense import input_handler, reset_camera
import argparse
import wrnchAI
import cv2
from visualizer import Visualizer
from traceback import print_exc

def wrnch_func(args):
    print("Inside wrnch_func...")
    print(args.pose_estimator, " = selected pose estimator")
    print(args.license_key, " = license key")

    reset_camera()
    input_handler()

    # from /usr/src/wrnchAI/wrSamples/python/pose3d_sample.py
    code = wrnchAI.license_check_string(args.license_key) if args.license_key \
        else wrnchAI.license_check()
    
    if code != 0:
        raise RuntimeError(wrnchAI.returncode_describe(code))

    # params
    params = wrnchAI.PoseParams()
    params.bone_sensitivity = wrnchAI.Sensitivity.high
    params.joint_sensitivity = wrnchAI.Sensitivity.high
    params.enable_tracking = True
    # Default Model resolution
    params.preferred_net_width = 328
    params.preferred_net_height = 184
    # model
    output_format = wrnchAI.JointDefinitionRegistry.get('j23')

    try:
        print('Initializing PoseEstimator...')
        estimator = wrnchAI.PoseEstimator(models_path=args.models_dir,
                                        license_string=args.license_key,
                                        params=params,
                                        gpu_id=0,
                                        output_format=output_format)
        # which type to process frame
        options = wrnchAI.PoseEstimatorOptions()
        options.estimate_3d = True
        print('Initialization done!')
    except:
        print_exc()

    print('Opening webcam...')
    with cv2.VideoCapture(-1) as cap:
        visualizer = Visualizer()

        joint_definition = estimator.human_3d_output_format()
        # bone_pairs = joint_definition.bone_pairs()

        while True:

            # capture frame by frame
            _, frame = cap.read()

            if frame is not None:

                # passing in frame
                estimator.process_frame(frame, options)
                
                # get the pose3d's last estimation by PoseEstimator, returns list of Pose2d <<<
                humans3d = estimator.humans_3d()

                # visualizer is using opencv: using frame and draws over it
                visualizer.draw_image(frame) ### overlaying skeleton

                for human in humans3d:
                    joints = human.joints()

                    visualizer.draw_points(joints)
                    visualizer.draw_lines(joints)

                visualizer.show()

            key = cv2.waitKey(1)

            if key & 255 == 27:
                break


def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--source', '-s', type=int, required=True)
    parser.add_argument('--models-dir', '-m', type=str, default="/usr/bin/wrModels")
    parser.add_argument('--pose-estimator', '-p', type=str, required=True)
    parser.add_argument('--webcam-index', '-i', type=int, default=0)
    parser.add_argument('--license-key', '-k', type=str, default="C53B58-66623B-4E7AAA-05BD50-028B95-093938")
    return parser

if __name__ == "__main__":
    print('1=camera_frame, 2=camera_video, 3=jpg/png')
    print('E.g. python3.7 wrnch.py -p wrnch -s 1')
    wrnch_func(create_parser().parse_args())