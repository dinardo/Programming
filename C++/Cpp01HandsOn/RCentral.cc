#include <cmath>


#include "RCentral.h"
#include "Linear.h"


void RCentral::Move (Vector2D& position, Vector2D& velocity, float mass, float dt) const
{
  float G = 10.0;
  float thetaStep = - M_PI / 1e2;
  Vector2D v = velocity / velocity.Mag();

  float myMag = velocity.Mag();
  velocity.x += G * mass * (cos(thetaStep)*v.x - sin(thetaStep)*v.y);
  velocity.y += G * mass * (sin(thetaStep)*v.x + cos(thetaStep)*v.y);
  velocity = velocity * myMag / velocity.Mag();
  position += velocity * dt;
}


Motion* RCentral::ChangeMotion () const
{
  return new Linear();
}
