/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of serialization/deserialization of PoseEstimators.

 First see wrPose2d_sample.cpp, which shares much of the same code with here.

 Key concepts:
    Serializing/deserializing a PoseEstimator
*/

#include <iostream>
#include <exception>
#include <chrono>

#include <wrnch/engine.hpp>

int main(int argc, const char* const* argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: wrSerialzation_sample_cpp <model path> [<license key>]\n";
        return -1;
    }

    char const* const modelsDirectory = argv[1];
    std::string licenseKey;

    wrReturnCode licenseCheckCode;
    if (argc == 3)
    {
        licenseKey = std::string(argv[2]);
        licenseCheckCode = wrnch::LicenseCheckString(licenseKey);
    }
    else
    {
        licenseCheckCode = wrnch::LicenseCheck();
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

    // Note: On CPU-Only build, setting these values has no effect on a resulting estimator
    params.SetPreferredNetWidth2d(328);
    params.SetPreferredNetHeight2d(184);

    wrnch::JointDefinition const outputFormat = wrnch::JointDefinitionRegistry::Get("j23");

    int const gpuIndex = 0;

    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };
    configParams.WithPoseParams(params).WithOutputFormat(outputFormat).WithDeviceId(gpuIndex);

    if (!licenseKey.empty())
    {
        configParams.WithLicenseString(licenseKey);
    }

    std::cout << "Initializing networks... (slow innit\?\?)\n";
    auto const initializationStartTime = std::chrono::high_resolution_clock::now();
    try
    {
        poseEstimator.Initialize(modelsDirectory, gpuIndex, params, outputFormat);
        /*
          initialize hands model just for fun. A deserialized PoseEstimator created by deserializing
          this poseEstimator should automatically have hands model initialized.
         */
        poseEstimator.InitializeHands2D(modelsDirectory);
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
    auto const initializationEndTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> const initializationTime
        = initializationEndTime - initializationStartTime;
    std::cout << "Initialization done. Took " << initializationTime.count() << " seconds.\n";

    /*
      Serializing/deserializing a poseEstimator. Such a serialized estimator can be persisted to
      disk. Note the speed of initializing a poseEstimator using wrnch::PoseEstimator::Deserialize
      vs the previous initialization.
     */
    std::cout << "serializing PoseEstimator to memory ...\n";
    try
    {
        wrnch::SerializedData serializedEstimator = poseEstimator.Serialize();
        std::cout << "done serializing PoseEstimator to memory.\nDeserializing to a fresh pose "
                     "estimator ... \n";
        auto const deserializationStartTime = std::chrono::high_resolution_clock::now();
        wrnch::PoseEstimator clonedPoseEstimator = wrnch::PoseEstimator::Deserialize(
            serializedEstimator.Data(), serializedEstimator.NumBytes(), gpuIndex);
        auto const deserializationEndTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> const deserializationTime
            = deserializationEndTime - deserializationStartTime;
        std::cout << "done deserializing. Took " << deserializationTime.count()
                  << " seconds. Compare that to initializing the other way!\n";

        if (clonedPoseEstimator == nullptr)
        {
            std::cerr << "unexpected error while deserializing a PoseEstimator\n";
            return -1;
        }

        /*
          All properties on the original serialized PoseEstimator should hold for the deserialized
          version. Here we check a few.
         */
        if (!clonedPoseEstimator.AreHandsInitialized())
        {
            std::cerr << "expected clonedPoseEstimator to have initialized hand models.\n";
            return -1;
        }
        if (clonedPoseEstimator.Is3dInitialized())
        {
            std::cerr << "clonedPoseEstimator unexpectedly had initialized 3d model.\n";
            return -1;
        }
        if (clonedPoseEstimator.IsHeadInitialized())
        {
            std::cerr << "clonedPoseEstimator unexpectedly had initialized head model.\n";
            return -1;
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
