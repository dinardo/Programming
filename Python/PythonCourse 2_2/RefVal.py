a = 1
def foo(a):
    a = 2
foo(a)
print a


b = [1]
def bar(b):
    b[0] = 2
bar(b)
print b[0]
