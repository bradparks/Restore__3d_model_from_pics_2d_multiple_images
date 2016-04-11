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

#include "mesh_refinement.hpp"

#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkUnsignedCharArray.h>

namespace ret {

    namespace rendering {

        cv::Mat GetNormal(vtkDataArray *const meshNormals, std::size_t idx) {
            auto normal = cv::Mat(3, 1, CV_64F, meshNormals->GetTuple(idx));
            normal.convertTo(normal, CV_32F);
            return normal;
        }

        cv::Point3d GetVertex(vtkSmartPointer<vtkPolyData> &mesh, size_t idx) {
            double v[3];
            mesh->GetPoint(idx, v);
            auto vertex = cv::Point3d(v[0], v[1], v[2]);
            mesh->GetPoints()->SetPoint(idx, vertex.x, vertex.y, vertex.z);
            return vertex;
        }

        std::size_t GetMiddleCamera(cv::Mat normal,
                                    const std::vector<Camera> &dataset) {

            auto best_angle = -1.0;
            auto cam_idx = 0;
            for (auto i = 0; i < dataset.size(); ++i) {
                auto current_angle = normal.dot(dataset[i].getDirection());
                if (current_angle > best_angle) {
                    best_angle = current_angle;
                    cam_idx = i;
                }
            }

            return cam_idx;
        }

        cv::Mat GetOpticalRay(cv::Point3d vertex, Camera &cam) {
            auto optic_ray = cv::Mat(vertex - cam.getCenter());
            cv::normalize(optic_ray, optic_ray);
            return optic_ray;
        }

        void RefineMesh(vtkSmartPointer<vtkPolyData> mesh,
                        const std::vector<Camera> &dataset) {

            auto *const meshNormals = mesh->GetPointData()->GetNormals();

            for (std::size_t idx = 0; idx < mesh->GetNumberOfPoints(); ++idx) {

                auto normal = GetNormal(meshNormals, idx);
                auto vertex = GetVertex(mesh, idx);
                int cam_idx = GetMiddleCamera(normal, dataset);

            }
        }
    }
}
