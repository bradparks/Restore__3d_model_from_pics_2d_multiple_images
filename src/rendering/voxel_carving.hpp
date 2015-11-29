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

#ifndef RESTORE_RENDERING_VOXEL_CARVING_HPP
#define RESTORE_RENDERING_VOXEL_CARVING_HPP

// C system files
// none

// C++ system files
#include <memory>
#include <utility>
#include <vector>

// header files of other libraries
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <opencv2/core/core.hpp>

// header files of project libraries
#include "common/camera.hpp"
#include "rendering/bounding_box.hpp"

namespace ret {

namespace rendering {

    /// @brief Start parameter used to calculate the offset when carving
    /// the visual hull
    template <typename T>
    struct start_params_t {
        T start_x, start_y, start_z;
        T voxel_width, voxel_height, voxel_depth;
    };
    typedef start_params_t<float> start_params;

    /// @brief Creates a rough 3D reconstruction (so called visual hull)
    /// from a set of @ref Camera. The physical dimension of the object is
    /// defined through a @ref BoundingBox. The visual hull is created piece
    /// by piece through calling carve for each camera in the set.
    /// @image html voxelcarving-squirrel.gif
    class VoxelCarving {
      public:
        /// @brief Given the voxel grid dimension this constructor fills up
        /// a voxel grid ready to be used for carving out a reconstruction
        /// @param bbox Dimensions of the bounding box
        /// @param voxel_grid_dim Dimension of the voxel grid
        VoxelCarving(const bb_bounds bbox, const std::size_t voxel_dim);

        VoxelCarving(VoxelCarving const&) = delete;
        VoxelCarving operator&=(VoxelCarving const&) = delete;

        /// @brief Creates a silhouette from the object, projects each
        /// voxel from the voxel grid into the the given camera and
        /// calculates the distance to the edge of the silhouette. Must be
        /// called for every @ref Camera in a set in order to create a
        /// visual hull
        /// @param cam current @ref Camera
        void carve(const Camera& cam);

        /// @brief Creates a visual hull from a camera set
        /// @param isolevel threshold used for surface extraction
        /// @return visual hull
        vtkSmartPointer<vtkPolyData> createVisualHull(
            const double isolevel = 0.0) const;

        /// @brief Manually adjust the binary image-based bounding box
        /// calculation in x and y direction
        /// @param margin_xy offset for x and y direction
        void setBoundingBoxMargin(const std::pair<float, float>& margin_xy);

        /// @brief Manually adjust the binary image-based bounding box
        /// calculation in x direction
        /// @param margin_x offset for x direction
        void setBoundingBoxXMargin(const float margin_x);

        /// @brief Manually adjust the binary image-based bounding box
        /// calculation in y direction
        /// @param margin_y offset for y direction
        void setBoundingBoxYMargin(const float margin_y);

      private:
        cv::Point3f calcVoxelPosInCamViewFrustum(const std::size_t i,
                                                 const std::size_t j,
                                                 const std::size_t k) const;
        start_params calcStartParameter(const bb_bounds& bbox) const;

        template <typename Coord, typename Size>
        inline bool inside(const Coord& im, const Size& size) {
            return (im.x > 0 && im.y > 0 && im.x < size.width &&
                    im.y < size.height);
        }

        std::size_t voxel_dim_, voxel_slice_, voxel_size_;
        std::unique_ptr<float[]> vox_array_;
        start_params params_;
        std::pair<float, float> bb_margin_;
    };
}
}

#endif
