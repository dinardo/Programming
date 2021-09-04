class cls:
    a = 1

class clsNew:
    c = 1

inst = cls()

print dir(cls)
print cls.__dict__
print '------'
print dir(inst)
print inst.__dict__

inst.a = 22
inst.b = 333

print '######'

print dir(cls)
print cls.__dict__ # Class attributes !
print '------'
print dir(inst)
print inst.__dict__ # Instance attributes !

print '######'

# Instance attributes are repeated when we make class change:
inst.__class__ = clsNew
print dir(clsNew)
print clsNew.__dict__ # Class attributes !
print '------'
print dir(inst)
print inst.__dict__
