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

// C system files
// none

// C++ system files
#include <cstdlib>

// header files of other libraries
#include <opencv2/imgproc/imgproc.hpp>
#include <vtkPointData.h>

// header files of project libraries
#include "light_dir_estimation.hpp"
#include "cv_utils.hpp"

using namespace ret::rendering;

LightDirEstimation::LightDirEstimation(const double vis_angle_thresh,
                                       const std::size_t sample_size,
                                       const std::size_t num_iterations)
    : vis_angle_thresh_(vis_angle_thresh),
      sample_size_(sample_size),
      num_iterations_(num_iterations) {
    std::srand(std::time(nullptr));
}

cv::Vec3f LightDirEstimation::execute(
    const Camera& cam, vtkSmartPointer<vtkPolyData> visual_hull) {

    assert(cam.getImage().channels() == 3);
    using calib::contour_point;
    const cv::Size img_size = cam.getImage().size();
    contour_points_.clear();

    cv::Mat Grayscale;
    cv::cvtColor(cam.getImage(), Grayscale, CV_BGR2GRAY);
    // get points on visual hull and corresponding surface normals
    for (vtkIdType idx = 0; idx < visual_hull->GetNumberOfPoints(); ++idx) {
        auto pt_vishull = getVertex(visual_hull, idx);
        auto normal = getNormal(visual_hull, idx);
        auto cam_normal = getCameraDirection(cam, img_size);
        double angle = normal.dot(cam_normal);
        if (angle >= vis_angle_thresh_) {
            auto coord = project<cv::Point2f, cv::Point3d>(cam, pt_vishull);
            contour_points_.emplace_back(
                contour_point(normal, Grayscale.at<uchar>(coord.y, coord.x)));
        }
    }

    std::vector<contour_point> sample_points;
    const auto cp_size = contour_points_.size();
    for (std::size_t idx = 0; idx < sample_size_; ++idx) {
        auto rnd = std::rand() % cp_size;
        sample_points.push_back(contour_points_[rnd]);
    }

    return estimateRansacLightDir(sample_points);
}

void LightDirEstimation::execute(const DataSet& ds,
                                 vtkSmartPointer<vtkPolyData> visual_hull) {

    using calib::contour_point;
    assert(ds.size() > 0);
    std::vector<cv::Vec3f> light_directions;
    for (const auto& cam : ds.getCameras()) {
        light_directions.emplace_back(execute(cam, visual_hull));
    }
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

cv::Vec3d LightDirEstimation::getVertex(
    vtkSmartPointer<vtkPolyData> visual_hull, const vtkIdType id) const {
    double v[3];
    visual_hull->GetPoint(id, v);
    return cv::Vec3d(v[0], v[1], v[2]);
}

cv::Vec3d LightDirEstimation::getNormal(
    vtkSmartPointer<vtkPolyData> visual_hull, const vtkIdType id) const {
    double n[3];
    visual_hull->GetPointData()->GetNormals()->GetTuple(id, n);
    return cv::Vec3d(n[0], n[1], n[2]);
}
