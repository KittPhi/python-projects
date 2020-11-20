/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use wrnch::ActivityModel to predict pose-based human activities.

Key concepts: activity prediction
*/

#include <wrnch/engine.hpp>
#include <Visualizer.hpp>
#include <SampleUtils.hpp>

#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
enum class Side : bool
{
    Left,
    Right
};
}

static void AnnotatePredictedClasses(Visualizer& visualizer,
                                     Side side,
                                     float const* classProbabilities,
                                     std::vector<std::string> const& classNames,
                                     wrnch::Pose2dView humanPose);

static std::pair<wrnch::ActivityModel, wrnch::ActivityModel> CreateModels(
    std::string const& modelsDirectory,
    std::string const& licenseKey,
    wrnch::PoseEstimator& poseEstimator,
    wrnch::PoseEstimatorOptions& poseOptions);

static std::vector<unsigned int> ComputeBonePairs(wrnch::JointDefinition humanJointFormat);

int main(int const argc, char const* const* const argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr
            << "Usage: wrActivities_sample_cpp <model path> [<video path> / camera index {0}]\n";
        return -1;
    }
    const char* const modelsDirectory = argv[1];
    const std::string inputVideo = (argc == 3) ? argv[2] : "0";
    std::cout << "Opening media\n";

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

    std::cout << "Initializing networks (this may take some time) ...\n";

    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorOptions poseOptions;

    try
    {
        std::pair<wrnch::ActivityModel, wrnch::ActivityModel> activityModels
            = CreateModels(modelsDirectory, licenseString, poseEstimator, poseOptions);

        std::cout << "Initialization done !\n";

        wrnch::JointDefinition const poseEstimatorHumanJointFormat
            = poseEstimator.GetHuman2DOutputFormat();

        unsigned const numBones = poseEstimatorHumanJointFormat.GetNumBones();

        std::vector<unsigned int> const bonePairs = ComputeBonePairs(poseEstimatorHumanJointFormat);

        std::vector<std::string> const classNames = activityModels.first.ClassNames();

        Visualizer visualizer;
        cv::Mat currentFrame;
        unsigned int const numJoints = poseEstimator.GetHuman2DOutputFormat().GetNumJoints();

        do
        {
            cap >> currentFrame;
            if (!currentFrame.empty())
            {
                visualizer.SetFrame(currentFrame);
                try
                {
                    int const imageWidth = currentFrame.cols;
                    int const imageHeight = currentFrame.rows;
                    poseEstimator.ProcessFrame(
                        currentFrame.data, imageWidth, imageHeight, poseOptions);
                    activityModels.first.ProcessPoses(poseEstimator, imageWidth, imageHeight);
                    activityModels.second.ProcessPoses(poseEstimator, imageWidth, imageHeight);
                }
                catch (std::exception const& e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return -1;
                }
                catch (...)
                {
                    std::cerr << "Unknown error occurred" << std::endl;
                    return -1;
                }

                for (wrnch::Pose2dView const pose : poseEstimator.Humans2d())
                {
                    visualizer.DrawPoints(pose.GetJoints(), numJoints);
                    visualizer.DrawLines(pose.GetJoints(), &bonePairs[0], numBones);

                    int const personId = pose.GetId();

                    for (Side const side : { Side::Left, Side::Right })
                    {
                        bool const isLeft = side == Side::Left;
                        wrnch::ActivityModel const& activityModel
                            = isLeft ? activityModels.first : activityModels.second;

                        /*
                          individualModel stores the estimated class probabilities for the person
                          with id `personId`, under the ActivityModel `activityModel` (which may
                          either be the "unreflected" ActivityModel
                          `activityModels.first`, or the "reflected" ActivityModel
                          `activityModels.second`.
                         */
                        wrnch::IndividualActivityModelView const individualModel
                            = activityModel.IndividualModel(personId);

                        assert(individualModel != nullptr);

                        float const* const classProbabilities = individualModel.Probabilities();

                        AnnotatePredictedClasses(
                            visualizer, side, classProbabilities, classNames, pose);
                    }
                }

                visualizer.Show();
            }

            char const key = visualizer.WaitKeyboardInput(1);

            if (key == 27 || key == 'q')
            {
                break;
            }
        } while (true);
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return -1;
    }

    return 0;
}

std::pair<wrnch::ActivityModel, wrnch::ActivityModel> CreateModels(
    std::string const& modelsDirectory,
    std::string const& licenseKey,
    wrnch::PoseEstimator& poseEstimator,
    wrnch::PoseEstimatorOptions& poseOptions)
{
    std::string const activityModelPath{ std::string(modelsDirectory)
                                         + "/wrnch_activity_v1.0.enc" };

    /*
      Builders are how we create `ActivityModel` instances
     */
    wrnch::ActivityModel::Builder builder(activityModelPath);

    std::pair<wrnch::ActivityModel, wrnch::ActivityModel> result = std::make_pair(
        /*
          the activity model wrnch_activity_v1.0.enc predicts 7 classes:
          [(left) wave, (left) stop, (left) come here, (left) fist pump]

          See wrnch_activity_v1.0.md for example gifs of these gestures.
         */

        /*
          Add a submodel to predict gestures on the left side of the body.
         */
        builder.Build(),
        /*
          Add a reflected submdel to predict gestures on the right side of the body.

          Due to the reflection, the predicted gestures of this submodel are effectively:
          * (right) Fist Pump
          * (right) Wave
          * (right) Come here
          * (right) Stop
          * Clap
          * Summon

          We say "effectively" because some of the strings in ClassNames() returned by
          this submodel will still have "left" in them.
         */
        builder.BuildReflected());

    /*
      NOTE: we've added two submodels, both of which predict "clap" and "summon". All other
      gestures predicted are unique to a side. How is one to predict Clap and Summon then?

      If one is only interested in detecting the presence of a clap or summon gesture (and not
      in their probabilities), then we can ask either submodel, or use their conjunction (eg
      predict clap if both submodels predict clap). It is not necessarily the
      case that both submodels will predict clap or summon simultaneously, though in our
      experience we find they often do agree.

      If one is interested in the probability of a clap or summon, then things are not so clear:
      we have two measures of probability for certain classes, and it's not clear how to combine
      them. One can use either submodel as a source of probabilitiy, or one can attempt to
      produce a fusion of the two probability vectors, predicting probabilities for the 11
      gestures: [ None, {left|right} fist pump, {left|right} wave, {left|right} come here,
      {left|right} stop, clap, summon]
     */

    /*
      For best results in this sample, the video input should have approximate aspect ratio of
      width/height ~= 328 / 184.

      For very different aspect ratios (eg, a "portrait" webcam), these numbers can be changed.
     */
    int const preferredPoseNetWidth{ 328 };
    int const preferredPoseNetHeight{ 184 };

    wrnch::ActivityPoseEstimatorRequirements requirements = builder.PoseEstimatorRequirements();

    poseEstimator = requirements.CreateCompatibleEstimator(modelsDirectory,
                                                           /*deviceId*/ 0,
                                                           preferredPoseNetWidth,
                                                           preferredPoseNetHeight,
                                                           licenseKey);

    poseOptions = requirements.CreateCompatibleOptions();

    assert(poseEstimator != nullptr);

    return result;
}

void AnnotatePredictedClasses(Visualizer& visualizer,
                              Side const side,
                              float const* const classProbabilities,
                              std::vector<std::string> const& classNames,
                              wrnch::Pose2dView const humanPose)
{
    std::string const& predictedClassName{
        classNames[std::max_element(classProbabilities, classProbabilities + classNames.size())
                   - classProbabilities]
    };

    float const* const humanJoints{ humanPose.GetJoints() };

    bool const isLeft = side == Side::Left;

    std::string const text{ (isLeft ? "Left" : "Right") + std::string(": ") + predictedClassName };

    cv::Size const textSize = visualizer.GetTextSize(text);
    int const verticalOffsetForText = isLeft ? 0 : textSize.height + 5;

    visualizer.DrawTextInFrame(text,
                               static_cast<int>(humanJoints[0] * visualizer.Frame().cols),
                               static_cast<int>(humanJoints[1] * visualizer.Frame().rows)
                                   + verticalOffsetForText);
}

std::vector<unsigned int> ComputeBonePairs(wrnch::JointDefinition humanJointFormat)
{
    unsigned const numBones = humanJointFormat.GetNumBones();
    std::vector<unsigned int> result(numBones * 2);
    humanJointFormat.GetBonePairs(result.data());
    return result;
}
