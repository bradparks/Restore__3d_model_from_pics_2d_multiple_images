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

#ifndef RENDERING_MESH_COLORING_HPP
#define RENDERING_MESH_COLORING_HPP

#include <vector>
#include <vtkSmartPointer.h>
class vtkPolyData;
namespace ret { class Camera; }

namespace ret {

namespace rendering {

    /** @brief Colorize a given 3D mesh using the surface normal for each
      * vertex of the mesh.
      * Extracts color information from the whole dataset and
      * colorizes each vertex of the mesh using a robust weighted
      * approach where inconsistent surface colors are filtered
      * @param mesh 3D reconstructed mesh
      * @param dataset Camera dataset for the given mesh */
    void Colorize(vtkSmartPointer<vtkPolyData> mesh,
                  std::vector<Camera> dataset);
} // namespace rendering
} // namespace ret

#endif
