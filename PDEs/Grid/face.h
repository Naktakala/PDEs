#ifndef FACE_H
#define FACE_H

#include "cartesian_vector.h"

#include <vector>
#include <cstddef>


namespace PDEs
{
  namespace Grid
  {

    /**
     * A class that represents a face on a cell.
     *
     * A face is defined as a <tt>dim - 1</tt>-dimensional object which, in
     * a collection, bounds a <tt>dim</tt>-dimensional cell. Faces in various
     * dimensions are:
     *  -   1D: Vertex, or 1 vertex
     *  -   2D: Edge, or connection of 2 vertices
     *  -   3D: Surface, or collection of several edges.
     *      -   Triangle
     *      -   Quadrilateral
     */
    class Face
    {
    public:
      std::vector<size_t> vertex_ids;

      /// The neighbor cell ID on the interior, the boundary ID otherwise.
      size_t neighbor_id = 0;
      bool has_neighbor = false;

      Normal normal;
      Centroid centroid;
      double area = 0.0;

    public:
      /// Return the contents of the face as a string.
      std::string str() const;

      /// Insert the contents of a face into an output stream.
      friend std::ostream&
      operator<<(std::ostream& os, const Face& face);
    };
  }
}
#endif //FACE_H
