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

// C system files
// none

// C++ system files
#include <memory>
#include <vector>
#include <utility>

// header files of other libraries
#include <opencv2/core/core.hpp>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

// header files of project libraries
#include "bounding_box.hpp"
#include "../common/camera.hpp"

namespace ret {

    namespace rendering {

        template <typename T>
        struct start_params_t {
            T start_x, start_y, start_z;
            T voxel_width, voxel_height, voxel_depth;
        };
        typedef start_params_t<float> start_params;

        class VoxelCarving {
          public:
            VoxelCarving(const bb_bounds bbox,
                         const std::size_t voxel_grid_dim);
            VoxelCarving(VoxelCarving const&) = delete;
            VoxelCarving operator&=(VoxelCarving const&) = delete;
            void carve(const Camera& cam);
            vtkSmartPointer<vtkPolyData> createVisualHull(
                const double isolevel = 0.0);
            void setBoundingBoxMargin(const std::pair<float, float>& margin_xy);
            void setBoundingBoxXMargin(const float margin_x);
            void setBoundingBoxYMargin(const float margin_y);

          private:
            std::size_t voxel_dim_, voxel_slice_, voxel_size_;
            std::unique_ptr<float[]> vox_array_;
            start_params params_;
            std::pair<float, float> bb_margin_;

            cv::Point3f calcVoxelPosInCamViewFrustum(const std::size_t i,
                                                     const std::size_t j,
                                                     const std::size_t k) const;
            start_params calcStartParameter(const bb_bounds& bbox) const;

            template <typename Coord, typename Size>
            inline bool inside(const Coord& im, const Size& size) {
                return (im.x > 0 && im.y > 0 && im.x < size.width &&
                        im.y < size.height);
            }
        };
    }
}

#endif
