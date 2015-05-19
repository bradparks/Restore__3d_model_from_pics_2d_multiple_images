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
#include <cstdlib>

// header files of other libraries
#include <opencv2/imgproc/imgproc.hpp>

// header files of project libraries
#include "voxel_carving.hpp"
#include "cv_utils.hpp"
#include "mc/marching_cubes.hpp"
#include "../common/utils.hpp"

using namespace ret::rendering;

VoxelCarving::VoxelCarving(const bb_bounds bbox,
                           const std::size_t voxel_grid_dim)
    : bbox_(bbox), voxel_grid_dim_(voxel_grid_dim) {

    voxel_slice_ = voxel_grid_dim * voxel_grid_dim;
    voxel_size_ = voxel_grid_dim * voxel_grid_dim * voxel_grid_dim;
    vox_array = ret::make_unique<float[]>(voxel_size_);
}

void VoxelCarving::carve(const Camera& cam) {

    cv::Mat Silhouette, DistImage;
    cv::Mat Mask = cam.getMask();
    cv::Canny(Mask, Silhouette, 0, 255);
    cv::bitwise_not(Silhouette, Silhouette);
    cv::distanceTransform(Silhouette, DistImage, CV_DIST_L2, 3);
    params_ = calcStartParameter(bbox_);
    cv::Size img_size = Mask.size();

    for (std::size_t i = 0; i < voxel_grid_dim_; ++i) {
        for (std::size_t j = 0; j < voxel_grid_dim_; ++j) {
            for (std::size_t k = 0; k < voxel_grid_dim_; ++k) {

                voxel v = calcVoxelPosInCamViewFrustum(i, j, k);
                auto im = project<cv::Point2f, voxel>(cam, v);
                auto dist = -1.0f;
                // check, if projected voxel is within image coords
                if (inside(im, img_size)) {
                    dist = DistImage.at<float>(im);
                    if (Mask.at<uchar>(im) == 0) { // outside
                        dist *= -1.0f;
                    }
                }

                const auto idx = k + j * voxel_grid_dim_ + i * voxel_slice_;
                if (dist < vox_array[idx]) {
                    vox_array[idx] = dist;
                }
            }
        }
    }
}

void VoxelCarving::exportToDisk() const {

    auto mc = ret::make_unique<mc::MarchingCubes>();
    const int voxel_dim = static_cast<int>(voxel_grid_dim_);
    mc->setParams(params_.start_x, params_.start_z, params_.start_y,
                  params_.voxel_width, params_.voxel_depth,
                  params_.voxel_height, 0.0f, voxel_dim, voxel_dim, voxel_dim);
    mc->execute(vox_array.get());
    mc->saveAsOBJ("export.obj");
}

voxel VoxelCarving::calcVoxelPosInCamViewFrustum(const std::size_t i,
                                                 const std::size_t j,
                                                 const std::size_t k) const {
    voxel v;
    v.xpos = params_.start_x + k * params_.voxel_width;
    v.ypos = params_.start_y + j * params_.voxel_height;
    v.zpos = params_.start_z + i * params_.voxel_depth;
    v.value = 1.0f;

    return v;
}

start_params VoxelCarving::calcStartParameter(const bb_bounds& bbox) const {

    auto PADDING = 0.38f;

    auto bb_width = std::abs(bbox.xmax - bbox.xmin) * (1.0f + 2.0f * PADDING);
    auto bb_height = std::abs(bbox.ymax - bbox.ymin) * (1.0f + 2.0f * PADDING);
    auto bb_depth = std::abs(bbox.zmax - bbox.zmin);

    auto offset_x = (bb_width - std::abs(bbox.xmax - bbox.xmin)) / 2.0f;
    auto offset_y = (bb_height - std::abs(bbox.ymax - bbox.ymin)) / 2.0f;

    start_params params;
    params.start_x = bbox.xmin - offset_x;
    params.start_y = bbox.ymin - offset_y;
    params.start_z = 0.0f;
    params.voxel_width = bb_width / voxel_grid_dim_;
    params.voxel_height = bb_height / voxel_grid_dim_;
    params.voxel_depth = bb_depth / voxel_grid_dim_;

    return params;
}
