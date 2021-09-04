from math import sin
from operator import neg, pos

from Vector import Vector2D

LH, RH = [object() for _ in range(2)]

def makeConstCircular(step, HAND):
    if HAND == LH:
        op = neg
    elif HAND == RH:
        op = pos

    def ConstCircular(posi, velo, dt):
        velo.rotate(step * op(dt))
        return posi + velo * dt, velo
    return ConstCircular

def makeLinear(step):
    t = [0]

    def Linear(posi, velo, dt):
        t[0] += dt
        return posi + velo * (1+sin(step*t[0])) * dt, velo
    return Linear
