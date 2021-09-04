#ifndef SQAREPARTICLE_H
#define SQAREPARTICLE_H


#include "Colour.h"
#include "Vector2D.h"
#include "Drawer.h"
#include "Particle.h"
#include "Motion.h"


class SquareParticle : public Particle
{

 public:
  SquareParticle (float, Vector2D, Vector2D, Colour, Motion*);
  ~SquareParticle ();

  void Display (Drawer*);


 private:

};

#endif
