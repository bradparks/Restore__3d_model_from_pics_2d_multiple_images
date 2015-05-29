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
#include <opencv2/imgproc/imgproc.hpp>

// header files of project libraries
#include "segmentation.hpp"

using ret::filtering::Segmentation;

cv::Mat Segmentation::binarize(const cv::Mat& Image, const cv::Scalar& thresh) {

    assert(Image.channels() == 3);
    cv::Mat Binary;
    cv::cvtColor(Image, Binary, CV_BGR2HSV);
    cv::inRange(Binary, thresh, cv::Scalar(255, 255, 255), Binary);

    return Binary;
}

cv::Mat Segmentation::grabCut(const cv::Mat& Image, unsigned char num_frags,
                              const cv::Point& w_from_to,
                              const cv::Point& h_from_to) {

    assert(Image.channels() == 3);
    assert((num_frags & (num_frags - 1)) == 0);
    auto w_frag = Image.cols / static_cast<float>(num_frags);
    auto h_frag = Image.rows / static_cast<float>(num_frags);
    cv::Mat Result, bgModel, fgModel;
    cv::Rect area(w_from_to.x * w_frag, w_from_to.y * h_frag,
                  h_from_to.x * w_frag, h_from_to.y * h_frag);
    cv::grabCut(Image, Result, area, bgModel, fgModel, 1,
                cv::GC_INIT_WITH_RECT);
    cv::compare(Result, cv::GC_PR_FGD, Result, cv::CMP_EQ);

    return Result;
}

cv::Mat Segmentation::createDistMap(const cv::Mat& Mask) {

    assert(Mask.channels() == 1);
    cv::Mat DistImage;
    cv::distanceTransform(createSilhouette(Mask), DistImage, CV_DIST_L2, 3);

    return DistImage;
}

cv::Mat Segmentation::createSilhouette(const cv::Mat& Mask) {

    assert(Mask.channels() == 1);
    cv::Mat Silhouette;
    cv::Canny(Mask, Silhouette, 0, 255);
    cv::bitwise_not(Silhouette, Silhouette);

    return Silhouette;
}
