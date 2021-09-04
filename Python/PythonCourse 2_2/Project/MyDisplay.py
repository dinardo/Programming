from time import time

class MyDisplay(object):
    
    def __init__(self, width, height, universe, DELTAT):
        self.universe = universe
        self.width = width
        self.height = height

        self.update(1/60.0)

    def update(self, dt):
        t0 = time()
        while time()-t0 < 30:
            self.universe.evolve(dt, self.bounds)

    def on_draw(self):
        pass

    def draw_circle(self, color, radius, x, y):
        pass

    @property
    def bounds(self):
        return (0, self.width, 0, self.height)
