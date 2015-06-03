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
#include <string>

// header files of other libraries
#include <opencv2/highgui/highgui.hpp>

// header files of project libraries
#include "common/dataset.hpp"
#include "common/utils.hpp"
#include "io/dataset_reader.hpp"
#include "io/assets_path.hpp"
#include "filtering/segmentation.hpp"
#include "rendering/bounding_box.hpp"
#include "rendering/voxel_carving.hpp"
#include "rendering/light_dir_estimation.hpp"

using namespace ret;
using namespace ret::io;
using namespace ret::calib;
using namespace ret::filtering;
using namespace ret::rendering;

DataSet loadDataSet(const std::string& path, const std::size_t num_imgs) {

    DataSetReader dsr(path);
    DataSet ds = dsr.load(num_imgs);
    for (std::size_t i = 0; i < num_imgs; ++i) {
        ds.getCamera(i).setMask(Segmentation::binarize(
            ds.getCamera(i).getImage(), cv::Scalar(0, 0, 30)));
    }

    return ds;
}

int main() {

    const std::size_t VOXEL_DIM = 128;
    const std::size_t NUM_IMGS = 36;
    DataSet ds = loadDataSet(std::string(ASSETS_PATH) + "/squirrel", NUM_IMGS);
    BoundingBox bbox(ds.getCamera(0), ds.getCamera((NUM_IMGS / 4) - 1));
    auto bb_bounds = bbox.getBounds();
    auto vc = ret::make_unique<VoxelCarving>(bb_bounds, VOXEL_DIM);
    for (const auto& cam : ds.getCameras()) {
        vc->carve(cam);
    }
    auto mesh = vc->createVisualHull();

    LightDirEstimation light;
    for (auto idx = 0; idx < NUM_IMGS; ++idx) {
        auto light_dir = light.execute(ds.getCamera(idx), mesh);
        cv::Mat light_dirs = light.displayLightDirections(
            ds.getCamera(idx).getImage(), light_dir);
        cv::imwrite("light_dir" + std::to_string(idx) + ".png", light_dirs);
    }

    return 0;
}
