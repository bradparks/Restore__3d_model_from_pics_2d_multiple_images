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

#include "rendering/mesh_coloring.hpp"

#include <cassert>
#include <cstddef>
#include <map>
#include <utility>

#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <vtkUnsignedCharArray.h>

#include "common/camera.hpp"
#include "rendering/cv_utils.hpp"

namespace ret {

namespace rendering {

    MeshColoring::MeshColoring()
        : colors_(vtkSmartPointer<vtkUnsignedCharArray>::New()),
          img_size_(cv::Size(0, 0)) {

        colors_->SetNumberOfComponents(3);
        colors_->SetName("Colors");
    }

    void MeshColoring::colorize(vtkSmartPointer<vtkPolyData> mesh,
                                std::vector<Camera> dataset) {

        assert(dataset.size() > 0);
        colors_->Reset();
        img_size_ = dataset[0].getImage().size();
        for (auto idx = 0; idx < mesh->GetNumberOfPoints(); ++idx) {

            double normal[3];
            mesh->GetPointData()->GetNormals()->GetTuple(idx, normal);
            cv::Mat sf_normal =
                (cv::Mat_<float>(3, 1) << normal[0], normal[1], normal[2]);
            std::map<double, std::size_t> angles;
            for (std::size_t j = 0; j < dataset.size(); ++j) {
                cv::Mat cam_normal                = dataset[j].getDirection();
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
                Camera curr_cam     = dataset[indexes[j]];
                cv::Point2i col_pix = project<cv::Point2f, cv::Point3d>(
                    curr_cam, cv::Point3d(v[0], v[1], v[2]));
                auto rgb =
                    curr_cam.getImage().at<cv::Vec3b>(col_pix.y, col_pix.x);
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

} // namespace rendering
} // namespace ret
