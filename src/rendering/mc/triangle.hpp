/******************************************************************************
 *                                                                            *
 * Authors:  Prof. Dr. Ulrich Schwanecke,                                     *
 *           M.Sc. Sebastian Otte,                                            *
 *           M.Sc. Henning Tjaden,                                            *
 *           M.Sc. Kai Wolf                                                   *
 *                                                                            *
 * Hochschule RheinMain                                                       *
 * University of Applied Sciences                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef RESTORE_RENDERING_MC_TRIANGLE_HPP
#define RESTORE_RENDERING_MC_TRIANGLE_HPP

// C system files
// none

// C++ system files
// none

// header files of other libraries
// none

// header files of project libraries
#include "vec3f.hpp"

namespace ret {

    namespace rendering {

        namespace mc {

            typedef struct triangle {
                union {
                    struct {
                        vec3f v1;
                        vec3f v2;
                        vec3f v3;
                    };
                    vec3f v[3];
                };
            } triangle;
        }
    }
}

#endif
