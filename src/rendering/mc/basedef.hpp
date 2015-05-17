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

#ifndef RESTORE_RENDERING_MC_BASEDEF_HPP
#define RESTORE_RENDERING_MC_BASEDEF_HPP

#define MC_GRID_SIZE_UNIT 32
#define MC_REVERSE_TRIANGLES true
#define MC_CUBE_ALIGN true

#define COMPUTE_INDEX(dim_x, dim_xz, x, z, y) \
    (x) + ((z) * (dim_x) + ((y) * (dim_xz)))

#define MC_VALUE(raw) raw

#define MC_WEIGHT(raw) raw.vals.weight

#define MC_ISOLEVEL_CHECK(raw, isolevel) ((MC_VALUE(raw)) < (isolevel))

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

namespace ret {

    namespace rendering {

        namespace mc {

            /// The typename grid_cell defines the base type of the grid cells.
            /// To access the value and the weight of a grid cell, use the
            /// marcos MC_VALUE and MC_WEIGHT. To change the way of accessing
            /// value and weight, you only need to modify these marcos.
            /// Note: The current implemenation uses the data structure
            /// grid_entry_t and the function convQ15ToFloat from
            /// "VoxelGridAccessTemp.h".
            typedef float grid_cell;

            typedef int int_type;
        }
    }
}

#endif
