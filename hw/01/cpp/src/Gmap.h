#pragma once

#include "Point.h"

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;

struct Dart {
// cells - ids of cells that form the dart
    int dart_id;
    int vertex_id;
    int edge_id;
    int face_id;
    int volume_id = 1;

// involutions: indices of other darts
    int a0;
    int a1;
    int a2;
    char a3;
};

struct Vertex {
    // the coordinates of this vertex:
    Point point;
    // unique vertex id
    int vertex_id;
    // a dart incident to this Vertex:
    int dart_id;

    // constructor without arguments
    Vertex() : point(Point())
    {}

    // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
    Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z))
    {}

};

struct Edge {
    // unique edge id
    int eid;
    // index of vertex at start of edge
    int start;
    // index of vertex at end of edge
    int end;
    // a dart incident to this Edge:
    int dart;

    int bary_id;

    // comparison operator
    bool operator==(const Edge &other) const{
        if (((start == other.start) and (end == other.end)) || ((start == other.end) and (end == other.start))) {
            return true;
        }
    }

    // function to compute the barycenter for this Edge (needed for triangulation output):
    // Point barycenter() {}
};

struct Face {
    // the vertices of this face:
public:
    // unique face id
    int fid;
    // a dart incident to this Face:
    int dart;
    // indices of vertices forming the face
    std::vector<int> index_list;
    // indices of edges forming the face
    std::vector<int> edge_list;
    // all darts that belong to this face
    std::vector<Dart> face_Darts;

    int bary_id;

    // function to compute the barycenter for this Face (maybe needed for triangulation output):
    ////   Point barycenter(std::vector<Point> face_vertices) {
    ////      float number_vertices = face_vertices.size()
    ////      for (i = 0; i <= number_vertices; number_vertices ++){
    ////          sum_vertices += p;
    ////      }
    ////      return sum_vertices/number_vertices;
    ////   }

};

struct Volume {
    // unique identifier for this volume
    int vid;
    // a dart incident to this Volume:
    int dart;
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
