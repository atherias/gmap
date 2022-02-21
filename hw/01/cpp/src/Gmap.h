#pragma once
//ioanna
#include "Point.h"

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;

/*
Below you find the basic elements that you need to build the generalised map.
The main thing you need to fill out are the links between the elements:
  * the involutions and cells on the Dart
  * the darts on the cells

One way to do this is by using pointers. eg. define a member on the dart struct like

  Struct Dart {
    // involutions:
    // indicate that a0 does not point to an object
    Dart* a0 = nullptr;
    // ...

    // cells:
    // ...
  
  };

Then you could create and link Darts like:
  
  Dart* dart_a = new Dart();
  Dart* dart_b = new Dart();

  dart_a->a0 = dart_b;
*/

struct Dart {
  // involutions:
  int a0;
  int a1;
  int a2;
  int a3;

  // cells - ids that link to other tables?:
  Vertex cell_0;
  Edge cell_1;
  Face cell_2;
  Volume cell_3;

};

struct Vertex {
    // vertex id
    char vertex_id;

  // the coordinates of this vertex:
  Point point;

  // constructor without arguments
  Vertex() : point(Point()) 
  {}

  // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
  Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z))
  {}

  // a dart incident to this Vertex:
  // ...

};

struct Edge {
//    //edge id - maybe just use index
//    char edge_id;

    // start and end point
    Vertex *edge_start;
    Vertex *edge_end;

    // a dart incident to this Edge:
  // ...

  // function to compute the barycenter for this Edge (needed for triangulation output):
  Point barycenter(Point edge_end, Point edge_start) {
      (edge_end - edge_start)/2;
  }
};

struct Face {
    // unique face id - use index
//    char face_id;

    // vector of face vertices
    std::vector<Vertex> face_vertices;

// a dart incident to this Face:
  // ...
  // ...

  // function to compute the barycenter for this Face (needed for triangulation output):
   Point barycenter(std::vector<Point> face_vertices) {
      float number_vertices = face_vertices.size()
      for (i = 0; i <= number_vertices; number_vertices ++){
          sum_vertices += p;
      }
      return sum_vertices/number_vertices;
   }

};

struct Volume {
    // unique volume id
    char vol_id;

    // vector of volume face vertices
    std::vector Point volume_vertices;

    // barycenter?

    // a dart incident to this Volume:
  // ...

};