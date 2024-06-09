#include "SquareParticle.h"


SquareParticle::SquareParticle (float ma, Vector2D pos, Vector2D velo, Colour c, Motion* mo) : Particle(ma, pos, velo, c, mo)
{
}


SquareParticle::~SquareParticle ()
{
}


void SquareParticle::Display (Drawer* sim)
{
  sim->draw_square (colour, position, mass);
}
