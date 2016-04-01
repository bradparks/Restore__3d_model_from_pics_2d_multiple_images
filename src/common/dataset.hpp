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

#ifndef COMMON_DATASET_HPP
#define COMMON_DATASET_HPP

#include <vector>

#include <opencv2/core/core.hpp>

#include "common/camera.hpp"

namespace ret {

class DataSet {
  public:
    DataSet() : cameras_() {}

    template <typename T>
    void setCameras(T&& cameras) {
        this->cameras_ = std::forward<T>(cameras);
    }

    std::vector<Camera> getCameras() const { return cameras_; }

    template <typename T>
    void setCamera(T&& camera, const std::size_t camIdx) {
        assert(camIdx < cameras_.size());
        cameras_[camIdx] = std::forward<T>(camera);
    }

    Camera& getCamera(const std::size_t camIdx) {
        assert(camIdx < cameras_.size());
        return cameras_[camIdx];
    }

    template <typename T>
    void addCamera(T&& camera) {
        cameras_.push_back(std::forward<T>(camera));
    }

    std::size_t size() const { return cameras_.size(); }

  private:
    std::vector<Camera> cameras_;
};
}  // namespace ret

#endif
