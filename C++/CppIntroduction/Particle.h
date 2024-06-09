#ifndef PARTICLE_H
#define PARTICLE_H


#include "Colour.h"
#include "Vector2D.h"
#include "Drawable.h"
#include "Motion.h"
#include "Linear.h"
#include "LCentral.h"
#include "RCentral.h"


class Particle : public Drawable
{
  friend class Linear;
  friend class LCentral;
  friend class RCentral;
  // === Friends === are allowed to access private and protected member data and functions if neded !
  // Friendship is NOT inherited !


 public:
  Particle (float, Vector2D, Vector2D, Colour, Motion*);
  void Move (float, float, float, float, float);
  ~Particle ();


 protected:
  float    mass;
  Vector2D position;
  Vector2D velocity; // [pixels/s]
  Colour   colour;
  Motion*  moto;

};

#endif
