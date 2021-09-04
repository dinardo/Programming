import itertools

# Version with generator functions
def myIterGen(iterable, start=0):
    for it in iterable:
        yield start, it
        start += 1

# Version with itertools module
def myIterIter(iterable, start=0):
    return itertools.izip(itertools.count(start), iterable)

# Version with class
class class_enumerate:

    def __init__(self, iterable, start=0):
        self._iterator = iter(iterable)
        self._n = start-1

    def next(self):
        self._n += 1
        return self._n, self._iterator.next()

    def __iter__(self):
        return self


# We are implementing a "conversation" from ouside world to our function myCount
class RestartCount(Exception):
    pass

# My version of the itertools.counter
def myCount(start=0,step=1):
    n = start
    original_step = step
    while True:
        try:
            new_step = yield n
            n += new_step
        except TypeError:
            n += step
        except RestartCount:
            n = start
            step = original_step
        else:
            step = new_step

c = myCount(10,2)
print c.next()
print c.send(10)
print c.next()
print c.throw(RestartCount) # yield rise the RestartCount exception
print c.next()
