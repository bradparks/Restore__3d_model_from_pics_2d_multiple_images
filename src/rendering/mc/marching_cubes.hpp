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
#include "basedef.hpp"
#include "triangle.hpp"

namespace ret {

    namespace rendering {

        namespace mc {

            class MarchingCubes {

              public:
                typedef std::vector<triangle> triangle_vector_type;
                MarchingCubes() = default;
                MarchingCubes(const float offset_x_, const float offset_y_,
                              const float offset_z_, const float voxel_width_,
                              const float voxel_height_,
                              const float voxel_depth_, const float isolevel_,
                              const int_type grid_dim_x_,
                              const int_type grid_dim_y_,
                              const int_type grid_dim_z_);
                virtual ~MarchingCubes();
                void setParams(const float offset_x_, const float offset_y_,
                               const float offset_z_, const float voxel_width_,
                               const float voxel_height_,
                               const float voxel_depth_, const float isolevel_,
                               const int_type grid_dim_x_,
                               const int_type grid_dim_y_,
                               const int_type grid_dim_z_);

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
                float offset_x_;
                float offset_y_;
                float offset_z_;

                /// dimensions of a voxel (if they are not cubic)
                float voxel_width_;
                float voxel_height_;
                float voxel_depth_;
                int_type grid_dim_x_;
                int_type grid_dim_y_;
                int_type grid_dim_z_;

                /// the iso value of the surface to be extracted
                float isolevel_;

                /// the triangle data of the currently extracted surface
                triangle_vector_type triangle_vector_;
            };
        }
    }
}

#include "marching_cubes.inl"

#endif
