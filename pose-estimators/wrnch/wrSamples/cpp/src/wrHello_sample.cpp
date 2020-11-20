/*
Copyright (c) 2018 wrnch Inc.
All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator to infer the presence of humans in an image

 Key concepts:
    Accessing pose data
    No throw pose estimator initialization, processing
    Frame processing
*/

#include <wrnch/engine.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <exception>
#include <iostream>
#include <vector>

int main(int argc, const char* const* argv)
{
    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: wrHello_sample_cpp <model path> <image path> [license key]\n";
        return -1;
    }

    const char* modelsDirectory = argv[1];
    const char* imageFilename = argv[2];
    std::string licenseKey;
    wrReturnCode licenseCheckCode;
    if (argc == 4)
    {
        licenseKey = std::string(argv[3]);
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

    wrnch::PoseEstimatorConfigParams configParams{ modelsDirectory };
    if (!licenseKey.empty())
    {
        configParams.WithLicenseString(licenseKey);
    }

    std::cout << "Initializing networks...\n";

    wrReturnCode status;
    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorOptions poseOptions;

    // Overloads which take a wrReturnCode will not throw exceptions.
    // You must check the value of the code on return and proceed
    // accordingly.
    poseEstimator.Initialize(configParams, status);
    if (status == wrReturnCode_OK)
    {
        std::cout << "Initialization done!\n";

        cv::Mat frame = cv::imread(imageFilename);
        std::cout << "Infering...\n";

        // Frame data must be in 3-channel OpenCV format,
        // i.e., row-major BGR sequence of intensities in the range [0,255]
        poseEstimator.ProcessFrame(frame.data, frame.cols, frame.rows, poseOptions, status);

        if (status == wrReturnCode_OK)
        {
            unsigned int numPersons = poseEstimator.GetNumHumans2D();
            std::cout << "Inference done! Found " << numPersons << " humans\n";

            // Access the available 2D pose data
            std::vector<wrnch::Pose2d> poses(numPersons, wrnch::Pose2d(0));
            poseEstimator.GetAllHumans2D(&poses[0]);
            wrnch::Pose2d& pose_0 = poses[0];
            std::cout << "person zero assigned id " << pose_0.GetId() << "\n";
            return 0;
        }
    }
    // Error occured
    std::cerr << "Error: " << wrnch::ReturnCodeToString(status) << std::endl;
    return -1;
}
