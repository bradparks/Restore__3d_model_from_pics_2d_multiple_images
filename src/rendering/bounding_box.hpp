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

#ifndef RESTORE_RENDERING_BOUNDING_BOX_HPP
#define RESTORE_RENDERING_BOUNDING_BOX_HPP

// C system files
// none

// C++ system files
// none

// header files of other libraries
#include <opencv2/core/core.hpp>

// header files of project libraries
#include "common/camera.hpp"

namespace ret {

namespace rendering {

    /// @brief Bounding box parameter used for estimating the 3D bounding
    /// box used for @ref VoxelCarving
    template <typename T>
    struct bb_bounds_t {
        T xmin, xmax;
        T ymin, ymax;
        T zmin, zmax;
    };
    typedef bb_bounds_t<float> bb_bounds;

    /// @brief Estimates the bounding box, which is the rectangular box
    /// having a minimum value and at the same time fully includes the
    /// photographed object. Since the rotation axis of the rotary table is
    /// defined with respect to the camera geometry used in the calibration
    /// process, the bounding box is calculated by backprojecting the
    /// silhouette bounding rectangles from two orthogonal camera views and
    /// then intersecting the resulting open volumes. The orientation of the
    /// bounding box according to the world coordinate system is as follows:
    /// @code
    ///         +---------+
    ///         |\         \ bbdepth
    ///         | +---------+
    ///     (z) | |         |
    /// (y)  ↑  + |         | bbheight
    ///  ↖  |   \| bbwidth |
    ///    \ |    +---------+
    ///     \|
    ///      +-------→ (x)
    /// @endcode
    class BoundingBox {
      public:
        /// @brief Sinks the given two cameras without doing any further
        /// initialization
        BoundingBox(Camera cam1, Camera cam2);

        /// @brief Estimates the bounding box by backprojecting the object
        /// silhouette bounding rectangles from the given two camera views
        /// @return bb_bounds Estimated bounding box
        bb_bounds getBounds() const;

      private:
        Camera cam1_, cam2_;

        cv::Rect getBoundingRect(const cv::Mat& Binary) const;
    };
}
}

#endif
