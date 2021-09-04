#include "CircularParticle.h"


CircularParticle::CircularParticle (float ma, Vector2D pos, Vector2D velo, Colour c, Motion* mo) : Particle(ma, pos, velo, c, mo)
{
}


CircularParticle::~CircularParticle ()
{
}


void CircularParticle::Display (Drawer* sim)
{
  sim->draw_circle (colour, position, mass);
}
