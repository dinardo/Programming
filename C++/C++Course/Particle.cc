#include "Particle.h"


Particle::Particle (float ma, Vector2D pos, Vector2D velo, Colour c, Motion* mo)
  : mass(ma), position(pos), velocity(velo), colour(c), moto(mo)
{
}


void Particle::Move (float limXmin, float limYmin, float limXmax, float limYmax, float dt)
{
  moto->Move(position, velocity, mass, dt);

  if (moto->CheckBoundaries(position, velocity, mass, limXmin, limYmin, limXmax, limYmax) == true)
    {
      Motion* old = moto;
      moto = moto->ChangeMotion();
      delete old;
    }
}


Particle::~Particle ()
{
}
