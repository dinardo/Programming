#include "Simulation.h"


Simulation* Simulation::simInstance = NULL;


Simulation* Simulation::GetInstance ()
{
  if (simInstance == NULL)
    simInstance = new Simulation();
  
  return simInstance;
}


void Simulation::Configure (int x_max_, int y_max_, const float dt_, int tStamp_)
{
  // Initialization member data for visualization
  x_max  = x_max_;
  y_max  = y_max_;
  dt     = dt_;
  tStamp = tStamp_;

  int argc    = 0;
  char** argv = NULL;


  // glutInit must be called before any other GL functions
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(x_max, y_max);
  glutCreateWindow("Particles");
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);

  // Register display as the function responsible for redrawing the screen
  glutDisplayFunc(display);
  
  // Run the update function 1 millisecond from now
  glutTimerFunc(1, this->update, 0);
}


Simulation::Simulation ()
  : x_min(viewport[0]), y_min(viewport[1]), x_max(viewport[2]), y_max(viewport[3])
{
}


Simulation::~Simulation ()
{
}


void Simulation::Run ()
{
  // Pass control over to the graphics framework
  glutMainLoop();
}


void Simulation::AddParticle (Particle* p)
{
  pVector.push_back(p);
}


void Simulation::MoveAll ()
{
  for (std::vector<Particle*>::iterator it = pVector.begin(); it != pVector.end(); it++)
    (*it)->Move(x_min,y_min,x_max,y_max,dt);
}


void Simulation::display ()
// What to do when the display is redrawn
{
  // Clear window
  glClear(GL_COLOR_BUFFER_BIT);


  // Draw the particle
  for (std::vector<Particle*>::iterator it = simInstance->pVector.begin(); it != simInstance->pVector.end(); it++)
    (*it)->Display(simInstance);


  // All the stuff we have drawn was stored in a buffer
  // Send the buffer contents to the physical screen
  glutSwapBuffers();
}


void Simulation::update (int)
// What to do when the state of the model is to be updated
{
  int znear = 0;
  int zfar  = 1;


  // Keep track of window resizing
  glGetIntegerv(GL_VIEWPORT, simInstance->viewport);
  glLoadIdentity();
  glOrtho(simInstance->x_min, simInstance->x_max, simInstance->y_min, simInstance->y_max, znear, zfar);


  simInstance->MoveAll();


  // Ensure that the screen is redrawn
  glutPostRedisplay();

  // Ensure we update again after a suitable delay
  int will_be_passed_to_update_and_ignored = 0;
  glutTimerFunc(simInstance->tStamp, update, will_be_passed_to_update_and_ignored);
}


void Simulation::draw_circle (Colour c, Vector2D pos, float radius)
{
  glColor3f(c.r1(), c.g1(), c.b1());
  
  glBegin(GL_LINE_LOOP);
  for (float angle = 0; angle < myTWOPI; angle += myTWOPI / 20.0)
    glVertex2f(pos.x + radius*cos(angle), pos.y + radius*sin(angle));
  glEnd(); // End of particle
}


void Simulation::draw_square (Colour c, Vector2D pos, float length)
{
  glColor3f(c.r1(), c.g1(), c.b1());
  
  glBegin(GL_LINE_LOOP);
  glVertex3f(pos.x+length, pos.y+length, 0);
  glVertex3f(pos.x-length, pos.y+length, 0);
  glVertex3f(pos.x-length, pos.y-length, 0);
  glVertex3f(pos.x+length, pos.y-length, 0);
  glEnd(); // End of particle
}
