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

#include "calibration/ransac.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

#include <opencv2/core/mat.hpp>

namespace ret {

namespace calib {

    Ransac::Ransac()
        : required_inliers_(0),
          iterations_(0),
          best_error_(std::numeric_limits<float>::max()),
          threshold_(0) {
        // initialize random seed
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    Ransac& Ransac::setObservationSet(
        std::vector<contour_point> observation_set) {
        observation_set_ = observation_set;
        return *this;
    }

    Ransac& Ransac::setModel(LightDirectionModel& model) {
        best_model_ = model;
        return *this;
    }

    Ransac& Ransac::setThreshold(const float thresh) {
        threshold_ = thresh;
        return *this;
    }

    Ransac& Ransac::setIterations(const std::size_t iterations) {
        iterations_ = iterations;
        return *this;
    }

    Ransac& Ransac::setRequiredInliers(const std::size_t required_inliers) {
        required_inliers_ = required_inliers;
        return *this;
    }

    bool Ransac::getBestModel(LightDirectionModel& model) {
        auto model_found       = false;
        std::size_t iterations = 0;

        while (iterations < iterations_) {
            auto maybe_inliers = getMaybeInliers();
            auto consensus_set = maybe_inliers;
            model =
                getModel(maybe_inliers[0], maybe_inliers[1], maybe_inliers[2]);

            for (const auto& date : observation_set_) {
                if (!isMember(date, maybe_inliers) && fitsModel(date, model)) {
                    consensus_set.push_back(date);
                }
            }

            if (consensus_set.size() >= required_inliers_) {
                model            = getModel(consensus_set);
                float this_error = getModelError(consensus_set, model);

                if (this_error < best_error_) {
                    best_consensus_set_ = consensus_set;
                    best_model_         = model;
                    best_error_         = this_error;
                    model_found         = true;
                }
            }

            iterations += 1;
        }

        model = best_model_;
        return model_found;
    }

    LightDirectionModel Ransac::getModel(const contour_point& cp1,
                                         const contour_point& cp2,
                                         const contour_point& cp3) const {
        LightDirectionModel model;
        cv::Mat N;
        N.push_back(cp1.normal);
        N.push_back(cp2.normal);
        N.push_back(cp3.normal);
        cv::Vec3f I(cp1.intensity, cp2.intensity, cp3.intensity);
        cv::Mat S = N.reshape(1, 3).inv() * cv::Mat(I);
        cv::normalize(S, S);

        return LightDirectionModel(cv::Vec3f(S));
    }

    LightDirectionModel Ransac::getModel(
        const std::vector<contour_point>& observation) const {
        // estimate light direction with SVD
        cv::Mat I, N;
        for (const auto& date : observation) {
            I.push_back(static_cast<float>(date.intensity));
            N.push_back(date.normal);
        }

        cv::Mat N1;
        cv::invert(N.reshape(1, static_cast<int>(observation.size())), N1,
                   cv::DECOMP_SVD);
        cv::Mat S = N1 * I;
        cv::normalize(S, S);

        return LightDirectionModel(cv::Vec3f(S));
    }

    bool Ransac::isMember(
        const contour_point& cp,
        const std::vector<contour_point>& maybe_inliers) const {
        for (const auto& date : maybe_inliers) {
            if (cp.normal == date.normal && cp.intensity == date.intensity) {
                return true;
            }
        }

        return false;
    }

    bool Ransac::fitsModel(const contour_point& cp,
                           const LightDirectionModel& model) const {
        return (getDistance(cp, model) < threshold_);
    }

    std::vector<contour_point> Ransac::getMaybeInliers() const {
        std::vector<contour_point> maybe_inliers;
        for (std::size_t i = 0; i < 3; ++i) {
            maybe_inliers.push_back(
                observation_set_.at(static_cast<std::size_t>(rand()) %
                                    observation_set_.size()));
        }

        return maybe_inliers;
    }

    int Ransac::getModelError(const std::vector<contour_point>& point_list,
                              const LightDirectionModel& model) const {

        auto max_error = std::numeric_limits<int>::max();
        std::vector<contour_point> consensus_set;

        for (const auto& point : point_list) {
            if (fitsModel(point, model)) {
                consensus_set.push_back(point);
            }
        }

        return static_cast<int>(static_cast<std::size_t>(max_error) -
                                consensus_set.size());
    }

    float Ransac::getDistance(const contour_point& cp,
                              const LightDirectionModel& model) const {
        // check dicrepancy between observed and predicted intensity
        cv::Vec3f l(model.x, model.y, model.z);
        return std::abs(l.dot(cp.normal) - (cp.intensity / 255.0f));
    }

} // namespace calib
} // namespace ret
