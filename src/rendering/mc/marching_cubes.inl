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

// C system files
// none

// C++ system files
// none

// header files of other libraries
// none

// header files of project libraries
#include "marching_cubes.hpp"

namespace ret {

    namespace rendering {
        namespace mc {

            inline const MarchingCubes::triangle_vector_type&
            MarchingCubes::getTriangles() const {
                return this->triangle_vector;
            }

            inline int_type MarchingCubes::getGridDimX() const {
                return this->grid_dim_x_;
            }

            inline int_type MarchingCubes::getGridDimY() const {
                return this->grid_dim_y_;
            }

            inline int_type MarchingCubes::getGridDimZ() const {
                return this->grid_dim_z_;
            }
        }
    }
}
