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

// THIS HASH FUNCTION WORKS!
bool check_one_key(std::unordered_map<int, int> m, int key){
    if (m.count(key) == 0){
        return false;
    }
    else {
        return true;
    }
}

// definition of pair
typedef std::pair<int, int> pair;

// THIS HASH FUNCTION DOESN'T WORK ! // Just added pair_hash and it worked? Doesn't make sense. (It's passed as an argument already??)
bool check_double_key(std::unordered_map<pair, int, pair_hash> m, pair key){
    //return 1;
    if (m.count(key) == 0){
        return false;
    }
    else {
        return true;
    }
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
    std::vector<Dart> darts;
    std::unordered_map<pair , int, pair_hash > unordered_map_2 = { }; // initialize the un_map empty


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
                std::vector<int> edge_indices;
                // REMOVE 1 WHILE WE READ SO THAT WE DON'T HAVE IN OUR MINDS TO REMOVE IT EVERY TIME.
                while (iss >> word) indices.push_back(std::stoi(word)-1);
                // KEN said make it work for all types of shapes. What we know is every vertex will have two darts within a face.

                // method that works for any polygon shape.


                //iterate over indices, add edges to edge vector
                for (i = 0; i < indices.size()-1; i++) {
                    // check if edge exists in vector

                    // prepare my pair - edge 1
                    std::pair<int, int> pair_edge1(indices[i], indices[i + 1]); //indices[0] , indices[1]
                    // and the opposite
                    std::pair<int, int> pair_edge11(indices[i + 1], indices[i]); //indices[0] , indices[1]
                    bool checker1 = check_double_key(unordered_map_2, pair_edge1);
                    bool checker11 = check_double_key(unordered_map_2, pair_edge11);
//                    std::cout << "checker edge 1: " << checker1 << std::endl;
                    if ((!(checker1)) && (!(checker11))) { //&& (checker1==0 || checker11==0) && (checker1==0 || checker11==1)){
                        //if (checker11==0){
                        // insert it in the unordered map
                        unordered_map_2[pair_edge1] = e;
                        //unordered_map_2.insert((indices[0], indices[1]),1);

                        // create the edge --> push it to the edges vector (do we need it?)
                        Edge new_edge = Edge{e, indices[i], indices[i + 1]};
                        edges.emplace_back(new_edge);
                        edge_indices.emplace_back(new_edge.eid);
                        e++;
                    }
                }


//                     create edge connecting begin to end
                int last_element = indices.size()-1;
                std::pair<int, int> pair_edge_final(indices[last_element], indices[0]);
                std::pair<int, int> pair_edge_final_reversed(indices[0], indices[last_element]);

                bool checker2 = check_double_key(unordered_map_2, pair_edge_final);
                bool checker22 = check_double_key(unordered_map_2, pair_edge_final_reversed);
                // check if vector exists
                if ((!(checker2)) && (!(checker22))) { //&& (checker1==0 || checker11==0) && (checker1==0 || checker11==1)){
                    //if (checker11==0){
                    // insert it in the unordered map
                    unordered_map_2[pair_edge_final] = e;
                    //unordered_map_2.insert((indices[0], indices[1]),1);
                    Edge new_edge = Edge{e, indices[last_element], indices[0]};
                    edges.emplace_back(new_edge);
                    e++;
                }


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
//                }


                // create face
                faces.emplace_back(Face{k, indices, edge_indices});
                k++;
            } // end of if face
        } // end of while loop
    }// end of if stream open

    // indicates if there is an error with the stream
    else{
        std::cout << "file not open" << std::endl;
    }

        // Create darts
        int dart_id = 0;
        int edge_it = 0;
        int dart_vol = 0;

        //iterate over each face
        for (int d = 0; d<faces.size(); d++){
            // within each face, iterate over edges
            for (faces[d].edge_list[edge_it=0]; edge_it < faces[d].edge_list.size(); edge_it++){
                // within each edge, get start and end, create darts
                int dart1_edge = faces[d].edge_list[edge_it];
                int dart1_vertex = edges[dart1_edge].start;
                int dart1_face = faces[d].fid;

                int dart2_edge = faces[d].edge_list[edge_it];
                int dart2_vertex = edges[dart1_edge].end;
                int dart2_face = faces[d].fid;

                Dart new_dart1({dart_id, dart1_vertex, dart1_edge, dart1_face, dart_vol});
                dart_id++;
                Dart new_dart2({dart_id, dart2_vertex, dart2_edge, dart2_face, dart_vol});
                dart_id++;
                darts.emplace_back(new_dart1);
                darts.emplace_back(new_dart2);
            }
        }

        // add involutions to darts
        for (int dart = 0; dart<darts.size(); dart++){
            dart.a0 = ;

        }


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


        // ## Output generalised map to CSV ##

        // ## Create triangles from the darts ##

        // ## Write triangles to obj ##


//    //print faces
//    for (auto i = faces.begin(); i != faces.end(); ++i) {
//        std::cout << "print faces" << std::endl;
//        // Task: understand when it can print and when it cannot
//        std::cout << i->fid << " " << i->index_list[0] << i->index_list[1] << i->index_list[2] << i->index_list[3] << std::endl;
//    }
//    //print edges
//    for (auto i = edges.begin(); i != edges.end(); ++i) {
//        std::cout << "print edges" << std::endl;
//        // Task: understand when it can print and when it cannot
//        std::cout << i->eid << " " << i->start << " " << i->end << std::endl;
//    }
    //print darts
    for (auto i = darts.begin(); i != darts.end(); ++i) {
        std::cout << "print darts" << std::endl;
        // Task: understand when it can print and when it cannot
        std::cout << i->dart_id << " " << i->face_id<< " " << i->edge_id << " " << i->vertex_id  << std::endl;
    }
        return 0;

} // end of main function