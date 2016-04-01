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

#ifndef CALIBRATION_RANSAC_HPP
#define CALIBRATION_RANSAC_HPP

#include <cstddef>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/core/operations.hpp>

#include "calibration/light_direction_model.hpp"

namespace ret {

namespace calib {

    /// @brief Contour point with surface normal and intensity
    template <typename T, typename Vec>
    struct contour_point_t {
        Vec normal;
        T intensity;
        contour_point_t(Vec norm, T intens) : normal(norm), intensity(intens) {}
    };
    typedef contour_point_t<unsigned char, cv::Vec3f> contour_point;

    /** @brief Implements Random sample consensus (RANSAC) algorithm to
      * estimate the parameter of the lambertian reflection model used to
      * estimate the light direction for a given @ref Camera image together
      * with an already reconstructed visual hull of a photographed object. */
    class Ransac {
      public:
        /** @brief Initializes default parameter for estimating light
          * direction */
        Ransac();

        /** @brief Set current obseration set
          * @param observation_set Obseration set */
        Ransac& setObservationSet(std::vector<contour_point> observation_set);

        /** @brief Set current best model for light direction estimation
          * @param model Light direction model */
        Ransac& setModel(LightDirectionModel& model);

        /** @brief Set threshold for deciding, if light direction votes for
          * current model
          * @param thresh Threshold value */
        Ransac& setThreshold(const float thresh);

        /** @brief Set number of iterations
          * @param iterations Number of iterations */
        Ransac& setIterations(const std::size_t iterations);

        /** @brief Set number of required inliers
          * @param required_inliers Number of required inliers */
        Ransac& setRequiredInliers(const std::size_t required_inliers);

        /** @brief Returns best model, if one was found
          * @param model Best model for light direction */
        bool getBestModel(LightDirectionModel& model);

        /** @brief Returns light direction model for given contour points
          * @param cp1 Contour point 1
          * @param cp2 Contour point 2
          * @param cp3 Contour point 3 */
        LightDirectionModel getModel(const contour_point& cp1,
                                     const contour_point& cp2,
                                     const contour_point& cp3) const;

        /** @brief Returns light direction model for given set of contour
          * points
          * @param observation Observation set */
        LightDirectionModel getModel(
            const std::vector<contour_point>& observation) const;

      private:
        bool isMember(const contour_point& cp,
                      const std::vector<contour_point>& maybe_inliers) const;
        bool fitsModel(const contour_point& cp,
                       const LightDirectionModel& model) const;
        std::vector<contour_point> getMaybeInliers() const;
        int getModelError(const std::vector<contour_point>& point_list,
                          const LightDirectionModel& model) const;
        float getDistance(const contour_point& cp,
                          const LightDirectionModel& model) const;

        std::vector<contour_point> observation_set_;
        std::vector<contour_point> best_consensus_set_;
        LightDirectionModel best_model_;
        std::size_t required_inliers_;
        std::size_t iterations_;
        float best_error_;
        float threshold_;
    };
}
}

#endif
