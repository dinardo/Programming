#ifndef CIRCULARPARTICLE_H
#define CIRCULARPARTICLE_H


#include "Colour.h"
#include "Vector2D.h"
#include "Drawer.h"
#include "Particle.h"
#include "Motion.h"


class CircularParticle : public Particle
{

 public:
  CircularParticle (float, Vector2D, Vector2D, Colour, Motion*);
  ~CircularParticle ();
  
  void Display (Drawer*);


 private:

};

#endif
