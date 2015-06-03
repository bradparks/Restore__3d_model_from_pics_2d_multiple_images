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

// C system files
// none

// C++ system files
// none

// header files of other libraries
#include <gtest/gtest.h>

// header files of project libraries
// none

TEST(SanityCheck, CheckIntrinsics) {
    // we hope that true is true
    ASSERT_TRUE(true);

    // these values should be equal
    ASSERT_EQ(2, 1 + 1);

    int x[] = {1, 2, 3};
    int y[] = {1, 2, 3};
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(x[i], y[i]);
    }

    double a = 1.51;
    double b = 1.52;
    ASSERT_NE(a, b);
}
