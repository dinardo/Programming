def show_args(fun):
    def decoFun(*args, **kwds):
        print args, kwds
        return fun(*args, **kwds)
    return decoFun

# Parametrize the decorator
def inc_result_by(n):
    def decorator(fn):
        def new_fn(*args, **kwds):
            return n + fn(*args, **kwds)
        return new_fn
    return decorator

@inc_result_by(3)
@show_args
def my_func(a,b):
    return a + b
