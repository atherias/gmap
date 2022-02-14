// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Gmap.h"

int main(int argc, const char * argv[]) {
  std::string file_in = "/home/ravi/git/geo1004.2022/hw/01/data/torus.obj";
  std::string file_out_obj = "/home/ravi/git/geo1004.2022/hw/01/data/torus_triangulated.obj";
  std::string file_out_csv_d = "/home/ravi/git/geo1004.2022/hw/01/data/torus_darts.csv";
  std::string file_out_csv_0 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_vertices.csv";
  std::string file_out_csv_1 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_edges.csv";
  std::string file_out_csv_2 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_faces.csv";
  std::string file_out_csv_3 = "/home/ravi/git/geo1004.2022/hw/01/data/torus_volume.csv";
  
  // ## Read OBJ file ##

  /* Example how to read vertices from an obj file:
  
    std::ifstream stream_in;
    stream_in.open(file_in);
    std::vector<Vertex> vertices;
    if (stream_in.is_open()) {
      std::string line;
      while (getline(stream_in, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        if (word == "v") {
          std::vector<float> coordinates;
          while (iss >> word) coordinates.push_back(std::stof(word));
          if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
          else vertices.push_back(Vertex());
        }
      }
    }
  */
  
  // ## Construct generalised map using the structures from Gmap.h ##
  
  // ## Output generalised map to CSV ##

  // ## Create triangles from the darts ##

  // ## Write triangles to obj ##
  
  return 0;
}
