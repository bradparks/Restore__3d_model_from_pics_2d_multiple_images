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

#ifndef RESTORE_COMMON_CAMERA_EXTRINSICS_HPP
#define RESTORE_COMMON_CAMERA_EXTRINSICS_HPP

// C system files
// none

// C++ system files
#include <cstring>
#include <ostream>

// header files of other libraries
#include <opencv2/core/core.hpp>

// header files of project libraries
// none

namespace ret {

class CameraExtrinsics {
  public:
    CameraExtrinsics()
        : R_(cv::Mat(3, 3, CV_32F, cv::Scalar::all(0))),
          t_(cv::Mat(4, 1, CV_32F, cv::Scalar::all(0))) {}

    CameraExtrinsics(const cv::Mat& R, const cv::Mat& t) : R_(R), t_(t) {
        assert(R.size() == cv::Size(3, 3) && R.type() == CV_32F);
        assert(t.size() == cv::Size(1, 4) && t.type() == CV_32F);
    }

    virtual ~CameraExtrinsics() = default;

    CameraExtrinsics(const CameraExtrinsics& other)
        : R_(other.R_), t_(other.t_) {}

    CameraExtrinsics& operator=(const CameraExtrinsics& other) {
        this->R_ = other.R_;
        this->t_ = other.t_;
        return *this;
    }

    friend bool operator==(const CameraExtrinsics& c1,
                           const CameraExtrinsics& c2) {
        auto sizeInBytesR =
            c1.R_.step[0] * static_cast<std::size_t>(c1.R_.rows);
        auto sizeInBytesT =
            c1.t_.step[0] * static_cast<std::size_t>(c1.t_.rows);
        return (std::memcmp(c1.R_.data, c2.R_.data, sizeInBytesR) == 0 &&
                std::memcmp(c1.t_.data, c2.t_.data, sizeInBytesT) == 0);
    }

    template <typename T>
    CameraExtrinsics& setRotationMatrix(T&& R) {
        assert(R.size() == cv::Size(3, 3) && R.type() == CV_32F);
        this->R_ = std::forward<T>(R);
        return *this;
    }

    cv::Mat getRotationMatrix() const { return this->R_; }

    template <typename T>
    CameraExtrinsics& setTranslationVector(T&& t) {
        assert(t.size() == cv::Size(1, 4) && t.type() == CV_32F);
        this->t_ = std::forward<T>(t);
        return *this;
    }

    cv::Mat getTranslationVector() const { return this->t_; }

    friend std::ostream& operator<<(std::ostream& os,
                                    const CameraExtrinsics& extr) {
        return os << extr.R_ << "\n" << extr.t_ << std::endl;
    }

  protected:
    cv::Mat R_, t_;
};
}

#endif
