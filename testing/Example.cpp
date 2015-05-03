
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef Triangulation::Edge_iterator TriEdgeIterator;

using std::cout; 
using std::cin;
using std::endl;

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

#define MAX_N 200
std::string infileName = "mapleLeaf";
bool polyInitialized = false;
int N = 0;
GLfloat vertices[MAX_N][2];

//keep global for simplicity reasons...consider changing later
static Polygon_2 p;

void createCGAL_Polygon()
{
  if (polyInitialized == false)
  {
    cout << "Error! Polygon has not yet been read from file mapleLeaf." << endl;
    return;
  }

  // Adds 2D points from global "vertices" array to global polygon p.

  printf("\nSetting up 2D Polygon Vertices:\n");

  for (int i = 0; i < N; i++)
  {
    p.push_back(Point(vertices[i][0], vertices[i][1]));
  }

  CGAL::set_pretty_mode(cout);
    cout << "created the polygon p:" << endl;
    cout << p << endl;
    cout << endl;
}

void inputPolygonFile()
{
 // Read polygon from file mapleLeaf and initialize both
 // vertices array and CGAL polygon.

//   File format: 1st line contains integer number of polygon vertices, which will be stored in N
//          Each subsequent line contains D floating point coordinates of a single vertex
//                delimited by white space.

  if (polyInitialized == true)
  {
    cout << "Warning! Polygon has already been initialized, so we cannot reinitialize." << endl;
    return;
  }

  // Open input file mapleLeaf
  std::ifstream inFile(infileName);
  if (!inFile.is_open())
  {
    cout << "Error attempting to open input file mapleLeaf!" << endl;
    return;
  }
  if (inFile.eof())
    {
      cout << "Error attempting to read number of polygon vertices from input file mapleLeaf!" << endl;
      inFile.close();
      return;
   }
  inFile >> N;
  if (N > MAX_N)
  {
    cout << "Error!  Number of polygon vertices exceeds maximum = " << MAX_N << endl;
    inFile.close();
    return;
  }
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      if (inFile.eof())
      {
        cout << "Error! Premature end of input file mapleLeaf." << endl;
        inFile.close();
        return;
      }
    inFile >> vertices[i][j];
    }
  } 

  inFile.close();
  polyInitialized = true;

  createCGAL_Polygon();  // Adds 2D points from global "vertices" array to global polygon p.
}

std::vector<Segment> internalVoronoiEdges(Polygon_2 p) {
  Triangulation t;
  std::vector<Segment> segments;
  t.insert(p.vertices_begin(), p.vertices_end());
  for(TriEdgeIterator e_it = t.edges_begin(); e_it != t.edges_end(); ++e_it) {
    CGAL::Object o = t.dual(e_it);
    Segment s;
    if(CGAL::assign(s, o))
    {
      segments.push_back(s);
    }
  }
  return segments;
}

void displayMedialAxis(Polygon_2 p) {
  std::vector<Segment> segments = internalVoronoiEdges(p);

  glLineWidth(3.0);
  glColor3f(1.0f,1.0f,1.0f);
  glBegin(GL_LINES);
  bool isInside0, isInside1;
  for(std::vector<Segment>::iterator si = segments.begin(); si != segments.end(); ++si) {
    isInside0 = (p.bounded_side(si->vertex(0)) == CGAL::ON_BOUNDED_SIDE/* || p.bounded_side(si->vertex(0)) == CGAL::ON_BOUNDARY*/);
    isInside1 = (p.bounded_side(si->vertex(1)) == CGAL::ON_BOUNDED_SIDE/* || p.bounded_side(si->vertex(1)) == CGAL::ON_BOUNDARY*/);
    if(isInside0 && isInside1) {
      glVertex2f(si->vertex(0).x(), si->vertex(0).y());
      glVertex2f(si->vertex(1).x(), si->vertex(1).y());      
    }
  }
  glEnd();
  glFlush();
}

void displayPolygonVert(Polygon_2 p) {
  if(polyInitialized == false) {
    cout << "Error, Polygon has not been read from file" << endl;
    return;
  }

  glPointSize(5.0);
  glColor3f(1.0f,0.0f,0.0f);
  glBegin(GL_POINTS);
  for(VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
  {
    glVertex2f(vi->x(),vi->y());
  }
  glEnd();
  glFlush();
}

void displayPolygonEdge(Polygon_2 p) {
  if(polyInitialized == false) {
    cout << "Error, Polygon has not been read from file" << endl;
    return;
  }

  glLineWidth(3.0);
  glColor3f(0.0f,1.0f,0.0f);
  glBegin(GL_LINE_STRIP);
  VertexIterator vi = p.vertices_begin();
  for(vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
  {
    glVertex2f(vi->x(),vi->y());
  }
  //to draw last edge from last point to beginning vertex, reset iterator
  vi = p.vertices_begin();
  glVertex2f(vi->x(),vi->y());

  glEnd();
  glFlush();
}

void glfwDisplay(){
  //glfw window display
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  window = glfwCreateWindow(640, 480, "Maple Leaf", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  while (!glfwWindowShouldClose(window))
  {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    //fits mapleLeaf: x(min,max) = (-100.54, 1.27) | y(min,max) = (-35.32, 78.27)
    if(infileName == "mapleLeaf"){
      glViewport(250, -150, width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-80.f * ratio, 80.f * ratio, -80.f, 80.f, 1.f, -1.f);
    }
    else {
      glViewport(-250, -150 , width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho( 45.f * -ratio, 45.f * ratio, -45.f, 45.f, 1.f, -1.f); 
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //draw CGAL polygon
    displayPolygonVert(p);
    displayPolygonEdge(p);
    displayMedialAxis(p);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

int main()
{
  //load polygon from file that contains vertices
  inputPolygonFile();

  /*
  CGAL::set_pretty_mode(std::cout);
  std::cout << "created the polygon p:" << std::endl;
  std::cout << p << std::endl;
  std::cout << std::endl;
  */

  // determine some properties of the polygon
  bool IsSimple    = p.is_simple();
  bool IsConvex    = p.is_convex();

  std::cout << "polygon p is";
  if (!IsSimple) std::cout << " not";
  std::cout << " simple." << std::endl;

  std::cout << "polygon p is";
  if (!IsConvex) std::cout << " not";
  std::cout << " convex." << std::endl;

  //print out boundary vertices and edges
  int n=0;
  for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
    std::cout << "vertex " << n++ << " = " << *vi << std::endl;
  std::cout << std::endl;

  n=0;
  for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
    std::cout << "edge " << n++ << " = " << *ei << std::endl;
  
  //GLFW window setup and display
  glfwDisplay();

  return 0;
}
