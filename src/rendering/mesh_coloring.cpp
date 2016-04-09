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
#include <type_traits>
#include <utility>

#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkUnsignedCharArray.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common/camera.hpp"
#include "rendering/cv_utils.hpp"

namespace ret {

namespace rendering {

    template<typename T>
    struct Color {
        Color() : r(0), g(0), b(0) { }
        Color<T> &operator+=(cv::Vec3b rhs) {
            r += rhs[2];
            g += rhs[1];
            b += rhs[0];
            return *this;
        }
        Color<T> &operator/=(std::size_t rhs) {
            r /= rhs;
            g /= rhs;
            b /= rhs;
            return *this;
        }

        T r, g, b;
    };
    static_assert(std::is_standard_layout<Color<double>>::value,
                  "Color isn't standard layout");

    cv::Vec3b getColor(const cv::Mat &img, cv::Point2f pt) {
        return img.at<cv::Vec3b>(pt.y, pt.x);
    }

    void Colorize(vtkSmartPointer<vtkPolyData> mesh,
                  std::vector<Camera> dataset) {

        assert(not dataset.empty());
        auto colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
        colors->SetNumberOfComponents(3);
        colors->SetName("Colors");
        auto *const meshNormals = mesh->GetPointData()->GetNormals();
        for (auto idx = 0; idx < mesh->GetNumberOfPoints(); ++idx) {

            auto normal = cv::Mat(3, 1, CV_64F, meshNormals->GetTuple(idx));
            normal.convertTo(normal, CV_32F);

            std::map<double, std::size_t> angles;
            for (std::size_t j = 0; j < dataset.size(); ++j) {
                angles[normal.dot(dataset[j].getDirection())] = j;
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
            Color<double> color;
            for (std::size_t j = 0; j < indexes.size(); ++j) {
                double v[3];
                mesh->GetPoint(idx, v);
                Camera cam = dataset[indexes[j]];
                auto col_pix = project<cv::Point2f, cv::Point3d>(
                    cam, cv::Point3d(v[0], v[1], v[2]));

                color += getColor(cam.getImage(), col_pix) * indexed_angles[j];
            }

            // weighted mean average color for each vertex
            color /= indexes.size();
            colors->InsertNextTuple(reinterpret_cast<double *>(&color));
        }
        mesh->GetPointData()->SetScalars(colors);
    }

} // namespace rendering
} // namespace ret
