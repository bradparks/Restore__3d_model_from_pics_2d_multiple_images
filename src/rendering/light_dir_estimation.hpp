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

#ifndef RENDERING_LIGHT_DIR_ESTIMATION_HPP
#define RENDERING_LIGHT_DIR_ESTIMATION_HPP

#include <cstddef>
#include <vector>

#include <vtkSmartPointer.h>
#include <opencv2/core/core.hpp>
#include <vtkType.h>

#include "calibration/ransac.hpp"

class vtkPolyData;
namespace ret { class Camera; }
namespace ret { class DataSet; }

namespace ret {

namespace rendering {

    /** @brief Calculates light directions for each @ref Camera image based
      * on the visual hull of an oject.
      * Light directions are estimated using the reconstructed surface normals
      * for each vertex using lambertian illumination model and a robust
      * @ref Ransac scheme for estimation */
    class LightDirEstimation {
      public:
        explicit LightDirEstimation(const double vis_angle_thresh    = 0.5,
                                    const std::size_t sample_size    = 1000,
                                    const std::size_t num_iterations = 2000);
        cv::Vec3f execute(Camera& cam,
                          vtkSmartPointer<vtkPolyData> visual_hull);
        void execute(DataSet& ds, vtkSmartPointer<vtkPolyData> visual_hull);
        cv::Mat displayLightDirections(const Camera& cam,
                                       const cv::Vec3f& max_consensus) const;

      private:
        cv::Vec3f estimateRansacLightDir(
            const std::vector<calib::contour_point> contour_points) const;
        std::vector<calib::contour_point> generateSamples() const;
        cv::Mat createNormalMapFromRandomContourPoints(
            const std::size_t rnd1, const std::size_t rnd2,
            const std::size_t rnd3) const;
        cv::Mat createIntensityVecFromRandomContourPoints(
            const std::size_t rnd1, const std::size_t rnd2,
            const std::size_t rnd3) const;

        double vis_angle_thresh_;
        std::size_t sample_size_;
        std::size_t num_iterations_;
        std::vector<calib::contour_point> contour_points_;
    };
} // namespace rendering
}  // namespace ret

#endif
