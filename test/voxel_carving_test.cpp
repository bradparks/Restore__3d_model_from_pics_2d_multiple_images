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

#include "../src/io/assets_path.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <utility>
#include <tuple>
#include <restore/common.hpp>
#include <restore/rendering.hpp>
#include <restore/io.hpp>
#include <restore/filtering.hpp>

using namespace ret::rendering;
using namespace ret::io;
using namespace ret::filtering;

class VoxelCarvingTest : public testing::Test {

  public:
    virtual void SetUp() {
        DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
        ds = dsr.load(NUM_IMGS);
        for (std::size_t i = 0; i < NUM_IMGS; ++i) {
            ds.getCamera(i).setMask(Segmentation::binarize(
                ds.getCamera(i).getImage(), cv::Scalar(0, 0, 40)));
        }
        BoundingBox bbox =
            BoundingBox(ds.getCamera(0), ds.getCamera(NUM_IMGS - 1));
        vc = ret::make_unique<VoxelCarving>(bbox.getBounds(), VOXEL_DIM);
    }

    std::unique_ptr<VoxelCarving> vc;
    ret::DataSet ds;
    const std::size_t VOXEL_DIM = 128;
    const std::size_t NUM_IMGS = 36;
};

TEST_F(VoxelCarvingTest, Carve) {

    for (std::size_t i = 0; i < ds.size(); ++i) {
        vc->carve(ds.getCamera(i));
    }
    vc->exportToDisk();
}
