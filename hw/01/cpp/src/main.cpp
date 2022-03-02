
// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>

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
    std::string file_in = "cube.obj";
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
                faces.emplace_back(Face{k, dart_id});

                // create a vector and add the vertex indices belonging to the current face
                // subtract 1 from index value to align index of obj with c++ index
                std::vector<int> indices;
                while (iss >> word) indices.push_back(std::stoi(word) - 1);

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


    // PRINTING DARTS, FACES, EDGES, VERTICES --- --- --- --- --- ---
    //print faces
//    std::cout << "print faces" << std::endl;
//    for (auto j = faces.begin(); j != faces.end(); ++j) {
//        // Task: understand when it can print and when it cannot
//        std::cout << j->fid << "| d" << j->dart << std::endl;
//    }
//    std::cout << " " << std::endl;
//    //print edges
//    std::cout << "print edges" << std::endl;
//    for (auto j = edges.begin(); j != edges.end(); ++j) {
//        // Task: understand when it can print and when it cannot
//        std::cout << j->eid << "| d=" << j->dart << std::endl;
//    }
//
//    std::cout << " " << std::endl;
//    //print vertices
//    std::cout << "print vertices" << std::endl;
//    for (auto j = vertices.begin(); j != vertices.end(); ++j) {
//        // Task: understand when it can print and when it cannot
//        std::cout << j->vertex_id << "| d=" << j->dart_id << "| x = " << j->point.x << "| y = " << j->point.y
//                  << "| z = " << j->point.z << std::endl;
//
//    }
//    std::cout << " " << std::endl;
//    //print darts
//    std::cout << "print darts" << std::endl;
//    for (auto j = darts.begin(); j != darts.end(); ++j) {
//        // Task: understand when it can print and when it cannot
//        std::cout << j->dart_id << " | a0= " << j->a0 << " | a1= " << j->a1 << " | a2= " << j->a2 << " | v"
//                  << j->vertex_id << " | e"
//                  << j->edge_id << " | f" << j->face_id << std::endl;
//    }


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
    std::vector<int> triangle_faces;
    //create a local "current_triangle_vertices" vector of integers
    std::vector<int> current_triangle_vertices;
    // keep track of visited edges
    std::vector<Edge> visited_edges;
    // keep track of visited faces
    std::vector<int> visited_faces;
    std::vector<Point> barycenters;
    std::unordered_map<std::string, Vertex> unordered_map_3 = {};
    std::stringstream streamy;

    // add all coordinates of vertices to unordered map as string
    for (auto a = vertices.begin(); a != vertices.end(); a++){
        char coord_x = streamy << a->point.x;
        float y = a->point.y;
        float z = a->point.z;
        unordered_map_3.insert ({coord_x,a});
    }

        // ## Create triangles from the darts ##
        for (auto z = darts.begin(); z != darts.end(); z++) {
            //get vertex index from dart, add this index to current_triangle_vertices
            current_triangle_vertices.emplace_back(z->vertex_id);

            //get edge id from dart, use id to get the edge from the edge vector and get the start and end vertex ids.
            int dart_edge = z->edge_id;
            int edge_start = edges[dart_edge].start;
            int edge_end = edges[dart_edge].end;

            //use these vertex ids to retrieve vertex coordinates, then calculate barycenter of the edge.
            Point barycenter_edge = (vertices[edge_start].point + vertices[edge_end].point) / 2;


//            //NEED TO CHECK IF BARYCENTER IS ALREADY STORED AS VERTEX. Have explored a few options, in order of likely success:
//          option 1: add coordinates of points as string (key) and point id (value) to unordered map and then check if the point is in unordered map

//          option 2: compare this point to points in vertices. Not working - doesn't seem to recognize when two points are equal
            for (auto l = vertices.begin(); l != vertices.end(); l++){
                if (l->point == barycenter_edge){

                }
            }
//
//            option 3: check if edge has been visited - but how to then figure out which vertex id it has??
//            auto found = std::find(visited_edges.begin(), visited_edges.end(), dart_edge);
//            if (found != visited_edges.end()) {
//                current_triangle_vertices.emplace_back(z->vertex_id);
//            }
//            else {
//                Vertex new_v(barycenter_edge.x, barycenter_edge.y, barycenter_edge.z);
//                new_v.vertex_id = vertices.size()+1;
//                vertices.emplace_back(new_v);
//                visited_edges.emplace_back(edges[dart_edge-1]);
//            }



            current_triangle_vertices.clear();
            }


//            //4. get face id from dart. use id to get the face from faces vector and get the indices of vertexes that make up the face.
//            int dart_face = z->face_id;
//            std::vector<int> face_vertices = faces[dart_face].index_list;
//            //use these vertex ids to retrieve points
//            std::vector<Point> face_vertices_coordinates;
//            for (auto w = face_vertices.begin(); w != face_vertices.end(); ++w){
//                face_vertices_coordinates.emplace_back(vertices[w]->point);
//            }
//
//            // Calculate barycenter of the face
//
//            Point barycenter_face (std::vector<Point> face_vertices) {
                ////      float number_vertices = face_vertices.size()
                ////      for (i = 0; i <= number_vertices; number_vertices ++){
                ////          sum_vertices += p;
                ////      }
            //check if barycenter is already in the vector of vertices. if so, retrieve vertex id and add to current_triangle_vertices.
            //If not, create vertex, add it to the vector of vertices, get its index and add to current_triangle_vertices.

            //5. once three vertices have been defined for this triangle, check if their order enables the correct orientation,

            //if not, change the order. then, add this vector of integers to the triangle_face vector.


        // ## Write triangles to obj ##
        //after all darts have been visited, write to file. First, write all vertices, then all faces.
        return 0;
    }


// notes --- TO BE DELETED IN THE END --- ---


//                    // Just Print CONTENTS OF THE UNORDERED MAP
//                    std::cout << "Contents of the unordered_map : \n";
//                    for (auto p : unordered_map_2)
//                        std::cout << "[" << (p.first).first << ", "
//                             << (p.first).second << "] ==> "
//                             << p.second << "\n";

//                    for (auto const &entry: unordered_map_1)
//                    {
//                        auto key_pair = entry.first;
//                        std::cout << "{" << key_pair.first << "," << key_pair.second << "}, "
//                                  << entry.second << std::endl;
//                    }

// --------------------------------------------- EDGES
//Edge new_edge1 =Edge{e, indices[0], indices[1]};
//bool wtfq = std::count(edges.begin(),edges.end(),new_edge1);
//                    if (!(std::count(edges.begin(),edges.end(),new_edge1))){
//                        edges.emplace_back(new_edge1);
//                        // increase e only if inserted in the vector
//                        e++;
//                    }

//Edge new_edge2 =Edge{e, indices[1], indices[2]};
//bool wtf = std::count(edges.begin(),edges.end(),new_edge2);

//                    if (!(std::count(edges.begin(),edges.end(),new_edge2))){
//                        edges.emplace_back(new_edge2);
//                        e++;
//                    }

//Edge new_edge3 =Edge{e, indices[2], indices[3]};
//                    if (!(std::count(edges.begin(),edges.end(),new_edge3))){
//                        edges.emplace_back(new_edge3);
//                        e++;
//                    }

//Edge new_edge4 =Edge{e, indices[3], indices[0]};
//                    if (!(std::count(edges.begin(),edges.end(),new_edge4))){
//                        edges.emplace_back(new_edge4);
//                        e++;
//                    }


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
