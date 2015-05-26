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

#ifndef RESTORE_RENDERING_CV_UTILS_HPP
#define RESTORE_RENDERING_CV_UTILS_HPP

// C system files
// none

// C++ system files
// none

// header files of other libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// header files of project libraries
#include "../common/camera.hpp"

namespace ret {

    template <typename coord, typename point>
    coord project(const Camera& cam, const point& v) {

        coord im;

        // project voxel into camera image coords
        auto P = cam.getProjectionMatrix();

        auto z = P.at<float>(2, 0) * v.x + P.at<float>(2, 1) * v.y +
                 P.at<float>(2, 2) * v.z + P.at<float>(2, 3);

        im.y = (P.at<float>(1, 0) * v.x + P.at<float>(1, 1) * v.y +
                P.at<float>(1, 2) * v.z + P.at<float>(1, 3)) /
               z;

        im.x = (P.at<float>(0, 0) * v.x + P.at<float>(0, 1) * v.y +
                P.at<float>(0, 2) * v.z + P.at<float>(0, 3)) /
               z;

        return im;
    }

    inline cv::Mat getCameraDirection(const Camera& cam,
                                      const cv::Size& img_size) {

        cv::Mat center = (cv::Mat_<float>(3, 1) << img_size.width / 2.0f,
                          img_size.height / 2.0f, 1.0f);

        cv::Mat X;
        cv::solve(cam.getCalibrationMatrix(), center, X, cv::DECOMP_LU);

        cv::Mat Rt;
        cv::transpose(cam.getRotationMatrix(), Rt);
        X = Rt * (X * (-1));

        return X / cv::norm(X);
    }
}

#endif
