// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Gmap.h"
#include <typeinfo>

Point midpoint_edge(Vertex one, Vertex two){
    float x = (one.point[0] + two.point[0]) / 2;
    float y = (one.point[1] + two.point[1]) / 2;
    float z = (one.point[2] + two.point[2]) / 2;
    return {x,y,z};
}



int main(int argc, const char * argv[]) {
//  std::string file_in = "torus.obj";
//  std::string file_out_obj = "/home/ravi/git/geo1004.2022/hw/01/data/torus_triangulated.obj";
//  std::string file_out_csv_d = "/home/ravi/git/geo1004.2022/hw/01/data/torus_darts.csv";
//  std::string file_out_csv_0 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_vertices.csv";
//  std::string file_out_csv_1 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_edges.csv";
//  std::string file_out_csv_2 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_faces.csv";
//  std::string file_out_csv_3 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_volume.csv";

    // ## Read OBJ file ##
    std::string file_in = "cube.obj";
//    std::string file_out_obj = "/home/ravi/git/geo1004.2022/hw/01/data/cube_triangulated.obj";
//    std::string file_out_csv_d = "/home/ravi/git/geo1004.2022/hw/01/data/cube_darts.csv";
//    std::string file_out_csv_0 = "/home/ravi/git/geo1004.2022/hw/01/data/cube_vertices.csv";
//    std::string file_out_csv_1 = "/home/ravi/git/geo1004.2022/hw/01/data/cube_edges.csv";
//    std::string file_out_csv_2 = "/home/ravi/git/geo1004.2022/hw/01/data/cube_faces.csv";
//    std::string file_out_csv_3 = "/home/ravi/git/geo1004.2022/hw/01/data/cube_volume.csv";

    // ## Read OBJ file ##
    // ## Read vertices + faces from an obj file:
    std::ifstream stream_in;
    stream_in.open(file_in);

    // create vectors to hold each structure type
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;

    //iterator for faces
    int k = 1;

    //iterator for edges
    int e = 1;

    //iterator for indices
    int i;

    if (stream_in.is_open()) {
        std::string line;
        // read file line by line
        while (getline(stream_in, line)) {
            std::istringstream iss(line);
            std::string word;
            // get first word
            iss >> word;
            // check if this is a vertex line
            if (word == "v") {
                //  make vector of this vertex's coordinates
                std::vector<float> coordinates;
                // iterate over each word in the line add coordinates to coordinates vector
                while (iss >> word) coordinates.push_back(std::stof(word));
                // once all coordinates added, add vertex to vertices vector
                if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
                else vertices.push_back(Vertex());
            }
                // check if this is a vertex line
            else if (word == "f") {
                std::vector<int> indices;
                while (iss >> word) indices.push_back(std::stof(word));
                // KEN said make it work for all types of shapes. What we know is every vertex will have two darts within a face.

                // method that works for any polygon shape.
                // create face
                faces.emplace_back(Face{k, indices});
                //add edges to edge vector
                for (i = 0; i < indices.size(); i++) {
                    // create edge
                    Edge new_edge = Edge{e, indices[i], indices[i + 1]};
                    // check if vector exists
                    if (!(std::count(edges.begin(), edges.end(), new_edge))) {
                        edges.emplace_back(new_edge);
//                      // increase e only if inserted in the vector
                        e++;
                    }
                }
                // create edge connecting begin to end
                Edge new_edge = Edge{e, indices[-1], indices[0]};
                // check if vector exists
                if (!(std::count(edges.begin(), edges.end(), new_edge))) {
                    edges.emplace_back(new_edge);
//                      // increase e only if inserted in the vector



//                if (indices.size() == 4) {
//                    //push vertices pointers to vertices vector, give an id in the face
//                    // store vertices instead, not pointers, maybe just remove AMPERSAND.
//                    faces.emplace_back(Face{k,((int) indices[0]), ((int) indices[1]), ((int) indices[2]), ((int) indices[3]),
//                                            vertices[indices[0]-1], vertices[indices[1]-1], vertices[indices[2]-1],
//                                            vertices[indices[3]-1]});
//
//                    //create edges // possibly do it with unordered_map
//                    // removed the ampersand, KEEP ALWAYS THE INDICES
//                    Edge new_edge1 =Edge{e, indices[0], indices[1]};
//                    bool wtfq = std::count(edges.begin(),edges.end(),new_edge1);
////                    if (!(std::count(edges.begin(),edges.end(),new_edge1))){
////                        edges.emplace_back(new_edge1);
////                        // increase e only if inserted in the vector
////                        e++;
////                    }
//
//                    Edge new_edge2 =Edge{e, indices[1], indices[2]};
//                    bool wtf = std::count(edges.begin(),edges.end(),new_edge2);
//
//                    if (!(std::count(edges.begin(),edges.end(),new_edge2))){
//                        edges.emplace_back(new_edge2);
//                        e++;
//                    }
//
//                    Edge new_edge3 =Edge{e, indices[2], indices[3]};
//                    if (!(std::count(edges.begin(),edges.end(),new_edge3))){
//                        edges.emplace_back(new_edge3);
//                        e++;
//                    }
//
//                    Edge new_edge4 =Edge{e, indices[3], indices[0]};
//                    if (!(std::count(edges.begin(),edges.end(),new_edge4))){
//                        edges.emplace_back(new_edge4);
//                        e++;
//                    }

                    //increase k for increasing the id of faces
                    k++;
//                }
                }
            }
        }











//// ## Print to see the vertices are stored correctly
//    for (auto i = vertices.begin(); i != vertices.end(); ++i){
//        // why not possible i.x όπως δείχνει στο τέλος του Point.h για να τυπώσει σημείο;
//        std::cout << i->point[0] << " " << i->point[1] << " " << i->point[2] << std::endl;
//    }
////

//// ## Print to see the vertices are stored correctly
//    //std::cout << faces.size() << std::endl;
//    for (auto i = faces.begin(); i != faces.end(); ++i){
//        std::cout << "print the faces" << std::endl;
//        std::cout << i->a << " " << i->b << " " << i->c << " " << i->d << std::endl;
//        // try to print the coordinates of the vertex:
//        std::cout << "print/access the vertices of the faces" << std::endl;
//        std::cout << vertices[i->a-1].point[0] << " " << vertices[i->a-1].point[1] << " " << vertices[i->a-1].point[2] << std::endl;
//        std::cout << vertices[i->b-1].point[0] << " " << vertices[i->b-1].point[1] << " " << vertices[i->b-1].point[2] << std::endl;
//        std::cout << vertices[i->c-1].point[0] << " " << vertices[i->c-1].point[1] << " " << vertices[i->c-1].point[2] << std::endl;
//        std::cout << vertices[i->d-1].point[0] << " " << vertices[i->d-1].point[1] << " " << vertices[i->d-1].point[2] << std::endl;
//    }


        // ## Construct generalised map using the structures from Gmap.h ##

        // Triangulation:
//    // Step 1: create a new vertex of each of the edges of the face (polygon)
//    for (auto i = faces.begin(); i != faces.end(); ++i){
//        std::cout << "print the faces" << std::endl;
//        std::cout << i->fid << " " << i->a1->point << " " << i->b1->point << " " << i->c1->point << " " << i->d1->point << std::endl;
//        // try to print the coordinates of the vertex:
//        std::cout << "print/access the vertices of the faces" << std::endl;
//        std::cout << vertices[i->a-1].point[0] << " " << vertices[i->a-1].point[1] << " " << vertices[i->a-1].point[2] << std::endl;
//        std::cout << vertices[i->b-1].point[0] << " " << vertices[i->b-1].point[1] << " " << vertices[i->b-1].point[2] << std::endl;
//        std::cout << vertices[i->c-1].point[0] << " " << vertices[i->c-1].point[1] << " " << vertices[i->c-1].point[2] << std::endl;
//        std::cout << vertices[i->d-1].point[0] << " " << vertices[i->d-1].point[1] << " " << vertices[i->d-1].point[2] << std::endl;
//        std::cout << vertices[(i.d)-1].point[0] << " " << vertices[i->d-1].point[1] << " " << vertices[i->d-1].point[2] << std::endl;

//        // NOT NEEDED FOR NOW: create a new vertex of each of the edges of the face (polygon)
//        Point midpoint1 = midpoint_edge(vertices[i->a-1], vertices[i->b-1]);
//        std::cout << "midpoint1 coordinates: " << midpoint1.x << " " << midpoint1.y << " " << midpoint1.z << std::endl;
//        Point midpoint2 = midpoint_edge(vertices[i->b-1], vertices[i->c-1]);
//        std::cout << "midpoint2 coordinates: " << midpoint2.x << " " << midpoint2.y << " " << midpoint2.z << std::endl;
//        Point midpoint3 = midpoint_edge(vertices[i->c-1], vertices[i->d-1]);
//        std::cout << "midpoint3 coordinates: " << midpoint3.x << " " << midpoint3.y << " " << midpoint3.z << std::endl;
//        Point midpoint4 = midpoint_edge(vertices[i->d-1], vertices[i->a-1]);
//        std::cout << "midpoint4 coordinates: " << midpoint4.x << " " << midpoint4.y << " " << midpoint4.z << std::endl;
//

//
//    }

        //print faces
        for (auto i = faces.begin(); i != faces.end(); ++i) {
            std::cout << "print faces" << std::endl;
            // Task: understand when it can print and when it cannot
            std::cout << i->fid << " " << i->index_list[0] << std::endl;
        }
        //print edges
        for (auto i = edges.begin(); i != edges.end(); ++i) {
            std::cout << "print edges" << std::endl;
            // Task: understand when it can print and when it cannot
            std::cout << i->eid << " " << i->start << " " << i->end << std::endl;
        }












        // ## Output generalised map to CSV ##

        // ## Create triangles from the darts ##

        // ## Write triangles to obj ##

        return 0;
    }
}