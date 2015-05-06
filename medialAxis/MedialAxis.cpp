/* Gary Tse
 * email: garystse at csu dot fullerton dot edu
 *
 * Description: Parsed an input file containing # of vertices and 
 *              vertices in x,y pairs for use in creating a polygon
 *              in 2 dimensions. Then, created a delaunay triangulation
 *              based on the convex hull of the polygon as defined by
 *              the vertices, then using that triangulation, come up with
 *              the Voronoi Diagram. Checked for internal edges
 *              only to come up with Medial axis of the polygon. Added 
 *              Voronoi Points for clarity.
*/

#include "MedialAxis.h"

void createCGALPolygon(Polygon_2 &p, int n, GLfloat vertices[MAX_N][2]) {
  assert(polyInitialized != false);

  printf("\nSetting up 2D Polygon Vertices:\n");

  for (int i = 0; i < n; i++)
  {
    p.push_back(Point(vertices[i][0], vertices[i][1]));
  }
  /*
  CGAL::set_pretty_mode(cout);
  cout << "created the polygon p:" << endl;
  cout << p << endl;
  cout << endl;
  */
}

Polygon_2 inputPolygonFile(std::string infileName) {

  Polygon_2 poly;
  int n = 0;
  GLfloat vertices[MAX_N][2];

  // Open input file
  std::ifstream inFile(infileName);
  assert(inFile.is_open());
  inFile >> n;

  if (n > MAX_N)
  {
    cout << "Polygon vertices beyond max allowed: " << MAX_N << endl;
    inFile.close();
    assert(n > MAX_N);
  }
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      if (inFile.eof())
      {
        inFile.close();
        assert(!inFile.eof());
      }
    inFile >> vertices[i][j];
    }
  } 

  inFile.close();
  polyInitialized = true;

  createCGALPolygon(poly, n, vertices);
  return poly;
}

std::vector<Point> internalVoronoiPoints(Polygon_2 p) {
  Triangulation t;
  std::vector<Point> points;
  t.insert(p.vertices_begin(), p.vertices_end());
  for(TriFaceIterator fi = t.finite_faces_begin(); fi != t.finite_faces_end(); ++fi) {
    Point pt = t.dual(fi);
    if(p.bounded_side(pt) == CGAL::ON_BOUNDED_SIDE)
    {
      points.push_back(pt);
      //CGAL::set_pretty_mode(cout);
      //cout << pt << endl;
    }
  }
  return points;
}

std::vector<Segment> internalVoronoiEdges(Polygon_2 p) {
  Triangulation t;
  std::vector<Segment> segments;
  bool isInside0, isInside1;
  t.insert(p.vertices_begin(), p.vertices_end());
  for(TriEdgeIterator ei = t.edges_begin(); ei != t.edges_end(); ++ei) {
    CGAL::Object o = t.dual(ei);
    Segment s;
    if(CGAL::assign(s, o))
    {
      isInside0 = (p.bounded_side(s.vertex(0)) == CGAL::ON_BOUNDED_SIDE/* || p.bounded_side(si->vertex(0)) == CGAL::ON_BOUNDARY*/);
      isInside1 = (p.bounded_side(s.vertex(1)) == CGAL::ON_BOUNDED_SIDE/* || p.bounded_side(si->vertex(1)) == CGAL::ON_BOUNDARY*/);
      if(isInside0 && isInside1) {
        segments.push_back(s);
      }
    }
  }
  return segments;
}

void displayMedialAxis(Polygon_2 p) {
  std::vector<Segment> segments = internalVoronoiEdges(p);
  std::vector<Point> points = internalVoronoiPoints(p);

  glLineWidth(3.0);
  glColor3f(1.0f,1.0f,1.0f);
  glBegin(GL_LINES);
  bool isInside0, isInside1;
  for(std::vector<Segment>::iterator si = segments.begin(); si != segments.end(); ++si) {
    glVertex2f(si->vertex(0).x(), si->vertex(0).y());
    glVertex2f(si->vertex(1).x(), si->vertex(1).y());
  }
  glEnd();

  glPointSize(10.0);
  glColor3f(1.0f,0.0f,1.0f);
  glBegin(GL_POINTS);
  for(std::vector<Point>::iterator pi = points.begin(); pi != points.end(); ++pi) {
    //isInside0 = (p.bounded_side(*pi) == CGAL::ON_BOUNDED_SIDE/* || p.bounded_side(si->vertex(0)) == CGAL::ON_BOUNDARY*/);
    //if(isInside0) {
      glVertex2f(pi->x(), pi->y());  
    //}
  }
  glEnd();

  glFlush();
}

void displayPolygonVert(Polygon_2 p) {
  assert(polyInitialized != false);

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
  assert(polyInitialized != false);

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

void glfwDisplay(Polygon_2 p, std::string infileName) {
  //glfw window display
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  window = glfwCreateWindow(640, 480, infileName, NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  while (!glfwWindowShouldClose(window)) {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    //fits mapleLeaf: x(min,max) = (-100.54, 1.27) | y(min,max) = (-35.32, 78.27)
    if(infileName == "mapleLeaf") {
      glViewport(250, -150, width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-80.f * ratio, 80.f * ratio, -80.f, 80.f, 1.f, -1.f);
    }
    //fits convex, "house" pentagon
    else if(infileName == "convex") {
      glViewport(-200, -150 , width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho( 63.f * -ratio, 63.f * ratio, -63.f, 63.f, 1.f, -1.f); 
    }
    //fits simple, "dipped" pentagon
    else if(infileName == "simple") {
      glViewport(-200, -150 , width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho( 45.f * -ratio, 45.f * ratio, -45.f, 45.f, 1.f, -1.f); 
    }
    //fits human, bad mouse drawing of a person
    else {
      glViewport(125, 100 , width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho( 200.f * -ratio, 200.f * ratio, -200.f, 200.f, 1.f, -1.f); 
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

int main(int argc, char **argv)
{
  std::string infileName = argv[1];
  //load polygon from file that contains vertices
  Polygon_2 p = inputPolygonFile(infileName);
  std::vector<Point> points = internalVoronoiPoints(p);

  // display polygon info based on CGAL Example polygon code.
  bool IsSimple    = p.is_simple();
  bool IsConvex    = p.is_convex();

  cout << "polygon p is";
  if (!IsSimple) cout << " not";
  cout << " simple." << endl;

  cout << "polygon p is";
  if (!IsConvex) cout << " not";
  cout << " convex." << endl;

  //print out boundary
  /*
  int n=0;
  for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
    std::cout << "vertex " << n++ << " = " << *vi << std::endl;
  std::cout << std::endl;

  n=0;
  for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
    std::cout << "edge " << n++ << " = " << *ei << std::endl;
  */
  //end

  //GLFW window setup and display
  glfwDisplay(p, infileName);

  return 0;
}
