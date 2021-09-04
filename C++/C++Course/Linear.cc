#include <cmath>


#include "Linear.h"
#include "LCentral.h"


void Linear::Move (Vector2D& position, Vector2D& velocity, float mass, float dt) const
{
  position += velocity * dt;
}


Motion* Linear::ChangeMotion () const
{
  return new LCentral();
}
