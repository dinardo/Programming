import tkinter as tk

from Colors import Colors

class TkinterDisplay(object):

    def __init__(self, width, height, universe, DELTAT):
        master = tk.Tk()
        self.universe = universe
        self.w = tk.Canvas(master, width=width, height=height, bg='black')
        self.w.pack()
        self.w.after(500, self.update, DELTAT)
        tk.mainloop()

    def update(self, dt):
        # Clear old ovals
        self.w.delete(tk.ALL)
        self.universe.evolve(dt, self.bounds)
        self.universe.draw(self)

        self.w.update()
        self.w.after(17, self.update, dt)

    def draw_circle(self, color, radius, x, y):
        height = self.w.winfo_height()
        self.w.create_oval(x-radius, height-(y-radius),
                           x+radius, height-(y+radius), outline=color.getRGBNames())

    @property
    def bounds(self):
        return (0, self.w.winfo_width(), 0, self.w.winfo_height())
