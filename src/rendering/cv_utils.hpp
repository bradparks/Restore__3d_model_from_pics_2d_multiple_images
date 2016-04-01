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

#ifndef RENDERING_CV_UTILS_HPP
#define RENDERING_CV_UTILS_HPP

#include <opencv2/core/core.hpp>

#include "common/camera.hpp"

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
}  // namespace ret

#endif
