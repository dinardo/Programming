import pyglet
from math import sin, cos, pi

from Colors import Colors

class PygletDisplay(object):

    def __init__(self, width, height, universe, DELTAT):
        self.universe = universe
        self.window = pyglet.window.Window(width, height)
        self.fps_display = pyglet.window.FPSDisplay(self.window)
        self.window.event(self.on_draw)
        pyglet.clock.schedule_interval(self.update, DELTAT)
        pyglet.app.run()

    def update(self, dt):
        self.universe.evolve(dt, self.bounds)

    def on_draw(self):
        self.window.clear()
        self.universe.draw(self)
        self.fps_display.draw()

    def draw_circle(self, color, radius, x, y):
        def circle_vertices():
            delta_angle = 2*pi / 20
            angle = 0
            while angle < 2*pi:
                yield x + radius * cos(angle)
                yield y + radius * sin(angle)
                angle += delta_angle

        pyglet.gl.glColor3f(*color.getRGB1s())
        vertex_order = range(20)
        pyglet.graphics.draw(20, pyglet.gl.GL_LINE_LOOP,
                             ('v2f', tuple(circle_vertices())))

    @property
    def bounds(self):
        return (0, self.window.width, 0, self.window.height)
