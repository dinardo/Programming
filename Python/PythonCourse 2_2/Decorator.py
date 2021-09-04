from functools import wraps

def one(_):
    return 1

def inc(n):
    return n+1

def inc_by(n):
    def inc(m):
        return n+m
    return inc

@inc_by(10)
@inc
@one
def hello():
    print "hi"

print hello




def increment_result_by(n):
    def increment(the_basic_func):
        @wrap(the_basic_func) # Copy all data related to the function in the decorator: e.g. the dictionary
        def the_enhanced_func(*args, **kwds):
            a = the_basic_func(*args, **kwds) + n
            print a
            return a
        return the_enhanced_func
    return increment

def report_results(the_basic_func):
    def the_enhanced_func(*args, **kwds):
        a = the_basic_func(*args, **kwds)
        print a
        return a
    return the_enhanced_func

def report_args(the_basic_func):
    def the_enhanced_func(*args, **kwds):
        print args, kwds
        return the_basic_func(*args, **kwds)
    return the_enhanced_func

@increment_result_by(10)
@report_results
@report_args
def my_add(a,b):
    return a+b

test = my_add(1,2)
