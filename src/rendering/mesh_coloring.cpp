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
#include <map>

// header files of other libraries
#include <opencv2/imgproc/imgproc.hpp>
#include <vtkPointData.h>

// header files of project libraries
#include "mesh_coloring.hpp"
#include "cv_utils.hpp"

using namespace ret::rendering;

MeshColoring::MeshColoring()
    : colors_(vtkSmartPointer<vtkUnsignedCharArray>::New()),
      img_size_(cv::Size(0, 0)) {

    colors_->SetNumberOfComponents(3);
    colors_->SetName("Colors");
}

void MeshColoring::colorize(vtkSmartPointer<vtkPolyData> mesh,
                            const std::vector<Camera> dataset) {

    assert(dataset.size() > 0);
    colors_->Reset();
    img_size_ = dataset[0].getImage().size();
    for (vtkIdType idx = 0; idx < mesh->GetNumberOfPoints(); ++idx) {

        double normal[3];
        mesh->GetPointData()->GetNormals()->GetTuple(idx, normal);
        cv::Mat sf_normal =
            (cv::Mat_<float>(3, 1) << normal[0], normal[1], normal[2]);
        std::map<double, std::size_t> angles;
        for (std::size_t j = 0; j < dataset.size(); ++j) {
            cv::Mat cam_normal = getCameraDirection(dataset[j]);
            angles[sf_normal.dot(cam_normal)] = j;
        }

        // camera image indexes and appropriate dot product
        std::vector<std::size_t> indexes;
        std::vector<double> indexed_angles;
        for (const auto angle_idx : angles) {
            if (angle_idx.first >= 0.5) {
                indexes.push_back(angle_idx.second);
                indexed_angles.push_back(angle_idx.first);
            }
        }

        // assert at least one camera image for color estimation
        if (indexes.empty()) {
            indexes.push_back(angles.rbegin()->second);
            indexed_angles.push_back(angles.rbegin()->first);
        }

        // get average color
        double r = 0, g = 0, b = 0;
        for (std::size_t j = 0; j < indexes.size(); ++j) {
            double v[3];
            mesh->GetPoint(idx, v);
            Camera curr_cam = dataset[indexes[j]];
            cv::Point2i col_pix = project<cv::Point2f, cv::Point3d>(
                curr_cam, cv::Point3d(v[0], v[1], v[2]));
            auto rgb = curr_cam.getImage().at<cv::Vec3b>(col_pix.y, col_pix.x);
            r += rgb[2] * indexed_angles[j];
            g += rgb[1] * indexed_angles[j];
            b += rgb[0] * indexed_angles[j];
        }

        // weighted mean average color
        r = (r / indexes.size()) / 255.0;
        g = (g / indexes.size()) / 255.0;
        b = (b / indexes.size()) / 255.0;

        // color each vertex according to pixel value
        colors_->InsertNextTuple3(r * 255.0, g * 255.0, b * 255.0);
    }
    mesh->GetPointData()->SetScalars(colors_);
}

cv::Mat MeshColoring::getCameraDirection(const Camera& cam) const {

    cv::Mat center = (cv::Mat_<float>(3, 1) << img_size_.width / 2.0f,
                      img_size_.height / 2.0f, 1.0f);

    cv::Mat X;
    cv::solve(cam.getCalibrationMatrix(), center, X, cv::DECOMP_LU);

    cv::Mat Rt;
    cv::transpose(cam.getRotationMatrix(), Rt);
    X = Rt * (X * (-1));

    return X / cv::norm(X);
}
