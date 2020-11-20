/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

#pragma once
#include <deque>

#include <wrnch/engine.hpp>
namespace wrSamples
{
class HandWaveDetector
{
  public:
    HandWaveDetector(const wrnch::JointDefinition& jointFormat,
                     unsigned int bufferSize = 5,
                     float xSpeedThreshold = 2.f,
                     float ySpeedThreshold = 6.f,
                     float displacementThreshold = 0.1f,
                     float armRaiseThreshold = 1.4f)
    : m_bufferSize(bufferSize)
    , m_xSpeedThreshold(xSpeedThreshold)
    , m_ySpeedThreshold(ySpeedThreshold)
    , m_displacementThreshold(displacementThreshold)
    , m_armRaiseThreshold(armRaiseThreshold)
    {
        // Gather some needed joint indices
        leftSide.wrist = jointFormat.GetJointIndex("LWRIST");
        leftSide.eye = jointFormat.GetJointIndex("LEYE");
        leftSide.elbow = jointFormat.GetJointIndex("LELBOW");
        rightSide.wrist = jointFormat.GetJointIndex("RWRIST");
        rightSide.eye = jointFormat.GetJointIndex("REYE");
        rightSide.elbow = jointFormat.GetJointIndex("RELBOW");
    }


    void Update(const wrnch::Pose2d& pose, float deltaT)
    {
        ProcessPoseData(pose);

        DetectHandWaves(deltaT);
    }

    bool LeftWaveDetected() const { return m_leftHandIsWaving; }
    bool RightWaveDetected() const { return m_rightHandIsWaving; }


  private:
    // Some helpful data containers
    struct Vec2f
    {
        float x;
        float y;
    };

    // Instead of storing the entire pose we process each pose and extract the needed information
    struct GestureData
    {
        bool leftVisibleAndRaised;
        bool rightVisibleAndRaised;

        // Position of hand relative to eyes
        Vec2f leftRelativePosition;
        Vec2f rightRelativePosition;

        // To scale within the frame we use the distance between the eyes
        float scale;
    };

    // Need joint indices for each side of the body
    struct RelevantJointIndices
    {
        int wrist;
        int eye;
        int elbow;
    };

    // Buffer of poses to track data over time
    std::deque<GestureData> m_poseDataBuffer;

    bool m_isFacing;
    const unsigned int m_bufferSize;
    const float m_xSpeedThreshold;
    const float m_ySpeedThreshold;
    const float m_displacementThreshold;
    const float m_armRaiseThreshold;

    bool m_leftHandIsWaving;
    bool m_rightHandIsWaving;

    RelevantJointIndices leftSide;
    RelevantJointIndices rightSide;

    void DetectHandWaves(double deltaT)
    {
        m_leftHandIsWaving = false;
        m_rightHandIsWaving = false;

        // Wait for a full buffer and person is facing the camera
        if (m_poseDataBuffer.size() == m_bufferSize && m_isFacing)
        {
            bool leftVisible = true;
            bool rightVisible = true;

            // Proceed if either left or right hand is visible
            for (unsigned int ix = 0; ix < m_poseDataBuffer.size(); ++ix)
            {
                GestureData& poseData = m_poseDataBuffer[ix];
                leftVisible &= poseData.leftVisibleAndRaised;
                rightVisible &= poseData.rightVisibleAndRaised;
                if (!leftVisible && !rightVisible)
                    break;
            }

            float scale = m_poseDataBuffer.at(0).scale;

            // For each hand, consider speed relative to the eye against thresholds
            if (leftVisible)
            {
                float xDist = 0;
                float yDist = 0;
                for (unsigned int i = 1; i < m_poseDataBuffer.size(); ++i)
                {
                    xDist += std::abs(m_poseDataBuffer[i - 1].leftRelativePosition.x
                                      - m_poseDataBuffer[i].leftRelativePosition.x);
                    yDist += std::abs(m_poseDataBuffer[i - 1].leftRelativePosition.y
                                      - m_poseDataBuffer[i].leftRelativePosition.y);
                }
                m_leftHandIsWaving = xDist / m_bufferSize * scale > m_displacementThreshold
                                     && xDist / m_bufferSize / deltaT * scale > m_xSpeedThreshold
                                     && yDist / m_bufferSize / deltaT * scale < m_ySpeedThreshold;
            }
            if (rightVisible)
            {
                float xDist = 0;
                float yDist = 0;
                for (unsigned int i = 1; i < m_poseDataBuffer.size(); ++i)
                {
                    xDist += std::abs(m_poseDataBuffer[i - 1].rightRelativePosition.x
                                      - m_poseDataBuffer[i].rightRelativePosition.x);
                    yDist += std::abs(m_poseDataBuffer[i - 1].rightRelativePosition.y
                                      - m_poseDataBuffer[i].rightRelativePosition.y);
                }
                m_rightHandIsWaving = xDist / m_bufferSize * scale > m_displacementThreshold
                                      && xDist / m_bufferSize / deltaT * scale > m_xSpeedThreshold
                                      && yDist / m_bufferSize / deltaT * scale < m_ySpeedThreshold;
            }
        }
    }

    void ProcessPoseData(const wrnch::Pose2d& pose)
    {
        GestureData newPoseData;
        Vec2f leftElbow = GetJoint(pose, leftSide.elbow);
        Vec2f leftWrist = GetJoint(pose, leftSide.wrist);
        Vec2f leftEye = GetJoint(pose, leftSide.eye);

        Vec2f rightElbow = GetJoint(pose, rightSide.elbow);
        Vec2f rightWrist = GetJoint(pose, rightSide.wrist);
        Vec2f rightEye = GetJoint(pose, rightSide.eye);

        // Check if facing camera, otherwise set as not facing and move on
        if (IsJointVisible(leftEye) && IsJointVisible(rightEye))
        {
            m_isFacing = true;
            newPoseData.scale = 1.f / GetJointDistance(pose, leftSide.eye, rightSide.eye);

            newPoseData.leftVisibleAndRaised
                = IsJointVisible(leftWrist) && IsJointVisible(leftElbow)
                  && (leftElbow.y - leftWrist.y) * newPoseData.scale > m_armRaiseThreshold;
            newPoseData.rightVisibleAndRaised
                = IsJointVisible(rightWrist) && IsJointVisible(rightElbow)
                  && (rightElbow.y - rightWrist.y) * newPoseData.scale > m_armRaiseThreshold;

            newPoseData.leftRelativePosition.x = leftWrist.x - leftEye.x;
            newPoseData.leftRelativePosition.y = leftWrist.y - leftEye.y;
            newPoseData.rightRelativePosition.x = rightWrist.x - rightEye.x;
            newPoseData.rightRelativePosition.y = rightWrist.y - rightEye.y;
        }
        else
        {
            m_isFacing = false;
        }

        m_poseDataBuffer.push_back(newPoseData);
        if (m_poseDataBuffer.size() > m_bufferSize)
        {
            m_poseDataBuffer.pop_front();
        }
    }

    // Some helper functions
    static float GetJointDistance(const wrnch::Pose2d& person,
                                  unsigned int jointIndex1,
                                  unsigned int jointIndex2)
    {
        Vec2f joint1 = GetJoint(person, jointIndex1);
        Vec2f joint2 = GetJoint(person, jointIndex2);

        if (IsJointVisible(joint1) && IsJointVisible(joint2))
        {
            return std::sqrt((joint1.x - joint2.x) * (joint1.x - joint2.x)
                             + (joint1.y - joint2.y) * (joint1.y - joint2.y));
        }

        return -1.f;
    }

    static Vec2f GetJoint(const wrnch::Pose2d& person, unsigned int jointIndex)
    {
        Vec2f joint;
        const float* joints = person.GetJoints();
        joint.x = joints[2 * jointIndex];
        joint.y = joints[2 * jointIndex + 1];

        return joint;
    }

    static bool IsJointVisible(const Vec2f& joint) { return joint.x >= 0 && joint.y >= 0; }
};
}  // namespace wrSamples
