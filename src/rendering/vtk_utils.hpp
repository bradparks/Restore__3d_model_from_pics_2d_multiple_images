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

#ifndef RENDERING_VTK_UTILS_HPP
#define RENDERING_VTK_UTILS_HPP

#include <vtkDataArray.h>
#include <opencv2/core/core.hpp>

namespace ret {

    inline cv::Vec3d GetNormal(vtkDataArray *const meshNormals,
                               std::size_t idx) {
        double n[3];
        meshNormals->GetTuple(idx, n);
        return cv::Vec3d(n[0], n[1], n[2]);
    }

    inline cv::Vec3d GetNormal(const vtkSmartPointer<vtkPolyData> &mesh,
                               std::size_t idx) {
        double n[3];
        mesh->GetPointData()->GetNormals()->GetTuple(idx, n);
        return cv::Vec3d(n[0], n[1], n[2]);
    }

    inline cv::Point3d GetVertex(const vtkSmartPointer<vtkPolyData> &mesh,
                                 size_t idx) {
        double v[3];
        mesh->GetPoint(idx, v);
        return cv::Point3d(v[0], v[1], v[2]);
    }
}

#endif
