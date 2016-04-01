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

#ifndef COMMON_POLYDATA_HPP
#define COMMON_POLYDATA_HPP

#include <utility>
#include <vector>

#include "common/types/triangle.hpp"
#include "common/types/vec3f.hpp"

namespace ret {

class PolyData {
  public:
    PolyData() : triangles_(), normals_() {}

    template <typename T>
    void setTriangles(T&& triangles) {
        triangles_ = std::forward<T>(triangles);
    }

    std::vector<triangle> getTriangles() const { return triangles_; }

    template <typename T>
    void setNormals(T&& normals) {
        normals_ = std::forward<T>(normals);
    }

    std::vector<vec3f> getNormals() const { return normals_; }

  private:
    std::vector<triangle> triangles_;
    std::vector<vec3f> normals_;
};
}

#endif
