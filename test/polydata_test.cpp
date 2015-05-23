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

// C system files
// none

// C++ system files
#include <vector>

// header files of other libraries
#include <gtest/gtest.h>

// header files of project libraries
#include <restore/types.hpp>
#include <restore/common.hpp>

using namespace ret;

TEST(PolyDataTest, GetSetTriangles) {

    std::vector<triangle> triangles;
    for (std::size_t i = 0; i < 4; ++i) {
        triangle tri;
        tri.comp.v1 = {static_cast<float>(i), 0.0f, 0.0f};
        tri.comp.v2 = {0.0f, static_cast<float>(i), 0.0f};
        tri.comp.v3 = {0.0f, 0.0f, static_cast<float>(i)};
        triangles.push_back(tri);
    }

    PolyData pd;
    pd.setTriangles(triangles);
    const std::vector<triangle> triangles2 = pd.getTriangles();
    ASSERT_EQ(triangles.size(), triangles2.size());
    for (std::size_t i = 0; i < triangles.size(); ++i) {
        ASSERT_EQ(triangles[i], triangles2[i]);
    }
}
