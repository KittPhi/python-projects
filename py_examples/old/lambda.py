#######################################################################################################################
"""
Use this code block to determine the Operating System and import packages that are installed in the directory of this
script.

Usage: In order to install Python packages to a directory named lib in current directory
    1. mkdir -p lib (mdir lib)
    2. cd lib
    3. pip install -t ./ package_name

"""
import platform
import sys
import os

# use this if lib is packaged with lambda
# dir_path = os.path.dirname(os.path.realpath(__file__))

sys.path.append("/gg/lib")
"""
End of block
"""
#######################################################################################################################

import cv2
import csv
import wrnchAI
import time
import json
import asyncio
import boto3

# sys.path.append('helpers')
# from helpers.visualizer import Visualizer
from helpers.utils import videocapture_context
from helpers.plotJoints import plotJoints
from helpers.checkValidCameraIndex import checkValidCameraIndex
from helpers.cameraIndex import *
from helpers.wrnchKey import *
from helpers.gernerateCsv import generateCsv
from helpers.mqtt_publish import *
from helpers.joint_labels import *
from helpers.s3_write import write_to_s3
from helpers.make_csv_from_dict import make_csv

client = boto3.client('iot-data')

def handler(event, context):
    wrnch_key = getWrchKey()
    models_path = "/usr/bin/wrModels"


    # todo: detect realsense index
    # camera_indexes = checkValidCameraIndex()  # list of camera indexes
    camera_indexes = checkValidCameraIndex(cameraIndex())
    print(camera_indexes)
    camera_index = int(camera_indexes[1])

    # todo input from event
    user = "john"
    test_time = time.strftime('%l.%M%p_%b_%d_%Y')
    # test time has a leading space
    test_time = str(test_time)[1:]
    output_path = "/gg/data/{}/{}/".format(user, test_time)
    frames_number = 200
    testTimeLength = 5
    test_type = 'chair'
    serial_number = 123
    topic = 'joints/{}'.format(str(serial_number))

    # todo: fix video save, see line 156
    # Define the codec and create VideoWriter object
    # fourcc = cv2.VideoWriter_fourcc('h', '2', '6', '4')
    # fourcc = cv2.VideoWriter_fourcc(*'X264')
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    fps = 20.0
    resolution = (640, 480)
    video_out = cv2.VideoWriter(output_path + 'output.avi', fourcc, fps, resolution)

    joints_filename = test_time + "_joints.csv"
    confidence_filename = test_time + "_confidence_joints.csv"
    confidence_human_filename = test_time + "_confidence_human.csv"

    # dictionary that will be saved as a json of the payloads
    big_payload = {}


    # payload to indicate starting
    payload = {
        'start_test': True,
        'test_time': test_time,
        'user': user,
        'output_path': output_path,
        'length_of_test': testTimeLength,
        'test_type': test_type,
        'serial_number': serial_number,
    }
    publish_mqtt(json.dumps(payload), topic)

    # generate the empty csv files
    generateCsv(output_path, joints_filename, confidence_filename, confidence_human_filename)


    """
    This part takes a long time to 'warm up'
    
    It still seems to be checking the license. If used on too many devices, it will still give an error
    """
    ### SETUP WRNCH
    # This appears to not be needed, but will read the license from the environment variable
    # if the key is not passed to it.
    # code = wrnchAI.license_check_string(wrnch_key) if wrnch_key \
    #     else wrnchAI.license_check()
    # if code != 0:
    #     raise RuntimeError(wrnchAI.returncode_describe(code))

    params = wrnchAI.PoseParams()
    params.bone_sensitivity = wrnchAI.Sensitivity.high
    params.joint_sensitivity = wrnchAI.Sensitivity.high
    params.enable_tracking = True

    # Default Model resolution
    params.preferred_net_width = 328
    params.preferred_net_height = 184

    # the part that takes so long
    output_format = wrnchAI.JointDefinitionRegistry.get('j25')
    estimator = wrnchAI.PoseEstimator(models_path=models_path,
                                      license_string=wrnch_key,
                                      params=params,
                                      gpu_id=0,
                                      output_format=output_format)

    options = wrnchAI.PoseEstimatorOptions()

    # todo: find realsense index using realsense sdk
    # this was here because video_index was reading a string from an input field,
    # Don't change until realsense index is built
    video_index = int(camera_index)
    with videocapture_context(video_index) as cap:
        
        # wrnch
        # visualizer = Visualizer()
        joint_definition = estimator.human_2d_output_format()
        bone_pairs = joint_definition.bone_pairs()

        # time record starts
        startTime = time.time()


        counter = 0
        while True:
            _, frame = cap.read()

            # todo: remove if not needed
            # currently not needed
            # # dimensions of frame
            # width = frame.shape[1]
            # height = frame.shape[0]
            # # frame counter
            # counter += 1

            if frame is not None:
                # wrnch
                estimator.process_frame(frame, options)
                humans2d = estimator.humans_2d()
                # visualizer.draw_image(frame)

                image_time = time.time()

                # todo: check to make sure that len(humans2d)>0, if not, this will break
                # try/catch might be needed
                try:
                    human = humans2d[0]
                except (RuntimeError, TypeError, NameError):
                    print("The Length of humans2d[0]:", len(humans2d[0]))
                    print(RuntimeError, TypeError, NameError)

                # for human in humans2d:
                joints = human.joints()
                confidenceScores = human.scores()
                confidence = human.score()

                # todo: make this an async function with threading, OR buffer and publish at the end
                # buffer is probably better option. Threading is already being used by one of the
                # wrnch helpers, which is why asyncio is failing
                if len(joints) >= 1:
                    # publish data to mqtt
                    # send a dictionary that zips joints_labels and timetamp + joints
                    joints_dict = dict(zip(joints_labels(), [image_time - startTime] + joints.tolist()))
                    joints_confidence_dict = dict(zip(joints_labels(),[image_time - startTime] + confidenceScores.tolist()))
                    # human_confidence is a single float value

                    # this is the payload for each joint, some metadata still needs to be added
                    # be sure to make it match the earlier payload
                    # This payload should indicate that this ins't the first frame => start_test == false
                    payload = {
                        'start_test': False,
                        'test_time' : test_time,
                        'user' : user,
                        'output_path' : output_path,
                        'length_of_test' : testTimeLength,
                        'test_type' : test_type,
                        'serial_number' : serial_number,
                        'joints' : joints_dict,
                        'joints_confidence' : joints_confidence_dict,
                        'human_confidence' : confidence
                    }

                    """
                    This seems to be slowing down record time
                    """
                    # todo: make async or buffer and publish at the end
                    # Buffer is the best option right now
                    # publish payload
                    # publish_mqtt(json.dumps(payload), topic)

                    # save payload to to big_payload
                    big_payload[image_time] = payload

                    # todo: explore a post-process to convert a dictionary to a csv without pandas
                    # Pandas is only used to build the csv, another script or lambda could convert
                    # a json to a csv
                    #
                    # write dcata to csv
                    with open(output_path + joints_filename, 'a') as realTimeCSV:
                        csvWriter = csv.writer(realTimeCSV)
                        csvWriter.writerow(
                            [image_time - startTime] + joints.tolist())


                    with open(output_path + confidence_filename, 'a') as realTimeConfidenceCSV:
                        csvWriter = csv.writer(
                            realTimeConfidenceCSV)
                        csvWriter.writerow(
                            [image_time - startTime] + confidenceScores.tolist())


                    with open(output_path + confidence_human_filename, 'a') as realTimeHumanCSV:
                        csvWriter = csv.writer(realTimeHumanCSV)
                        csvWriter.writerow(
                            [image_time - startTime] + [str(confidence)])

                    # wrnch
                    # visualizer.draw_points(joints)
                    # visualizer.draw_lines(joints, bone_pairs)

                # wrnch
                # visualizer.show()

                # todo: Fix the video output
                # save the video
                # video_out = cv2.VideoWriter(output_path + 'motion_analysis.mp4', fourcc, fps, resolution)
                # video_out = cv2.VideoWriter(output_path + 'output.avi', -1, 20.0, (640, 480)) # this one might work - tested in windows
                # font = cv2.FONT_HERSHEY_SIMPLEX
                # cv2.putText(frame,
                #             image_time,
                #             (50, 50),
                #             font, 1,
                #             (0, 255, 255),
                #             2,
                #             cv2.)
                video_out.write(frame)

            if time.time() - startTime >= testTimeLength:

                # todo: rewrite make_csv to match payload metadata
                # make_csv(big_payload, output_path + "all_" + joints_filename)
                with open(output_path + "payload.json", "w") as f:
                    f.write(json.dumps(big_payload))

                # todo: remove plots before deploying
                plotJoints(output_path, joints_filename,
                           confidence_filename, confidence_human_filename)

                # todo: direct save all csvs for now
                # write_to_s3(bucket_name="motion-analysis", file_name=output_path + joints_filename)

                # todo: decompose this buffer by reading the dictionay keys and then publishing each value
                # Cannot publish entire payload at once
                # publish_mqtt(json.dumps(big_payload), topic)


                break


# run locally for testing
if __name__ == '__main__':
    handler('', '')