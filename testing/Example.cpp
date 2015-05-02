
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;

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
bool polyInitialized = false;
int N = 0;
GLfloat vertices[MAX_N][2];

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
  std::ifstream inFile("mapleLeaf");
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
      } // end if
    inFile >> vertices[i][j];
  } // end for j
} // end for i

inFile.close();
polyInitialized = true;

createCGAL_Polygon();  // Adds 2D points from global "vertices" array to global polygon p.
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
    glVertex2i(vi->x(),vi->y());
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
    glVertex2i(vi->x(),vi->y());
  }
  //to draw last edge from last point to beginning vertex, reset iterator
  vi = p.vertices_begin();
  glVertex2i(vi->x(),vi->y());

  glEnd();
  glFlush();
}

int main()
{
  // create a polygon and put some points in it
  /*Polygon_2 p;
  p.push_back(Point(0,0));
  p.push_back(Point(4,0));
  p.push_back(Point(4,4));
  p.push_back(Point(2,2));
  p.push_back(Point(0,4));*/

  inputPolygonFile();

  CGAL::set_pretty_mode(std::cout);
  std::cout << "created the polygon p:" << std::endl;
  std::cout << p << std::endl;
  std::cout << std::endl;

  // determine some properties of the polygon
  bool IsSimple    = p.is_simple();
  bool IsConvex    = p.is_convex();
  bool IsClockwise = (p.orientation() == CGAL::CLOCKWISE);
  double Area      = p.area();

  std::cout << "polygon p is";
  if (!IsSimple) std::cout << " not";
  std::cout << " simple." << std::endl;

  std::cout << "polygon p is";
  if (!IsConvex) std::cout << " not";
  std::cout << " convex." << std::endl;

  std::cout << "polygon p is";
  if (!IsClockwise) std::cout << " not";
  std::cout << " clockwise oriented." << std::endl;

  std::cout << "the area of polygon p is " << Area << std::endl;
  std::cout << std::endl;

  // apply some algorithms
  Point q(-53,21);
  std::cout << "created point q = " << q << std::endl;
  std::cout << std::endl;

  bool IsInside = (p.bounded_side(q) == CGAL::ON_BOUNDED_SIDE);
  std::cout << "point q is";
  if (!IsInside) std::cout << " not";
  std::cout << " inside polygon p." << std::endl;
  std::cout << std::endl;

  // traverse the vertices and the edges
  int n=0;
  for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
    std::cout << "vertex " << n++ << " = " << *vi << std::endl;
  std::cout << std::endl;

  n=0;
  for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
    std::cout << "edge " << n++ << " = " << *ei << std::endl;

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
    glViewport(250, -150, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-80.f * ratio, 80.f * ratio, -80.f, 80.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //draw CGAL polygon
    displayPolygonVert(p);
    displayPolygonEdge(p);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
