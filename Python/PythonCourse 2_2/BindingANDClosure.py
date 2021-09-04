# Wrong lexical closure
def wfoo(n):
    r = []
    for i in xrange(n):
        def closure():
            return i
        r.append(closure)
    return r
# i is re-binded at every time but is the same i for every closure

for fn in wfoo(5):
    print fn()



# Correct lexical closure
def cfoo(n):
    r = []
    for i in xrange(n):
        def make_closure(i):
            def closure():
                return i
            return closure
        r.append(make_closure(i))
    return r
# There is a new i for every closure

for fn in cfoo(5):
    print fn()
