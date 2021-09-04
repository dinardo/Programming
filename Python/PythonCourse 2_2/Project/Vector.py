# This is a factory of classes
#from __future__ import division
from math import sqrt, sin, cos

def Vector(compNames):

    myDict = {name:pos for (pos,name) in enumerate(compNames)}

    class Vector(object):

        def __init__(self, *compVals):
            if len(compNames) != len(compVals):
                raise TypeError('Parameters not of right lenght')
            self._data = list(compVals)
            self.tolerance = 1e-4

        def __getitem__(self, i):
            return self._data[i]

        def __setitem__(self, i, val):
            self._data[i] = val

        def __add__(self, v):
            return Vector(*[self._data[i] + v[i] for i in range(len(self._data))])

        def __sub__(self, v):
            return Vector(*[self._data[i] - v[i] for i in range(len(self._data))])

        def __truediv__(self, s):
            if type(s) == Vector:
                raise TypeError('You can not multiply two vectors')
            elif s == 0.0:
                raise ZeroDivisionError('You can not divide by zero')
            else:
                return Vector(*[self._data[i]/s for i in range(len(self._data))])

        def __mul__(self, s):
            if type(s) == Vector:
                raise TypeError('You can not multiply two vectors')
            else:
                return Vector(*[self._data[i]*s for i in range(len(self._data))])

        __rmul__ = __mul__

        def __eq__(self, v):
            for i,a in enumerate(self._data):
                if (a != v[i]):
                    return False
            return True

        def __str__(self):
            return '<vector: %s>' %self._data

        def mag(self):
            m = 0.0
            for i,a in enumerate(self._data):
                m += a*a
            return sqrt(m)

        def scalarProd(self, v):
            p = 0.0
            for i,a in enumerate(self._data):
                p += a * v[i]
            return p

        def project(self, v):
            if v.mag() > self.tolerance:
                return v * self.scalarProd(v) / (v.mag() * v.mag())
            else:
                return v * 0.0

        # Only implemented for 2D vectors
        def rotate(self, a):
            self.x, self.y = cos(a)*self.x + sin(a)*self.y, -sin(a)*self.x + cos(a)*self.y

    def make_accessors(pos):
        def get(self):
            return self._data[pos]
        def set(self, value):
            self._data[pos] = value
        return get, set

    for n,p in myDict.items():
        setattr(Vector, n, property(*make_accessors(p)))
        # The asterisc in front of make_accessors split the arguments in multiple
        # Think of it as: Vector.n = property(get,set)

    return Vector

Vector2D = Vector('xy')
Vector3D = Vector('xyz')
