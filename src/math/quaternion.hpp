// Copyright (c) 2015-2016, Kai Wolf
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

#ifndef MATH_QUATERNION_HPP
#define MATH_QUATERNION_HPP

#include <ostream>

#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>

namespace ret {

namespace math {

    class Quaternion {
      public:
        /// Constructors
        Quaternion();
        Quaternion(double s, double x, double y, double z);
        Quaternion(double s, const cv::Vec3d& vec);
        Quaternion(const Quaternion& other) = default;

        /// Operators
        Quaternion& operator=(const Quaternion& other) = default;
        bool operator==(const Quaternion& other) const;
        bool operator!=(const Quaternion& other) const;
        Quaternion operator+(const Quaternion& other) const;
        Quaternion& operator+=(const Quaternion& other);
        Quaternion operator-(const Quaternion& other) const;
        Quaternion operator*(const Quaternion& other) const;
        cv::Vec3d operator*(const cv::Vec3d& vec) const;
        Quaternion operator*(double scalar) const;
        Quaternion operator/(double scalar) const;
        Quaternion& operator*=(double scalar);

        /// functions
        double dot(Quaternion& other) const;
        double magnitude() const;
        Quaternion& normalize();
        Quaternion conj() const;
        Quaternion inv() const;
        cv::Vec3d toEulerAngles() const;
        cv::Mat toRotMatrix() const;
        cv::Mat toRotMatrix4x4() const;
        cv::Vec4d getComponents() const;
        friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
            return os << "[" << q.s_ << ", (" << q.vec_[0] << ", " << q.vec_[1]
                      << ", " << q.vec_[2] << ")]";
        }

        /// static functions
        static Quaternion fromRotMatrix(const cv::Mat& rotMatrix);
        static Quaternion fromRodriguesVec(const cv::Vec3d& rodrigues);
        static Quaternion fromEulerAngles(const cv::Vec3d& eulerAngles);
        static Quaternion fromAxisAngle(const cv::Vec3d& axis, double angle);
        static Quaternion betweenTwoVecs(const cv::Vec3d& a,
                                         const cv::Vec3d& b);
        static Quaternion lookAt(const cv::Vec3d& a, const cv::Vec3d& b);
        static Quaternion slerp(const Quaternion& q1, const Quaternion& q2,
                                double t);
        static Quaternion log(const Quaternion& q);
        static Quaternion exp(const Quaternion& q);
        static Quaternion pow(const Quaternion& q, double t);

      private:
        /// real part
        double s_;
        /// imaginary parts
        cv::Vec3d vec_;
    };
}  // namespace math
} // namespace ret

#endif
