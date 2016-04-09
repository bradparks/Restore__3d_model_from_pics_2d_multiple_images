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

#include <benchmark/benchmark.h>
#include <opencv2/core/core.hpp>

#include <cmath>
#include <memory>

#include "common/dataset.hpp"
#include "common/utils.hpp"
#include "filtering/segmentation.hpp"
#include "gui/main_window.hpp"
#include "io/assets_path.hpp"
#include "io/dataset_reader.hpp"
#include "rendering/bounding_box.hpp"
#include "rendering/mesh_coloring.hpp"
#include "rendering/voxel_carving.hpp"

using namespace ret;
using namespace ret::io;
using namespace ret::filtering;
using namespace ret::rendering;

static void BM_ImageSegmentation(benchmark::State& state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        const int num_imgs = 36;
        DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
        auto ds = dsr.load(num_imgs);

        state.ResumeTiming();
        for (auto idx = 0; idx < num_imgs; ++idx) {
            ds->getCamera(idx).setMask(Binarize(
                ds->getCamera(idx).getImage(), cv::Scalar(0, 0, 30)));
        }
    }
}
BENCHMARK(BM_ImageSegmentation);

static void BM_BoundingBox(benchmark::State& state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        const int num_imgs = 36;
        DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
        auto ds = dsr.load(num_imgs);

        for (auto idx = 0; idx < num_imgs; ++idx) {
            ds->getCamera(idx).setMask(Binarize(
                ds->getCamera(idx).getImage(), cv::Scalar(0, 0, 30)));
        }
        state.ResumeTiming();
        BoundingBox bbox =
            BoundingBox(ds->getCamera(0), ds->getCamera((num_imgs / 4) - 1));
        auto vc = ret::make_unique<VoxelCarving>(bbox.getBounds(), 128);
    }
}
BENCHMARK(BM_BoundingBox);

static void BM_VoxelCarving(benchmark::State& state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        const int num_imgs = 36;
        DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
        auto ds = dsr.load(num_imgs);

        for (auto idx = 0; idx < num_imgs; ++idx) {
            ds->getCamera(idx).setMask(Binarize(
                ds->getCamera(idx).getImage(), cv::Scalar(0, 0, 30)));
        }
        BoundingBox bbox =
            BoundingBox(ds->getCamera(0), ds->getCamera((num_imgs / 4) - 1));
        auto vc = ret::make_unique<VoxelCarving>(bbox.getBounds(), 128);
        state.ResumeTiming();
        for (const auto &cam : ds->getCameras()) vc->carve(cam);
    }
}
BENCHMARK(BM_VoxelCarving);

static void BM_ColorMesh(benchmark::State& state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        const int num_imgs = 36;
        DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
        auto ds = dsr.load(num_imgs);

        for (auto idx = 0; idx < num_imgs; ++idx) {
            ds->getCamera(idx).setMask(Binarize(
                ds->getCamera(idx).getImage(), cv::Scalar(0, 0, 30)));
        }
        BoundingBox bbox =
            BoundingBox(ds->getCamera(0), ds->getCamera((num_imgs / 4) - 1));
        auto vc = ret::make_unique<VoxelCarving>(bbox.getBounds(), 128);
        for (const auto &cam : ds->getCameras()) {
            vc->carve(cam);
        }
        auto visual_hull = vc->createVisualHull();
        state.ResumeTiming();
        Colorize(visual_hull, ds->getCameras());
    }
}
BENCHMARK(BM_ColorMesh);

BENCHMARK_MAIN();
