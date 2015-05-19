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

#ifndef RESTORE_COMMON_CAMERA_HPP
#define RESTORE_COMMON_CAMERA_HPP

// C system files
// none

// C++ system files
// none

// header files of other libraries
#include <opencv2/core/core.hpp>

// header files of project libraries
#include "camera_intrinsics.hpp"
#include "camera_extrinsics.hpp"

namespace ret {

    class Camera : public CameraIntrinsics, public CameraExtrinsics {
      public:
        Camera() = default;

        Camera(const cv::Mat Image) {
            this->Image_ = Image;
        }

        template <typename T>
        Camera& setProjectionMatrix(T&& P) {
            assert(P.size() == cv::Size(4, 3) && P.type() == CV_32F);
            this->P_ = std::forward<T>(P);
            return *this;
        }

        cv::Mat getProjectionMatrix() const {
            return this->P_;
        }

        cv::Mat getPose() const {
            cv::Mat Pose(3, 4, CV_32F, cv::Scalar::all(0));
            R_.copyTo(Pose.colRange(0, 2).rowRange(0, 2));
            t_.copyTo(Pose.colRange(2, 3).rowRange(0, 2));
            return Pose;
        }

        cv::Point3d getCenter() const {

            cv::Mat P_023;
            for (auto idx : {0, 2, 3}) P_023.push_back(P_.col(idx));
            cv::Mat P_013;
            for (auto idx : {0, 1, 3}) P_013.push_back(P_.col(idx));

            double x = cv::determinant(P_(cv::Range(0, 3), cv::Range(1, 4)));
            double y = -cv::determinant(P_023.reshape(0, 3).t());
            double z = cv::determinant(P_013.reshape(0, 3).t());
            double t = -cv::determinant(P_(cv::Range(0, 3), cv::Range(0, 3)));

            return cv::Point3d(x / t, y / t, z / t);
        }

        template <typename T>
        Camera& setImage(T&& Image) {
            this->Image_ = std::forward<T>(Image);
            return *this;
        }

        cv::Mat getImage() const {
            return Image_;
        }

        template <typename T>
        Camera& setMask(T&& Mask) {
            this->Mask_ = std::forward<T>(Mask);
            return *this;
        }

        cv::Mat getMask() const {
            return Mask_;
        }

      private:
        cv::Mat P_;
        cv::Mat Image_;
        cv::Mat Mask_;
    };
}

#endif
