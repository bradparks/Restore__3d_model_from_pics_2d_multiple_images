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

// C system files
// none

// C++ system files
// none

// header files of other libraries
// none

// header files of project libraries
// none

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
