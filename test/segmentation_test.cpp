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

TEST(SegmentationTest, GrabCutImage) {

//    cv::Mat tmp =
//        cv::imread("/Users/kai/Projekte/Restore/assets/aeffle/image_00.png");
//    cv::Mat Binary = Segmentation::grabCut(tmp);
//    cv::imshow("Binary", Binary);
//    cv::waitKey();
}
