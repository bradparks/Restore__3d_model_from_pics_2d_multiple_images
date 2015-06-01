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

#ifndef RESTORE_RENDERING_LIGHT_DIR_ESTIMATION_HPP
#define RESTORE_RENDERING_LIGHT_DIR_ESTIMATION_HPP

// C system files
// none

// C++ system files
#include <vector>

// header files of other libraries
#include <opencv2/core/core.hpp>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

// header files of project libraries
#include <restore/common.hpp>
#include <restore/calibration.hpp>

namespace ret {

    namespace rendering {

        /// @brief Calculates light directions for each @ref Camera image based
        /// on the visual hull of an oject. Light directions are estimated using
        /// the reconstructed surface normals for each vertex using lambertian
        /// illumination model and a robust @ref Ransac scheme for estimation
        class LightDirEstimation {
          public:
            LightDirEstimation(const double vis_angle_thresh = 0.5,
                               const std::size_t sample_size = 1000,
                               const std::size_t num_iterations = 2000);
            cv::Vec3f execute(Camera& cam,
                              vtkSmartPointer<vtkPolyData> visual_hull);
            void execute(DataSet& ds, vtkSmartPointer<vtkPolyData> visual_hull);
            cv::Mat displayLightDirections(
                const Camera& cam, const cv::Vec3f& max_consensus) const;

          private:
            double vis_angle_thresh_;
            std::size_t sample_size_;
            std::size_t num_iterations_;
            std::vector<calib::contour_point> contour_points_;

            cv::Vec3f estimateRansacLightDir(
                const std::vector<calib::contour_point> contour_points) const;
            std::vector<calib::contour_point> generateSamples() const;
            cv::Mat createNormalMapFromRandomContourPoints(
                const std::size_t rnd1, const std::size_t rnd2,
                const std::size_t rnd3) const;
            cv::Mat createIntensityVecFromRandomContourPoints(
                const std::size_t rnd1, const std::size_t rnd2,
                const std::size_t rnd3) const;
            cv::Vec3d getVertex(vtkSmartPointer<vtkPolyData> visual_hull,
                                const vtkIdType id) const;
            cv::Vec3d getNormal(vtkSmartPointer<vtkPolyData> visual_hull,
                                const vtkIdType id) const;
        };
    }
}

#endif
