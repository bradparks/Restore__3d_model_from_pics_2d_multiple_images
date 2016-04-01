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

#ifndef CALIBRATION_LIGHT_DIRECTION_MODEL_HPP
#define CALIBRATION_LIGHT_DIRECTION_MODEL_HPP

#include <opencv2/core/core.hpp>

namespace ret {

namespace calib {

    /** @brief Light direction model used for estimating a light direction
      * for a given @ref Camera image */
    template <typename T, typename Vec>
    class LightDirectionModel_ {
      public:
        LightDirectionModel_() : x(0), y(0), z(0) {}
        LightDirectionModel_(T x1, T y1, T z1) : x(x1), y(y1), z(z1) {}
        explicit LightDirectionModel_(const Vec l)
            : x(l[0]), y(l[1]), z(l[2]) {}
        T x, y, z;
    };
    typedef LightDirectionModel_<float, cv::Vec3f> LightDirectionModel;
}  // namespace calib
}  // namespace ret

#endif
