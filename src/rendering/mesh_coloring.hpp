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

#ifndef RESTORE_RENDERING_MESH_COLORING_HPP
#define RESTORE_RENDERING_MESH_COLORING_HPP

// C system files
// none

// C++ system files
#include <vector>

// header files of other libraries
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>
#include <opencv2/core/core.hpp>

// header files of project libraries
#include "common/camera.hpp"

namespace ret {

namespace rendering {

    /// @brief Colorize a given 3D mesh using the surface normal for each
    /// vertex of the mesh. The color is extracted from the photographs of
    /// the object. For each vertex, there is a set of @ref Camera images
    /// to extract the color from.
    class MeshColoring {
      public:
        /// @brief Init the color array used for 3D mesh colorization
        MeshColoring();

        MeshColoring(MeshColoring const&) = delete;
        MeshColoring operator&=(MeshColoring const&) = delete;

        /// @brief Extracts color information from the whole dataset and
        /// colorizes each vertex of the mesh using a robust weighted
        /// approach where inconsistent surface colors are filtered
        /// @param mesh 3D reconstructed mesh
        /// @param dataset Camera dataset for the given mesh
        void colorize(vtkSmartPointer<vtkPolyData> mesh,
                      std::vector<Camera> dataset);

      private:
        vtkSmartPointer<vtkUnsignedCharArray> colors_;
        cv::Size img_size_;
    };
}
}

#endif
