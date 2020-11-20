/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator to detect humans in images and access pose
 and mask data

 Key concepts:
    Accessing 2D pose data
    Accessing mask data
    PoseParams -- Sensitivity, network dimension,
    PoseEstimator output format
    PoseEstimatorOptions -- Estimate mask
    Pose iterator
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
        std::cerr << "Usage: wrPose2d_sample_cpp <model path> [<video path> / camera index {0}]\n";
        return -1;
    }

    const char* modelsDirectory = argv[1];
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

    wrnch::PoseParams params;
    params.SetBoneSensitivity(wrSensitivity_HIGH);
    params.SetJointSensitivity(wrSensitivity_HIGH);
    params.SetEnableTracking(true);

    // Set network input resolution.
    // Different resolutions may impact performace depending on the size and aspect ratio of the
    // input image. The values used here correspond to a 16:9 aspect ratio.
    //
    // Note: If these parameters are not set, the default resolution for the model will be used
    // Note: On CPU-Only build, setting these values has no effect on a resulting estimator
    params.SetPreferredNetWidth2d(328);
    params.SetPreferredNetHeight2d(184);

    // Default skeleton for pose2d is j20 but j23 provides better bone pairs for visualization
    // See the wrJointDefinitions_sample which prints the available definitions, joints and bone
    // pairs
    wrnch::JointDefinition outputFormat = wrnch::JointDefinitionRegistry::Get("j23");

    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };
    configParams.WithPoseParams(params).WithOutputFormat(outputFormat);

    if (!licenseString.empty())
    {
        configParams.WithLicenseString(licenseString);
    }

    wrnch::PoseEstimator poseEstimator;

    // Initialization and frame processing may throw various exceptions.
    // For the noexcept version, use the appropriate overloads which accept a wrReturnCode
    std::cout << "Initializing networks...\n";
    try
    {
        poseEstimator.Initialize(configParams);
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
    unsigned const numBones = outputFormat.GetNumBones();
    std::vector<unsigned int> bonePairs(numBones * 2);
    outputFormat.GetBonePairs(bonePairs.data());

    // Estimate mask for greenscreen
    wrnch::PoseEstimatorOptions poseOptions;
    poseOptions.SetEstimateMask(true);

    Visualizer visualizer;

    bool displayGreenScreen = false;
    cv::Mat currentFrame;

    const unsigned int numJoints = poseEstimator.GetHuman2DOutputFormat().GetNumJoints();

    do
    {
        cap >> currentFrame;
        if (!currentFrame.empty())
        {
            visualizer.SetFrame(currentFrame);
            try
            {
                poseEstimator.ProcessFrame(
                    currentFrame.data, currentFrame.cols, currentFrame.rows, poseOptions);
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

            // Draw joint positions and skeleton while iterating over 2d human poses
            // with a C++11 (or greater) compiler, one can instead iterate over poses with:
            /*
              for (auto pose : poseEstimator.Humans2d())
              {
                  // do something with pose
              }
             */
            for (wrnch::PoseEstimator::Humans2dIter it = poseEstimator.Humans2dBegin();
                 it < poseEstimator.Humans2dEnd();
                 it++)
            {
                wrnch::Pose2dView pose = *it;
                visualizer.DrawPoints(pose.GetJoints(), numJoints);
                visualizer.DrawLines(pose.GetJoints(), &bonePairs[0], numBones);
            }

            // Draw green screen from mask
            if (displayGreenScreen)
            {
                int maskHeight, maskWidth, maskDepth;
                poseEstimator.GetMaskDims(maskWidth, maskHeight, maskDepth);

                const unsigned char* maskData = poseEstimator.GetMaskView();

                visualizer.DrawMask(maskData, maskWidth, maskHeight);
            }
            visualizer.DrawTextInFrame("Toggle greenscreen (g)", 0, visualizer.GetHeight());

            visualizer.Show();
        }

        char key = visualizer.WaitKeyboardInput(1);
        if (key == 'g')
        {
            displayGreenScreen = !displayGreenScreen;
        }

        if (key == 27)  // esc key pressed
        {
            break;
        }
    } while (true);

    return 0;
}
