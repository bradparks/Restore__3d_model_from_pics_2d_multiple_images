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
#include <string>

// header files of other libraries
#include <vtkVersion.h>
#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkPropPicker.h>
#include <vtkPointData.h>
#include <vtkLight.h>
#include <vtkLine.h>
#include <vtkPLYWriter.h>
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

enum class Axis { X, Y, Z };

DataSet loadDataSet(const std::string& path, const std::size_t num_imgs) {

    DataSetReader dsr(path);
    DataSet ds = dsr.load(num_imgs);
    for (std::size_t i = 0; i < num_imgs; ++i) {
        ds.getCamera(i).setMask(Segmentation::binarize(
            ds.getCamera(i).getImage(), cv::Scalar(0, 0, 30)));
    }

    return ds;
}

void displayBoundingBox(const bb_bounds& bbox,
                        vtkSmartPointer<vtkRenderer> renderer) {

    auto bb_actor = vtkSmartPointer<vtkCubeAxesActor>::New();
    bb_actor->SetBounds(bbox.xmin, bbox.xmax, bbox.ymin, bbox.ymax, 0.0,
                        std::abs(bbox.zmax - bbox.zmin));
    bb_actor->SetCamera(renderer->GetActiveCamera());
    bb_actor->DrawXGridlinesOn();
    bb_actor->DrawYGridlinesOn();
    bb_actor->DrawZGridlinesOn();
    renderer->AddActor(bb_actor);
}

void displayCamera(const Camera& camera, vtkSmartPointer<vtkRenderer> renderer,
                   double& cam_color) {

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

void displayGridAxis(const Axis axis, vtkSmartPointer<vtkRenderer> renderer) {

    const double LINE_LENGTH = 30.0;

    auto axis_line = vtkSmartPointer<vtkLineSource>::New();
    axis_line->SetPoint1(0.0, 0.0, 0.0);
    auto axis_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    axis_mapper->SetInputConnection(axis_line->GetOutputPort());
    auto axis_actor = vtkSmartPointer<vtkActor>::New();
    axis_actor->GetProperty()->SetLineWidth(3);
    axis_actor->SetMapper(axis_mapper);

    switch (axis) {
        case Axis::X:
            axis_line->SetPoint2(LINE_LENGTH, 0.0, 0.0);
            axis_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
            break;
        case Axis::Y:
            axis_line->SetPoint2(0.0, LINE_LENGTH, 0.0);
            axis_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
            break;
        default:
        case Axis::Z:
            axis_line->SetPoint2(0.0, 0.0, LINE_LENGTH);
            axis_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
            break;
    }

    renderer->AddActor(axis_actor);
}

void displayVisualHull(vtkSmartPointer<vtkPolyData> mesh,
                       vtkSmartPointer<vtkRenderer> renderer) {

    auto mesh_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION < 6
    mesh_mapper->SetInput(mesh);
#else
    mesh_mapper->SetInputData(mesh);
#endif
    auto mesh_actor = vtkSmartPointer<vtkActor>::New();
    mesh_actor->GetProperty()->SetColor(0.7, 0.7, 0.7);
    mesh_actor->GetProperty()->SetSpecular(0.4);
    mesh_actor->GetProperty()->SetAmbient(0.5);
    mesh_actor->GetProperty()->SetInterpolationToPhong();
    mesh_actor->SetMapper(mesh_mapper);
    renderer->AddActor(mesh_actor);
}

int main() {

    const std::size_t VOXEL_DIM = 128;
    const std::size_t NUM_IMGS = 36;
    std::string model("squirrel");

    DataSet ds = loadDataSet(std::string(ASSETS_PATH) + "/" + model, NUM_IMGS);

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

    BoundingBox bbox(ds.getCamera(0), ds.getCamera((NUM_IMGS / 4) - 1));
    auto bb_bounds = bbox.getBounds();
    auto vc = ret::make_unique<VoxelCarving>(bb_bounds, VOXEL_DIM);
    displayBoundingBox(bb_bounds, renderer);

    double cam_color = 1.0;
    for (const auto& camera : ds.getCameras()) {
        vc->carve(camera);
        displayCamera(camera, renderer, cam_color);
    }

    auto mesh = vc->createVisualHull();
    displayVisualHull(mesh, renderer);

    // setup lighting
    auto light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    renderer->AddLight(light);

    // setup orientation marker
    displayGridAxis(Axis::X, renderer);
    displayGridAxis(Axis::Y, renderer);
    displayGridAxis(Axis::Z, renderer);

    auto plyExporter = vtkSmartPointer<vtkPLYWriter>::New();
#if VTK_MAJOR_VERSION < 6
    plyExporter->SetInput(mesh);
#else
    plyExporter->SetInputData(mesh);
#endif
    plyExporter->SetFileName(model.append(".ply").c_str());
    plyExporter->Update();
    plyExporter->Write();

    render_window->Render();
    render_window_interactor->Start();

    return 0;
}
