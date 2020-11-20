/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator to detect human hands in images and access
 poses data

 Key concepts:
    Accessing hand pose data
    Accessing hand bounding box data
    PoseEstimatorOptions -- Estimate hands
    PoseEstimatorOptions -- Estimate hand boxes
    No throw pose estimator initialization, processing
    Frame processing
*/

#include <iostream>
#include <exception>

#include <wrnch/engine.hpp>
#include <Visualizer.hpp>
#include <SampleUtils.hpp>
using namespace wrSamples;

/**
 *  Licensing:
 *    If you received a license key, e.g. XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX,
 *      set licenseKey to your key, or set the environment variable WRNCH_LICENSE to this value.
 *    If you received a license file, ensure it is saved to your home folder and there
 *      is nothing else to do
 */
static constexpr const char* licenseKey = nullptr;

static void DrawHands(wrnch::PoseEstimator::PoseHandsIter handsBegin,
                      wrnch::PoseEstimator::PoseHandsIter handsEnd,
                      Visualizer& visualizer,
                      const unsigned int* bonePairs,
                      const unsigned int numHandJoints,
                      const unsigned int numBones)
{
    for (wrnch::PoseEstimator::PoseHandsIter it = handsBegin; it < handsEnd; ++it)
    {
        wrnch::Pose2dView pose = *it;
        visualizer.DrawPoints(pose.GetJoints(), numHandJoints);
        visualizer.DrawLines(pose.GetJoints(), &bonePairs[0], numBones);
    }
}

int main(int argc, const char** argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: wrHands_sample_cpp <model path> [<video path> / camera index {0}]\n";
        return -1;
    }

    const char* const modelsDirectory = argv[1];
    std::string inputVideo = (argc == 3) ? argv[2] : "0";
    std::string licenseString;

    wrReturnCode licenseCheckCode;
    if (licenseKey == nullptr)
    {
        licenseCheckCode = wrnch::LicenseCheck();
    }
    else
    {
        licenseString = std::string(licenseKey);
        licenseCheckCode = wrnch::LicenseCheckString(licenseString);
    }

    if (licenseCheckCode != wrReturnCode_OK)
    {
        std::cerr << wrReturnCode_Translate(licenseCheckCode) << "\n";
        return -1;
    }

    wrReturnCode status;
    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };

    if (!licenseString.empty())
    {
        configParams.WithLicenseString(licenseString);
    }

    std::cout << "Initializing networks...\n";

    // Overloads which take a wrReturnCode will not throw exceptions.
    // You must check the value of the code on return and proceed
    // accordingly
    poseEstimator.Initialize(configParams, status);
    if (status != wrReturnCode_OK)
    {
        std::cerr << "Error: " << wrnch::ReturnCodeToString(status) << std::endl;
        return -1;
    }
    // Initialize the hand model and set hand pose option
    poseEstimator.InitializeHands2D(status, modelsDirectory);
    if (status != wrReturnCode_OK)
    {
        std::cerr << "Error: " << wrnch::ReturnCodeToString(status) << std::endl;
        return -1;
    }

    wrnch::PoseEstimatorOptions poseOptions;
    poseOptions.SetEstimateAllHandBoxes(true);
    poseOptions.SetEstimateHands(true);
    poseOptions.SetMainPersonId(wrnch_MAIN_ID_ALL);

    std::cout << "Initialization done !\n";

    std::cout << "Opening media\n";
    cv::VideoCapture cap;
    std::pair<int, bool> webcamIndexOptional = GetWebcamIndexMaybe(inputVideo);
    if (webcamIndexOptional.second)
    {
        cap.open(webcamIndexOptional.first);
    }
    else
    {
        cap.open(inputVideo);
    }
    if (!cap.isOpened())
    {
        std::cerr << "Cannot open media\n";
        return -1;
    }

    // Pairs of indices into the returned pose data for drawing bones
    wrnch::JointDefinition jointsDefinition = poseEstimator.GetHandOutputFormat();
    std::vector<unsigned int> bonePairs(jointsDefinition.GetNumBones() * 2);
    jointsDefinition.GetBonePairs(&bonePairs[0]);

    Visualizer visualizer;

    const unsigned int numHandJoints = poseEstimator.GetHandOutputFormat().GetNumJoints();
    const unsigned int numBones = jointsDefinition.GetNumBones();

    cv::Mat currentFrame;
    do
    {
        cap >> currentFrame;
        if (!currentFrame.empty())
        {
            visualizer.SetFrame(currentFrame);
            poseEstimator.ProcessFrame(
                currentFrame.data, currentFrame.cols, currentFrame.rows, poseOptions, status);

            if (status != wrReturnCode_OK)
            {
                std::cerr << "Error: " << wrnch::ReturnCodeToString(status) << std::endl;
                return -1;
            }

            DrawHands(poseEstimator.LeftHandsBegin(),
                      poseEstimator.LeftHandsEnd(),
                      visualizer,
                      &bonePairs[0],
                      numHandJoints,
                      numBones);

            DrawHands(poseEstimator.RightHandsBegin(),
                      poseEstimator.RightHandsEnd(),
                      visualizer,
                      &bonePairs[0],
                      numHandJoints,
                      numBones);

            for (wrnch::PoseEstimator::HandBoxIter it = poseEstimator.HandBoxesBegin();
                 it < poseEstimator.HandBoxesEnd();
                 it++)
            {
                wrnch::HandBoxPairView pair = *it;
                wrnch::Box2dView leftHandBox = pair.GetLeft();
                wrnch::Box2dView rightHandBox = pair.GetRight();
                visualizer.DrawBox(leftHandBox.GetMinX(),
                                   leftHandBox.GetMinY(),
                                   leftHandBox.GetWidth(),
                                   leftHandBox.GetHeight());
                visualizer.DrawBox(rightHandBox.GetMinX(),
                                   rightHandBox.GetMinY(),
                                   rightHandBox.GetWidth(),
                                   rightHandBox.GetHeight());
            }
            visualizer.Show();
        }

        if (visualizer.WaitKeyboardInput(1) == 27)
        {
            break;
        }
    } while (true);


    return 0;
}
