/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample demonstrates the use of the pose estimator, using the wrnch C-API, to infer the
 presence of humans in an image

 Key concepts:
    Accessing pose data
    Frame processing
*/

#include <wrnch/engine.h>
#include "wrOpenCvHelpers.h"

#include <stdio.h>


int main(int argc, char** argv)
{
    wrCvMatHandle mat;
    wrPoseEstimatorHandle poseEstimator;
    wrPoseEstimatorOptionsHandle options;
    unsigned int numPersons;
    wrReturnCode returnCode;
    const char* imageFilename;
    const char* modelsDirectory;
    const char* licenseKey = NULL;

    if (argc < 3 || argc > 4)
    {
        printf("Usage: pose2d_sample_c <model path> <image path> [license key]\n");
        return -1;
    }

    modelsDirectory = argv[1];
    imageFilename = argv[2];

    wrReturnCode licenseCheckCode;
    if (argc == 4)
    {
        licenseKey = argv[3];
        licenseCheckCode = wrLicense_CheckString(licenseKey);
    }
    else
    {
        licenseCheckCode = wrLicense_Check();
    }

    if (licenseCheckCode != wrReturnCode_OK)
    {
        printf("%s\n", wrReturnCode_Translate(licenseCheckCode));
        return -1;
    }

    if ((returnCode = wrCvMat_LoadImage(imageFilename, &mat)) != wrReturnCode_OK)
    {
        printf(
            "%s\nError loading image at %s\n", wrReturnCode_Translate(returnCode), imageFilename);
        return -1;
    }

    printf("Initializing networks ...\n");

    wrPoseEstimatorConfigParams configData = wrPoseEstimatorConfigParams_Create(modelsDirectory);
    if (licenseKey != NULL)
    {
        wrPoseEstimatorConfigParams_SetLicenseString(configData, licenseKey);
    }

    returnCode = wrPoseEstimator_CreateFromConfig(&poseEstimator, configData);
    wrPoseEstimatorConfigParams_Destroy(configData);

    if (returnCode != wrReturnCode_OK)
    {
        printf("Error creating pose estimator\n%s\n", wrReturnCode_Translate(returnCode));
        return -1;
    }

    printf("Initialization done!\n");

    options = wrPoseEstimatorOptions_Create();

    printf("Inferring ...\n");

    wrPoseEstimator_ProcessFrame(
        poseEstimator, wrCvMat_GetData(mat), wrCvMat_GetCols(mat), wrCvMat_GetRows(mat), options);

    numPersons = wrPoseEstimator_GetNumHumans2D(poseEstimator);

    printf("Inference done! Found %u humans\n", numPersons);

    wrPoseEstimatorOptions_Destroy(options);
    wrPoseEstimator_Destroy(poseEstimator);
    wrCvMat_Release(mat);

    return 0;
}
