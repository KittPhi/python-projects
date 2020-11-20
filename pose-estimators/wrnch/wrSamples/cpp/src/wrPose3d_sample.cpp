/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator to detect humans in images and access
 3D pose data.
 The 3D points returned by the pose estimator can be rotated using the m,n keys.


 Key concepts:
    Accessing 3D pose data
    PoseEstimatorOptions -- Estimate 3D
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

int main(int argc, const char* const* argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: wrPose3d_sample_cpp <model path> [<video path> / camera index {0}]\n";
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

    wrnch::JointDefinition outputFormat = wrnch::JointDefinitionRegistry::Get("j23");
    wrnch::PoseParams params;

    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };
    configParams.WithPoseParams(params).WithOutputFormat(outputFormat);

    if (!licenseString.empty())
    {
        configParams.WithLicenseString(licenseString);
    }

    std::cout << "Initializing networks...\n";
    try
    {
        // Create and initialize the pose estimator from the configuration
        poseEstimator.Initialize(configParams);

        // Initialize the 3D model and set 3D pose option
        poseEstimator.Initialize3D(modelsDirectory);
        wrnch::PoseEstimatorOptions poseOptions;
        poseOptions.SetEstimate3D(true);
        poseOptions.SetMainPersonId(wrnch_MAIN_ID_ALL);

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

        wrnch::JointDefinition outputformat3d = poseEstimator.GetHuman3DRawOutputFormat();
        const unsigned int numJoints = outputformat3d.GetNumJoints();
        const unsigned int numBones = outputformat3d.GetNumBones();
        std::vector<unsigned int> bonePairs(numBones * 2);
        outputformat3d.GetBonePairs(bonePairs.data());
        outputformat3d.PrintJointDefinition();

        Visualizer visualizer;
        int rotationAngleY = 0;
        cv::Mat currentFrame;
        do
        {
            cap >> currentFrame;
            if (!currentFrame.empty())
            {
                cv::Mat backgroundFrame(currentFrame.cols, currentFrame.rows, CV_8UC3);
                backgroundFrame = cv::Scalar(255, 255, 255);
                visualizer.GetBillBoard(currentFrame, backgroundFrame, 0, rotationAngleY, 0);
                visualizer.SetFrame(backgroundFrame);

                // May throw
                poseEstimator.ProcessFrame(
                    currentFrame.data, currentFrame.cols, currentFrame.rows, poseOptions);

                for (wrnch::Pose3dView pose : poseEstimator.Humans3dRaw())
                {
                    std::vector<float> positions;
                    const float* pos = pose.GetPositions();
                    visualizer.DrawLines3D(
                        pos, numJoints, bonePairs, numBones, 0, rotationAngleY, 0);
                }

                visualizer.FlipFrame();
                visualizer.DrawTextInFrame(
                    "Rotate image about y axis clockwise (m), counter-clockwise (n)",
                    0,
                    visualizer.GetHeight());
                visualizer.Show();
            }
            char key = visualizer.WaitKeyboardInput(1);
            if (key == 27)  // esc key pressed
            {
                break;
            }
            if (key == 'm')
            {
                if (rotationAngleY < 75)
                    rotationAngleY += 15;
            }
            if (key == 'n')
            {
                if (rotationAngleY > -75)
                    rotationAngleY -= 15;
            }

        } while (true);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred" << std::endl;
        return -1;
    }
    return 0;
}
