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
#include <iostream>
#include <string>

// header files of other libraries
#include <vtkVersion.h>
#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkPropPicker.h>
#include <vtkPointData.h>
#include <vtkLight.h>
#include <vtkLine.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCubeAxesActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

// header files of project libraries
#include <restore/common.hpp>
#include <restore/rendering.hpp>
#include <restore/filtering.hpp>
#include <restore/io.hpp>

using namespace ret;
using namespace ret::io;
using namespace ret::rendering;
using namespace ret::filtering;

int main() {

    const std::size_t VOXEL_DIM = 128;
    const std::size_t NUM_IMGS = 36;

    // load dataset
    DataSetReader dsr(std::string(ASSETS_PATH) + "/squirrel2");
    DataSet ds = dsr.load(NUM_IMGS);
    for (std::size_t i = 0; i < NUM_IMGS; ++i) {
        ds.getCamera(i).setMask(Segmentation::binarize(
            ds.getCamera(i).getImage(), cv::Scalar(0, 0, 30)));
    }

    // setup vtk renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(.45, .45, .9);
    renderer->SetBackground2(.0, .0, .0);
    renderer->GradientBackgroundOn();
    auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
    render_window->AddRenderer(renderer);
    auto render_window_interactor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    render_window_interactor->SetRenderWindow(render_window);

    BoundingBox bbox =
        BoundingBox(ds.getCamera(0), ds.getCamera((NUM_IMGS / 4) - 1));
    auto bb_bounds = bbox.getBounds();
    auto vc = ret::make_unique<VoxelCarving>(bb_bounds, VOXEL_DIM);

    // display bounding box
    auto bb_actor = vtkSmartPointer<vtkCubeAxesActor>::New();
    bb_actor->SetBounds(bb_bounds.xmin, bb_bounds.xmax, bb_bounds.ymin,
                        bb_bounds.ymax, 0.0,
                        std::abs(bb_bounds.zmax - bb_bounds.zmin));
    bb_actor->SetCamera(renderer->GetActiveCamera());
    bb_actor->DrawXGridlinesOn();
    bb_actor->DrawYGridlinesOn();
    bb_actor->DrawZGridlinesOn();
    renderer->AddActor(bb_actor);

    //    auto bb_x = vtkSmartPointer<vtkLineSource>::New();
    //    bb_x->SetPoint1(bb_bounds.xmin, bb_bounds.ymin, 0.0);
    //    bb_x->SetPoint2(bb_bounds.xmax, bb_bounds.ymin, 0.0);
    //    auto bb_x_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //    bb_x_mapper->SetInputConnection(bb_x->GetOutputPort());
    //    auto bb_x_actor = vtkSmartPointer<vtkActor>::New();
    //    bb_x_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    //    bb_x_actor->GetProperty()->SetLineWidth(1);
    //    bb_x_actor->SetMapper(bb_x_mapper);
    //    renderer->AddActor(bb_x_actor);

    double cam_color = 1.0;
    for (const auto& camera : ds.getCameras()) {
        // carve visual hull
        vc->carve(camera);

        // visualize all cameras
        auto cam_source = vtkSmartPointer<vtkConeSource>::New();
        cam_source->SetHeight(4.0);
        cam_source->SetRadius(2.0);
        cam_source->SetResolution(4);
        auto center = camera.getCenter();
        cam_source->SetCenter(center.x, center.y, center.z);
        auto lookat = getCameraDirection(camera, camera.getImage().size());
        cam_source->SetDirection(lookat.at<float>(0, 0), lookat.at<float>(0, 1),
                                 lookat.at<float>(0, 2));
        auto cam_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        cam_mapper->SetInputConnection(cam_source->GetOutputPort());
        auto cam_actor = vtkSmartPointer<vtkActor>::New();
        cam_actor->SetMapper(cam_mapper);
        cam_actor->SetOrigin(center.x, center.y, center.z);
        cam_color *= 0.9;
        cam_actor->GetProperty()->SetColor(0.0, cam_color, 0.0);
        renderer->AddActor(cam_actor);
    }

    // display visual hull
    auto mesh_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION < 6
    mesh_mapper->SetInput(vc->createVisualHull());
#else
    mesh_mapper->SetInputData(vc->createVisualHull());
#endif
    auto mesh_actor = vtkSmartPointer<vtkActor>::New();
    mesh_actor->GetProperty()->SetColor(0.7, 0.7, 0.7);
    mesh_actor->GetProperty()->SetSpecular(0.4);
    mesh_actor->GetProperty()->SetAmbient(0.5);
    mesh_actor->GetProperty()->SetInterpolationToPhong();
    mesh_actor->SetMapper(mesh_mapper);
    renderer->AddActor(mesh_actor);

    // setup lighting
    auto light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    renderer->AddLight(light);

    // setup orientation marker
    auto xaxis_line = vtkSmartPointer<vtkLineSource>::New();
    xaxis_line->SetPoint1(0.0, 0.0, 0.0);
    xaxis_line->SetPoint2(30.0, 0.0, 0.0);
    auto xaxis_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    xaxis_mapper->SetInputConnection(xaxis_line->GetOutputPort());
    auto xaxis_actor = vtkSmartPointer<vtkActor>::New();
    xaxis_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    xaxis_actor->GetProperty()->SetLineWidth(3);
    xaxis_actor->SetMapper(xaxis_mapper);
    renderer->AddActor(xaxis_actor);

    auto yaxis_line = vtkSmartPointer<vtkLineSource>::New();
    yaxis_line->SetPoint1(0.0, 0.0, 0.0);
    yaxis_line->SetPoint2(0.0, 30.0, 0.0);
    auto yaxis_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    yaxis_mapper->SetInputConnection(yaxis_line->GetOutputPort());
    auto yaxis_actor = vtkSmartPointer<vtkActor>::New();
    yaxis_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    yaxis_actor->GetProperty()->SetLineWidth(3);
    yaxis_actor->SetMapper(yaxis_mapper);
    renderer->AddActor(yaxis_actor);

    auto zaxis_line = vtkSmartPointer<vtkLineSource>::New();
    zaxis_line->SetPoint1(0.0, 0.0, 0.0);
    zaxis_line->SetPoint2(0.0, 0.0, 30.0);
    auto zaxis_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    zaxis_mapper->SetInputConnection(zaxis_line->GetOutputPort());
    auto zaxis_actor = vtkSmartPointer<vtkActor>::New();
    zaxis_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    zaxis_actor->GetProperty()->SetLineWidth(3);
    zaxis_actor->SetMapper(zaxis_mapper);
    renderer->AddActor(zaxis_actor);

    render_window->Render();
    render_window_interactor->Start();

    return 0;
}
