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

#include <cstddef>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include "common/camera.hpp"
#include "rendering/vtk_utils.hpp"

namespace ret {

    namespace rendering {

        std::size_t GetMiddleCamera(cv::Vec3d normal,
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
