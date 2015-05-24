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
#include <algorithm>
#include <limits>

// header files of other libraries
#include <opencv2/imgproc/imgproc.hpp>
#include <vtkVersion.h>
#include <vtkFloatArray.h>
#include <vtkStructuredPoints.h>
#include <vtkPointData.h>
#include <vtkMarchingCubes.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkReverseSense.h>

// header files of project libraries
#include "voxel_carving.hpp"
#include "cv_utils.hpp"
#include "../filtering/segmentation.hpp"
#include "../common/utils.hpp"

using namespace ret;
using namespace ret::rendering;
using namespace ret::filtering;

VoxelCarving::VoxelCarving(const bb_bounds bbox, const std::size_t voxel_dim)
    : voxel_dim_(voxel_dim),
      voxel_slice_(voxel_dim * voxel_dim),
      voxel_size_(voxel_dim * voxel_dim * voxel_dim),
      vox_array_(ret::make_unique<float[]>(voxel_size_)),
      params_(calcStartParameter(bbox)) {
    std::fill_n(vox_array_.get(), voxel_size_,
                std::numeric_limits<float>::max());
}

void VoxelCarving::carve(const Camera& cam) {

    const cv::Mat Mask = cam.getMask();
    const cv::Mat DistImage = Segmentation::createDistMap(Mask);
    const cv::Size img_size = Mask.size();

    for (std::size_t i = 0; i < voxel_dim_; ++i) {
        for (std::size_t j = 0; j < voxel_dim_; ++j) {
            for (std::size_t k = 0; k < voxel_dim_; ++k) {

                voxel v = calcVoxelPosInCamViewFrustum(i, j, k);
                auto coord = project<cv::Point2f, voxel>(cam, v);
                auto dist = -1.0f;
                if (inside(coord, img_size)) {
                    dist = DistImage.at<float>(coord);
                    if (Mask.at<uchar>(coord) == 0) { // outside
                        dist *= -1.0f;
                    }
                }

                const auto idx = k + j * voxel_dim_ + i * voxel_slice_;
                if (dist < vox_array_[idx]) vox_array_[idx] = dist;
            }
        }
    }
}

vtkSmartPointer<vtkPolyData> VoxelCarving::createVisualHull(
    const double isolevel) {

    // create vtk visualization pipeline from voxel grid
    auto spoints = vtkSmartPointer<vtkStructuredPoints>::New();
    auto vdim = static_cast<int>(voxel_dim_);
    spoints->SetDimensions(vdim, vdim, vdim);
    spoints->SetSpacing(params_.voxel_depth, params_.voxel_height,
                        params_.voxel_width);
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

    auto lr_trans = vtkSmartPointer<vtkTransform>::New();
    double elements[16] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    lr_trans->SetMatrix(elements);
    auto trans_filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    trans_filter->SetTransform(lr_trans);
    trans_filter->SetInputConnection(mc_source->GetOutputPort());
    auto orientation = vtkSmartPointer<vtkReverseSense>::New();
    orientation->SetInputConnection(trans_filter->GetOutputPort());
    orientation->ReverseNormalsOn();
    orientation->Update();

    return orientation->GetOutput();
}

voxel VoxelCarving::calcVoxelPosInCamViewFrustum(const std::size_t i,
                                                 const std::size_t j,
                                                 const std::size_t k) const {
    voxel v;
    v.xpos = params_.start_x + static_cast<float>(k) * params_.voxel_width;
    v.ypos = params_.start_y + static_cast<float>(j) * params_.voxel_height;
    v.zpos = params_.start_z + static_cast<float>(i) * params_.voxel_depth;
    v.value = 1.0f;

    return v;
}

start_params VoxelCarving::calcStartParameter(const bb_bounds& bbox) const {

    auto MARGIN_X = 0.06f;
    auto MARGIN_Y = 0.20f;

    auto bb_width = std::abs(bbox.xmax - bbox.xmin) * (1.0f + 2.0f * MARGIN_X);
    auto bb_height = std::abs(bbox.ymax - bbox.ymin) * (1.0f + 2.0f * MARGIN_Y);
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
