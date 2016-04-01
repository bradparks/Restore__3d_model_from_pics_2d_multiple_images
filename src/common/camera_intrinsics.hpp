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

#ifndef COMMON_CAMERA_INTRINSICS_HPP
#define COMMON_CAMERA_INTRINSICS_HPP

#include <cassert>
#include <ostream>

#include <opencv2/core/core.hpp>

namespace ret {

class CameraIntrinsics {
  public:
    CameraIntrinsics()
        : K_((cv::Mat_<float>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 1)),
          dist_(cv::Mat::zeros(1, 4, CV_64F)) {}

    explicit CameraIntrinsics(const cv::Mat& K)
        : K_(K), dist_(cv::Mat::zeros(1, 4, CV_64F)) {
        assert(K.size() == cv::Size(3, 3) && K.type() == CV_32F);
    }

    virtual ~CameraIntrinsics() = default;

    CameraIntrinsics(const CameraIntrinsics& other)
        = default;

    CameraIntrinsics& operator=(const CameraIntrinsics& other) = default;

    friend bool operator==(const CameraIntrinsics& c1,
                           const CameraIntrinsics& c2) {
        auto sizeInBytesK =
            c1.K_.step[0] * static_cast<std::size_t>(c1.K_.rows);
        auto sizeInBytesDist =
            c1.dist_.step[0] * static_cast<std::size_t>(c1.dist_.rows);
        return (memcmp(c1.K_.data, c2.K_.data, sizeInBytesK) == 0 &&
                memcmp(c1.dist_.data, c2.dist_.data, sizeInBytesDist) == 0);
    }

    template <typename T>
    CameraIntrinsics& setCalibrationMatrix(T&& K) {
        assert(K.size() == cv::Size(3, 3) && K.type() == CV_32F);
        this->K_ = std::forward<T>(K);
        return *this;
    }

    cv::Mat getCalibrationMatrix() const { return K_; }

    template <typename T>
    CameraIntrinsics& setDistortionCoeffs(T&& dist) {
        assert(dist.size() == cv::Size(4, 1) && dist.type() == CV_64F);
        this->dist_ = std::forward<T>(dist);
        return *this;
    }

    cv::Mat getDistortionCoeffs() const { return dist_; }

    friend std::ostream& operator<<(std::ostream& os,
                                    const CameraIntrinsics& intr) {
        return os << intr.K_;
    }

  protected:
    cv::Mat K_;
    cv::Mat dist_;
};
} // namespace ret

#endif
