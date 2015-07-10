// Copyright (c) 2015, Kai Wolf
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// C system files
// none

// C++ system files
// none

// header files of other libraries
#include <gtest/gtest.h>

// header files of project libraries
#include "common/camera_extrinsics.hpp"

using ret::CameraExtrinsics;

TEST(CameraExtrinsicsTest, CreateCameraExtrinsics) {
    CameraExtrinsics ce;
    ASSERT_EQ(ce.getRotationMatrix().channels(), 1);
    ASSERT_EQ(ce.getTranslationVector().channels(), 1);

    ASSERT_TRUE(ce.getRotationMatrix().type() == CV_32F);
    ASSERT_TRUE(ce.getTranslationVector().type() == CV_32F);

    ASSERT_EQ(ce.getRotationMatrix().total(), 9);
    ASSERT_EQ(ce.getTranslationVector().total(), 4);
}

TEST(CameraExtrinsicsTest, CopyCameraExtrinsics) {
    cv::Mat R1 = (cv::Mat_<float>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
    cv::Mat t1 = (cv::Mat_<float>(4, 1) << 0, 0, 0, 1);
    CameraExtrinsics ce1(R1, t1);
    CameraExtrinsics ce2 = ce1;
    ASSERT_EQ(ce1, ce2);

    CameraExtrinsics ce3 = ce2;
    ASSERT_EQ(ce2, ce3);

    cv::Mat R2 = (cv::Mat_<float>(3, 3) << 1, 0, 1, 0, 1, 0, 1, 0, 1);
    cv::Mat t2 = (cv::Mat_<float>(4, 1) << 0, 0, 1, 1);
    CameraExtrinsics ce4(R2, t2);
    ASSERT_FALSE(ce3 == ce4);
}

TEST(CameraExtrinsicsTest, GetSetRotationMatrix) {
    CameraExtrinsics ce1;
    ce1.setRotationMatrix(cv::Mat(3, 3, CV_32F, cv::Scalar::all(1)));
    cv::Mat R(3, 3, CV_32F, cv::Scalar::all(1));
    cv::Mat diff = ce1.getRotationMatrix() != R;
    ASSERT_TRUE(cv::countNonZero(diff) == 0);
}

TEST(CameraExtrinsicsTest, GetSetTranslationVector) {
    CameraExtrinsics ce1;
    ce1.setTranslationVector(cv::Mat(4, 1, CV_32F, cv::Scalar::all(2)));
    cv::Mat t(4, 1, CV_32F, cv::Scalar::all(2));
    cv::Mat diff = ce1.getTranslationVector() != t;
    ASSERT_TRUE(cv::countNonZero(diff) == 0);
}

TEST(CameraExtrinsicsTest, CheckAssertSetRotationMatrix) {
    CameraExtrinsics ce1;
    cv::Mat R(5, 3, CV_32F, cv::Scalar::all(0.5));
    ASSERT_DEATH(ce1.setRotationMatrix(R), "");
}

TEST(CameraExtrinsicsTest, CheckAssertSetTranslationVector) {
    CameraExtrinsics ce1;
    cv::Mat t(1, 4, CV_32F, cv::Scalar::all(1));
    ASSERT_DEATH(ce1.setTranslationVector(t), "");
}

TEST(CameraExtrinsicsTest, CheckAssertCorrectType) {
    cv::Mat R(3, 3, CV_8U, cv::Scalar::all(2));
    cv::Mat t(4, 1, CV_8U, cv::Scalar::all(1));

    ASSERT_DEATH(CameraExtrinsics ce1(R, t), "");

    CameraExtrinsics ce2;
    ASSERT_DEATH(ce2.setRotationMatrix(R), "");
    ASSERT_DEATH(ce2.setTranslationVector(t), "");
}
