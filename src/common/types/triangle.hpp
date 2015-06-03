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

#ifndef RESTORE_COMMON_TYPES_TRIANGLE_HPP
#define RESTORE_COMMON_TYPES_TRIANGLE_HPP

// C system files
// none

// C++ system files
// none

// header files of other libraries
// none

// header files of project libraries
#include "common/types/vec3f.hpp"

namespace ret {

    struct triangle {
        struct {
            vec3f v1;
            vec3f v2;
            vec3f v3;
        } comp;

        inline bool operator==(const triangle& other) const {
            return (comp.v1 == other.comp.v1 && comp.v2 == other.comp.v2 &&
                    comp.v3 == other.comp.v3);
        }
    };
}

#endif
