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
// none

// header files of other libraries
#include <gtest/gtest.h>

// header files of project libraries
#include <restore/math.hpp>

using namespace ret::math;

TEST(UtilsTest, ClampMax) {

    double maxVal = 17.0;
    ASSERT_DOUBLE_EQ(maxVal, clamp(23.0, 0.0, maxVal));
}

TEST(UtilsTest, ClampMin) {

    double minVal = 3.0;
    ASSERT_DOUBLE_EQ(minVal, clamp(1.7, minVal, 10.0));
}

TEST(UtilsTest, ClampNegativeNumber) {

    double minVal = 0.0;
    ASSERT_DOUBLE_EQ(minVal, clamp(-3.2, minVal, 10.0));
}

TEST(UtilsTest, SignPositive) {

    ASSERT_EQ(1, signum(17));
}

TEST(UtilsTest, SignNeutral) {

    ASSERT_EQ(0, signum(0));
}

TEST(UtilsTest, SignNegative) {

    ASSERT_EQ(-1, signum(-23));
}

TEST(UtilsTest, Rad2DegTest) {

    double angle;
    for (int i = -10; i < 10; ++i) {
        angle = rad2deg(i * M_PI);
        ASSERT_DOUBLE_EQ(i * 180, angle);
    }
}

TEST(UtilsTest, Deg2RadTest) {

    double angle;
    for (int i = -10; i < 10; ++i) {
        angle = deg2rad(i * 180);
        ASSERT_DOUBLE_EQ(i * M_PI, angle);
    }
}
