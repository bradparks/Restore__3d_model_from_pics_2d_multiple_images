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

#ifndef RESTORE_RENDERING_MESH_COLORING_HPP
#define RESTORE_RENDERING_MESH_COLORING_HPP

// C system files
// none

// C++ system files
#include <vector>

// header files of other libraries
#include <opencv2/core/core.hpp>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkUnsignedCharArray.h>

// header files of project libraries
#include "../common/camera.hpp"

namespace ret {

    namespace rendering {

        class MeshColoring {
          public:
            MeshColoring();
            MeshColoring(MeshColoring const&) = delete;
            MeshColoring operator&=(MeshColoring const&) = delete;
            void colorize(vtkSmartPointer<vtkPolyData> mesh,
                          const std::vector<Camera> dataset);

          private:
            vtkSmartPointer<vtkUnsignedCharArray> colors_;
            cv::Size img_size_;

            cv::Mat getCameraDirection(const Camera& cam) const;
        };
    }
}

#endif
