################################
# Synopsis:                    #
# python Universe.py [tk, pyg] #
################################


import cProfile

#from mock import Mock
from Colors import Colors

import sys
from operator import gt, lt
from math import sqrt, sin, cos, pi
from random import random

from Vector import Vector, Vector2D
from MotionFactory import LH, RH, makeLinear, makeConstCircular

PROTON, ELECTRON, NEUTRON, SUN, MERCURY, VENUS, EARTH, MARS, JUPITER = [object() for _ in range(9)]
COLLIDE, NOCOLLIDE, GRAVITY, NOGRAVITY = [object() for _ in range(4)]

def beep():
#    print '\a'
    print('beep...')

class Proton(object):
    def __init__(self):
        self.type   = PROTON
        self.color  = Colors()
        self.color.setRGBNames('white')
        self.decay_time = None
        self.charge = +1
        self.mass   = 10.0
        self.radius = 20.0

class Electron(object):
    def __init__(self):
        self.type   = ELECTRON
        self.color  = Colors()
        self.color.setRGBNames('green')
        self.decay_time = None
        self.charge = -1
        self.mass   = 2.0
        self.radius = 10.0

class Neutron(object):
    def __init__(self):
        self.type   = NEUTRON
        self.color  = Colors()
        self.color.setRGBNames('red')
        self.decay_time = 40*random()
        self.charge = 0
        self.mass   = 10.0
        self.radius = 20.0

class Sun(object):
    def __init__(self):
        self.type   = SUN
        self.color  = Colors()
        self.color.setRGBNames('yellow')
        self.decay_time = None
        self.charge = 0
        self.mass   = 2.0
        self.radius = 25.0

class Mercury(object):
    def __init__(self):
        self.type   = MERCURY
        self.color  = Colors()
        self.color.setRGBNames('gray')
        self.decay_time = None
        self.charge = 0
        self.mass   = 3.3e-7
        self.radius = 2.5

class Venus(object):
    def __init__(self):
        self.type   = VENUS
        self.color  = Colors()
        self.color.setRGBNames('pink')
        self.decay_time = None
        self.charge = 0
        self.mass   = 6e-6
        self.radius = 6.0

class Earth(object):
    def __init__(self):
        self.type   = EARTH
        self.color  = Colors()
        self.color.setRGBNames('blue')
        self.decay_time = None
        self.charge = 0
        self.mass   = 6e-6
        self.radius = 6.0

class Mars(object):
    def __init__(self):
        self.type   = MARS
        self.color  = Colors()
        self.color.setRGBNames('red')
        self.decay_time = None
        self.charge = 0
        self.mass   = 6.4e-7
        self.radius = 3.0

class Jupiter(object):
    def __init__(self):
        self.type   = JUPITER
        self.color  = Colors()
        self.color.setRGBNames('cyan')
        self.decay_time = None
        self.charge = 0
        self.mass   = 1e-5 # Should be: 1.9e-3
        self.radius = 10.0

class Particle(object):

    def __init__(self, spaceV, velocV, motion, kind_):
        self.posi = spaceV
        self.velo = velocV
        self.assignMotion(motion)
        self.kind = {PROTON: Proton,
                     ELECTRON: Electron,
                     NEUTRON: Neutron,
                     SUN: Sun,
                     MERCURY: Mercury,
                     VENUS: Venus,
                     EARTH: Earth,
                     MARS: Mars,
                     JUPITER: Jupiter}[kind_]()
        self._t = 0.0

    def move(self,dt):
        self.posi, self.velo = self.motion(self.posi, self.velo, dt)
        self.evolve(dt)

    def kineticE(self):
        return 0.5 * self.kind.mass * self.velo.mag() * self.velo.mag()

    def evolve(self, dt):
        # Evolve the particle clock
        self._t += dt

        # Particle decay
        if self.kind.type == NEUTRON and self._t > self.kind.decay_time:
            self.kind = Proton()
            self._t = 0.0

    def assignMotion(self, newMotion):
        self.motion = newMotion

    def chunk(self, n, iterable):
        iterator = iter(iterable)
        while True:
            try:
                yield [next(iterator) for _ in range(n)]
            except StopIteration:
                return

    def gravity(self, other, dt):
        G = 6.673e-11 * 1e16 #[m]3 [kg]-1 [s]-2 --> 1e18 [10^6km]3 [10^30kg]-1 [y/10]-2
        # Earth-Sun distance ~150 [10^6km]
        # Sun mass ~2 [10^30kg]
        # Time dt ~1/100 [y/10]
        # Earth mass ~6e-6 [10^30kg]
        # Earth speed ~90 [10^6km] [y/10]-1
        dist = other.posi - self.posi
        Force = G * self.kind.mass * other.kind.mass / (dist.mag() * dist.mag())

        if self.kind.mass != 0.0 and other.kind.mass !=0:
            self.velo  += (dist / dist.mag() * Force * dt) / self.kind.mass
            other.velo -= (dist / dist.mag() * Force * dt) / other.kind.mass

        return other

    def collide(self, other):
        if self.kind.mass != 0.0 and other.kind.mass and self.kind.radius + other.kind.radius > (self.posi - other.posi).mag():
            tolerance = 1e-3
            dist = other.posi - self.posi
            newDist = dist / dist.mag() * (self.kind.radius + other.kind.radius) * (1 + tolerance)
            self.posi  -= (newDist - dist) / (self.kind.radius + other.kind.radius) * self.kind.radius
            other.posi += (newDist - dist) / (self.kind.radius + other.kind.radius) * other.kind.radius

            P1 = self.kind.mass * self.velo
            P2 = other.kind.mass * other.velo

            Ptot = P1 + P2
            Ptot_par = Ptot.project(dist)
            Ptot_ort = Ptot - Ptot_par

            P1_par = P1.project(dist)
            P2_par = P2.project(dist)

            P1_ort = P1.project(Ptot_ort)
            P2_ort = P2.project(Ptot_ort)

            dP_mm = 2.0 * self.kind.mass * other.kind.mass / (self.kind.mass + other.kind.mass) * (P1_par.mag() / self.kind.mass + P2_par.mag() / other.kind.mass)
            dP_mp = 2.0 * self.kind.mass * other.kind.mass / (self.kind.mass + other.kind.mass) * (P1_par.mag() / self.kind.mass - P2_par.mag() / other.kind.mass)

            if P1_par.scalarProd(P2_par) > 0.0:
                if self.velo.project(dist).mag() > other.velo.project(dist).mag():
                    P1_par -= P1_par / P1_par.mag() * dP_mp
                    P2_par += P2_par / P2_par.mag() * dP_mp
                else:
                    P1_par += P1_par / P1_par.mag() * (-dP_mp)
                    P2_par -= P2_par / P2_par.mag() * (-dP_mp)
            else:
#                if P1_par.mag() == 0.0 and P2_par.mag() < P2_par.tolerance:
#                    P1_par, P2_par = P2_par / P2_par.mag() * dP_mm, P2_par - P2_par / P2_par.mag() * dP_mm
#                elif P1_par.mag() < P1_par.tolerance and P2_par.mag() == 0.0:
#                    P1_par, P2_par = P1_par - P1_par / P1_par.mag() * dP_mm, P1_par / P1_par.mag() * dP_mm
                if P1_par.mag() < P1_par.tolerance:
                    P1_par, P2_par = P2_par / P2_par.mag() * dP_mm, P2_par - P2_par / P2_par.mag() * dP_mm
                elif P2_par.mag() < P2_par.tolerance:
                    P1_par, P2_par = P1_par - P1_par / P1_par.mag() * dP_mm, P1_par / P1_par.mag() * dP_mm
                else:
                    P1_par, P2_par = P1_par - P1_par / P1_par.mag() * dP_mm, P2_par - P2_par / P2_par.mag() * dP_mm

            self.velo  = (P1_ort + P1_par) / self.kind.mass
            other.velo = (P2_ort + P2_par) / other.kind.mass

        return other

    # In python we can pass operator as parameters of a function
    def check_bound(self, newPosi, bound, op, r, indx):
        if op(newPosi,bound):
            self.posi[indx] = bound + r
            self.velo[indx] = - self.velo[indx]
            beep()

    def boundary_bounce(self, bounds):
        self.bounds = bounds
        for i,(l,h) in enumerate(self.chunk(2,bounds)):
            self.check_bound(self.posi[i] + self.kind.radius, h, gt, -self.kind.radius, i)
            self.check_bound(self.posi[i] - self.kind.radius, l, lt,  self.kind.radius, i)


class Universe(object):

    def __init__(self, width, height, DisplayType, DELTAT, COLLISION, FORCEFIELD):
        self.particles   = []
        self.width, self.height = width, height
        self.DisplayType = DisplayType
        self.dt          = DELTAT
        self.collision   = COLLISION
        self.forceField  = FORCEFIELD

    def add_particle(self, new_particle):
        self.particles.append(new_particle)

    def go(self):
        width, height, dt = self.width, self.height, self.dt
        del self.width, self.height, self.dt
        self.display = self.DisplayType(width, height, self, dt)

    def checkGravity(self, indx, dt):
        particleA = self.particles[indx]

        for i,particleB in enumerate(self.particles):
            if i > indx:
                self.particles[i] = particleA.gravity(particleB, dt)

    def checkCollisions(self, indx):
        particleA = self.particles[indx]

        for i,particleB in enumerate(self.particles):
            if i > indx:
                self.particles[i] = particleA.collide(particleB)

    def evolve(self, dt, bounds):
        V4D    = Vector('abcd')
        worldB = V4D(*bounds)

        for i,particle in enumerate(self.particles):
            particle.move(dt)

            particle.boundary_bounce(worldB)

            if self.forceField == GRAVITY:
                self.checkGravity(i, dt)

            if self.collision == COLLIDE:
                self.checkCollisions(i)

    def draw(self, display):
        for p in self.particles:
            display.draw_circle(p.kind.color, p.kind.radius, *p.posi)

if sys.argv[1] == 'pyg':
    from PygletDisplay import PygletDisplay as Display
if sys.argv[1] == 'tk':
    from TkinterDisplay import TkinterDisplay as Display

width  = 1200
height = 700
DELTAT = 1/60.0
InitialSpace = []
InitialVelocity = []


#########################################
# To test the performance with cProfile #
#########################################
#from MyDisplay import MyDisplay
#universe = Universe(width, height, MyDisplay, DELTAT, NOCOLLIDE, NOGRAVITY)
universe = Universe(width, height, Display, DELTAT, COLLIDE, GRAVITY)

# Instantiate and initialize particles for the Universe
InitialSpace.append(Vector2D( 100.0, height/2.0))
InitialSpace.append(Vector2D( 200.0, height/2.0))
InitialSpace.append(Vector2D( 800.0, height/2.0))
InitialSpace.append(Vector2D( 900.0, height/2.0))
InitialSpace.append(Vector2D(1000.0, height/2.0))
InitialSpace.append(Vector2D(1100.0, height/2.0))

InitialVelocity.append(Vector2D( 0.0,  0.0))
InitialVelocity.append(Vector2D(50.0, 50.0))
InitialVelocity.append(Vector2D(60.0, 60.0))
InitialVelocity.append(Vector2D(70.0, 70.0))
InitialVelocity.append(Vector2D(80.0, 10.0))
InitialVelocity.append(Vector2D(90.0, 90.0))

#universe.add_particle(Particle(InitialSpace[0], InitialVelocity[0], makeLinear(0.0),                  PROTON))
#universe.add_particle(Particle(InitialSpace[1], InitialVelocity[1], makeConstCircular(2.0*pi/4.0,RH), ELECTRON))
#universe.add_particle(Particle(InitialSpace[2], InitialVelocity[2], makeConstCircular(2.0*pi/8.0,LH), NEUTRON))
#universe.add_particle(Particle(InitialSpace[3], InitialVelocity[3], makeConstCircular(2.0*pi/5.0,LH), PROTON))
#universe.add_particle(Particle(InitialSpace[4], InitialVelocity[4], makeLinear(4.0),                  NEUTRON))
#universe.add_particle(Particle(InitialSpace[5], InitialVelocity[5], makeLinear(0.0),                  ELECTRON))

InitialSpace.append(Vector2D(300.0, height/2.0))
InitialSpace.append(Vector2D(300.0 +  58.0, height/2.0))
InitialSpace.append(Vector2D(300.0 + 108.0, height/2.0))
InitialSpace.append(Vector2D(300.0 + 150.0, height/2.0))
InitialSpace.append(Vector2D(300.0 + 250.0, height/2.0))
InitialSpace.append(Vector2D(300.0 + 850.0, height/2.0))

InitialVelocity.append(Vector2D( 0.0,  0.0))
InitialVelocity.append(Vector2D( 0.0,148.0))
InitialVelocity.append(Vector2D( 0.0,105.0))
InitialVelocity.append(Vector2D( 0.0, 90.0))
InitialVelocity.append(Vector2D( 0.0, 72.0))
InitialVelocity.append(Vector2D( 0.0, 41.0))

universe.add_particle(Particle(InitialSpace[6],  InitialVelocity[6],  makeLinear(0.0), SUN))
universe.add_particle(Particle(InitialSpace[7],  InitialVelocity[7],  makeLinear(0.0), MERCURY))
universe.add_particle(Particle(InitialSpace[8],  InitialVelocity[8],  makeLinear(0.0), VENUS))
universe.add_particle(Particle(InitialSpace[9],  InitialVelocity[9],  makeLinear(0.0), EARTH))
universe.add_particle(Particle(InitialSpace[10], InitialVelocity[10], makeLinear(0.0), MARS))
universe.add_particle(Particle(InitialSpace[11], InitialVelocity[11], makeLinear(0.0), JUPITER))

universe.go()
#cProfile.run('universe.go()','Part.stats')


#############################################################
# To check if the parameters passed to a method are correct #
#############################################################

# Here we mock a class
myMock = Mock()
u = Universe(None, None, myMock, None)
u.add_particle(Particle(InitialSpace[0], InitialVelocity[0], makeLinear(0.0),            PROTON))
u.add_particle(Particle(InitialSpace[1], InitialVelocity[1], makeConstCircular(2*pi,RH), ELECTRON))
u.draw(myMock)

assert ('draw_circle', (25.0, width / 2.0 , height / 2.0), {}) in myMock.method_calls
assert ('draw_circle', (15.0, width / 2.0 , height / 2.0), {}) in myMock.method_calls
assert len(myMock.method_calls) == 2

# Here we mock a method
Universe.evolve = Mock()
print(u.evolve.called)
print(u.evolve.call_args)
