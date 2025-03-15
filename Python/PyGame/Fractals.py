#!/usr/bin/python3

from math import sqrt

############################
# Import the pygame module #
############################
import pygame

# Import pygame.locals for easier access to key coordinates
from pygame.locals import (K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ESCAPE, KEYDOWN, QUIT)

#################
# Fractal curve #
#################
def Koch(depth, screen, color, width, xs, ys, xe, ye):
    side      = sqrt((xe-xs)**2 + (ye-ys)**2) / 3
    height    = side * sqrt(3)/2

    if ye-ys != 0:
        direction = abs((xe-xs)/(ye-ys))
        dx        = height / sqrt(1 + direction**2) * (1 if ((xe > xs) and (ye >= ys)) or ((xe < xs) and (ye > ys)) else -1)
        dy        = abs(dx) * direction * (-1 if ((xe > xs) and (ye >= ys)) or ((xe > xs) and (ye < ys)) else 1)
    else:
        dx        = 0
        dy        = height * (-1 if ((xe > xs) and (ye >= ys)) or ((xe > xs) and (ye < ys)) else 1)

    xs_ = xs
    ys_ = ys
    xe_ = xs + (xe-xs)/3
    ye_ = ys + (ye-ys)/3

    if depth != 0:
        Koch(depth - 1, screen, color, width, xs_, ys_, xe_, ye_)
    else:
        pygame.draw.line(screen, color, (xs_, ys_), (xe_, ye_), width)

    xs_ = xs + (xe-xs)/3
    ys_ = ys + (ye-ys)/3
    xe_ = (xs+xe)/2 + dx
    ye_ = (ys+ye)/2 + dy

    if depth != 0:
        Koch(depth - 1, screen, color, width, xs_, ys_, xe_, ye_)
    else:
        pygame.draw.line(screen, color, (xs_, ys_), (xe_, ye_), width)

    xs_ = (xs+xe)/2 + dx
    ys_ = (ys+ye)/2 + dy
    xe_ = xs + (xe-xs)*2/3
    ye_ = ys + (ye-ys)*2/3

    if depth != 0:
        Koch(depth - 1, screen, color, width, xs_, ys_, xe_, ye_)
    else:
        pygame.draw.line(screen, color, (xs_, ys_), (xe_, ye_), width)

    xs_ = xs + (xe-xs)*2/3
    ys_ = ys + (ye-ys)*2/3
    xe_ = xe
    ye_ = ye

    if depth != 0:
        Koch(depth - 1, screen, color, width, xs_, ys_, xe_, ye_)
    else:
        pygame.draw.line(screen, color, (xs_, ys_), (xe_, ye_), width)


####################################################
# Define constants for the screen width and height #
####################################################
SCREEN_WIDTH  = 1200
SCREEN_HEIGHT = 1000

pygame.init()

############################
# Create the screen object #
############################
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('Fractals')

##############
# Line color #
##############
RED   = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE  = (0, 0, 255)

#################
# Draw Fractals #
#################
Koch(5, screen, BLUE, 1, 100, SCREEN_HEIGHT/2, SCREEN_WIDTH-100, SCREEN_HEIGHT/2)
Koch(5, screen, RED,  1, SCREEN_WIDTH-100, SCREEN_HEIGHT/2, 100, SCREEN_HEIGHT/2)
pygame.display.update()

#################
# Infinite loop #
#################
running = True
while running:
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
