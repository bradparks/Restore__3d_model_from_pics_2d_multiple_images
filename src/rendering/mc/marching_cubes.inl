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

#include "rendering/mc/marching_cubes.hpp" // IWYU pragma: export
#include "rendering/mc/basedef.hpp"

namespace ret {

namespace rendering {
    namespace mc {

        inline const MarchingCubes::triangle_vector_type&
        MarchingCubes::getTriangles() const {
            return triangle_vector_;
        }

        inline int_type MarchingCubes::getGridDimX() const {
            return grid_dim_x_;
        }

        inline int_type MarchingCubes::getGridDimY() const {
            return grid_dim_y_;
        }

        inline int_type MarchingCubes::getGridDimZ() const {
            return grid_dim_z_;
        }
    }
}
}
