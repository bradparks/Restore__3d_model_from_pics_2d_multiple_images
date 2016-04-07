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

#include "model_widget.hpp"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

namespace ret {

    ModelWidget::ModelWidget(QWidget *parent) : QVTKWidget(parent) {

        initRenderPipeline();
        setBackgroundColor();
    }

    void ModelWidget::initRenderPipeline() {

        vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();
        model_mapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
        model_mapper_->SetInputConnection(sphereSource->GetOutputPort());
        model_actor_ = vtkSmartPointer<vtkActor>::New();
        model_actor_->SetMapper(model_mapper_);
        renderer_ = vtkSmartPointer<vtkRenderer>::New();
        render_window_ = GetRenderWindow();
        render_window_->AddRenderer(renderer_);
        renderer_->AddActor(model_actor_);
    }

    void ModelWidget::setBackgroundColor() {

        renderer_->SetBackground(.45, .45, .9);
        renderer_->SetBackground2(.0, .0, .0);
        renderer_->GradientBackgroundOn();

    }
} // namespace ret
