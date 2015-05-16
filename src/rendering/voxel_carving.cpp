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

#include "voxel_carving.hpp"
#include "cv_utils.hpp"
#include "../common/utils.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace ret::rendering;

VoxelCarving::VoxelCarving(const std::size_t voxel_grid_dim,
                           const cv::Size& img_size)
    : img_size_(img_size), voxel_grid_dim_(voxel_grid_dim) {

    voxel_slice_ = voxel_grid_dim * voxel_grid_dim;
    voxel_size_ = voxel_grid_dim * voxel_grid_dim * voxel_grid_dim;
    vox_array = ret::make_unique<float[]>(voxel_size_);
}

void VoxelCarving::carve(const Camera& cam, const start_params& params) {

    assert(cam.getImage().size() == img_size_);
    cv::Mat Silhouette, DistImage;
    auto Mask = cam.getMask();
    cv::Canny(Mask, Silhouette, 0, 255);
    cv::bitwise_not(Silhouette, Silhouette);
    cv::distanceTransform(Silhouette, DistImage, CV_DIST_L2, 3);

    for (std::size_t i = 0; i < voxel_grid_dim_; ++i) {
        for (std::size_t j = 0; j < voxel_grid_dim_; ++j) {
            for (std::size_t k = 0; k < voxel_grid_dim_; ++k) {

                // estimate voxel position inside camera view frustum
                voxel v;
                v.xpos = params.start_x + i * params.voxel_width;
                v.ypos = params.start_y + j * params.voxel_height;
                v.zpos = params.start_z + k * params.voxel_depth;
                v.value = 1.0f;

                auto im = project<cv::Point2f, voxel>(cam, v);
                auto dist = -1.0f;

                // check, if projected voxel is within image coords
                if (inside(im)) {
                    dist = DistImage.at<float>(im);
                    if (Mask.at<uchar>(im) == 0) {
                        dist *= -1.0f;
                    }
                }

                const auto vox_idx = i * voxel_slice_ + j * voxel_grid_dim_ + k;
                if (dist < vox_array[vox_idx]) {
                    vox_array[vox_idx] = dist;
                }
            }
        }
    }
}
