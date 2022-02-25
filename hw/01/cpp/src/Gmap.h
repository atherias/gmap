#pragma once

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
    //  // cells - ids that link to other tables?:
    int dart_id;
    int vertex_id;
    int edge_id;
    int face_id;
    int volume_id;

//  // involutions: should be pointers to other darts
    int a0;
    int a1;
    int a2;
    int a3;




    // previous implementation
//    int dart_id; // id of the dart
//    int vertex_id; // vertex id (it needs only one)
//    int edge_id; // edge id
//    int face_id; // face id
//    int volume_id; // volume id

    // involutions:
    // ..

    // cells:
    // ...

};

struct Vertex {
    // the coordinates of this vertex:
    Point point;

    // vertex id
    int vertex_id;
    int dart_id;



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
    int eid;
    // removed ampersand
    int start;
    int end;
    int dart;

    // comparison operator
    bool operator==(const Edge &other) const{
        if (((start == other.start) and (end == other.end)) || ((start == other.end) and (end == other.start))) {
            return true;
        }
    }
    // a dart incident to this Edge:
    // ...

    // function to compute the barycenter for this Edge (needed for triangulation output):
    // Point barycenter() {}
};

struct Face {
    // the vertices of this face:
public:
    int fid;
    int dart;
    std::vector<int> index_list;
    std::vector<int> edge_list;
    std::vector<Dart> face_Darts;

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

    //Face() { };


    // a dart incident to this Face:
    // ...

    // function to compute the barycenter for this Face (needed for triangulation output):
    // Point barycenter() {}

};

struct Volume {
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