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

#ifndef RESTORE_MATH_DUAL_QUATERNION_HPP
#define RESTORE_MATH_DUAL_QUATERNION_HPP

// C system files
// none

// C++ system files
#include <vector>

// header files of other libraries
#include <opencv2/core/core.hpp>

// header files of project libraries
#include "quaternion.hpp"

namespace ret {

    namespace math {

        class DualQuaternion {

          public:
            DualQuaternion();
            DualQuaternion(const DualQuaternion& other);
            DualQuaternion(const Quaternion& real, const Quaternion& dual);
            DualQuaternion(const Quaternion& real, const cv::Vec3d vec);
            DualQuaternion(const cv::Vec3d rodrigues, const cv::Vec3d vec);
            DualQuaternion(const cv::Mat& Rot, const cv::Vec3d vec);
            DualQuaternion(const cv::Mat& Pose);
            DualQuaternion(const cv::Vec3d axis, double angle,
                           const cv::Vec3d vec);

            Quaternion getRotation() const;
            cv::Vec3d getTranslation() const;
            cv::Mat getPose() const;

            bool operator==(const DualQuaternion& other) const;
            bool operator!=(const DualQuaternion& other) const;
            DualQuaternion operator+(const DualQuaternion& other) const;
            DualQuaternion& operator+=(const DualQuaternion& other);
            DualQuaternion operator-(const DualQuaternion& other) const;
            DualQuaternion& operator-=(const DualQuaternion& other);
            DualQuaternion operator*(double scale) const;
            DualQuaternion operator*=(double scale);
            DualQuaternion operator/(double scale) const;
            DualQuaternion& operator/=(double scale);
            DualQuaternion operator*(const DualQuaternion& other) const;
            DualQuaternion& operator*=(DualQuaternion& other);

            DualQuaternion conj() const;
            DualQuaternion inv() const;
            DualQuaternion& normalize();
            double dot(DualQuaternion& other) const;
            double magnitude() const;

            static DualQuaternion sclerp(const DualQuaternion& dq1,
                                         const DualQuaternion& dq2, double t);
            static DualQuaternion log(DualQuaternion& dq);
            static DualQuaternion exp(DualQuaternion& dq);
            static DualQuaternion pow(DualQuaternion& dq, double t);
            static DualQuaternion dlb(const std::vector<DualQuaternion>& dqs);
            static DualQuaternion dlb(const std::vector<DualQuaternion>& dqs,
                                      const std::vector<double>& weights);

          private:
            void initialize(const Quaternion& real, const cv::Vec3d& vec);
            Quaternion real_;
            Quaternion dual_;
        };
    }
}

#endif
