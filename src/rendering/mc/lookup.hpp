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

#ifndef RESTORE_RENDERING_MC_LOOKUP_HPP
#define RESTORE_RENDERING_MC_LOOKUP_HPP

#include "vec3f.hpp"
#include "triangle.hpp"
#include "marching_cubes.hpp"

namespace ret {

    namespace rendering {
        namespace mc {

            namespace lookup {

                typedef short lut_type;

                extern const lut_type edge_table[256];
                extern const lut_type triangle_table[256][16];
                extern const lut_type alt_triangle_table[256][16];
            }
        }
    }
}

#endif
