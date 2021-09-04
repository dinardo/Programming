# Meaning of binding:

l1 = [1, 2, 3]
l2 = l1
print l2 == l1, l2 is l1
l2 += [4, 5]
print l1, l2
print l1 == l2, l2 is l1 

i1 = 0
i2 = i1
print i2 == i1, i2 is i1
i2 += 1
print i1, i2
print i1 == i2, i2 is i1 

# += creates a new memory because the tuple is immutable
t1 = (1, 2, 3)
t2 = t1
print t2 == t1, t2 is t1
t2 += (4, 5)
print t1, t2
print t1 == t2, t2 is t1
