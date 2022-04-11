#ifndef GRID_STRUCTS_H
#define GRID_STRUCTS_H

#include <string>
#include <vector>
#include <memory>

/// Coordinate system types.
enum class CoordinateSystem
{
  CARTESIAN   = 1,  ///< \f$ (x, y, z) \f$ coordinates
  CYLINDRICAL = 2,  ///< \f$ (r, z, \varphi) \f$ coordinates
  SPHERICAL   = 3   ///< \f$ (r, \varphi, \theta) \f$ coordinates
};

class Point;
typedef Point Vertex;
typedef Point Centroid;
typedef Point Normal;

class Mesh;


/// Create a 1D mesh from a list of vertices.
std::shared_ptr<Mesh>
create_1d_mesh(const std::vector<double>& vertices,
               const CoordinateSystem coordinate_system =
                   CoordinateSystem::CARTESIAN);

/// Create a zoned 1D mesh.
std::shared_ptr<Mesh>
create_1d_mesh(const std::vector<double>& zone_edges,
               const std::vector<size_t>& zone_subdivisions,
               const std::vector<int>& material_ids,
               const CoordinateSystem coordinate_system =
                   CoordinateSystem::CARTESIAN);


#include "point.h"

#endif //GRID_STRUCTS_H
