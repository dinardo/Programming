from time import time


# Usage: myTimer(Fibo,10)
#def myTimer(fun, *args, **kwds):
#    start = time()
#    result = fun(*args, **kwds)
#    end = time()
#    return end - start, result


# Usage: myTimer(lambda:Fibo(10))
def myTimer(fun):
    start = time()
    result = fun()
    end = time()
    return end - start, result


# Recursive Fibonacci
def Fibo(n):
   if n <= 2:
       return 1
   else:
       return Fibo(n-1) + Fibo(n-2)


# Non recursive Fibonacci
# def Fibo(n):
#     c,p = 1,1
#     while n > 1:
#         c, p = c+p, c
#         n -= 1
#     return c


# Memoizer with lexical closure 
def memo(fun):
    cache = {}

    def mfun(*args):
        try:
            return cache[args]
        except KeyError:
            cache[args] = fun(*args)
            return cache[args]

    return mfun


# Memoizer with class
#class memo:
#    def __init__(self, fun):
#        self._cache = {}
#        self._localFun = fun

#    def __call__(self, *args):
#        try:
#            return self._cache[args]
#        except KeyError:
#            self._cache[args] = self._localFun(*args)
#            return self._cache[args]
