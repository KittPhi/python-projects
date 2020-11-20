/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
  This sample demonstrates the use of wrnch pose-based gesture detection using a C API.
 */

#include <wrnch/engine.h>
#include "wrOpenCvHelpers.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char const* const activityModelName = "wrnch_activity_v1.0.enc";
static int const defaultNetWidth = 328;
static int const defaultNetHeight = 184;

static wrReturnCode BuildModels(char const* modelsDirectory,
                                char const* licenseKey,
                                wrActivityModelHandle* outLeftModel,
                                wrActivityModelHandle* outRightModel,
                                wrPoseEstimatorHandle* outPoseEstimator,
                                wrPoseEstimatorOptionsHandle* outOptions);
static char* ActivityModelPath(char const* modelsDirectory);

typedef enum
{
    Side_LEFT = 0,
    Side_RIGHT = 1
} Side;

static void DisplayResultsForModel(wrActivityModelHandleConst activityModel,
                                   wrPoseEstimatorHandleConst poseEstimator,
                                   Side side);
static char const** GetClassNames(wrActivityModelHandleConst activityModel);
static void PrintClassNames(char const* const* classNames, int numClasses);

int main(int const argc, char const** argv)
{
    wrCvMatHandle mat;
    wrActivityModelHandle leftModel, rightModel;
    wrPoseEstimatorHandle poseEstimator;
    wrPoseEstimatorOptionsHandle poseOptions;
    wrReturnCode returnCode;
    const char* imageFilename;
    const char* modelsDirectory;
    const char* licenseKey = NULL;

    if (argc < 3 || argc > 4)
    {
        printf("Usage: activities_sample_c <model path> <image path> [license key]\n");
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

    if ((returnCode
         = BuildModels(modelsDirectory, licenseKey, &leftModel, &rightModel, &poseEstimator, &poseOptions))
        != wrReturnCode_OK)
    {
        printf("%s\nError building models\n", wrReturnCode_Translate(returnCode));
        return -1;
    }

    int const imageWidth = wrCvMat_GetCols(mat);
    int const imageHeight = wrCvMat_GetRows(mat);

    wrPoseEstimator_ProcessFrame(
        poseEstimator, wrCvMat_GetData(mat), imageWidth, imageHeight, poseOptions);
    wrActivityModel_ProcessPoses(leftModel, poseEstimator, imageWidth, imageHeight);
    wrActivityModel_ProcessPoses(rightModel, poseEstimator, imageWidth, imageHeight);

    DisplayResultsForModel(leftModel, poseEstimator, Side_LEFT);
    DisplayResultsForModel(leftModel, poseEstimator, Side_RIGHT);

    wrPoseEstimatorOptions_Destroy(poseOptions);
    wrPoseEstimator_Destroy(poseEstimator);
    wrActivityModel_Destroy(leftModel);
    wrActivityModel_Destroy(rightModel);
    wrCvMat_Release(mat);

    return 0;
}

wrReturnCode BuildModels(char const* modelsDirectory,
                         char const* licenseKey,
                         wrActivityModelHandle* const outLeftModel,
                         wrActivityModelHandle* const outRightModel,
                         wrPoseEstimatorHandle* const outPoseEstimator,
                         wrPoseEstimatorOptionsHandle* const outOptions)
{
    wrActivityModelBuilderHandle builder;
    char* activityModelPath;
    wrReturnCode retCode;
    wrPoseEstimatorRequirementsHandle poseRequirements;

    activityModelPath = ActivityModelPath(modelsDirectory);

    if (!activityModelPath)
    {
        return wrReturnCode_BAD_ALLOC;
    }

    retCode = wrActivityModelBuilder_Create(activityModelPath, &builder);
    if (retCode != wrReturnCode_OK)
    {
        free((void*) activityModelPath);
        return retCode;
    }

    retCode = wrActivityModelBuilder_Build(builder, outLeftModel);
    if (retCode != wrReturnCode_OK)
    {
        free((void*) activityModelPath);
        wrActivityModelBuilder_Destroy(builder);
        return retCode;
    }

    printf("Creating activity models ...\n");
    retCode = wrActivityModelBuilder_BuildReflected(builder, outRightModel);
    if (retCode != wrReturnCode_OK)
    {
        wrActivityModel_Destroy(*outLeftModel);
        free((void*) activityModelPath);
        wrActivityModelBuilder_Destroy(builder);
        return retCode;
    }

    retCode = wrActivityModelBuilder_PoseEstimatorRequirements(builder, &poseRequirements);
    if (retCode != wrReturnCode_OK)
    {
        wrActivityModel_Destroy(*outRightModel);
        wrActivityModel_Destroy(*outLeftModel);
        free((void*) activityModelPath);
        wrActivityModelBuilder_Destroy(builder);
        return retCode;
    }

    printf("done creating activity models.\nCreating pose estimator (this is slow) ...\n");
    retCode = wrPoseEstimatorRequirements_CreateCompatibleEstimator(poseRequirements,
                                                                    outPoseEstimator,
                                                                    modelsDirectory,
                                                                    licenseKey,
                                                                    /*deviceId*/ 0,
                                                                    defaultNetWidth,
                                                                    defaultNetHeight);
    if (retCode != wrReturnCode_OK)
    {
        wrPoseEstimatorRequirements_Destroy(poseRequirements);
        wrActivityModel_Destroy(*outRightModel);
        wrActivityModel_Destroy(*outLeftModel);
        free((void*) activityModelPath);
        wrActivityModelBuilder_Destroy(builder);
        return retCode;
    }
    printf("done creating pose estimator\n");

    *outOptions = wrPoseEstimatorRequirements_CreateCompatibleOptions(poseRequirements);
    if (*outOptions == NULL)
    {
        wrPoseEstimator_Destroy(*outPoseEstimator);
        wrPoseEstimatorRequirements_Destroy(poseRequirements);
        wrActivityModel_Destroy(*outRightModel);
        wrActivityModel_Destroy(*outLeftModel);
        free((void*) activityModelPath);
        wrActivityModelBuilder_Destroy(builder);
        return retCode;
    }

    free((void*) activityModelPath);
    wrActivityModelBuilder_Destroy(builder);

    return wrReturnCode_OK;
}

char* ActivityModelPath(char const* const modelsDirectory)
{
    long unsigned const lenModelsDirectoryName = strlen(modelsDirectory);
    char* res = (char*) malloc(sizeof(char)
                               * (lenModelsDirectoryName + /*for slash separator*/ 1
                                  + strlen(activityModelName) + /*for null char*/ 1));

    if (!res) {
        return NULL;
    }

    strcpy(res, modelsDirectory);
    res[lenModelsDirectoryName] = '/';
    strcpy(res + lenModelsDirectoryName + 1, activityModelName);

    return res;
}

char const** GetClassNames(wrActivityModelHandleConst const activityModel)
{
    char const** classNames
        = (char const**) malloc(sizeof(char*) * wrActivityModel_NumClasses(activityModel));
    assert(classNames != NULL);

    wrActivityModel_ClassNames(activityModel, classNames);

    return classNames;
}

void PrintClassNames(char const* const* const classNames, int const numClasses)
{
    int index;

    for (index = 0; index < numClasses; ++index)
    {
        printf("%s\n", classNames[index]);
    }
}

void DisplayResultsForModel(wrActivityModelHandleConst const activityModel,
                            wrPoseEstimatorHandleConst const poseEstimator,
                            Side side)
{
    int numClasses, argMax, personId, index;
    wrPose2dHandleConst poseIter;
    wrIndividualActivityModelHandleConst individualActivityModel;
    float const* probabilities;
    char const** classNames;

    numClasses = wrActivityModel_NumClasses(activityModel);
    assert(numClasses > 0);

    printf("Activity Submodel %d:\nClass names:\n", side);

    classNames = GetClassNames(activityModel);
    PrintClassNames(classNames, numClasses);
    printf("\n");

    for (poseIter = wrPoseEstimator_GetHumans2DBegin(poseEstimator);
         poseIter != wrPoseEstimator_GetHumans2DEnd(poseEstimator);
         poseIter = wrPoseEstimator_GetPose2DNext(poseIter))
    {
        personId = wrPose2d_GetId(poseIter);

        individualActivityModel = wrActivityModel_IndividualModel(activityModel, personId);
        assert(individualActivityModel != NULL);

        printf("for activity submodel %d, person with personId %d, estimated class probabilities "
               "are:\n[ ",
               side,
               personId);

        probabilities = wrIndividualActivityModel_Probabilities(individualActivityModel);

        argMax = 0;
        for (index = 0; index < numClasses; ++index)
        {
            printf("%f ", probabilities[index]);

            if (probabilities[index] > probabilities[argMax])
            {
                argMax = index;
            }
        }

        printf("]\n");
        printf("Max probability at index %d of %f\n", argMax, probabilities[argMax]);
        printf("Predicted gesture: %s\n\n", classNames[argMax]);
    }

    free(classNames);
}
