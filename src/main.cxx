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

// C system files
// none

// C++ system files
#include <iostream>

// header files of other libraries
#include <opencv2/highgui/highgui.hpp>
#include <vtkPolyData.h>
#include <vtkPLYWriter.h>

// header files of project libraries
#include "common/dataset.hpp"
#include "common/utils.hpp"
#include "io/dataset_reader.hpp"
#include "io/assets_path.hpp"
#include "filtering/segmentation.hpp"
#include "rendering/bounding_box.hpp"
#include "rendering/mesh_coloring.hpp"
#include "rendering/voxel_carving.hpp"

using ret::DataSet;
using ret::io::DataSetReader;
using ret::filtering::Segmentation;
using ret::rendering::BoundingBox;
using ret::rendering::MeshColoring;
using ret::rendering::VoxelCarving;

void exportMesh(vtkSmartPointer<vtkPolyData> mesh,
                const std::string& filename) {

    auto plyExporter = vtkSmartPointer<vtkPLYWriter>::New();
#if VTK_MAJOR_VERSION < 6
    plyExporter->SetInput(mesh);
#else
    plyExporter->SetInputData(mesh);
#endif
    plyExporter->SetFileName(filename.c_str());
    plyExporter->SetColorModeToDefault();
    plyExporter->SetArrayName("Colors");
    plyExporter->Update();
    plyExporter->Write();
}

int main() {

    const int num_imgs = 36;
    DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel");
    auto ds = dsr.load(num_imgs);

    for (auto idx = 0; idx < num_imgs; ++idx) {
        ds.getCamera(idx).setMask(Segmentation::binarize(
                        ds.getCamera(idx).getImage(), cv::Scalar(0, 0, 30)));
    }
    BoundingBox bbox =
                BoundingBox(ds.getCamera(0), ds.getCamera((num_imgs / 4) - 1));
    auto vc = ret::make_unique<VoxelCarving>(bbox.getBounds(), 128);
    for (const auto& cam : ds.getCameras()) {
        vc->carve(cam);
    }
    auto visual_hull = vc->createVisualHull();
    auto coloring = ret::make_unique<MeshColoring>();
    coloring->colorize(visual_hull, ds.getCameras());
    exportMesh(visual_hull, "visual_hull.ply");

    return 0;
}
