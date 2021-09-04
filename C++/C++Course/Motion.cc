#include "Motion.h"


bool Motion::CheckBoundaries (Vector2D& position,
			      Vector2D& velocity,
			      float mass,
			      float limXmin,
			      float limYmin,
			      float limXmax,
			      float limYmax)
{
  
  bool hit = false;
  
  // Maybe bounce off the right edge
  if (position.x + mass > limXmax)
    {
      position.x -= (position.x + mass - limXmax);
      velocity.x = - velocity.x;
      hit = true;
    }

  // Maybe bounce off the top edge
  if (position.y + mass > limYmax)
    {
      position.y -= (position.y + mass - limYmax);
      velocity.y = - velocity.y;
      hit = true;
    }

  // Maybe bounce off the left edge
  if (position.x - mass < limXmin)
    {
      position.x += (limXmin + mass - position.x);
      velocity.x = - velocity.x;
      hit = true;
    }

  // Maybe bounce off the bottom edge
  if (position.y - mass < limYmin)
    {
      position.y += (limYmin + mass - position.y);
      velocity.y = - velocity.y;
      hit = true;
    }

  return hit;
}
