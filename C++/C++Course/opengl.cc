// To compile on Linux, try one of:
// g++ -ansi -pedantic -Wall opengl.cc -g -L /usr/lib   -lglut
// g++ -ansi -pedantic -Wall opengl.cc -g -L /usr/lib64 -lglut

// To compile on OS X, try:
// g++ -ansi -pedantic -Wall opengl.cc -g -I/usr/X11/include -framework GLUT -framework OpenGL


#include "Particle.h"
#include "SquareParticle.h"
#include "CircularParticle.h"
#include "Simulation.h"
#include "Colour.h"
#include "Vector2D.h"
#include "Linear.h"
#include "LCentral.h"
#include "RCentral.h"


int main (int argc, char* argv[])
{
  // Initial screen size
  int x_max = 900;
  int y_max = 500;
  const int timestep_in_milliseconds = 16;


  // Initial position of the particle
  Simulation* mySim = Simulation::GetInstance();
  mySim->Configure(x_max, y_max, timestep_in_milliseconds/1000.0, timestep_in_milliseconds);

  // Add new particles  
  mySim->AddParticle (new SquareParticle   (10.0, Vector2D(x_max / 2.0, y_max / 2.0),         Vector2D( 120.0,  190.0), Colour::rgb1(1.0,0.0,0.0), new Linear()));
  mySim->AddParticle (new CircularParticle (15.0, Vector2D(x_max / 2.0, y_max / 2.0),         Vector2D(-020.0,  190.0), Colour::rgb255(0,128,0),   new RCentral()));
  mySim->AddParticle (new CircularParticle (20.0, Vector2D(x_max / 2.0, y_max / 2.0),         Vector2D(-120.0, -100.0), Colour::rgb1(0.0,0.0,1.0), new LCentral()));
  mySim->AddParticle (new SquareParticle   (25.0, Vector2D(x_max / 2.0, y_max / 2.0),         Vector2D(-020.0, -190.0), Colour::rgb255(128,0,200), new LCentral()));
  mySim->AddParticle (new CircularParticle (30.0, Vector2D(x_max / 2.0, y_max / 2.0),         Vector2D( 020.0,  190.0), Colour::rgb255(0,255,0),   new RCentral()));
  mySim->AddParticle (new SquareParticle   (35.0, Vector2D(x_max / 2.0 + 150.0, y_max / 2.0), Vector2D(-200.0,  -50.0), Colour::rgb255(0,255,255), new LCentral()));
  mySim->AddParticle (new SquareParticle   (40.0, Vector2D(x_max / 2.0, y_max / 2.0),         Vector2D(-200.0, -090.0), Colour::rgb255(255,255,0), new Linear()));
 
  // On air
  mySim->Run();
}
