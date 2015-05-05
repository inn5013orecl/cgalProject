#include <iostream>
#include <fstream>
#include <list>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <GLFW/glfw3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef Triangulation::Edge_iterator TriEdgeIterator;
typedef Triangulation::Face_handle FaceHandle;
typedef Triangulation::Finite_faces_iterator TriFaceIterator;

using std::cout; 
using std::cin;
using std::endl;

#define MAX_N 300
bool polyInitialized = false;

//GLFW functions
static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void glfwDisplay(std::string infileName);

//My functions
void createCGAL_Polygon(Polygon_2 &p, int n, GLfloat vertices[MAX_N][2]);
Polygon_2 inputPolygonFile(std::string infileName);
std::vector<Segment> internalVoronoiEdges(Polygon_2 p);
std::vector<Segment> internalVoronoiEdges(Polygon_2 p);
void displayMedialAxis(Polygon_2 p);
void displayPolygonVert(Polygon_2 p);
void displayPolygonEdge(Polygon_2 p);


