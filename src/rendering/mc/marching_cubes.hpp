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

#include <vector>

#include "basedef.hpp"
#include "triangle.hpp"

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
                              const int_type grid_dim_y,
                              const int_type grid_dim_z);
                virtual ~MarchingCubes();
                void setParams(const float offset_x, const float offset_y,
                               const float offset_z, const float voxel_width,
                               const float voxel_height,
                               const float voxel_depth, const float isolevel,
                               const int_type grid_dim_x,
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
                void saveAsOBJ(const char* name) const;

              private:
                /// origin of the voxel grid
                float offset_x;
                float offset_y;
                float offset_z;

                /// dimensions of a voxel (if they are not cubic)
                float voxel_width;
                float voxel_height;
                float voxel_depth;
                int_type grid_dim_x;
                int_type grid_dim_y;
                int_type grid_dim_z;

                /// the iso value of the surface to be extracted
                float isolevel;

                /// the triangle data of the currently extracted surface
                triangle_vector_type triangle_vector;
            };
        }
    }
}

#include "marching_cubes.inl"

#endif
