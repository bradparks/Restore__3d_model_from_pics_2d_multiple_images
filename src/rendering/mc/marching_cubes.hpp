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

#ifndef RESTORE_RENDERING_MC_MARCHING_CUBES_HPP
#define RESTORE_RENDERING_MC_MARCHING_CUBES_HPP

// C system files
// none

// C++ system files
#include <vector>

// header files of other libraries
// none

// header files of project libraries
#include "common/types/triangle.hpp"
#include "common/types/vec3f.hpp"
#include "rendering/mc/basedef.hpp"

namespace ret {

namespace rendering {

    namespace mc {

        class MarchingCubes {

          public:
            typedef std::vector<triangle> triangle_vector_type;
            MarchingCubes();
            MarchingCubes(const float offset_x, const float offset_y,
                          const float offset_z, const float voxel_width,
                          const float voxel_height, const float voxel_depth,
                          const float isolevel, const int_type grid_dim_x,
                          const int_type grid_dim_y, const int_type grid_dim_z);
            virtual ~MarchingCubes();
            void setParams(const float offset_x, const float offset_y,
                           const float offset_z, const float voxel_width,
                           const float voxel_height, const float voxel_depth,
                           const float isolevel, const int_type grid_dim_x,
                           const int_type grid_dim_y,
                           const int_type grid_dim_z);

            /// The main function extracting an iso-surface from a given
            /// dataset. Must be executed before getTriangles() should be
            /// invoked.
            /// @param *grid the voxelgrid from which the iso-surface shall
            /// be extracted
            void execute(const float* grid);

            /// Returns the triangles of the extracted iso-surface.
            /// Should be invoked after execute().
            const triangle_vector_type& getTriangles() const;

            int_type getGridDimX() const;
            int_type getGridDimY() const;
            int_type getGridDimZ() const;

            /// @brief Saves the surface data as an obj-file.
            /// @param name the name of the obj-file (without .obj)
            void saveASOBJ(const char* name,
                           const triangle_vector_type& triangles,
                           const std::vector<vec3f>& normals) const;

          private:
            /// origin of the voxel grid
            float offset_x_;
            float offset_y_;
            float offset_z_;

            /// dimensions of a voxel (if they are not cubic)
            float voxel_width_;
            float voxel_height_;
            float voxel_depth_;

            /// the iso value of the surface to be extracted
            float isolevel_;
            int_type grid_dim_x_;
            int_type grid_dim_y_;
            int_type grid_dim_z_;

            /// the triangle data of the currently extracted surface
            triangle_vector_type triangle_vector_;
        };

        inline void interpolate(vec3f& result, const vec3f& p1, const vec3f& p2,
                                const grid_cell& valp1, const grid_cell& valp2,
                                const float isolevel) {

            const float mu = ((isolevel - MC_VALUE(valp1)) /
                              (MC_VALUE(valp2) - MC_VALUE(valp1)));
            result.x = p1.x + mu * (p2.x - p1.x);
            result.y = p1.y + mu * (p2.y - p1.y);
            result.z = p1.z + mu * (p2.z - p1.z);
        }
    }
}
}

#include "marching_cubes.inl"

#endif
