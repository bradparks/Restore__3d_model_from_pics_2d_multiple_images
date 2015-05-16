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

#include "bounding_box.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace ret::rendering;

BoundingBox::BoundingBox(const Camera& cam1, const Camera& cam2)
    : cam1_(cam1), cam2_(cam2) {}

bb_bounds BoundingBox::getBounds() const {

    // get 2d bounding rect around object
    auto rect1 = getBoundingRect(cam1_.getMask());
    auto rect2 = getBoundingRect(cam2_.getMask());

    // get corners from 2d bounding rects
    auto p2 = (cv::Mat_<float>(3, 1) << rect1.x, rect1.y + rect1.height, 1.0f);
    auto p4 = (cv::Mat_<float>(3, 1) << rect1.x + rect1.width, rect1.y, 1.0f);
    auto p6 = (cv::Mat_<float>(3, 1) << rect2.x, rect2.y + rect2.height, 1.0f);
    auto p8 = (cv::Mat_<float>(3, 1) << rect2.x + rect2.width, rect2.y, 1.0f);

    // convert to world points
    auto K1_inv = cam1_.getCalibrationMatrix().inv();
    auto K2_inv = cam2_.getCalibrationMatrix().inv();
    cv::Mat x2 = K1_inv * p2;
    cv::Mat x4 = K1_inv * p4;
    cv::Mat x6 = K2_inv * p6;
    cv::Mat x8 = K2_inv * p8;

    // get world points
    auto t1 = (K1_inv * cam1_.getProjectionMatrix()).col(3);
    auto t2 = (K2_inv * cam2_.getProjectionMatrix()).col(3);

    // get distance
    auto t1_norm = cv::norm(t1);
    auto t2_norm = cv::norm(t2);

    // get world points of 2d rect
    x2 = t1_norm * x2 / cv::norm(x2);
    x4 = t1_norm * x4 / cv::norm(x4);
    x6 = t2_norm * x6 / cv::norm(x6);
    x8 = t2_norm * x8 / cv::norm(x8);

    bb_bounds bounds;
    bounds.xmin = x2.at<float>(0, 0);
    bounds.xmax = x4.at<float>(0, 0);
    bounds.ymin = x6.at<float>(0, 0);
    bounds.ymax = x8.at<float>(0, 0);

    if (x2.at<float>(1, 0) - x4.at<float>(1, 0) >
        x6.at<float>(1, 0) - x8.at<float>(1, 0)) {
        bounds.zmin = x4.at<float>(1, 0);
        bounds.zmax = x2.at<float>(1, 0);
    } else {
        bounds.zmin = x8.at<float>(1, 0);
        bounds.zmax = x6.at<float>(1, 0);
    }

    return bounds;
}

cv::Rect BoundingBox::getBoundingRect(cv::Mat Binary) const {

    assert(Binary.channels() == 1);
    cv::dilate(Binary, Binary, cv::Mat());
    int top = Binary.rows, bottom = -1, right = -1, left = Binary.cols;
    for (int y = 0; y < Binary.rows; ++y) {
        for (int x = 0; x < Binary.cols; ++x) {
            if (Binary.at<uchar>(y, x) == 0) {
                if (y > bottom) bottom = y;
                if (x > right) right = x;
                if (y < top) top = y;
                if (x < left) left = x;
            }
        }
    }

    return cv::Rect(left, top, right - left, bottom - top);
}
