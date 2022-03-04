
// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>

#include "Gmap.h"
#include <typeinfo>

// to be used for triangulation, otherwise DELETE
Point midpoint_edge(Vertex one, Vertex two){
float x = (one.point[0] + two.point[0]) / 2;
float y = (one.point[1] + two.point[1]) / 2;
float z = (one.point[2] + two.point[2]) / 2;
return {x,y,z};
}

//## Unordered Map ##
// definition of pair
typedef std::pair<int, int> pair;

// function that passes a pair as a key and checks whether this key already exists in the unordered map
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

    // ## Define input and output files ##
    std::string file_in = "torus.obj";
    std::string file_out_obj = "torus_triangulated.obj";
    std::string file_out_csv_d = "torus_darts.csv";
    std::string file_out_csv_0 = "torus_vertices.csv";
    std::string file_out_csv_1 = "torus_edges.csv";
    std::string file_out_csv_2 = "torus_faces.csv";
    std::string file_out_csv_3 = "torus_volume.csv";


    // create vectors to hold each structure type
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;
    std::vector<Dart> darts;
    std::vector<Volume> volumes;

    std::unordered_map<pair, int, pair_hash> unordered_map_2 = {}; // initialize the un_map empty

    //Initialize iterators to be used in loops
    // for faces
    int k = 1;
    // for edges
    int e = 1;
    // for vertex indices
    int i;
    // for dart id
    int dart_id = 1;
    // for vertices
    int vertex_id = 1;


    // instantiate volume and add to volume vector (only one volume in this assignment)
    Volume volume1 = {1, 1};
    volumes.emplace_back(volume1);

    // ## Read OBJ file ##
    // ## Read vertices + faces from an obj file:
    std::ifstream stream_in;
    stream_in.open(file_in);
    if (stream_in.is_open()) {
        std::string line;
        while (getline(stream_in, line)) {
            std::istringstream iss(line);
            std::string word;
            iss >> word;

            // If line begins with "v",  instantiate a vertex and add it to vertices vector
            if (word == "v") {
                std::vector<float> coordinates;
                while (iss >> word) coordinates.push_back(std::stof(word));
                // check if the vertex has 3 coordinates, otherwise create empty point
                if (coordinates.size() == 3) {
                    Vertex vertex = Vertex(coordinates[0], coordinates[1], coordinates[2]);
                    vertex.vertex_id = vertex_id;
                    vertices.emplace_back(vertex);
                    vertex_id++;
                } else vertices.push_back(Vertex());
            }
            // If line begins with "f", instantiate a face and add to faces vector, assign the current dart_id:
            if (word == "f") {
                // create a vector and add the vertex indices belonging to the current face
                // subtract 1 from index value to align index of obj with c++ index
                std::vector<int> indices;
                while (iss >> word) indices.push_back(std::stoi(word) - 1);

                faces.emplace_back(Face{k, dart_id, indices});

                // create a vector that will contain the darts of the current face
                std::vector<Dart> darts_face;

                // ## Storing edges ##
                // Iterate over indices to determine start and end of each edge of the current face
                // Create edges from these indices and add to to edges vector if they are not already found
                // in the unordered map (to avoid duplicate edges)

                // --- --- --- --- ---  PROCESS OF ALL EDGES EXCEPT THE LAST ONE --- --- --- ---- ----
                for (i = 0; i < indices.size() - 1; i++) {
                    // create a pair of indices that designate the start and end of the edge
                    std::pair<int, int> pair_edge1(indices[i], indices[i + 1]);
                    // create another pair that flips the start and end of the edge
                    std::pair<int, int> pair_edge11(indices[i + 1], indices[i]);

                    // instantiate a checker for each pair: these search the unordered map and return True if the pair is found
                    bool checker1 = check_double_key(unordered_map_2, pair_edge1);
                    bool checker11 = check_double_key(unordered_map_2, pair_edge11);

                    // check if both checkers return False: this means the edge does not already exist in the unordered map
                    if ((!(checker1)) && (!(checker11))) {
                        // insert the pair in the unordered map, instantiate the new edge and add to edges vector
                        unordered_map_2[pair_edge1] = e;
                        Edge new_edge = Edge{e, indices[i], indices[i + 1], dart_id};
                        edges.emplace_back(new_edge);

                        // create first dart that corresponds to this new edge, add it to the darts vector and the darts_face vector
                        Dart new_dart({dart_id, indices[i] + 1, e, k, 1, dart_id + 1});
                        darts.emplace_back(new_dart);
                        darts_face.emplace_back(new_dart);

                        // assign the dart_id to the first vertex used to create the dart, then increment dart_id
                        (vertices[indices[i]]).dart_id = dart_id;
                        dart_id++;

                        // create second dart that corresponds to this new edge, add it to the darts vector and the darts_face vector
                        Dart new_dart1({dart_id, indices[i + 1] + 1, e, k, 1, dart_id - 1});
                        darts.emplace_back(new_dart1);
                        darts_face.emplace_back(new_dart1);
                        dart_id++;

                        // increment e: this will be the next edge_id
                        e++;

                    }

                        // if the edge does not already exist, only create the darts that correspond to it on the current face
                    else {
                        // find the edge_id of the current edge by retrieving it from the unordered map
                        // the order of the vertices defining the start and end of the edge matter, therefore find which pair exists in the map
                        if (checker1) {
                            if (unordered_map_2.at(pair_edge1)) {
                                // retrieve index of the pair in the map, create a new dart using the retrieved edge id
                                // add to darts vector and darts face vector
                                // for each dart created, assign dart_id to the first vertex used to create the dart, increment dart_id
                                int e_existing = unordered_map_2.at(pair_edge1);

                                Dart new_dart({dart_id, indices[i] + 1, e_existing, k, 1, dart_id + 1});
                                darts.emplace_back(new_dart);
                                darts_face.emplace_back(new_dart);
                                (vertices[indices[i]]).dart_id = dart_id;
                                dart_id++;

                                Dart new_dart1({dart_id, indices[i + 1] + 1, e_existing, k, 1, dart_id - 1});
                                darts.emplace_back(new_dart1);
                                darts_face.emplace_back(new_dart1);
                                dart_id++;
                            }
                        }

                        // check if pair of edge vertices exists when order is reversed (start becomes end)
                        if (checker11) {
                            if (unordered_map_2.at(pair_edge11)) {
                                // retrieve index of the pair in the map, create a new dart using the retrieved edge id
                                // add to darts vector and darts face vector
                                // for each dart created, assign dart_id to the first vertex used to create the dart, increment dart_id
                                int e_existing = unordered_map_2.at(pair_edge11);


                                Dart new_dart({dart_id, indices[i] + 1, e_existing, k, 1, dart_id + 1});
                                darts.emplace_back(new_dart);
                                darts_face.emplace_back(new_dart);
                                (vertices[indices[i]]).dart_id = dart_id;
                                dart_id++;

                                Dart new_dart1({dart_id, indices[i + 1] + 1, e_existing, k, 1, dart_id - 1});
                                darts.emplace_back(new_dart1);
                                darts_face.emplace_back(new_dart1);
                                dart_id++;
                            }
                        }
                    } // end of else

                } // end of iteration over the indices
                // --- --- --- --- ---  END OF ALL EDGES BUT ONE --- --- --- ---- ----

                // beginning of LAST EDGE ! -----------------------------------------------------------------------------
                // Last Edge: create edge pair connecting last vertex to first vertex (close the face) and apply the same procedure as above

                int last_element = indices.size() - 1;

                // create a pair of indices that designate the start and end point of the edge
                std::pair<int, int> pair_edge_final(indices[last_element], indices[0]);
                // create another pair that flips the start and end points of the edge
                std::pair<int, int> pair_edge_final_reversed(indices[0], indices[last_element]);

                bool checker2 = check_double_key(unordered_map_2, pair_edge_final);
                bool checker22 = check_double_key(unordered_map_2, pair_edge_final_reversed);


                // check if both checkers return False: this means the edge does not already exist in the unordered map
                if (!(checker2) && !(checker22)) {
                    unordered_map_2[pair_edge_final] = e;
                    Edge new_edge = Edge{e, indices[last_element], indices[0], dart_id};
                    edges.emplace_back(new_edge);

                    Dart new_dart({dart_id, indices[last_element] + 1, e, k, 1, dart_id + 1});
                    darts.emplace_back(new_dart);
                    darts_face.emplace_back(new_dart);
                    (vertices[indices[last_element]]).dart_id = dart_id;
                    dart_id++;

                    Dart new_dart1({dart_id, indices[0] + 1, e, k, 1, dart_id - 1});
                    darts.emplace_back(new_dart1);
                    darts_face.emplace_back(new_dart1);
                    dart_id++;

                    e++;
                } else {
                    // find the edge_id of the current edge by retrieving it from the unordered map
                    // the order of the vertices defining the start and end of the edge matter, therefore find which pair exists in the map
                    if (checker2) {
                        if (unordered_map_2.at(pair_edge_final)) {
                            int e_existing = unordered_map_2.at(pair_edge_final);

                            Dart new_dart({dart_id, indices[last_element] + 1, e_existing, k, 1, dart_id + 1});
                            darts.emplace_back(new_dart);
                            darts_face.emplace_back(new_dart);
                            (vertices[indices[last_element]]).dart_id = dart_id;
                            dart_id++;

                            Dart new_dart1({dart_id, indices[0] + 1, e_existing, k, 1, dart_id - 1});
                            darts.emplace_back(new_dart1);
                            darts_face.emplace_back(new_dart1);
                            dart_id++;

                        }
                    }
                    if (checker22) {
                        if (unordered_map_2.at(pair_edge_final_reversed)) {
                            int e_existing = unordered_map_2.at(pair_edge_final_reversed);

                            Dart new_dart({dart_id, indices[last_element] + 1, e_existing, k, 1, dart_id + 1});
                            darts.emplace_back(new_dart);
                            darts_face.emplace_back(new_dart);
                            vertices[indices[last_element]].dart_id = dart_id;
                            dart_id++;

                            Dart new_dart1({dart_id, indices[0] + 1, e_existing, k, 1, dart_id - 1});
                            darts.emplace_back(new_dart1);
                            darts_face.emplace_back(new_dart1);
                            dart_id++;

                        }
                    }
                } // end of else
                // --- -----------  ------ - END OF LAST EDGE --- ---  ---- --- --- --- --- -  - - - -

                // THE PROBLEM is solved. CANNOT ASSIGNE VALUES. SOS -- S0S -- S0S --> DIFFERENCE WITH for (auto d: darts)
                // https://stackoverflow.com/questions/51954966/iterate-through-a-vector-of-objects-and-find-a-variable-that-matches-one-pulled


                // ## GET ALPHA 1 ##
                // Iterate over all darts in the darts vector
                // find a dart from the current face that has the same face id and vertex id but different edge id
                // the id of the dart that satisfies these conditions is the result of involution alpha 1
                for (auto d = darts.begin(); d != darts.end(); d++) {
                    for (auto j: darts_face) {
                        if (d->face_id == j.face_id) {
                            if (d->vertex_id == j.vertex_id) {
                                if (d->edge_id != j.edge_id) {
                                    d->a1 = j.dart_id;
                                }
                            }
                        }
                    }
                }



                // increment k before moving to the next face and clear the vector of darts for the current face
                k++;
                darts_face.clear();
            } // end of if word == f
        } // end of while

        // AFTER READING THE FILE
        // ## GET ALPHA 2 ##
        /* find alpha 2 for every dart by searching for another dart in any other face, with the same 0-cell and 1-cell */
        for (auto dd = darts.begin(); dd != darts.end(); dd++) {
            dd->a3 = '-';
            for (auto dj: darts) {
                if (dd->vertex_id == dj.vertex_id) {
                    if (dd->edge_id == dj.edge_id) {
                        if (dd->face_id != dj.face_id) {
                            dd->a2 = dj.dart_id;
                        }
                    }
                }
            }
        }

    } // here we close the FILE --> END


    // --- --- --- --- --- --- OUTPUTS CSV FILES --- --- --- --- --- ---

    // ## Output generalised map to CSV ##

    // ## 1) output darts.csv file ##
    std::ofstream stream_out1;
    stream_out1.open(file_out_csv_d);
    stream_out1 << "id;a0;a1;a2;v;e;f;v" << std::endl;
    for (auto j = darts.begin(); j != darts.end(); ++j) {
        stream_out1 << j->dart_id << ";" << j->a0 << ";" << j->a1 << ";" << j->a2 << ";" << j->a3 << ";" << j->vertex_id << ";"
                    << j->edge_id << ";" << j->face_id << ";" << 1 << std::endl;
    }

    // ## 2) output vertices.csv file ##
    std::ofstream stream_out2;
    stream_out2.open(file_out_csv_0);
    stream_out2 << "id;dart;x;y;z" << std::endl;
    for (auto j = vertices.begin(); j != vertices.end(); ++j) {
        stream_out2 << j->vertex_id << ";" << j->dart_id << ";" << j->point.x << ";" << j->point.y << ";" << j->point.z
                    << std::setprecision(2) << std::fixed << std::endl;
    }

    // ## 3) output edges.csv file ##
    std::ofstream stream_out3;
    stream_out3.open(file_out_csv_1);
    stream_out3 << "id;dart" << std::endl;
    for (auto j = edges.begin(); j != edges.end(); ++j) {
        stream_out3 << j->eid << ";" << j->dart << std::endl;
    }

    // ## 4) output faces.csv file ##
    std::ofstream stream_out4;
    stream_out4.open(file_out_csv_2);
    stream_out4 << "id;dart" << std::endl;
    for (auto j = faces.begin(); j != faces.end(); ++j) {
        stream_out4 << j->fid << ";" << j->dart << std::endl;
    }

    // ## 5) output volume.csv file ##
    std::ofstream stream_out5;
    stream_out5.open(file_out_csv_3);
    stream_out5 << "id;dart" << std::endl;
    for (auto j = volumes.begin(); j != volumes.end(); ++j) {
        stream_out5 << j->vid << ";" << j->dart << std::endl;

    }
    // --- --- --- --- TRIANGULATION --- --- --- ---
    // ## Create triangles from the darts ##

    // LOOP through the edges: create the barycenters
    for (auto z=edges.begin(); z!=edges.end();z++){
        //for every edge, get the start and end vertex ids.
        int edge_start = z->start;
        int edge_end = z->end;

        //use these vertex ids to retrieve vertex coordinates, then calculate barycenter of the edge.
        z->bary_id = vertex_id;
        Point barycenter_edge = (vertices[edge_start].point + vertices[edge_end].point) / 2;
        Vertex new_vertex_la = Vertex({barycenter_edge.x, barycenter_edge.y, barycenter_edge.z});
        new_vertex_la.vertex_id = vertex_id;
        vertices.emplace_back(new_vertex_la);
        vertex_id++;

    }

    // LOOP through the faces: create the barycenters
    for (auto x=faces.begin(); x!=faces.end();x++){
        //for every face, get the vertex ids
        //use these vertex ids to retrieve vertex coordinates, then calculate barycenter of the face
        x->bary_id = vertex_id;
        Point barycenter_face = (vertices[x->index_list[0]].point + vertices[x->index_list[1]].point + vertices[x->index_list[2]].point +
                                 vertices[x->index_list[3]].point) / 4;
        Vertex new_vertex_bla = Vertex({barycenter_face.x, barycenter_face.y, barycenter_face.z});
        new_vertex_bla.vertex_id = vertex_id;
        vertices.emplace_back(new_vertex_bla);

        vertex_id++;
    }

    std::vector<std::vector<int>> triangles;
    // ITERATE THROUGH THE DARTS
    for (auto z = darts.begin(); z != darts.end(); z++) {
        //get edge id, face id and vertex id from the dart, use these to form vertex ids for each triangle
        int dart_edge = z->edge_id;
        int dart_face = z->face_id;

        std::vector<int> triangle_vertices;
        triangle_vertices.emplace_back(z->vertex_id);
        triangle_vertices.emplace_back(edges[dart_edge-1].bary_id);
        triangle_vertices.emplace_back(faces[dart_face-1].bary_id);

        triangles.emplace_back(triangle_vertices);
    }


    // --- --- --- --- --- --- OUTPUTS OBJ FILE --- --- --- --- --- ---

    // ## Output triangulation to OBJ ##

    // ## 1) output triangulation obj file ##
    // iterate over vertices vector and write coordinates
    std::ofstream stream_out10;
    stream_out10.open(file_out_obj);
    stream_out10 << " " << std::endl;
    for (auto j = vertices.begin(); j != vertices.end(); ++j) {
        stream_out10 << "v " << j->point.x << " " << j->point.y << " " << j->point.z << std::endl;
    }


    // Ensure all faces are facing the same direction:
    // since we know that triangles were constructed with vertex, edge barycenter, face barycenter,
    // we alternate order of the writing of the face vertices for every other line.
    int skgit= 1;
    for (auto j: triangles) {
        if (skgit%2){
            stream_out10 << "f " << j[0] << " " << j[1] << " " << j[2] << std::endl;
        }
        else{
            stream_out10 << "f " << j[1] << " " << j[0] << " " << j[2] << std::endl;
        }
        skgit++;
    }


    return 0;
}
