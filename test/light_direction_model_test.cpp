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
#include <restore/calibration.hpp>

using namespace ret::calib;

TEST(LightDirectionModelTest, AssertMembersInitialized) {

    LightDirectionModel ldm;
    ASSERT_FLOAT_EQ(ldm.x, 0);
    ASSERT_FLOAT_EQ(ldm.y, 0);
    ASSERT_FLOAT_EQ(ldm.z, 0);
}

TEST(LightDirectionModelTest, AssertGivenMembersAreSet) {

    LightDirectionModel ldm(2.3f, 4.5f, 6.7f);
    ASSERT_FLOAT_EQ(ldm.x, 2.3f);
    ASSERT_FLOAT_EQ(ldm.y, 4.5f);
    ASSERT_FLOAT_EQ(ldm.z, 6.7f);
}

TEST(LightDirectionModelTest, AssertMembersFromVecAreInitialized) {

    cv::Vec3f vec(1.2f, 2.3f, 3.4f);
    LightDirectionModel ldm(vec);
    ASSERT_FLOAT_EQ(ldm.x, 1.2f);
    ASSERT_FLOAT_EQ(ldm.y, 2.3f);
    ASSERT_FLOAT_EQ(ldm.z, 3.4f);
}
