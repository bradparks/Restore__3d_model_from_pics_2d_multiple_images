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

#ifndef RESTORE_MATH_UTILS_HPP
#define RESTORE_MATH_UTILS_HPP

// C system files
// none

// C++ system files
#include <limits>

// header files of other libraries
#include <opencv2/core/core.hpp>

// header files of project libraries
// none

namespace ret {

namespace math {

    template <typename T>
    T clamp(T val, T min, T max) {
        return val < min ? min : (val > max ? max : val);
    }

    template <typename T>
    int signum(T val) {
        return (val > 0.0) - (val < 0.0);
    }

    template <typename T>
    double rad2deg(T val) {
        return val * 180.0 / M_PI;
    }

    template <typename T>
    double deg2rad(T val) {
        return val * M_PI / 180.0;
    }

    template <typename T>
    cv::Mat rotMatrix(double angle, T axis, bool rad) {
        if (!rad) {
            angle = deg2rad(angle);
        }

        auto s  = sin(angle);
        auto c  = cos(angle);
        auto mc = 1.0 - c;

        axis /= norm(axis);
        double x = axis[0];
        double y = axis[1];
        double z = axis[2];

        // clang-format off
        cv::Mat r = (cv::Mat_<double>(3, 3) <<
                     x*x*mc + c,   x*y*mc - z*s, x*z*mc + y*s,
                     x*y*mc + z*s, y*y*mc + c,   y*z*mc - x*s,
                     x*z*mc - y*s, y*z*mc + x*s, z*z*mc + c);
        // clang-format on

        return r;
    }

    template <typename T>
    bool equals(T x, T y) {
        return fabs(x - y) < std::numeric_limits<T>::epsilon();
    }
}
}

#endif
