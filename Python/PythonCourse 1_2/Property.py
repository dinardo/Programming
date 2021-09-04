# For python we have to inherti from builtin classes to have a "New style" class

class Rectangle(object):
    def __init__(self, w, h):
        self.w = w
        self.h = h

    @property
    def a(self):
        return self.w * self.h

    @a.setter
    def a(self, new_value):
        self.w = float(new_value) / self.h


r = Rectangle(2,3)
assert r.w == 2 # Rise an exception if not true
assert r.h == 3
assert r.a == 6
r.a = 12
assert r.w == 4

print 'Happy'
