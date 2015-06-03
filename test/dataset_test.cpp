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
#include "common/dataset.hpp"
#include "common/camera.hpp"

using ret::DataSet;
using ret::Camera;

TEST(DataSetTest, CreateDataSet) {
    DataSet ds;
    ds.addCamera(Camera());
    Camera cam2;
    ds.addCamera(cam2);
    ASSERT_EQ(ds.size(), 2);
}

TEST(DataSetTest, GetSetDataSetCamera) {
    DataSet ds;
    ds.addCamera(Camera());
    ds.addCamera(Camera());
    ds.setCamera(Camera(), 1);
    Camera cam = ds.getCamera(0);
    ASSERT_EQ(ds.size(), 2);
}

TEST(DataSetTest, GetSetDataSetCameras) {
    DataSet ds;
    std::vector<Camera> cams = {Camera(), Camera(), Camera()};
    ds.setCameras(cams);
    ASSERT_EQ(ds.size(), 3);
}
