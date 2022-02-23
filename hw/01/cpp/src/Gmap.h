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

//bool edge_exists (Edge E, std::vector<Edge> edge_vector) {
//    for (int index = 0; index < edge_vector.size(); index++) {
//        if (E == edge_vector[index]){
//        return true;
//        }
//    }
//}

struct Dart {

//  // cells - ids that link to other tables?:
  int dart_id;
  int vertex_id;
  int edge_id;
  int face_id;
  int volume_id;

//  // involutions: should be pointers to other darts
  Dart* a0;
  Dart* a1;
  Dart* a2;
  Dart* a3;

};

struct Vertex {
    // vertex id
//    char vertex_id;

  // the coordinates of this vertex:
  Point point;

  // constructor without arguments
  Vertex() : point(Point()) 
  {}

  // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
  Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z))
  {}

  // a dart incident to this Vertex:
  // ... define like a method vertex.alpha0()

};

struct Edge {
    int eid;
    int start;
    int end;

//    // comparison operator - not working??
//    bool operator==(const Edge &other) const{
//        if (((start == other.start) and (end == other.end)) || ((start == other.end) and (end == other.start))) {
//            return true;
//        }
//    }


    // a dart incident to this Edge:
    // ...

    // function to compute the barycenter for this Edge (needed for triangulation output):
    // Point barycenter() {}
};

struct Face {
    // the vertices of this face:
public:
    int fid;
    std::vector<int> index_list;
    std::vector<int> edge_list;

//    int a, b, c, d;
//
//    Vertex a1;
//    Vertex b1;
//    Vertex c1;
//    Vertex d1;

    friend std::ostream &operator << (std::ostream &os, const Vertex& rhs){
        os << "Vertex = " << rhs.point;
        return os;
    }



// a dart incident to this Face:
  // ...
  // ...

  // function to compute the barycenter for this Face (needed for triangulation output):
//   Point barycenter(std::vector<Point> face_vertices) {
//      float number_vertices = face_vertices.size()
//      for (i = 0; i <= number_vertices; number_vertices ++){
//          sum_vertices += p;
//      }
//      return sum_vertices/number_vertices;
//   }

};

struct Volume {
    // unique volume id
//    char vol_id;
//
//    // vector of volume face vertices
//    std::vector Point volume_vertices;
//
//    // barycenter?

    // a dart incident to this Volume:
  // ...

};

// UNORDERED MAP
#include <unordered_map>
//#include <utility>

// define an unordered map that accepts two values for key

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
        //return std::hash<T1>()(size.());
    }
};