#ifndef SIMULATION_H
#define SIMULATION_H


#ifdef __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif


#include <cmath>
#include <vector>


#include "Vector2D.h"
#include "Particle.h"
#include "Drawer.h"


class Simulation : public Drawer
// === Singleton === implementation of the "Simulation" :
// we always have one instance of the class !
{

 public:
  static Simulation* GetInstance ();
  void Configure (int, int, const float, const int);
  void AddParticle (Particle* p);
  void MoveAll ();
  void Run ();
  void draw_circle (Colour c, Vector2D position, float radius);
  void draw_square (Colour c, Vector2D position, float length);
  ~Simulation ();


 private:
  Simulation ();
  static void display ();
  static void update (int);
  static Simulation* simInstance;


  // A place to store the output of glGetIntegerv
  int viewport[4];

  int& x_min;
  int& y_min;
  int& x_max;
  int& y_max;
  float dt;
  int tStamp;
  std::vector<Particle*> pVector;

};

const double myTWOPI = 2.0 * M_PI;
// Here we have linkage problem:
// Either we define myTWOPI as "const" or as "static"
// which make internal linkage only for myTWOPI
// To force external linkage one has to use "extern"

#endif
