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
#include <opencv2/highgui/highgui.hpp>

// header files of project libraries
#include <restore/filtering.hpp>

using ret::filtering::Segmentation;

TEST(SegmentationTest, BinarizeImage) {

    cv::Mat Original(240, 320, CV_8UC3, cv::Scalar::all(17));
    cv::Mat Binary = Segmentation::binarize(Original, cv::Scalar(200));
    int size = Binary.rows * Binary.cols;
    ASSERT_EQ(size, size - cv::countNonZero(Binary));
}

TEST(SegmentationTest, AssertColorImageWhenBinarizing) {

    cv::Mat Original(240, 320, CV_8U, cv::Scalar::all(17));
    ASSERT_DEATH(Segmentation::binarize(Original, cv::Scalar(200)), "");
}

TEST(SegmentationTest, CreateSilhouetteMask) {

    cv::Mat Original(240, 320, CV_8U, cv::Scalar::all(255));
    cv::Mat Black(20, 20, CV_8U, cv::Scalar::all(0));
    cv::Mat SubRegion = Original(cv::Rect(100, 100, Black.cols, Black.rows));
    Black.copyTo(SubRegion);
    ASSERT_GT(cv::countNonZero(Segmentation::createSilhouette(Original)),
              cv::countNonZero(Original));
}

TEST(SegmentationTest, CreateDistMap) {

    cv::Mat Original(240, 320, CV_8U, cv::Scalar::all(255));
    cv::Mat Black(20, 1, CV_8U, cv::Scalar::all(0));
    cv::Mat SubRegion = Original(cv::Rect(100, 100, Black.cols, Black.rows));
    Black.copyTo(SubRegion);
    cv::Mat Distmap = Segmentation::createDistMap(Original);
    ASSERT_TRUE(Distmap.type() == CV_32F);

    ASSERT_LT(Distmap.at<float>(100, 99), Distmap.at<float>(100, 100));
    ASSERT_GT(Distmap.at<float>(100, 100), Distmap.at<float>(100, 101));
}

TEST(SegmentationTest, GrabCutWithNotPowerOf2NumFrags) {

    cv::Mat Tmp(240, 320, CV_8UC3, cv::Scalar::all(255));
    ASSERT_DEATH(Segmentation::grabCut(Tmp, 15, cv::Point(), cv::Point()), "");
}

TEST(SegmentationTest, GrabCutImage) {

    cv::Mat Original(240, 320, CV_8UC3, cv::Scalar::all(255));
    cv::Mat Black(20, 20, CV_8UC3, cv::Scalar::all(0));
    cv::Mat SubRegion = Original(cv::Rect(100, 100, Black.cols, Black.rows));
    Black.copyTo(SubRegion);

    cv::Mat Segmented =
        Segmentation::grabCut(Original, 16, cv::Point(0, 0), cv::Point(8, 8));
    ASSERT_TRUE(Segmented.at<uchar>(110, 110) == 255);
    ASSERT_TRUE(Segmented.at<uchar>(200, 200) == 0);
}
