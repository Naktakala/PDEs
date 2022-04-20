#ifndef FACE_H
#define FACE_H

#include "grid_structs.h"
#include <vector>

/**
 * \brief A class that represents a face on a Cell.
 *
 * A Face is defined as a <tt>dim - 1</tt>-dimensional object which, in
 * a collection, bounds a <tt>dim</tt>-dimensional Cell. Face objects in various
 * dimensions are:
 *  -   1D: Vertex
 *  -   2D: Edge, or connection of 2 vertices
 *  -   3D: Surface, or collection of several edges.
 *      -   Triangle
 *      -   Quadrilateral
 */
class Face
{
public:
  std::vector<size_t> vertex_ids; ///< The vertex IDs that belong to this face.

  bool has_neighbor = false; ///< Flag for having a neighbor cell.
  size_t neighbor_id = 0;    ///< The neighbor cell ID or boundary ID.

  Normal normal;     ///< The face's outward pointing normal vector.
  Centroid centroid; ///< The centroid of the face.
  double area = 0.0; ///< The area of the face.

public:
  Face() = default;                    ///< Default constructor.
  Face(const Face& other);             ///< Copy constructor.
  Face(Face&& other);                  ///< Move constructor.
  Face& operator=(const Face& other);  ///< Assignment operator.

public:
  /// Get the contents of the Face to a string.
  std::string to_string() const;
};


#endif //FACE_H
