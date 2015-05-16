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

#ifndef RESTORE_RENDERING_VOXEL_CARVING_HPP
#define RESTORE_RENDERING_VOXEL_CARVING_HPP

#include "../common/camera.hpp"
#include <memory>
#include <opencv2/core/core.hpp>

namespace ret {

    namespace rendering {

        template <typename T>
        struct voxel_t {
            T xpos, ypos, zpos;
            T res;
            T value;
        };
        typedef voxel_t<float> voxel;

        template <typename T>
        struct start_params_t {
            T start_x, start_y, start_z;
            T voxel_width, voxel_height, voxel_depth;
        };
        typedef start_params_t<float> start_params;

        class VoxelCarving {
          public:
            VoxelCarving(const std::size_t voxel_grid_dim,
                         const cv::Size& img_size);
            VoxelCarving(VoxelCarving const&) = delete;
            VoxelCarving operator&=(VoxelCarving const&) = delete;
            void carve(const Camera& cam, const start_params& params);

          private:
            cv::Size img_size_;
            std::size_t voxel_grid_dim_, voxel_slice_, voxel_size_;
            std::unique_ptr<float[]> vox_array;

            template <typename T>
            inline bool inside(const T& im) {
                return (im.x > 0 && im.y > 0 && im.x < img_size_.width &&
                        im.y < img_size_.height);
            }
        };
    }
}

#endif
