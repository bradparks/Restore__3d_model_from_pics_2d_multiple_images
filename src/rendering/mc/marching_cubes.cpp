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

#include "rendering/mc/marching_cubes.hpp"

// C system files
// none

// C++ system files
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

// header files of other libraries
// none

// header files of project libraries
#include "common/types/triangle.hpp"
#include "common/types/vec3f.hpp"
#include "rendering/mc/basedef.hpp"
#include "rendering/mc/lookup.hpp"

using namespace ret::rendering::mc;

/// This struct represents a half of 4 cubes in a grid of float values.
/// half_cube4 helps to access and copy data of cube4 structs.
///
///        8
///       /|
///      5 |
///     /| |
///    2 | |
///    | | 7
///    | |/|
///    | 4 |
///    |/| |
///    1 | |
///    | | 6
///    | |/
///    | 3
///    |/
///    0
///
template <typename base_type>
union half_cube4 {
    struct {
        base_type c0, c1, c2;
        base_type c3, c4, c5;
        base_type c6, c7, c8;
    } idx;
    grid_cell data[9];
};

/// This struct represents 4 cubes in a grid of float values. The indices are
/// ordered with the objective to optimize memory access.
///
///        8-----------17
///       /|          /|
///      5-----------14|
///     /| |        /| |
///    2-----------11| |
///    | | 7-------|-|-16
///    | |/|       | |/|
///    | 4---------|-13|
///    |/| |       |/| |
///    1-----------10| |
///    | | 6-------|-|-15
///    | |/        | |/
///    | 3---------|-12
///    |/          |/
///    0-----------9
///
template <typename base_type>
union cube4 {
    struct {
        base_type c0, c1, c2;
        base_type c3, c4, c5;
        base_type c6, c7, c8;
        base_type c9, c10, c11;
        base_type c12, c13, c14;
        base_type c15, c16, c17;
    } idx;
    struct {
        half_cube4<base_type> hc0;
        half_cube4<base_type> hc1;
    } hc;
    base_type data[18];
};

typedef cube4<grid_cell> cube4c;
typedef cube4<ret::vec3f> cube4vec3f;
typedef cube4<short> cube4s;

/// Just a single cube. Here the order of the incidices are given as common.
///
///      7-----------6
///     /|          /|
///    3-----------2 |
///    | |         | |
///    | |         | |
///    | 4---------|-5
///    |/          |/
///    0-----------1
///
template <typename base_type>
union cube {
    struct {
        base_type c0, c1, c2, c3;
        base_type c4, c5, c6, c7;
    } idx;

    base_type data[8];

#if MC_CUBE_ALIGN
    enum { INNER_SIZE = sizeof(base_type) * 8 };
    enum { BLOCK_SIZE = 16 };
    enum {
        ALIGN_SIZE = (((INNER_SIZE / BLOCK_SIZE) * BLOCK_SIZE) +
                      (BLOCK_SIZE - (INNER_SIZE % BLOCK_SIZE)))
    };
    unsigned char __padding[ALIGN_SIZE];
#endif
};

typedef cube<ret::vec3f> cubevec3f;

/// Template based index-mapping from cube4 to cube. This maps our cube4 index
/// pattern to cubes with the standard index pattern.
///
///        8-----------17
///       /|          /|
///      5-----------14|
///     /| |        /| |
///    2-----------11| |            7-----------6
///    | | 7-------|-|-16          /|          /|
///    | |/|   Q3  | |/|          3-----------2 |
///    | 4---------|-13|    ==>   | |         | |
///    |/| | Q1    |/| |          | |         | |
///    1-----------10| |          | 4---------|-5
///    | | 6-------|-|-15         |/          |/
///    | |/    Q2  | |/           0-----------1
///    | 3---------|-12
///    |/    Q0    |/
///    0-----------9
///
///       Q3
///      /|
///     Q1|
///     | |
///     | Q2
///     |/
///     Q0
///
template <unsigned int cube>
struct cubeidx {
    enum { i0 = (cube > 1) ? (cube + 1) : (cube) };
    enum {
        i1 = i0 + 9,
        i2 = i1 + 1,
        i3 = i0 + 1,
        i4 = i0 + 3,
        i5 = i1 + 3,
        i6 = i5 + 1,
        i7 = i4 + 1
    };
};

MarchingCubes::MarchingCubes()
    : offset_x_(0.0f),
      offset_y_(0.0f),
      offset_z_(0.0f),
      voxel_width_(0.0f),
      voxel_height_(0.0f),
      voxel_depth_(0.0f),
      isolevel_(0.0f),
      grid_dim_x_(0),
      grid_dim_y_(0),
      grid_dim_z_(0),
      triangle_vector_() {}

MarchingCubes::MarchingCubes(const float offset_x, const float offset_y,
                             const float offset_z, const float voxel_width,
                             const float voxel_height, const float voxel_depth,
                             const float isolevel, const int_type grid_dim_x,
                             const int_type grid_dim_y,
                             const int_type grid_dim_z)
    : offset_x_(offset_x),
      offset_y_(offset_y),
      offset_z_(offset_z),
      voxel_width_(voxel_width),
      voxel_height_(voxel_height),
      voxel_depth_(voxel_depth),
      isolevel_(isolevel),
      grid_dim_x_(grid_dim_x),
      grid_dim_y_(grid_dim_y),
      grid_dim_z_(grid_dim_z),
      triangle_vector_() {}

MarchingCubes::~MarchingCubes() {
    triangle_vector_.clear();
}

void MarchingCubes::setParams(const float offset_x, const float offset_y,
                              const float offset_z, const float voxel_width,
                              const float voxel_height, const float voxel_depth,
                              const float isolevel, const int_type grid_dim_x,
                              const int_type grid_dim_y,
                              const int_type grid_dim_z) {

    offset_x_ = offset_x;
    offset_y_ = offset_y;
    offset_z_ = offset_z;
    voxel_width_ = voxel_width;
    voxel_height_ = voxel_height;
    voxel_depth_ = voxel_depth;
    isolevel_ = isolevel;
    grid_dim_x_ = grid_dim_x;
    grid_dim_y_ = grid_dim_y;
    grid_dim_z_ = grid_dim_z;
}

void MarchingCubes::saveASOBJ(const char* name,
                              const triangle_vector_type& triangles,
                              const std::vector<vec3f>& normals) const {

    std::ofstream output(name, std::ofstream::binary);

    int_type face = 1;
    std::size_t n_idx = 0;

    for (triangle_vector_type::const_iterator it = triangles.begin();
         it != triangles.end(); it++) {

        // using "\n" instead of std::endl to avoid permanent flushing.
        output.write("v ", 2);
        output << it->comp.v1.x;
        output.write(" ", 1);
        output << it->comp.v1.y;
        output.write(" ", 1);
        output << it->comp.v1.z;

        output.write("\nv ", 3);
        output << it->comp.v2.x;
        output.write(" ", 1);
        output << it->comp.v2.y;
        output.write(" ", 1);
        output << it->comp.v2.z;

        output.write("\nv ", 3);
        output << it->comp.v3.x;
        output.write(" ", 1);
        output << it->comp.v3.y;
        output.write(" ", 1);
        output << it->comp.v3.z;

        for (int i = 0; i < 3; ++i) {
            output.write("\nvn ", 4);
            output << normals[n_idx].x;
            output.write(" ", 1);
            output << normals[n_idx].y;
            output.write(" ", 1);
            output << normals[n_idx].z;
        }

#if MC_REVERSE_TRIANGLES

        output.write("\nf ", 3);
        output << face;
        output.write("//", 2);
        output << face;
        output.write(" ", 1);

        output << (face + 1);
        output.write("//", 2);
        output << (face + 1);
        output.write(" ", 1);

        output << (face + 2);
        output.write("//", 2);
        output << (face + 2);
        output.write(" ", 1);
#else

        output.write("\nf ", 3);
        output << face + 2;
        output.write("//", 2);
        output << face + 2;
        output.write(" ", 1);

        output << (face + 1);
        output.write("//", 2);
        output << (face + 1);
        output.write(" ", 1);

        output << (face);
        output.write("//", 2);
        output << (face);
        output.write(" ", 1);
#endif

        output.write("\n", 1);

        face += 3;
        n_idx += 1;
    }
    (void) output.close();
}

/// @brief Macro for index computation
#define MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, y) \
    (x) + ((z) * (dim_x) + ((y) * (dim_xz)))

/// Once instantiated with a cube-number (0,..,3) this function-template
/// generates a triangulation function specialized for the corresponding cube.
template <unsigned int cube>
void triangulate(const cube4c& values, const cubevec3f& points,
                 MarchingCubes::triangle_vector_type& triangles,
                 const float isolevel) {

    ret::vec3f vertlist[12];

    // Determine the index in the edge table, which tells us which vertices are
    // inside of the surface.
    const int_type cubecase =
        ((MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i0], isolevel)
          << 0) | // 1
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i1], isolevel)
          << 1) | // 2
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i2], isolevel)
          << 2) | // 4
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i3], isolevel)
          << 3) | // 8
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i4], isolevel)
          << 4) | // 16
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i5], isolevel)
          << 5) | // 32
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i6], isolevel)
          << 6) | // 64
         (MC_ISOLEVEL_CHECK(values.data[cubeidx<cube>::i7], isolevel)
          << 7) // 128
         );

    // Check if the cube is entirely in/out of the surface?
    if ((cubecase == 0) || (cubecase == 255)) return;

    // Find the vertices where the surface intersects the cube.
    const int_type edge = lookup::edge_table[cubecase];

    // count bits.
    if (edge & 1) {
        interpolate(vertlist[0], points.data[0], points.data[1],
                    values.data[cubeidx<cube>::i0],
                    values.data[cubeidx<cube>::i1], isolevel);
    }
    if (edge & 2) {
        interpolate(vertlist[1], points.data[1], points.data[2],
                    values.data[cubeidx<cube>::i1],
                    values.data[cubeidx<cube>::i2], isolevel);
    }
    if (edge & 4) {
        interpolate(vertlist[2], points.data[2], points.data[3],
                    values.data[cubeidx<cube>::i2],
                    values.data[cubeidx<cube>::i3], isolevel);
    }
    if (edge & 8) {
        interpolate(vertlist[3], points.data[3], points.data[0],
                    values.data[cubeidx<cube>::i3],
                    values.data[cubeidx<cube>::i0], isolevel);
    }
    if (edge & 16) {
        interpolate(vertlist[4], points.data[4], points.data[5],
                    values.data[cubeidx<cube>::i4],
                    values.data[cubeidx<cube>::i5], isolevel);
    }
    if (edge & 32) {
        interpolate(vertlist[5], points.data[5], points.data[6],
                    values.data[cubeidx<cube>::i5],
                    values.data[cubeidx<cube>::i6], isolevel);
    }
    if (edge & 64) {
        interpolate(vertlist[6], points.data[6], points.data[7],
                    values.data[cubeidx<cube>::i6],
                    values.data[cubeidx<cube>::i7], isolevel);
    }
    if (edge & 128) {
        interpolate(vertlist[7], points.data[7], points.data[4],
                    values.data[cubeidx<cube>::i7],
                    values.data[cubeidx<cube>::i4], isolevel);
    }
    if (edge & 256) {
        interpolate(vertlist[8], points.data[0], points.data[4],
                    values.data[cubeidx<cube>::i0],
                    values.data[cubeidx<cube>::i4], isolevel);
    }
    if (edge & 512) {
        interpolate(vertlist[9], points.data[1], points.data[5],
                    values.data[cubeidx<cube>::i1],
                    values.data[cubeidx<cube>::i5], isolevel);
    }
    if (edge & 1024) {
        interpolate(vertlist[10], points.data[2], points.data[6],
                    values.data[cubeidx<cube>::i2],
                    values.data[cubeidx<cube>::i6], isolevel);
    }
    if (edge & 2048) {
        interpolate(vertlist[11], points.data[3], points.data[7],
                    values.data[cubeidx<cube>::i3],
                    values.data[cubeidx<cube>::i7], isolevel);
    }
    ret::triangle tri;

    // grab components.
    const lookup::lut_type sheets = lookup::alt_triangle_table[cubecase][0];
    const lookup::lut_type* idxptr =
        &(lookup::alt_triangle_table[cubecase][sheets + 1]);

    for (lookup::lut_type s = 1; s <= sheets; s++) {
        lookup::lut_type vertices_num = lookup::alt_triangle_table[cubecase][s];

        tri.comp.v1 = vertlist[idxptr[0]];

        // this small switch block is faster than lookups + for-loops!
        // DO NOT CHANGE CASE-ORDER OR ADD BREAKs: the order and that there
        // are no breaks is both essential for the correctness and
        // runtime speed of the algorithm.
        switch (vertices_num) {
            case (7):
                tri.comp.v3 = vertlist[idxptr[5]];
                tri.comp.v2 = vertlist[idxptr[6]];
                triangles.push_back(tri);
            case (6):
                tri.comp.v3 = vertlist[idxptr[4]];
                tri.comp.v2 = vertlist[idxptr[5]];
                triangles.push_back(tri);
            case (5):
                tri.comp.v3 = vertlist[idxptr[3]];
                tri.comp.v2 = vertlist[idxptr[4]];
                triangles.push_back(tri);
            case (4):
                tri.comp.v3 = vertlist[idxptr[2]];
                tri.comp.v2 = vertlist[idxptr[3]];
                triangles.push_back(tri);
            default:
            case (3):
                tri.comp.v3 = vertlist[idxptr[1]];
                tri.comp.v2 = vertlist[idxptr[2]];
                triangles.push_back(tri);
        };

        idxptr += vertices_num;
    }
}

void perform_slices(const float* fgrid, const int_type dim_x,
                    const int_type dim_z, const int_type dim_y,
                    const float offset_x, const float offset_z,
                    const float offset_y, const float voxel_width,
                    const float voxel_depth, const float voxel_height,
                    const float isolevel,
                    MarchingCubes::triangle_vector_type& triangles) {

    const grid_cell* grid = (const grid_cell*) fgrid;

    int_type pre1, pre2;

    const int_type dim_xz = dim_x * dim_z;

    cube4c values;
    cubevec3f points;

    const int_type lbd_x = 0;
    const int_type ubd_x = dim_x - 1;
    const int_type lbd_z = 0;
    const int_type ubd_z = dim_z - 2;
    const int_type lbd_y = 0;
    const int_type ubd_y = dim_y - 2;

    float off_y1 = offset_y + (lbd_y * voxel_height);
    float off_y2 = off_y1 + voxel_height;
    float off_y3 = off_y2 + voxel_height;

    float off_z1, off_z2, off_z3;
    float off_x1, off_x2;

    // iterate over all voxels segmented in cube4s.
    //
    //      8-----------17
    //     /|          /|
    //    5-----------14|
    //   /| |        /| |     ===> ...
    //  2-----------11| |
    //  | | 7-------|-|-16
    //  | |/|       | |/|
    //  | 4---------|-13|
    //  |/| |       |/| |     ===> ...
    //  1-----------10| |
    //  | | 6-------|-|-15
    //  | |/        | |/
    //  | 3---------|-12
    //  |/          |/
    //  0-----------9
    int_type y, z, x;

    for (y = lbd_y; y < ubd_y; y += 2) {

        off_z1 = offset_z + (lbd_z * voxel_depth);
        off_z2 = off_z1 + voxel_depth;
        off_z3 = off_z2 + voxel_depth;

        const int_type yy = y + 1;
        const int_type yyy = yy + 1;

        for (z = lbd_z; z < ubd_z; z += 2) {

            off_x1 = offset_x + (lbd_x * voxel_width);
            off_x2 = off_x1 + voxel_width;

            const int_type zz = z + 1;
            const int_type zzz = zz + 1;

            // read first half cube as second. it will be copied into the first
            // half cube in the next for-loop.
            values.hc.hc1.idx.c0 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, z, y)];
            values.hc.hc1.idx.c3 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, zz, y)];
            values.hc.hc1.idx.c6 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, zzz, y)];
            values.hc.hc1.idx.c1 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, z, yy)];
            values.hc.hc1.idx.c4 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, zz, yy)];
            values.hc.hc1.idx.c7 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, zzz, yy)];
            values.hc.hc1.idx.c2 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, z, yyy)];
            values.hc.hc1.idx.c5 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, zz, yyy)];
            values.hc.hc1.idx.c8 =
                grid[MC_COMPUTE_INDEX(dim_x, dim_xz, lbd_x, zzz, yyy)];

            // used for pre-check.
            pre2 = (MC_ISOLEVEL_CHECK(values.hc.hc1.data[0], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[1], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[2], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[3], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[4], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[5], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[6], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[7], isolevel) +
                    MC_ISOLEVEL_CHECK(values.hc.hc1.data[8], isolevel));

            for (x = lbd_x + 1; x < ubd_x; x++) {

                // copy second half cube to first half cube.
                pre1 = pre2;
                values.hc.hc0 = values.hc.hc1;

                // read second halfcube.
                values.hc.hc1.idx.c0 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, y)];
                values.hc.hc1.idx.c3 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, y)];
                values.hc.hc1.idx.c6 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zzz, y)];
                values.hc.hc1.idx.c1 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, yy)];
                values.hc.hc1.idx.c4 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, yy)];
                values.hc.hc1.idx.c7 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zzz, yy)];
                values.hc.hc1.idx.c2 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, yyy)];
                values.hc.hc1.idx.c5 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, yyy)];
                values.hc.hc1.idx.c8 =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zzz, yyy)];

                // Short pre-check.
                pre2 = (MC_ISOLEVEL_CHECK(values.hc.hc1.data[0], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[1], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[2], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[3], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[4], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[5], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[6], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[7], isolevel) +
                        MC_ISOLEVEL_CHECK(values.hc.hc1.data[8], isolevel));
                pre1 += pre2;

                if ((pre1 != 18) && (pre1 != 0)) {

                    points.idx.c0.x = off_x1;
                    points.idx.c1.x = off_x2;
                    points.idx.c2.x = off_x2;
                    points.idx.c3.x = off_x1;

                    points.idx.c4.x = off_x1;
                    points.idx.c5.x = off_x2;
                    points.idx.c6.x = off_x2;
                    points.idx.c7.x = off_x1;

                    // cube 0
                    points.idx.c0.z = off_z1;
                    points.idx.c0.y = off_y1;
                    points.idx.c1.z = off_z1;
                    points.idx.c1.y = off_y1;
                    points.idx.c2.z = off_z1;
                    points.idx.c2.y = off_y2;
                    points.idx.c3.z = off_z1;
                    points.idx.c3.y = off_y2;

                    points.idx.c4.z = off_z2;
                    points.idx.c4.y = off_y1;
                    points.idx.c5.z = off_z2;
                    points.idx.c5.y = off_y1;
                    points.idx.c6.z = off_z2;
                    points.idx.c6.y = off_y2;
                    points.idx.c7.z = off_z2;
                    points.idx.c7.y = off_y2;

                    // triangulate.
                    triangulate<0>(values, points, triangles, isolevel);

                    // cube 1
                    points.idx.c0.z = off_z1;
                    points.idx.c0.y = off_y2;
                    points.idx.c1.z = off_z1;
                    points.idx.c1.y = off_y2;
                    points.idx.c2.z = off_z1;
                    points.idx.c2.y = off_y3;
                    points.idx.c3.z = off_z1;
                    points.idx.c3.y = off_y3;

                    points.idx.c4.z = off_z2;
                    points.idx.c4.y = off_y2;
                    points.idx.c5.z = off_z2;
                    points.idx.c5.y = off_y2;
                    points.idx.c6.z = off_z2;
                    points.idx.c6.y = off_y3;
                    points.idx.c7.z = off_z2;
                    points.idx.c7.y = off_y3;

                    // triangulate.
                    triangulate<1>(values, points, triangles, isolevel);

                    // cube 2
                    points.idx.c0.z = off_z2;
                    points.idx.c0.y = off_y1;
                    points.idx.c1.z = off_z2;
                    points.idx.c1.y = off_y1;
                    points.idx.c2.z = off_z2;
                    points.idx.c2.y = off_y2;
                    points.idx.c3.z = off_z2;
                    points.idx.c3.y = off_y2;

                    points.idx.c4.z = off_z3;
                    points.idx.c4.y = off_y1;
                    points.idx.c5.z = off_z3;
                    points.idx.c5.y = off_y1;
                    points.idx.c6.z = off_z3;
                    points.idx.c6.y = off_y2;
                    points.idx.c7.z = off_z3;
                    points.idx.c7.y = off_y2;

                    // triangulate.
                    triangulate<2>(values, points, triangles, isolevel);

                    // cube 3
                    points.idx.c0.z = off_z2;
                    points.idx.c0.y = off_y2;
                    points.idx.c1.z = off_z2;
                    points.idx.c1.y = off_y2;
                    points.idx.c2.z = off_z2;
                    points.idx.c2.y = off_y3;
                    points.idx.c3.z = off_z2;
                    points.idx.c3.y = off_y3;

                    points.idx.c4.z = off_z3;
                    points.idx.c4.y = off_y2;
                    points.idx.c5.z = off_z3;
                    points.idx.c5.y = off_y2;
                    points.idx.c6.z = off_z3;
                    points.idx.c6.y = off_y3;
                    points.idx.c7.z = off_z3;
                    points.idx.c7.y = off_y3;

                    // triangulate.
                    triangulate<3>(values, points, triangles, isolevel);
                }

                // update x positions.
                off_x1 = off_x2;
                off_x2 = off_x1 + voxel_width;
            }

            // update z positions.
            off_z1 = off_z3;
            off_z2 = off_z1 + voxel_depth;
            off_z3 = off_z2 + voxel_depth;
        }

        // update y positions.
        off_y1 = off_y3;
        off_y2 = off_y1 + voxel_height;
        off_y3 = off_y2 + voxel_height;
    }

    // is there a remaining y-slice?
    if ((dim_y % 2) == 0) {

        // perform last y-slice.
        y = ubd_y;
        const int_type yy = y + 1;
        off_y1 = offset_y + static_cast<float>(y) * voxel_height;
        off_y2 = off_y1 + voxel_height;

        off_z1 = offset_z + (lbd_z * voxel_depth);
        off_z2 = off_z1 + voxel_depth;

        for (z = lbd_z; z <= ubd_z; z += 1) {

            off_x1 = offset_x + (lbd_x * voxel_width);
            off_x2 = off_x1 + voxel_width;

            const int_type zz = z + 1;

            for (x = lbd_x; x < ubd_x; x++) {

                const int_type xx = x + 1;

                values.data[cubeidx<0>::i0] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, y)];
                values.data[cubeidx<0>::i1] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, z, y)];
                values.data[cubeidx<0>::i2] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, z, yy)];
                values.data[cubeidx<0>::i3] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, yy)];
                values.data[cubeidx<0>::i4] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, y)];
                values.data[cubeidx<0>::i5] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, zz, y)];
                values.data[cubeidx<0>::i6] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, zz, yy)];
                values.data[cubeidx<0>::i7] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, yy)];

                points.idx.c0.z = off_z1;
                points.idx.c0.y = off_y1;
                points.idx.c1.z = off_z1;
                points.idx.c1.y = off_y1;
                points.idx.c2.z = off_z1;
                points.idx.c2.y = off_y2;
                points.idx.c3.z = off_z1;
                points.idx.c3.y = off_y2;

                points.idx.c4.z = off_z2;
                points.idx.c4.y = off_y1;
                points.idx.c5.z = off_z2;
                points.idx.c5.y = off_y1;
                points.idx.c6.z = off_z2;
                points.idx.c6.y = off_y2;
                points.idx.c7.z = off_z2;
                points.idx.c7.y = off_y2;

                // triangulate.
                triangulate<0>(values, points, triangles, isolevel);
            }
        }
    }

    // is there a remaining z-slice?
    if ((dim_z % 2) == 0) {

        // perform last z-slice.
        z = ubd_z;
        const int_type zz = z + 1;
        off_z1 = offset_z + static_cast<float>(z) * voxel_depth;
        off_z2 = off_z1 + voxel_height;

        // ignore possible last (y) row in next section, if y-slice has been
        // proceed.
        const int iubd_y = ubd_y - (((dim_y % 2) == 0) ? (1) : (0));

        off_y1 = offset_y + (lbd_y * voxel_height);
        off_y2 = off_y1 + voxel_height;
        for (y = lbd_y; y <= iubd_y; y += 1) {

            const int yy = y + 1;

            off_x1 = offset_x + (lbd_x * voxel_width);
            off_x2 = off_x1 + voxel_width;

            for (x = lbd_x; x < ubd_x; x++) {

                const int_type xx = x + 1;

                values.data[cubeidx<0>::i0] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, y)];
                values.data[cubeidx<0>::i1] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, z, y)];
                values.data[cubeidx<0>::i2] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, z, yy)];
                values.data[cubeidx<0>::i3] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, z, yy)];
                values.data[cubeidx<0>::i4] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, y)];
                values.data[cubeidx<0>::i5] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, zz, y)];
                values.data[cubeidx<0>::i6] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, xx, zz, yy)];
                values.data[cubeidx<0>::i7] =
                    grid[MC_COMPUTE_INDEX(dim_x, dim_xz, x, zz, yy)];

                points.idx.c0.z = off_z1;
                points.idx.c0.y = off_y1;
                points.idx.c1.z = off_z1;
                points.idx.c1.y = off_y1;
                points.idx.c2.z = off_z1;
                points.idx.c2.y = off_y2;
                points.idx.c3.z = off_z1;
                points.idx.c3.y = off_y2;

                points.idx.c4.z = off_z2;
                points.idx.c4.y = off_y1;
                points.idx.c5.z = off_z2;
                points.idx.c5.y = off_y1;
                points.idx.c6.z = off_z2;
                points.idx.c6.y = off_y2;
                points.idx.c7.z = off_z2;
                points.idx.c7.y = off_y2;

                // triangulate.
                triangulate<0>(values, points, triangles, isolevel);
            }
        }
    }
}

void MarchingCubes::execute(const float* grid) {

    triangle_vector_.clear();

    // As first estimation for the expected number of triangles we use
    // sqrt(cubes).
    triangle_vector_.reserve(static_cast<std::size_t>(
        sqrt(grid_dim_x_ * grid_dim_y_ * grid_dim_z_)));

    perform_slices(grid, grid_dim_x_, grid_dim_z_, grid_dim_y_, offset_x_,
                   offset_z_, offset_y_, voxel_width_, voxel_depth_,
                   voxel_height_, isolevel_, triangle_vector_);
}

namespace ret {

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
