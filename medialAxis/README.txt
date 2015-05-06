Graduate Project “Medial Axis of Simple Polygon” README

Gary Tse
email: garystse at csu dot fullerton dot edu

5/5/2015

Summary

This project focuses on finding and drawing the medial axis of any simple polygon. This includes both convex as well as concave polygons. The medial axis is calculated by finding the internal Voronoi edges of the polygon. As long as the polygon has a ‘dense enough’ (high amount of points that make up the) boundary, the medial axis can be approximated correctly by the internal Voronoi edges. It takes advantage of the CGAL library and its dependencies, such as boost and gmp to provide data structures and functionalities that help with the process and its accuracy. It uses GLFW3 to draw the polygon inputed and medial axis lines and points.

In ‘MedialAxis.h’ I include all the required headers, typedef all commonly used classes, and initialize the functions required.

In ‘MedialAxis.cpp’ I have implemented all functionalities.

‘inputPolygonFile()’ reads in a polygon’s information based on an input file specified as an argument in the command line prompt when running the program. It also creates a Polygon_2 object that will be pass by reference later. The input file defines a polygon, and the structure required is a file where the first number specifies number of vertices and the rest of the numbers are floats that come in x,y coordinate in that order. Each number is separated by a space or line break. The function will return an error if the number of coordinates aren’t an even pair. The coordinates are read into a GLFloat array which is passed into the function ‘createCGALPolygon()’ to create the polygon using the CGAL data structure and class Polygon_2.

‘createCGALPolygon()’ takes in 3 parameters, a reference to the polygon passed in, the total number of vertices, and the array of vertices created in ‘inputPolygonFile()’. This function simply adds all points into the Polygon_2 object created in ‘inputPolygonFile()’. After the polygon object has the vertices added in as points the function is complete. Since the Polygon_2 object was passed in by reference, after control is returned to ‘inputPolygonFile()’, the polygon is simply returned.

‘internalVoronoiPoints()’ reads in the polygon and performs a Delaunay triangulation based on the vertices of the polygon. Based on the triangulation, the finite faces (ones where none of the vertices that make up the face is the infinite vertex) are iterated through to find their circumcenter, which happens to be the Voronoi vertices of the polygon. I check to make sure those points are bound within the polygon and if they are, save them in a vector of Points to be used later to draw them.

‘internalVoronoiEdges()’ reads in the polygon and performs a Delaunay triangulation again based on the vertices of the polygon. This time, the edges of each triangulation are iterated through to find their duals, or the line segments and rays that passes through them. These make up the Voronoi edges. I then check to make sure both vertices that make up the edge’s endpoints are within the bounds of the polygon. If so, the edge is pushed in a vector of CGAL Segments, which saves information about the 2 vertices that make up the line segment, to be used later to draw them.

‘displayMedialAxis()’ reads in the polygon and calls the ‘internalVoronoiEdges()’ and ‘internalVoronoiPoints()’ functions. It takes the return values and using openGL, iterates through their respective datatypes (segments and points respectively) and draws them using GL_LINES and GL_POINTS respectively. Points are in purple, while the edges are in white.

‘displayPolygonVert()’ and ‘displayPolygonEdge()’ simply iterates through the polygon’s vertices and builds them according to the ‘glBegin()’ parameter. GL_POINTS for points, and GL_LINE_STRIP. The points are drawn in red and the boundary edges of the polygon are draw in green.

In ‘main()’, I added some extra information about the polygon that will be printed to console.

When the program is run, a 640x480 GLFW window will be made that displays the polygon, the medial axis, and the Voronoi points that make it up.

The GLFW windows are hardcoded to specific screen coordinates for 4 polygons: ‘mapleLeaf’, ‘convex’, ‘simple’, and ‘human’. Other (correctly formatted) simple polygon files totaling up to 300 vertices can be read and will draw properly, but might not show up correctly in the window due to the hardcoded positions and camera parameters.

One limitation in the program is a ‘not significantly’ dense polygon boundary being read in. That is, a polygon that is not made up of many points. This reduces the accuracy of the medial axis based on since the Delaunay triangulation is made up of less triangles, hence there are less Voronoi edges. If a polygon does not have enough points making up its boundary, no medial axis will be visible according to the specifications of my code.

One planned update is the detect the ratio of points to edges of the boundary and based on that, decide whether to dynamically add midpoints to each edge so as to double the polygon’s point density before calculation a Delaunay triangulation. The number of iterations done can be based on the initial number of points and edges, so a smaller number means more iterations.

Building

The assignment was tested and run on OS X using the makefile provided by Professor Shafae. Within this repository, I copied the config and the Makefile, so it has a similar make structure to previous assignments. CGAL was installed using macports, which installed all CGAL dependencies as well, including boost, gmp, and mpfr.

This submission assumes that the Titan OpenGL Kit, CGAL, and CGAL dependencies (specified above) are installed or located somewhere within the system. Before compiling the program, edit the line labeled ‘OPENGL_KIT_HOME’ in Makefile.darwin within the ‘config’ folder to provide the correct path and file name where the Titan OpenGL Kit is located.

To CFFLAGS, add the file path where CGAL headers, boost headers, ‘gmp.h’, ‘mpfr.h’, and ‘mpf2mpfr.h’ are located.

To LLDLIBS, add: -lCGAL -lCGAL_Core -lgmp
To LDFLAGS, add the file path where the libraries stated in the previous line are located

The makefile will generate the target: MedialAxis.

Dependencies

Below are the dependencies used and where download links and install instructions are found.

CGAL 4.5.2 - http://www.cgal.org/
Boost 1.58 - http://www.boost.org/
Gmp 6.0 - https://gmplib.org/
Mpfr 3.1 - http://www.mpfr.org/
GLFW 3 - http://www.glfw.org/


Running

Due to my inexperience with makefiles, I can only guarantee that this program will compile and run in a Mac OS X environment using command line prompts.

Prompt > ./Medial Axis [name of polygon data file]

Four polygon data files are included that are guaranteed to be visible and fully functional:

mapleLeaf
convex
simple
human


References and tools

http://doc.cgal.org/latest/Manual/manual.html

http://stackoverflow.com/questions/1069523/find-medial-axis-of-a-polygon-using-c-sharp

http://doc.cgal.org/latest/Manual/packages.html - Operations on Polygons demo, to draw polygon.

O. Aichholzer, W. Aigner, F. Aurenhammer, T. Hackl, M. Rabl and B. Juttler. Medial Axis Computation for Planar Free-Form Shapes. May 2008

ChiKit Au. A simple algorithm for medial axis transform computation. January 2012


