# This is also an example of laziness
def TestGen(start,step):
    n = start
    defaultStep = step
    while True:
        try:
            newStep = yield n
        except Reset:
            step = defaultStep
        if newStep != None:
            step = newStep
        n += step

class Reset(Exception):
    pass

a = TestGen(1,2)
print a.next()
print a.send(1)
a.throw(Reset)
print a.next()

#class TestGen:
#    def __init__(self):
#        self._current = 0

#    def next(self):
#        if self._current > 2:
#            raise StopIteration
#        self._current += 1
#        return self._current

#    def __iter__(self):
#        return self
