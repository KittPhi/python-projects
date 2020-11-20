/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/
#pragma once

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// Some preset colours
const cv::Scalar WRNCH_BLUE(226.95, 168.3, 38.25);
const cv::Scalar RED(22.5, 22.5, 229.5);
const cv::Scalar GREEN_SCREEN_COLOR(0.09, 0.73, 0.3);
namespace wrSamples
{
class Visualizer
{
  public:
    explicit Visualizer(std::string name = "wrnch sample");
    ~Visualizer();
    void SetFrame(cv::Mat& frame);
    void DrawPoints(const float* points,
                    unsigned int numPoints,
                    const cv::Scalar& color = WRNCH_BLUE,
                    float pointSize = 8.f);

    void DrawPoints3D(const float* points,
                      unsigned int numPoints,
                      const cv::Scalar& color = WRNCH_BLUE,
                      float pointSize = 8.f);

    void DrawLines(const float* points,
                   const unsigned int* pairs,
                   unsigned int numPairs,
                   const cv::Scalar& color = WRNCH_BLUE,
                   int thickness = 3);

    void DrawBox(float x,
                 float y,
                 float width,
                 float height,
                 const cv::Scalar& color = WRNCH_BLUE,
                 float thickness = 3.f);

    void DrawArrow(float baseX,
                   float baseY,
                   float tipX,
                   float tipY,
                   const cv::Scalar& color = RED,
                   float thickness = 3.f);
    void DrawMask(const unsigned char* maskData, int maskWidth, int maskHeight);
    cv::Mat ThresholdMask(const unsigned char* maskData,
                          int maskWidth,
                          int maskHeight,
                          unsigned char minVal = 100,
                          unsigned char maxVal = 200);
    void DrawTextInFrame(const std::string& text,
                         int x,
                         int y,
                         const cv::Scalar& color = RED,
                         float scale = 1.f,
                         float thickness = 1.f);

    void Show() const;
    int GetWidth() const;
    int GetHeight() const;
    char WaitKeyboardInput(int milliseconds) const;
    cv::Mat const Frame() const { return m_frame; }
    cv::Size GetTextSize(std::string const& text, float scale = 1.f, float thickness = 1.f);
    void GetBillBoard(
        const cv::Mat& inputFrame, cv::Mat& warpedFrame, int angleX, int angleY, int angleZ);
    void DrawLines3D(const float* points3d,
                     int numJoints,
                     const std::vector<unsigned int>& bonePairs,
                     unsigned int numBones,
                     int angleX,
                     int angleY,
                     int angleZ);

    void DrawArrows3D(const float* jointPositions3d,
                      int numJoints,
                      const std::vector<float>& xAxis,
                      const std::vector<float>& yAxis,
                      const std::vector<float>& zAxis,
                      int angleX,
                      int angleY,
                      int angleZ);

    void FlipFrame();
    static int FontFace();


  private:
    cv::Mat m_frame;
    std::string m_name;
    struct TranslationParams3D
    {
        float xOffset = -1.0f;
        float yOffset = -1.0f;
        bool isValid = false;
    };

    TranslationParams3D m_translationParams3D;

    enum LineTypes
    {
        FILLED = -1,
        LINE_4 = 4,   // 4-connected line
        LINE_8 = 8,   // 8-connected line
        LINE_AA = 16  // Antialiased line
    };
};

inline Visualizer::Visualizer(std::string name)
: m_name(std::move(name))
{
    cv::namedWindow(m_name, cv::WINDOW_NORMAL);
}

inline Visualizer::~Visualizer() { cv::destroyWindow(m_name); }

inline void Visualizer::SetFrame(cv::Mat& frame) { m_frame = frame; }

inline void Visualizer::FlipFrame()
{
    cv::Mat flippedFrame;
    cv::flip(m_frame, flippedFrame, 1);
    m_frame = flippedFrame;
}

inline void Visualizer::DrawPoints(const float* points,
                                   unsigned int numPoints,
                                   const cv::Scalar& color,
                                   float pointSize)
{
    const int w = GetWidth();
    const int h = GetHeight();

    for (unsigned i = 0; i < numPoints; i++)
    {
        auto x = points[2 * i] * w;
        auto y = points[2 * i + 1] * h;
        if (x >= 0 && y >= 0)
        {
            cv::circle(
                m_frame, cv::Point(x, y), static_cast<int>(pointSize), color, FILLED, LINE_AA);
        }
    }
}

inline void Visualizer::DrawPoints3D(const float* points,
                                     unsigned int numPoints,
                                     const cv::Scalar& color,
                                     float pointSize)
{
    const int w = GetWidth();
    const int h = GetHeight();

    for (unsigned i = 0; i < numPoints; i++)
    {
        auto x = points[3 * i] * w;
        auto y = points[3 * i + 1] * h;
        // auto z = points[3 * i + 2] Depth is stored here

        if (x >= 0 && y >= 0)
        {
            cv::circle(
                m_frame, cv::Point(x, y), static_cast<int>(pointSize), color, FILLED, LINE_AA);
        }
    }
}

inline void Visualizer::DrawLines(const float* points,
                                  const unsigned int* pairs,
                                  unsigned int numPairs,
                                  const cv::Scalar& color,
                                  int thickness)
{
    const int w = GetWidth();
    const int h = GetHeight();

    for (unsigned int i = 0; i < numPairs; i++)
    {
        unsigned const idx1 = pairs[2 * i];
        unsigned const idx2 = pairs[2 * i + 1];
        auto x1 = points[idx1 * 2] * w;
        auto y1 = points[idx1 * 2 + 1] * h;
        auto x2 = points[idx2 * 2] * w;
        auto y2 = points[idx2 * 2 + 1] * h;

        if (x1 >= 0 && x2 >= 0 && y1 >= 0 && y2 >= 0)
        {
            cv::line(m_frame, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness, LINE_AA);
        }
    }
}


inline void Visualizer::DrawBox(
    float x, float y, float width, float height, const cv::Scalar& color, float thickness)
{
    const int w = GetWidth();
    const int h = GetHeight();

    cv::Rect rect(x * w, y * h, width * w, height * h);
    cv::rectangle(m_frame, rect, color, static_cast<int>(thickness), LINE_AA);
}

inline void Visualizer::DrawArrow(
    float baseX, float baseY, float tipX, float tipY, const cv::Scalar& color, float thickness)
{
    const int w = GetWidth();
    const int h = GetHeight();

    if (baseX >= 0 && tipX >= 0)
    {
        cv::line(m_frame,
                 cv::Point(static_cast<int>(baseX * w), static_cast<int>(baseY * h)),
                 cv::Point(static_cast<int>(tipX * w), static_cast<int>(tipY * h)),
                 color,
                 static_cast<int>(thickness),
                 LINE_AA);
    }
}

/* NOTES ON MASK:
        The mask returned from the engine is comprised of up to four 8-bit single channel
   images, stored contiguously. PoseEstimator::GetMaskDims(...) provides the width and height of
   each mask (typically smaller than the original image) with the depth corresponding to the
   number of masks returned. The four available masks are: (0) body; (1) right hand; (2) left
   hand; and (3) both hands.

    Each value (between 0 and 255) indicates a confidence level that there is a human at that
    position. These values can be used directly or modified to suit your needs. Here, we intend
   on drawing a greenScreen over our image, so we must resize the mask to match the image size.
    Further, we use ThresholdMask(...) to convert the 8-bit values into floating points and
    clipping values below 100 to 0, values above 200 to 1, and linearly interpolating in
   between.

        From the thresholded mask, alpha, we display: alpha*m_frame + (1-alpha)*maskFill
*/
inline void Visualizer::DrawMask(const unsigned char* maskData,
                                 const int maskWidth,
                                 const int maskHeight)
{
    cv::Size frameSize(GetWidth(), GetHeight());

    // Threshold and resize/reshape to size of original 3-channel image
    cv::Mat thresholdedMask = ThresholdMask(maskData, maskWidth, maskHeight);
    cv::Mat alpha;

    cv::resize(thresholdedMask,
               alpha,
               frameSize,
               0,
               0,
#if CV_VERSION_MAJOR >= 3
               cv::INTER_CUBIC
#else
               CV_INTER_CUBIC
#endif
    );

    cv::threshold(alpha, alpha, 1.0f, 1.0f, cv::THRESH_TRUNC);
    cv::threshold(alpha, alpha, 0.0f, 1.0f, cv::THRESH_TOZERO);

    cv::Mat channels[] = { alpha, alpha, alpha };
    cv::merge(channels, 3, alpha);

    // Prepare green screen
    cv::Mat beta = cv::Scalar(1, 1, 1) - alpha;
    cv::Mat maskFill(frameSize, CV_32FC3, GREEN_SCREEN_COLOR);
    cv::multiply(beta, maskFill, beta);

    // Prepare unmasked portion of image
    m_frame.convertTo(m_frame, CV_32FC3, 1 / 255.0);
    cv::multiply(alpha, m_frame, alpha);

    // Combine
    cv::add(alpha, beta, m_frame);
}

inline cv::Mat Visualizer::ThresholdMask(const unsigned char* maskData,
                                         const int maskWidth,
                                         const int maskHeight,
                                         const unsigned char minVal,
                                         const unsigned char maxVal)
{
    cv::Mat result(maskHeight, maskWidth, CV_32FC1);

    /*
      openCV does not allow a mat to be constructed with a const data pointer.
      We const cast here, but only perform read-only operations on the underlying mat
     */
    cv::Mat maskMat(maskHeight, maskWidth, CV_8UC1, const_cast<unsigned char*>(maskData));

    for (int row = 0; row < maskMat.rows; ++row)
    {
        const unsigned char* sourceRowPtr = maskMat.ptr(row);
        float* destRowPtr = result.ptr<float>(row);
        for (int col = 0; col < maskMat.cols; ++col)
        {
            const unsigned char sourceValue = *sourceRowPtr;
            if (sourceValue < minVal)
            {
                *destRowPtr = 0.0f;
            }
            else if (sourceValue > maxVal)
            {
                *destRowPtr = 1.0f;
            }
            else
            {
                *destRowPtr = (sourceValue - minVal) / static_cast<float>(maxVal - minVal);
            }

            sourceRowPtr++;
            destRowPtr++;
        }
    }
    return result;
}

inline int Visualizer::FontFace() { return cv::FONT_HERSHEY_PLAIN; }


inline void Visualizer::DrawTextInFrame(
    const std::string& text, int x, int y, const cv::Scalar& color, float scale, float thickness)
{
    cv::Size const textSize = GetTextSize(text, scale, thickness);

    if (x + textSize.width > GetWidth())
        x -= textSize.width;
    if (y + textSize.height > GetHeight())
        y -= textSize.height;

    cv::putText(m_frame,
                text,
                cv::Point(x, y),
                FontFace(),
                scale,
                color,
                static_cast<int>(thickness),
                LINE_AA);
}

inline cv::Size Visualizer::GetTextSize(std::string const& text, float scale, float thickness)
{
    int baseline;
    return cv::getTextSize(text, FontFace(), scale, thickness, &baseline);
}


inline void Visualizer::Show() const { cv::imshow(m_name, m_frame); }

inline int Visualizer::GetWidth() const { return m_frame.cols; }

inline int Visualizer::GetHeight() const { return m_frame.rows; }

inline char Visualizer::WaitKeyboardInput(int milliseconds) const
{
    return cv::waitKey(milliseconds);
}

namespace detail
{
const float FOV = 30.0f;
const float SCALE = 0.8f;


inline double deg2Rad(double deg) { return deg * (M_PI / 180); }

// code for Transformation and Warp Matrix based on :
// https://stackoverflow.com/questions/17087446/how-to-calculate-perspective-transform-for-opencv-from-rotation-angles

inline cv::Mat GetTransformationMatrix(cv::Size sz, double theta, double phi, double gamma)
{
    cv::Mat retMatrix = cv::Mat(4, 4, CV_64FC1);
    double st = sin(deg2Rad(theta));
    double ct = cos(deg2Rad(theta));
    double sp = sin(deg2Rad(phi));
    double cp = cos(deg2Rad(phi));
    double sg = sin(deg2Rad(gamma));
    double cg = cos(deg2Rad(gamma));

    double halfFovy = FOV * 0.5;
    double d = hypot(sz.width, sz.height);
    double h = d / (2.0 * sin(deg2Rad(halfFovy)));
    double n = h - (d / 2.0);
    double f = h + (d / 2.0);

    cv::Mat Rtheta = cv::Mat::eye(
        4, 4, CV_64FC1);  // Allocate 4x4 rotation matrix around Z-axis by theta degrees
    cv::Mat Rphi = cv::Mat::eye(
        4, 4, CV_64FC1);  // Allocate 4x4 rotation matrix around X-axis by phi degrees
    cv::Mat Rgamma = cv::Mat::eye(
        4, 4, CV_64FC1);  // Allocate 4x4 rotation matrix around Y-axis by gamma degrees

    cv::Mat T
        = cv::Mat::eye(4, 4, CV_64FC1);  // Allocate 4x4 translation matrix along Z-axis by -h units
    cv::Mat P = cv::Mat::zeros(4, 4, CV_64FC1);  // Allocate 4x4 projection matrix

    // Rtheta
    Rtheta.at<double>(0, 0) = Rtheta.at<double>(1, 1) = ct;
    Rtheta.at<double>(0, 1) = -st;
    Rtheta.at<double>(1, 0) = st;
    // Rphi
    Rphi.at<double>(1, 1) = Rphi.at<double>(2, 2) = cp;
    Rphi.at<double>(1, 2) = -sp;
    Rphi.at<double>(2, 1) = sp;
    // Rgamma
    Rgamma.at<double>(0, 0) = Rgamma.at<double>(2, 2) = cg;
    Rgamma.at<double>(0, 2) = -sg;
    Rgamma.at<double>(2, 0) = sg;

    // T
    T.at<double>(2, 3) = -h;
    // P
    P.at<double>(0, 0) = P.at<double>(1, 1) = 1.0 / tan(deg2Rad(halfFovy));
    P.at<double>(2, 2) = -(f + n) / (f - n);
    P.at<double>(2, 3) = -(2.0 * f * n) / (f - n);
    P.at<double>(3, 2) = -1.0;
    // Compose transformations
    retMatrix = P * T * Rphi * Rtheta * Rgamma;  // Matrix-multiply to produce master matrix

    return retMatrix;
}

// Given a transformation matrix computes the warp matrix
inline cv::Mat GetWarpMatrix(cv::Size sz, const cv::Mat& F, cv::Point2f* topLeft = nullptr)
{
    cv::Mat retMatrix;
    // float fovy = this->fovy;
    float halfFovy = FOV / 2;
    // float scale = this->scale;
    double d = hypot(sz.width, sz.height);
    double sideLength = SCALE * d / cos(deg2Rad(halfFovy));
    // Transform 4x4 points
    double ptsIn[4 * 3];
    double ptsOut[4 * 3];
    double halfW = sz.width / 2, halfH = sz.height / 2;

    ptsIn[0] = -halfW;
    ptsIn[1] = halfH;
    ptsIn[3] = halfW;
    ptsIn[4] = halfH;
    ptsIn[6] = halfW;
    ptsIn[7] = -halfH;
    ptsIn[9] = -halfW;
    ptsIn[10] = -halfH;
    ptsIn[2] = ptsIn[5] = ptsIn[8] = ptsIn[11] = 0;  // Set Z component to zero for all 4 components

    cv::Mat ptsInMat(1, 4, CV_64FC3, ptsIn);
    cv::Mat ptsOutMat(1, 4, CV_64FC3, ptsOut);

    cv::perspectiveTransform(ptsInMat, ptsOutMat, F);  // Transform points

    // Get 3x3 transform and warp image
    cv::Point2f ptsInPt2f[4];
    cv::Point2f ptsOutPt2f[4];

    for (int i = 0; i < 4; i++)
    {
        cv::Point2f ptIn(ptsIn[i * 3 + 0], ptsIn[i * 3 + 1]);
        cv::Point2f ptOut(ptsOut[i * 3 + 0], ptsOut[i * 3 + 1]);
        ptsInPt2f[i] = ptIn + cv::Point2f(halfW, halfH);
        ptsOutPt2f[i] = (ptOut + cv::Point2f(1, 1)) * (sideLength * 0.5);
    }

    if (topLeft)
    {
        *topLeft = ptsOutPt2f[3];
    }

    retMatrix = cv::getPerspectiveTransform(ptsInPt2f, ptsOutPt2f);

    return retMatrix;
}

inline std::vector<float> Project3DTo2D(const float* points3d,
                                        int numJoints,
                                        cv::Size sz,
                                        const cv::Mat F,
                                        float xOffset,
                                        float yOffset)
{
    std::vector<float> points2d;
    float halfFovy = FOV / 2;
    int width = sz.width;
    int height = sz.height;
    double d = hypot(width, height);
    double sideLength = SCALE * d / cos(deg2Rad(halfFovy));

    // Center 3d points and scale them to image space
    std::vector<cv::Point3f> points3dScaled;
    for (int i = 0; i < 3 * numJoints; i += 3)
    {
        float xScaled = (points3d[i] - 0.5) * width;
        float yScaled = (points3d[i + 1] - 0.5) * height;
        float zScaled = (0.5 - points3d[i + 2]) * height;
        cv::Point3f curPoint = cv::Point3f(xScaled, yScaled, zScaled);
        points3dScaled.push_back(curPoint);
    }

    std::vector<cv::Point3f> points3dTransformed;
    cv::perspectiveTransform(points3dScaled, points3dTransformed, F);
    for (unsigned int i = 0; i < points3dTransformed.size(); i++)
    {
        points2d.push_back((points3dTransformed[i].x + 1) * (sideLength * 0.5) + xOffset);
        points2d.push_back((points3dTransformed[i].y + 1) * (sideLength * 0.5) + yOffset);
    }

    return points2d;
}
}
inline void Visualizer::GetBillBoard(
    const cv::Mat& inputFrame, cv::Mat& warpedFrame, int angleX, int angleY, int angleZ)
{
    cv::Mat F = detail::GetTransformationMatrix(inputFrame.size(), angleX, angleZ, angleY);
    // Perform a one time initialization where we get an offset to center image at 50,50, when
    // viewed with no rotation
    cv::Mat warpMatrix;  // Transformation matrix for billboard
    if (!m_translationParams3D.isValid)
    {
        cv::Point2f topLeft;
        warpMatrix = detail::GetWarpMatrix(inputFrame.size(), F, &topLeft);
        m_translationParams3D.xOffset = 50 - topLeft.x;
        m_translationParams3D.yOffset = 50 - topLeft.y;
        if (topLeft.x >= 0 && topLeft.x < m_frame.cols && topLeft.y >= 0
            && topLeft.y < m_frame.rows)
        {
            m_translationParams3D.isValid = true;
        }
    }

    else
    {
        warpMatrix = detail::GetWarpMatrix(inputFrame.size(), F);
    }

    cv::Mat warpedFrame_tmp;
    cv::warpPerspective(
        inputFrame, warpedFrame_tmp, warpMatrix, inputFrame.size(), cv::INTER_LANCZOS4);

    // translate warped frame to 50,50
    cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1,
                         0,
                         m_translationParams3D.xOffset,
                         0,
                         1,
                         m_translationParams3D.yOffset);

    if (m_translationParams3D.isValid)
    {
        cv::warpAffine(warpedFrame_tmp,
                       warpedFrame,
                       trans_mat,
                       warpedFrame_tmp.size(),
                       cv::INTER_LANCZOS4,
                       cv::BORDER_CONSTANT);
    }
}


inline void Visualizer::DrawLines3D(const float* points3d,
                                    int numJoints,
                                    const std::vector<unsigned int>& bonePairs,
                                    unsigned int numBones,
                                    int angleX,
                                    int angleY,
                                    int angleZ)
{
    cv::Mat F = detail::GetTransformationMatrix(m_frame.size(), angleX, angleZ, angleY);

    std::vector<float> points2d = detail::Project3DTo2D(points3d,
                                                        numJoints,
                                                        m_frame.size(),
                                                        F,
                                                        m_translationParams3D.xOffset,
                                                        m_translationParams3D.yOffset);
    // check if joints are valid
    std::vector<bool> jointMasks;
    for (int i = 0; i < 3 * numJoints; i += 3)
    {
        if (points3d[i] < 0 || points3d[i + 1] < 0)
        {
            jointMasks.push_back(false);
        }
        else
        {
            jointMasks.push_back(true);
        }
    }
    // Draw lines only for valid bone pairs
    for (unsigned int i = 0; i < numBones; i++)
    {
        unsigned const idx1 = bonePairs[2 * i];
        unsigned const idx2 = bonePairs[2 * i + 1];
        if (jointMasks[idx1] && jointMasks[idx2])
        {
            auto x1 = points2d[idx1 * 2];
            auto y1 = points2d[idx1 * 2 + 1];
            auto x2 = points2d[idx2 * 2];
            auto y2 = points2d[idx2 * 2 + 1];
            cv::line(m_frame, cv::Point(x1, y1), cv::Point(x2, y2), WRNCH_BLUE, 8, LINE_AA);
        }
    }
}


inline void Visualizer::DrawArrows3D(const float* jointPositions3d,
                                     int numJoints,
                                     const std::vector<float>& xAxis,
                                     const std::vector<float>& yAxis,
                                     const std::vector<float>& zAxis,
                                     int angleX,
                                     int angleY,
                                     int angleZ)
{
    cv::Mat F = detail::GetTransformationMatrix(m_frame.size(), angleX, angleZ, angleY);

    std::vector<float> points2d = detail::Project3DTo2D(jointPositions3d,
                                                        numJoints,
                                                        m_frame.size(),
                                                        F,
                                                        m_translationParams3D.xOffset,
                                                        m_translationParams3D.yOffset);

    float axisScaleFactor = 0.05;
    int width = m_frame.cols;
    int height = m_frame.rows;

    for (unsigned int i = 0; i < points2d.size(); i += 2)
    {
        DrawArrow(points2d[i] / width,
                  points2d[i + 1] / height,
                  points2d[i] / width - xAxis[i] * axisScaleFactor,
                  points2d[i + 1] / height - xAxis[i + 1] * axisScaleFactor,
                  cv::Scalar(255, 0, 0));

        DrawArrow(points2d[i] / width,
                  points2d[i + 1] / height,
                  points2d[i] / width - yAxis[i] * axisScaleFactor,
                  points2d[i + 1] / height - yAxis[i + 1] * axisScaleFactor,
                  cv::Scalar(0, 255, 0));

        DrawArrow(points2d[i] / width,
                  points2d[i + 1] / height,
                  points2d[i] / width - zAxis[i] * axisScaleFactor,
                  points2d[i + 1] / height - zAxis[i + 1] * axisScaleFactor,
                  cv::Scalar(0, 0, 255));
    }
}
}  // namepace wrSamples