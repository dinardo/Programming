from libc.math cimport sqrt

cdef class Vector2D(object):

    cdef object _data

    def __init__(self,x,y):

        self._data = [x,y]

    def __getitem__(self,i):
        return self._data[i]

    def __setitem__(self,i,val):
        self._data[i] = val

    def __add__(self,v):
        return Vector2D(*[self._data[i] + v[i] for i in xrange(len(self._data))])

    def __mul__(self,s):
        if type(s) == Vector2D:
            raise TypeError('You can not multiply two vectors')
        else:
            return Vector2D(self._data[0]*s, self._data[1]*s)

    __rmul__ = __mul__

    def __str__(self):
        return '<vector: %s>' %self._data

    cpdef double mag(Vector2D self):
        cdef double x
        x = self._data[0]
        cdef double y
        y = self._data[1]
        return sqrt(x*x+y*y)

    @property
    def x(self):
        return self._data[0]
    @x.setter
    def x(self,v):
        self._data[0] = v

    @property
    def y(self):
        return self._data[1]
    @y.setter
    def y(self,v):
        self._data[1] = v
