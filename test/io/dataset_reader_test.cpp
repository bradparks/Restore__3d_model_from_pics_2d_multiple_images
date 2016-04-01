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

#include <gtest/gtest.h>

#include "common/dataset.hpp"
#include "io/dataset_reader.hpp"
#include "io/assets_path.hpp"

using ret::DataSet;
using ret::io::DataSetReader;

TEST(DataSetReaderTest, ReadDirectory) {

    const std::string assetsPath(ASSETS_PATH);
    DataSetReader dsr(assetsPath + "/squirrel");
    const int numImages = 36;
    DataSet ds = dsr.load(numImages);
    ASSERT_EQ(ds.size(), numImages);

    for (std::size_t i = 0; i < ds.size(); ++i) {
        ret::Camera cam = ds.getCamera(i);
        ASSERT_TRUE(cam.getCalibrationMatrix().size() == cv::Size(3, 3));
        ASSERT_TRUE(cam.getDistortionCoeffs().size() == cv::Size(4, 1));
        ASSERT_TRUE(cam.getProjectionMatrix().size() == cv::Size(4, 3));
        ASSERT_TRUE(cam.getRotationMatrix().size() == cv::Size(3, 3));
    }
}
