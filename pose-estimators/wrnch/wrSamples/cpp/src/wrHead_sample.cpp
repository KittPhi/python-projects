/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator to detect human heads in images and access
 poses, orientation, and facial features

 Key concepts:
    Accessing head pose data
    PoseEstimatorOptions -- Estimate heads
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
        std::cerr << "Usage: wrHead_sample_cpp <model path> [<video path> / camera index {0}]\n";
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

    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };

    if (!licenseString.empty())
    {
        configParams.WithLicenseString(licenseString);
    }

    std::cout << "Initializing networks...\n";
    try
    {
        // Create and initialize the pose estimator from the configuration
        wrnch::PoseEstimator poseEstimator;
        poseEstimator.Initialize(configParams);  // May throw

        // Initialize the head pose model and set head pose estimation
        poseEstimator.InitializeHead(modelsDirectory);
        wrnch::PoseEstimatorOptions poseOptions;
        poseOptions.SetEstimateHead(true);
        poseOptions.SetEstimatePoseFace(true);

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

        wrnch::JointDefinition faceOutputFormat = poseEstimator.GetFaceOutputFormat();

        // Pairs of indices into the returned pose data for drawing bones
        std::vector<unsigned int> bonePairs(faceOutputFormat.GetNumBones() * 2);
        faceOutputFormat.GetBonePairs(bonePairs.data());

        const unsigned int numJoints = faceOutputFormat.GetNumJoints();

        Visualizer visualizer;

        cv::Mat currentFrame;
        do
        {
            cap >> currentFrame;
            if (!currentFrame.empty())
            {
                visualizer.SetFrame(currentFrame);

                // May throw
                poseEstimator.ProcessFrame(
                    currentFrame.data, currentFrame.cols, currentFrame.rows, poseOptions);

                std::cout << "Inference done! Found " << poseEstimator.GetNumHeads()
                          << " head(s) and " << poseEstimator.GetNumPoseFace() << " face(s)\n";

                // Draw bounding boxes
                for (wrnch::PoseEstimator::PoseHeadIter it = poseEstimator.PoseHeadsBegin();
                     it < poseEstimator.PoseHeadsEnd();
                     it++)
                {
                    wrnch::PoseHeadView pose = *it;
                    wrnch::Box3dView boundingBox = pose.GetFaceBoundingBox();
                    visualizer.DrawBox(boundingBox.GetMinX(),
                                       boundingBox.GetMinY(),
                                       boundingBox.GetWidth(),
                                       boundingBox.GetHeight(),
                                       WRNCH_BLUE,
                                       2.f);
                }

                // Draw facial feature landmarks and an arrow which shows facial orientation
                for (wrnch::PoseEstimator::PoseFaceIter it = poseEstimator.PoseFacesBegin();
                     it < poseEstimator.PoseFacesEnd();
                     it++)
                {
                    wrnch::PoseFaceView face = *it;
                    visualizer.DrawPoints(face.GetLandmarks(), numJoints, WRNCH_BLUE, 2.0f);
                    visualizer.DrawLines(face.GetLandmarks(),
                                         &bonePairs[0],
                                         faceOutputFormat.GetNumBones(),
                                         WRNCH_BLUE,
                                         1);

                    wrnch::ArrowView arrow = face.GetFaceArrow();
                    visualizer.DrawArrow(
                        arrow.GetBaseX(), arrow.GetBaseY(), arrow.GetTipX(), arrow.GetTipY());
                }

                visualizer.Show();
            }

            if (visualizer.WaitKeyboardInput(1) == 27)
            {
                break;
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
