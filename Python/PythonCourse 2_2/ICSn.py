class foo:

    a = 3

f = foo()


dir(foo)
dir(f)
foo.__dict__
f.__dict__

f.a = 10
f.b = 100

dir(foo)
dir(f)
foo.__dict__
f.__dict__
