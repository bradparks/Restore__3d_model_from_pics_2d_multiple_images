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
#include "common/utils.hpp"
#include "rendering/cv_utils.hpp"
#include "rendering/vtk_utils.hpp"

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

    Color<double> getAverageColor(const vtkSmartPointer<vtkPolyData> &mesh,
                                  const std::vector<Camera> &dataset, int idx,
                                  const std::map<size_t, double> &angles) {
        Color<double> color;
        for (const auto &angle : angles) {
            double v[3];
            mesh->GetPoint(idx, v);
            Camera cam = dataset[angle.first];
            auto col_pix = project<cv::Point2f, cv::Point3d>(
                cam, cv::Point3d(v[0], v[1], v[2]));
            color += getColor(cam.getImage(), col_pix) * angle.second;
        }

        // weighted mean average color for each vertex
        color /= angles.size();
        return color;
    }

    void Colorize(vtkSmartPointer<vtkPolyData> mesh,
                  std::vector<Camera> dataset) {

        assert(not dataset.empty());
        auto colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
        colors->SetNumberOfComponents(3);
        colors->SetName("Colors");
        auto *const meshNormals = mesh->GetPointData()->GetNormals();
        for (auto idx = 0; idx < mesh->GetNumberOfPoints(); ++idx) {

            // camera image indexes and appropriate dot product
            std::map<std::size_t, double> angles;
            auto normal = GetNormal(meshNormals, idx);
            angles[0] = normal.dot(dataset[0].getDirection());
            for (std::size_t j = 1; j < dataset.size(); ++j) {
                auto angle = normal.dot(dataset[j].getDirection());
                if (angle >= 0.5) {
                    angles[j] = angle;
                }
            }

            Color<double> color = getAverageColor(mesh, dataset, idx, angles);
            colors->InsertNextTuple(reinterpret_cast<double *>(&color));
        }
        mesh->GetPointData()->SetScalars(colors);
    }

} // namespace rendering
} // namespace ret
