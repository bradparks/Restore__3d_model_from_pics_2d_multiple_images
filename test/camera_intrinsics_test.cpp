/*
 * Copyright (c) 2015, Kai Wolf
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

// C system files
// none

// C++ system files
// none

// header files of other libraries
#include <gtest/gtest.h>

// header files of project libraries
#include <restore/common.hpp>

using ret::CameraIntrinsics;

TEST(CameraIntrinsicsTest, CreateCameraIntrinsics) {
    CameraIntrinsics ci;
    ASSERT_EQ(ci.getCalibrationMatrix().channels(), 1);

    ASSERT_TRUE(ci.getCalibrationMatrix().type() == CV_32F);

    ASSERT_EQ(ci.getCalibrationMatrix().total(), 9);
}

TEST(CameraIntrinsicsTest, CopyCameraIntrinsics) {
    cv::Mat K1 = (cv::Mat_<float>(3, 3) << 0, 0, 0, 0, 1, 0, 0, 0, 1);
    CameraIntrinsics c1(K1);
    CameraIntrinsics c2 = c1;
    ASSERT_EQ(c1, c2);

    CameraIntrinsics c3 = c2;
    ASSERT_EQ(c2, c3);

    cv::Mat K2 = (cv::Mat_<float>(3, 3) << 0, 1, 0, 0, 1, 0, 0, 0, 1);
    CameraIntrinsics c4(K2);
    ASSERT_FALSE(c3 == c4);
}

TEST(CameraIntrinsicsTest, GetSetCalibrationMatrix) {
    CameraIntrinsics c1;
    c1.setCalibrationMatrix(cv::Mat(3, 3, CV_32F, cv::Scalar::all(17)));
    cv::Mat K(3, 3, CV_32F, cv::Scalar::all(17));
    cv::Mat diff = c1.getCalibrationMatrix() != K;
    ASSERT_TRUE(cv::countNonZero(diff) == 0);
}

TEST(CameraIntrinsicsTest, CheckAssertSetCalibMat) {
    CameraIntrinsics c1;
    cv::Mat K(3, 4, CV_32F, cv::Scalar::all(17));
    ASSERT_DEATH(c1.setCalibrationMatrix(K), "");
}

TEST(CameraIntrinsicsTest, CheckAssertCreateFromCalibMat) {
    cv::Mat K(4, 3, CV_32F, cv::Scalar::all(23));
    ASSERT_DEATH(CameraIntrinsics c1(K), "");
}

TEST(CameraIntrinsicsTest, CheckAssertCorrectType) {
    cv::Mat K(3, 3, CV_8U, cv::Scalar::all(42));
    ASSERT_DEATH(CameraIntrinsics c1(K), "");

    CameraIntrinsics c2;
    ASSERT_DEATH(c2.setCalibrationMatrix(K), "");
}
