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

#ifndef COMMON_TYPES_VEC3F_HPP
#define COMMON_TYPES_VEC3F_HPP

namespace ret {

struct vec3f {
    float x;
    float y;
    float z;

    vec3f() = default;

    vec3f(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}

    inline bool operator==(const vec3f& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    inline vec3f cross(const vec3f& other) const {
        return vec3f(y * other.z - z * other.y, z * other.x - x * other.z,
                     x * other.y - y * other.x);
    }
};
}  // namespace ret

#endif
