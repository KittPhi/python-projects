/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator data and a simple heuristic to detect waving
 hands.  The implementation of the detector is in HandWaveDetector.hpp

 Key concepts:
    Accessing 2D pose data
    Using 2D pose data to detect gestures
    Frame processing
*/

#include <iostream>
#include <exception>
#include <map>
#include <ctime>

#include <wrnch/engine.hpp>
#include <Visualizer.hpp>
#include <SampleUtils.hpp>

#include <HandWaveDetector.hpp>

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
    std::string wavingTag("Waving");

    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: wrGesture_sample_cpp <model path> [<video path> / camera index {0}]\n";
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

    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };

    if (!licenseString.empty())
    {
        configParams.WithLicenseString(licenseString);
    }

    // Initialization and frame processing may throw various exceptions.
    // For the noexcept version, use the appropriate overloads which accept a wrReturnCode
    std::cout << "Initializing networks...\n";
    try
    {
        // Create and initialize the pose estimator with selected options
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

    Visualizer visualizer;

    bool keepRunning = true;

    cv::Mat currentFrame;

    std::vector<wrnch::Pose2d> personsFound(poseEstimator.GetMaxPersons(), wrnch::Pose2d(0));
    std::map<int, HandWaveDetector> waveDetectors;

    wrnch::JointDefinition outputFormat = poseEstimator.GetHuman2DOutputFormat();
    int leftWristIdx = outputFormat.GetJointIndex("LWRIST");
    int rightWristIdx = outputFormat.GetJointIndex("RWRIST");

    std::clock_t lastFrameTime = std::clock();
    wrnch::PoseEstimatorOptions poseOptions;  // default pose options
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

            unsigned int numPersons
                = poseEstimator.GetHumans2DLimited(personsFound.data(), personsFound.size());
            std::cout << "Inference done! Found " << numPersons << " person(s)\n";

            std::clock_t currentTime = std::clock();
            double deltaT = std::difftime(currentTime, lastFrameTime) / CLOCKS_PER_SEC;
            lastFrameTime = currentTime;

            // Calculate some good sizes for text in the visualization
            float visScale = currentFrame.rows / 720.f;
            float textSize = 4.f * visScale;
            float textThickness = 3.f * visScale;
            int baseline;
            cv::Size tagSize = getTextSize(std::string("Waving"),
                                           cv::FONT_HERSHEY_PLAIN,
                                           textSize,
                                           (int) textThickness,
                                           &baseline);

            for (unsigned int ix = 0; ix < numPersons; ++ix)
            {
                const wrnch::Pose2d& pose = personsFound[ix];
                int id = pose.GetId();

                // Update the hand wave detectors and process new frames
                auto lowerBound = waveDetectors.lower_bound(id);
                if (lowerBound == waveDetectors.end()
                    || waveDetectors.key_comp()(id, lowerBound->first))
                {
                    lowerBound = waveDetectors.insert(
                        lowerBound, std::make_pair(id, HandWaveDetector(outputFormat)));
                }

                lowerBound->second.Update(pose, deltaT);

                const float* humanJoints = pose.GetJoints();

                // Draw a tag on any waving hands found
                if (lowerBound->second.LeftWaveDetected())
                {
                    visualizer.DrawTextInFrame(
                        wavingTag,
                        humanJoints[2 * leftWristIdx] * currentFrame.cols - tagSize.width / 2,
                        humanJoints[2 * leftWristIdx + 1] * currentFrame.rows + tagSize.height,
                        RED,
                        textSize,
                        textThickness);
                }
                if (lowerBound->second.RightWaveDetected())
                {
                    visualizer.DrawTextInFrame(
                        wavingTag,
                        humanJoints[2 * rightWristIdx] * currentFrame.cols - tagSize.width / 2,
                        humanJoints[2 * rightWristIdx + 1] * currentFrame.rows + tagSize.height,
                        RED,
                        textSize,
                        textThickness);
                }
            }

            visualizer.Show();
        }

        char key = visualizer.WaitKeyboardInput(1);
        if (key == 27)  // esc key pressed
        {
            keepRunning = false;
        }
    } while (keepRunning);

    return 0;
}
