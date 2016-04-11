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

#include "rendering/light_dir_estimation.hpp"

#include <cassert>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>

#include "calibration/light_direction_model.hpp"
#include "common/camera.hpp"
#include "common/dataset.hpp"
#include "rendering/cv_utils.hpp"
#include "rendering/vtk_utils.hpp"

namespace ret {

namespace rendering {

    LightDirEstimation::LightDirEstimation(const double vis_angle_thresh,
                                           const std::size_t sample_size,
                                           const std::size_t num_iterations)
        : vis_angle_thresh_(vis_angle_thresh),
          sample_size_(sample_size),
          num_iterations_(num_iterations),
          contour_points_() {
        std::srand(std::time(nullptr));
    }

    cv::Vec3f LightDirEstimation::execute(
        Camera& cam, vtkSmartPointer<vtkPolyData> visual_hull) {

        assert(cam.getImage().channels() == 3);
        contour_points_.clear();

        cv::Mat Grayscale;
        cv::cvtColor(cam.getImage(), Grayscale, CV_BGR2GRAY);
        // get points on visual hull and corresponding surface normals
        for (vtkIdType idx = 0; idx < visual_hull->GetNumberOfPoints(); ++idx) {
            auto pt_vishull = GetVertex(visual_hull, idx);
            auto normal = GetNormal(visual_hull, idx);
            auto cam_normal = cam.getDirection();
            auto angle      = normal.dot(cam_normal);
            if (angle >= vis_angle_thresh_) {
                auto coord = project<cv::Point2f, cv::Point3d>(cam, pt_vishull);
                contour_points_.emplace_back(
                    calib::contour_point(normal, Grayscale.at<uchar>(coord)));
            }
        }

        return estimateRansacLightDir(generateSamples());
    }

    void LightDirEstimation::execute(DataSet& ds,
                                     vtkSmartPointer<vtkPolyData> visual_hull) {

        assert(ds.size() > 0);
        std::vector<cv::Vec3f> light_directions;
        for (auto& cam : ds.getCameras()) {
            light_directions.emplace_back(execute(cam, visual_hull));
        }
    }

    cv::Mat LightDirEstimation::displayLightDirections(
        const Camera& cam, const cv::Vec3f& max_consensus) const {

        const auto Image = cam.getImage();
        assert(Image.channels() == 3);
        const auto img_size  = Image.size();
        const auto heat_step = 7;
        const auto radius    = img_size.height / 2;
        const cv::Point center(img_size.width / 2, img_size.height / 2);

        // create composed image with light directions
        cv::Mat Composed(img_size.height, img_size.width * 2, CV_8UC3);
        Image.copyTo(Composed(cv::Rect(cv::Point(0, 0), img_size)));

        // draw all calculated light directions in HSV color space
        cv::Mat Normals(img_size, CV_8UC3, cv::Scalar(255, 255, 255));
        cv::circle(Normals, center, radius, cv::Scalar(255, 0, 0), -1, 8);
        cv::cvtColor(Normals, Normals, CV_BGR2HSV);
        for (std::size_t idx = 0; idx < 75000; ++idx) {
            auto rnd1 = rand() % contour_points_.size();
            auto rnd2 = rand() % contour_points_.size();
            auto rnd3 = rand() % contour_points_.size();
            auto N = createNormalMapFromRandomContourPoints(rnd1, rnd2, rnd3);
            auto I =
                createIntensityVecFromRandomContourPoints(rnd1, rnd2, rnd3);
            auto l = cv::normalize(cv::Vec3f(cv::Mat(N.inv() * I)));
            cv::Point p((l[1] * radius + center.x), l[0] * radius + center.y);
            cv::Scalar heat(Normals.at<cv::Vec3b>(p)[0],
                            Normals.at<cv::Vec3b>(p)[1],
                            Normals.at<cv::Vec3b>(p)[2]);
            heat[0] = heat[0] - heat_step < 0 ? 0 : heat[0] - heat_step;
            cv::circle(Normals, p, 1, heat, -1, CV_AA);
        }
        cv::cvtColor(Normals, Normals, CV_HSV2BGR);

        // blur results for better viewing
        cv::medianBlur(Normals, Normals, 3);

        // draw RANSAC estimated light direction
        cv::circle(Normals, cv::Point((max_consensus[1] * radius + center.x),
                                      (max_consensus[0] * radius + center.y)),
                   3, cv::Scalar(0, 0, 0), -1);
        Normals.copyTo(Composed(
            cv::Rect(img_size.width, 0, img_size.width, img_size.height)));

        return Composed;
    }

    cv::Vec3f LightDirEstimation::estimateRansacLightDir(
        const std::vector<calib::contour_point> contour_points) const {
        calib::LightDirectionModel model;
        calib::Ransac ransac;
        ransac.setObservationSet(contour_points)
            .setModel(model)
            .setIterations(num_iterations_)
            .setRequiredInliers(3)
            .setThreshold(1.0f);
        ransac.getBestModel(model);

        cv::Vec3f light(model.x, model.y, model.z);
        cv::normalize(light, light);
        return light;
    }

    std::vector<calib::contour_point> LightDirEstimation::generateSamples()
        const {
        std::vector<calib::contour_point> sample_points;
        const auto cp_size = contour_points_.size();
        for (std::size_t idx = 0; idx < sample_size_; ++idx) {
            auto rnd = rand() % cp_size;
            sample_points.push_back(contour_points_[rnd]);
        }

        return sample_points;
    }

    cv::Mat LightDirEstimation::createNormalMapFromRandomContourPoints(
        const std::size_t rnd1, const std::size_t rnd2,
        const std::size_t rnd3) const {
        cv::Mat N =
            (cv::Mat_<float>(3, 3) << contour_points_[rnd1].normal[0],
             contour_points_[rnd1].normal[1], contour_points_[rnd1].normal[2],
             contour_points_[rnd2].normal[0], contour_points_[rnd2].normal[1],
             contour_points_[rnd2].normal[2], contour_points_[rnd3].normal[0],
             contour_points_[rnd3].normal[1], contour_points_[rnd3].normal[2]);

        return N;
    }

    cv::Mat LightDirEstimation::createIntensityVecFromRandomContourPoints(
        const std::size_t rnd1, const std::size_t rnd2,
        const std::size_t rnd3) const {
        cv::Mat I =
            (cv::Mat_<float>(3, 1) << contour_points_[rnd1].intensity,
             contour_points_[rnd2].intensity, contour_points_[rnd3].intensity);
        return I;
    }

} // namespace rendering
} // namespace ret
