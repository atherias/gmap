GEO1004 - TU Delft
Triangulating a polygonal mesh with generalised maps
Submitted March 5 2022
Authors: Ioanna Panagiotidou (5616646), Tendai Mbwanda (5215544), Adele Therias (5485193)

The code completes the following tasks:
1. reads a 3D polygonal mesh from an OBJ file,
2. stores the polygonal mesh in a generalised map,
3. outputs all the darts and cells from the generalised map to .csv files
4. outputs a triangulation of the polygonal mesh to a new OBJ file.

**Important**

- Ensure that you place the .obj input file in the CMake file for this project.
- Specify pathway of input .obj file on line 42
- Specify names of output .obj and .csv files on lines 43-48

To compile and run:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./hw01

You can also open the `cpp` folder directly in CLion.