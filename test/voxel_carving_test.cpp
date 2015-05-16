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
        vc = ret::make_unique<VoxelCarving>(VOXEL_DIM, cv::Size(1280, 960));
        DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
        ds = dsr.load(NUM_IMGS);
        for (std::size_t i = 0; i < NUM_IMGS; ++i) {
            ds.getCamera(i).setMask(Segmentation::binarize(
                ds.getCamera(i).getImage(), cv::Scalar(0, 0, 40)));
        }
    }

    std::unique_ptr<VoxelCarving> vc;
    ret::DataSet ds;
    const std::size_t VOXEL_DIM = 64;
    const std::size_t NUM_IMGS = 36;

    using Pair = const std::pair<float, float>;   // min, max
    using BBox = std::tuple<float, float, float>; // width, height, depth
};

TEST_F(VoxelCarvingTest, Carve) {

    Pair xdim(-6.21639f, 7.62138f);
    Pair ydim(-10.2796f, 12.1731f);
    Pair zdim(-14.0349f, 12.5358f);
    BBox bb = std::make_tuple(std::abs(xdim.second - xdim.first) * 1.15f,
                              std::abs(ydim.second - ydim.first) * 1.15f,
                              std::abs(zdim.second - zdim.first) * 1.05f);
    start_params params;
    params.start_x = xdim.first - std::abs(xdim.second - xdim.first) * 0.15f;
    params.start_y = ydim.first - std::abs(ydim.second - ydim.first) * 0.15f;
    params.start_z = 0.0f;
    params.voxel_width = std::get<0>(bb) / VOXEL_DIM;
    params.voxel_height = std::get<1>(bb) / VOXEL_DIM;
    params.voxel_depth = std::get<2>(bb) / VOXEL_DIM;
    vc->carve(ds.getCamera(0), params);
}
