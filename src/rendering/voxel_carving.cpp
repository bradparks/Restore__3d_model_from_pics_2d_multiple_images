// Copyright (c) 2015, Kai Wolf
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

#include "rendering/voxel_carving.hpp"

// C system files
// none

// C++ system files
#include <limits>

// header files of other libraries
#include <vtkFloatArray.h>
#include <vtkMarchingCubes.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkStructuredPoints.h>

// header files of project libraries
#include "common/utils.hpp"
#include "filtering/segmentation.hpp"
#include "rendering/cv_utils.hpp"

using namespace ret;
using namespace ret::rendering;
using namespace ret::filtering;

VoxelCarving::VoxelCarving(const bb_bounds bbox, const std::size_t voxel_dim)
    : voxel_dim_(voxel_dim),
      voxel_slice_(voxel_dim * voxel_dim),
      voxel_size_(voxel_dim * voxel_dim * voxel_dim),
      vox_array_(ret::make_unique<float[]>(voxel_size_)),
      params_(calcStartParameter(bbox)),
      bb_margin_(std::make_pair(0.10f, 0.10f)) {
    std::fill_n(vox_array_.get(), voxel_size_,
                std::numeric_limits<float>::max());
}

void VoxelCarving::carve(const Camera& cam) {

    const auto Mask = cam.getMask();
    const auto DistImage = Segmentation::createDistMap(Mask);
    const auto img_size = Mask.size();

    for (auto i = 0; i < voxel_dim_; ++i) {
        for (auto j = 0; j < voxel_dim_; ++j) {
            for (auto k = 0; k < voxel_dim_; ++k) {

                auto voxel = calcVoxelPosInCamViewFrustum(i, j, k);
                auto coord = project<cv::Point2f, cv::Point3f>(cam, voxel);
                auto dist = -1.0f;
                if (inside(coord, img_size)) {
                    dist = DistImage.at<float>(coord);
                    if (!Mask.at<uchar>(coord)) dist *= -1.0f; // outside
                }

                const auto idx = k + j * voxel_dim_ + i * voxel_slice_;
                if (dist < vox_array_[idx]) vox_array_[idx] = dist;
            }
        }
    }
}

vtkSmartPointer<vtkPolyData>
VoxelCarving::createVisualHull(const double isolevel) const {

    // create vtk visualization pipeline from voxel grid
    auto spoints = vtkSmartPointer<vtkStructuredPoints>::New();
    auto vdim = static_cast<int>(voxel_dim_);
    spoints->SetDimensions(vdim, vdim, vdim);
    spoints->SetSpacing(params_.voxel_width, params_.voxel_height,
                        params_.voxel_depth);
    spoints->SetOrigin(params_.start_x, params_.start_y, params_.start_z);

    auto farray = vtkSmartPointer<vtkFloatArray>::New();
    auto vsize = static_cast<vtkIdType>(voxel_size_);
    farray->SetNumberOfValues(vsize);
    farray->SetArray(vox_array_.get(), vsize, 1);
    spoints->GetPointData()->SetScalars(farray);

    // create iso surface with marching cubes
    auto mc_source = vtkSmartPointer<vtkMarchingCubes>::New();
#if VTK_MAJOR_VERSION < 6
    mc_source->SetInput(spoints);
#else
    mc_source->SetInputData(spoints);
#endif
    mc_source->SetNumberOfContours(1);
    mc_source->SetValue(0, isolevel);

    // calculate surface normals
    auto surface_normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    surface_normals->SetInputConnection(mc_source->GetOutputPort());
    surface_normals->SetFeatureAngle(60.0);
    surface_normals->ComputePointNormalsOn();
    surface_normals->Update();

    return surface_normals->GetOutput();
}

void VoxelCarving::setBoundingBoxMargin(
    const std::pair<float, float>& margin_xy) {
    bb_margin_ = margin_xy;
}

void VoxelCarving::setBoundingBoxXMargin(const float margin_x) {

    bb_margin_.first = margin_x;
}

void VoxelCarving::setBoundingBoxYMargin(const float margin_y) {

    bb_margin_.second = margin_y;
}

cv::Point3f VoxelCarving::calcVoxelPosInCamViewFrustum(
    const std::size_t i, const std::size_t j, const std::size_t k) const {

    cv::Point3f voxel;
    voxel.x = params_.start_x + static_cast<float>(k) * params_.voxel_width;
    voxel.y = params_.start_y + static_cast<float>(j) * params_.voxel_height;
    voxel.z = params_.start_z + static_cast<float>(i) * params_.voxel_depth;

    return voxel;
}

start_params VoxelCarving::calcStartParameter(const bb_bounds& bbox) const {

    auto bb_width =
        std::abs(bbox.xmax - bbox.xmin) * (1.0f + 2.0f * bb_margin_.first);
    auto bb_height =
        std::abs(bbox.ymax - bbox.ymin) * (1.0f + 2.0f * bb_margin_.second);
    auto bb_depth = std::abs(bbox.zmax - bbox.zmin);

    auto offset_x = (bb_width - std::abs(bbox.xmax - bbox.xmin)) / 2.0f;
    auto offset_y = (bb_height - std::abs(bbox.ymax - bbox.ymin)) / 2.0f;

    start_params params;
    params.start_x = bbox.xmin - offset_x;
    params.start_y = bbox.ymin - offset_y;
    params.start_z = 0.0f;
    params.voxel_width = bb_width / static_cast<float>(voxel_dim_);
    params.voxel_height = bb_height / static_cast<float>(voxel_dim_);
    params.voxel_depth = bb_depth / static_cast<float>(voxel_dim_);

    return params;
}
