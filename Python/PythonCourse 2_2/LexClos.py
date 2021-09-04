def make_adder(n):
    print locals().keys()
    global a
    a = n
    def adder(b):
        return a + b
    return adder

add3 = make_adder(3)
add4 = make_adder(4)

print add3(10)
print add4(10)
