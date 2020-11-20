/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator to detect humans in images and access
 3D pose data post-IK, including joint rotations.

 The joint rotations are visualized as a rotation of a coordinate system with respect to the T-pose.
 The 3D points returned by the pose estimator can be rotated using the m,n keys.

 Key concepts:
    Accessing 3D pose data
    PoseEstimatorOptions -- Estimate 3D
    Frame processing
    Accessing rotations defined as a quaternion x,y,z,w
    Vizualizing return of GetRotations()
    Visualizing return of GetPositions()
*/
#include <iostream>
#include <exception>
#include <vector>
#include <string>

#include <wrnch/engine.hpp>
#include <Visualizer.hpp>
#include <Quaternion.hpp>
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

namespace
{
std::vector<float> scalePositions(const float* positions, int numJoints2D)
{
    // get min and max positions for x,y,z
    float minPos[] = { 1000.0, 1000.0, 1000.0 };
    float maxPos[] = { -1000.0, -1000.0, -1000.0 };
    float scaleFactor[3];
    for (int d = 0; d < 3; d++)
    {
        for (int i = d; i < numJoints2D * 3; i += 3)
        {
            if (positions[i] < minPos[d])
            {
                minPos[d] = positions[i];
            }
            if (positions[i] > maxPos[d])
            {
                maxPos[d] = positions[i];
            }
        }
        scaleFactor[d] = (maxPos[d] - minPos[d]);
    }

    float xyScaleFactor = (scaleFactor[1] > scaleFactor[0]) ? scaleFactor[1] : scaleFactor[0];
    std::vector<float> scaledPos;
    scaledPos.reserve(numJoints2D * 3);
    for (int i = 0; i < numJoints2D * 3; i += 3)
    {
        scaledPos.push_back(0.5 + (positions[i] - (minPos[0] + maxPos[0]) * 0.5) / xyScaleFactor);
        scaledPos.push_back(1.0 - (positions[i + 1] - minPos[1]) / xyScaleFactor);
        scaledPos.push_back(0.5 * (1 - (positions[i + 2] - minPos[2]) / scaleFactor[2]));
    }

    return scaledPos;
}
}

int main(int argc, const char* const* argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cout
            << "Usage: wrPose3dPostIK_sample_cpp <model path> [<video path> / camera index {0}]\n";
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
    wrnch::JointDefinition input2DJointDefinition = wrnch::JointDefinitionRegistry::Get("j23");
    int gpuIndex = 0;

    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };
    configParams.WithPoseParams(params)
        .WithOutputFormat(input2DJointDefinition)
        .WithDeviceId(gpuIndex);

    if (!licenseString.empty())
    {
        configParams.WithLicenseString(licenseString);
    }

    std::cout << "Initializing networks...\n";
    try
    {
        poseEstimator.Initialize(configParams);
        poseEstimator.Initialize3D(modelsDirectory);
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

    // Get joint  output format for 3-d post-IK
    wrnch::JointDefinition outputFormat = poseEstimator.GetHuman3DOutputFormat();
    outputFormat.PrintJointDefinition();

    wrnch::PoseEstimatorOptions poseOptions;
    poseOptions.SetEstimateMask(false);
    poseOptions.SetEstimate3D(true);
    poseOptions.SetUseIK(true);
    poseOptions.SetEstimateHands(false);
    poseOptions.SetMainPersonId(wrnch_MAIN_ID_LARGEST);

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

    const unsigned int numJoints = outputFormat.GetNumJoints();
    std::cout << outputFormat.GetName() << " numJoints:" << numJoints << std::endl;
    // These joint pairs are just for visualization purposes. They represent pairs of joints
    // on which to draw connections to represent a skeleton.

    unsigned int const numBones = outputFormat.GetNumBones();
    std::vector<unsigned int> visualizationBonePairs(numBones * 2);
    outputFormat.GetBonePairs(visualizationBonePairs.data());

    unsigned int visualizationJoints = visualizationBonePairs.size() / 2;

    Visualizer visualizer;

    cv::Mat currentFrame;
    int rotationAngleY = 0;
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

            for (wrnch::Pose3dView pose : poseEstimator.Humans3d())
            {
                const float* rotations = pose.GetRotations();
                const float* positions = pose.GetPositions();
                std::vector<float> scaledPos = scalePositions(positions, numJoints);
                visualizer.DrawLines3D(scaledPos.data(),
                                       numJoints,
                                       visualizationBonePairs,
                                       visualizationJoints,
                                       0,
                                       rotationAngleY,
                                       0);
                std::vector<float> x_axes;
                std::vector<float> y_axes;
                std::vector<float> z_axes;
                // increment rot index by 4 since x,y,z,w
                for (unsigned int rot_idx = 0; rot_idx < numJoints * 4; rot_idx += 4)
                {
                    Quaternion currentQ(rotations[rot_idx],
                                        rotations[rot_idx + 1],
                                        rotations[rot_idx + 2],
                                        rotations[rot_idx + 3]);
                    float x_axis[3] = { 1, 0, 0 };
                    float y_axis[3] = { 0, 1, 0 };
                    float z_axis[3] = { 0, 0, 1 };
                    // rotate coordinate system based on quaternion returned by poseEstimator
                    currentQ.rotate(x_axis);
                    currentQ.rotate(y_axis);
                    currentQ.rotate(z_axis);
                    // rotate coordinate system based on user input
                    float rotationAngleYRad = detail::deg2Rad(rotationAngleY);
                    Quaternion userQ(0, sin(rotationAngleYRad / 2), 0, cos(rotationAngleYRad / 2));
                    userQ.rotate(x_axis);
                    userQ.rotate(y_axis);
                    userQ.rotate(z_axis);

                    x_axes.push_back(x_axis[0]);
                    x_axes.push_back(x_axis[1]);
                    y_axes.push_back(y_axis[0]);
                    y_axes.push_back(y_axis[1]);
                    z_axes.push_back(z_axis[0]);
                    z_axes.push_back(z_axis[1]);
                }
                visualizer.DrawArrows3D(
                    scaledPos.data(), numJoints, x_axes, y_axes, z_axes, 0, rotationAngleY, 0);
            }

            visualizer.FlipFrame();
            visualizer.DrawTextInFrame(
                "Rotate image about y axis clockwise (m), counter-clockwise (n)",
                0,
                visualizer.GetHeight());
            visualizer.Show();
        }  // if !currentFrame.empty()

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


    return 0;
}
