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
    std::vector<Dart> darts_face;
    std::unordered_map<pair , int, pair_hash > unordered_map_2 = { }; // initialize the un_map empty


    //iterator for faces
    int k = 1;

    //iterator for edges
    int e = 1;

    //iterator for indices
    int i;

    //iterator for darts
    int dart_id =1;

    //iterator for vertices
    int vertex_id =1;

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
                Vertex vertex = Vertex(coordinates[0], coordinates[1], coordinates[2]);
                vertex.vertex_id = vertex_id;
                vertices.emplace_back(vertex);
                vertex_id++;
            }
                // check if this is a vertex line
            else if (word == "f") {
                // create face
                faces.emplace_back(Face{k, dart_id});
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

                    // check if both checkers are not true (edge is not found in either direction) => create the edge and two darts
                    if ((!(checker1)) && (!(checker11))) {
                        // insert it in the unordered map
                        unordered_map_2[pair_edge1] = e;
                        //unordered_map_2.insert((indices[0], indices[1]),1);

                        // create the edge --> push it to the edges vector (do we need it?)
                        Edge new_edge = Edge{e, indices[i], indices[i + 1], dart_id};
                        edges.emplace_back(new_edge);
//                        edge_indices.emplace_back(new_edge.eid);

                        Dart new_dart({dart_id, indices[i], e, k, 1, dart_id+1});
                        darts.emplace_back(new_dart);
                        // add the dart to the darts_face
                        darts_face.emplace_back(new_dart);
                        // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                        (vertices[indices[i]]).dart_id = dart_id;

                        dart_id++;

                        Dart new_dart1({dart_id, indices[i+1], e, k, 1, dart_id-1});
                        darts.emplace_back(new_dart1);
                        darts_face.emplace_back(new_dart1);
                        // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                        (vertices[indices[i]]).dart_id = dart_id;

                        dart_id++;
                        e++;
                    }
                    else{
                        // LOGIC SAYS: every time we add an edge, or we don't add it but we retrieve its name from the unordered map, we create a DART.
                        // here, the Edge IS IN the MAP, so firstly we retrieve its name, then we create the dart.

                        // we are missing e! we have to find it. we are retrieving it from the unordered map.


                        if (checker1) {
                            if (unordered_map_2.at(pair_edge1)) {
                                int value_edge = unordered_map_2.at(pair_edge1);
//                                std::cout <<"Value from umap = " << unordered_map_2.at(pair_edge1) << std::endl;
                                int e_existing = unordered_map_2.at(pair_edge1);
                                Dart new_dart({dart_id, indices[i], e_existing, k, 1, dart_id+1});
                                darts.emplace_back(new_dart);
                                darts_face.emplace_back(new_dart);
                                // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                                (vertices[indices[i]]).dart_id = dart_id;

                                dart_id++;

                                Dart new_dart1({dart_id, indices[i+1], e_existing, k, 1, dart_id-1});
                                darts.emplace_back(new_dart1);
                                darts_face.emplace_back(new_dart1);
                                // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                                (vertices[indices[i]]).dart_id = dart_id;

                                dart_id++;

                            }
                        }
                        if (checker11) {
                            if (unordered_map_2.at(pair_edge11)) {
                                int value_edge = unordered_map_2.at(pair_edge11);
//                                std::cout << "Value from umap = " << unordered_map_2.at(pair_edge11) << std::endl;
                                int e_existing = unordered_map_2.at(pair_edge11);
                                Dart new_dart({dart_id, indices[i], e_existing, k, 1, dart_id+1});
                                darts.emplace_back(new_dart);
                                darts_face.emplace_back(new_dart);
                                // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                                (vertices[indices[i]]).dart_id = dart_id;

                                dart_id++;

                                Dart new_dart1({dart_id, indices[i+1], e_existing, k, 1, dart_id-1});
                                darts.emplace_back(new_dart1);
                                darts_face.emplace_back(new_dart1);
                                // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                                (vertices[indices[i]]).dart_id = dart_id;

                                dart_id++;

                            }
                        }
                    } // end of else
                } // end of iteration over the indices

//                     create edge connecting begin to end (4th edge of the face)
                int last_element = indices.size()-1;
                std::pair<int, int> pair_edge_final(indices[last_element], indices[0]);
                std::pair<int, int> pair_edge_final_reversed(indices[0], indices[last_element]);

                bool checker2 = check_double_key(unordered_map_2, pair_edge_final);
                bool checker22 = check_double_key(unordered_map_2, pair_edge_final_reversed);
                // check if vector exists
                if ((!(checker2)) && (!(checker22))) {
                    // insert it in the unordered map
                    unordered_map_2[pair_edge_final] = e;
                    //unordered_map_2.insert((indices[0], indices[1]),1);
                    Edge new_edge = Edge{e, indices[last_element], indices[0],dart_id};
                    edges.emplace_back(new_edge);


                    Dart new_dart({dart_id, indices[last_element], e, k, 1, dart_id+1});
                    darts.emplace_back(new_dart);
                    darts_face.emplace_back(new_dart);
                    // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                    (vertices[indices[i]]).dart_id = dart_id;

                    dart_id++;

                    Dart new_dart1({dart_id, indices[0], e, k, 1, dart_id-1});
                    darts.emplace_back(new_dart1);
                    darts_face.emplace_back(new_dart1);
                    // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                    (vertices[indices[i]]).dart_id = dart_id;

                    dart_id++;
                    e++;
                }
                else{
                    // LOGIC SAYS: every time we add an edge, or we don't add it but we retrieve its name from the unordered map, we create a DART.
                    // here, the Edge IS IN the MAP, so firstly we retrieve its name, then we create the dart.

                    // we are missing e! we have to find it. we are retrieving it from the unordered map.


                    if (checker2) {
                        if (unordered_map_2.at(pair_edge_final)) {
                            int value_edge = unordered_map_2.at(pair_edge_final);
//                            std::cout <<"Value from umap = " << unordered_map_2.at(pair_edge_final) << std::endl;
                            int e_existing = unordered_map_2.at(pair_edge_final);

                            Dart new_dart({dart_id, indices[last_element], e_existing, k, 1, dart_id+1});
                            darts.emplace_back(new_dart);
                            darts_face.emplace_back(new_dart);
                            // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                            (vertices[indices[i]]).dart_id = dart_id;

                            dart_id++;

                            Dart new_dart1({dart_id, indices[0], e_existing, k, 1, dart_id-1});
                            darts.emplace_back(new_dart1);
                            darts_face.emplace_back(new_dart1);
                            // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                            (vertices[indices[i]]).dart_id = dart_id;

                            dart_id++;

                        }
                    }
                    if (checker22) {
                        if (unordered_map_2.at(pair_edge_final_reversed)) {
                            int value_edge = unordered_map_2.at(pair_edge_final_reversed);
//                            std::cout << "Value from umap = " << unordered_map_2.at(pair_edge_final_reversed) << std::endl;
                            int e_existing = unordered_map_2.at(pair_edge_final_reversed);
                            Dart new_dart({dart_id, indices[last_element], e_existing, k, 1, dart_id+1});
                            darts.emplace_back(new_dart);
                            darts_face.emplace_back(new_dart);
                            // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                            (vertices[indices[i]]).dart_id = dart_id;

                            dart_id++;

                            Dart new_dart1({dart_id, indices[0], e_existing, k, 1, dart_id-1});
                            darts.emplace_back(new_dart1);
                            darts_face.emplace_back(new_dart1);
                            // LOGIC SAYS: every time we add a dart, we should add it to the vertex we used for the creation of the dart. In one of the two.
                            (vertices[indices[i]]).dart_id = dart_id;

                            dart_id++;

                        }
                    }
                } // end of else

// add involutions for alpha 1 on current face


//                for (auto d : darts){
//                    for (auto j:darts_face){
//                        if (d.face_id == j.face_id){
//                            if (d.vertex_id == j.vertex_id){
//                                if (d.edge_id != j.edge_id){
////                                    Dart* pd = &d;
//                                    int involution = j.dart_id;
//                                    d.a1 = involution;
//                                }
//                            }
//                        }
//                    }
//                }
//                darts_face.clear();

                k++;
            } // end of if face
        } // end of while loop
    }// end of if stream open

    // indicates if there is an error with the stream
    else{
        std::cout << "file not open" << std::endl;
    }

//        // Create darts
//        int dart_id = 0;
//        int edge_it = 0;
//        int dart_vol = 0;
//
//        //iterate over each face
//        for (int d = 0; d<faces.size(); d++){
//            // within each face, iterate over edges
//            for (faces[d].edge_list[edge_it=0]; edge_it < faces[d].edge_list.size(); edge_it++){
//                // within each edge, get start and end, create darts
//                int dart1_edge = faces[d].edge_list[edge_it];
//                int dart1_vertex = edges[dart1_edge].start;
//                int dart1_face = faces[d].fid;
//
//                int dart2_edge = faces[d].edge_list[edge_it];
//                int dart2_vertex = edges[dart1_edge].end;
//                int dart2_face = faces[d].fid;
//
//                Dart new_dart1({dart_id, dart1_vertex, dart1_edge, dart1_face, dart_vol});
//                dart_id++;
//                Dart new_dart2({dart_id, dart2_vertex, dart2_edge, dart2_face, dart_vol});
//                dart_id++;
//                darts.emplace_back(new_dart1);
//                darts.emplace_back(new_dart2);
//            }
//        }
//
//        // add involutions to darts
//        for (int dart = 0; dart<darts.size(); dart++){
//            dart.a0 = ;
//
//        }


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


    //print faces
    std::cout << "print faces" << std::endl;
    for (auto j = faces.begin(); j != faces.end(); ++j) {
        // Task: understand when it can print and when it cannot
        std::cout << j->fid << "| dart=" << j->dart << std::endl;
    }
    std::cout << " " << std::endl;
    //print vertices
    std::cout << "print vertices" << std::endl;
    for (auto j = vertices.begin(); j != vertices.end(); ++j) {
        // Task: understand when it can print and when it cannot
        std::cout << j->vertex_id << "| d=" << j->dart_id << "| x = " << j->point.x << "| y = " << j->point.y
                  << "| z = " << j->point.z << std::endl;

    }
    std::cout << " " << std::endl;
    //print edges
    std::cout << "print edges" << std::endl;
    for (auto j = edges.begin(); j != edges.end(); ++j) {
        // Task: understand when it can print and when it cannot
        std::cout << j->eid << "| start=" << j->start << "| end=" << j->end << "| dart=" << j->dart << std::endl;
    }
    std::cout << " " << std::endl;
    //print darts
    std::cout << "print darts" << std::endl;
    for (auto j = darts.begin(); j != darts.end(); ++j) {
        // Task: understand when it can print and when it cannot
        std::cout << j->dart_id << "| face= " << j->face_id<< "| edge = " << j->edge_id << "| vertex = " << j->vertex_id << "| a0= " << j->a0  << "| a1= " << j->a1  << std::endl;
    }
        return 0;

} // end of main function