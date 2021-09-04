from PIL import Image
import numpy as np

NCOLS     = 400
NROWS     = 192
NROW_CORE =   8
HITPERCOL =   1

def makeFrame(groupNumber,array):
    for col in range(NCOLS):
        for i in range(HITPERCOL):
            row  = (NROW_CORE * col + i * NROWS//HITPERCOL)%NROWS
            row += groupNumber
            row %= NROWS
            if array[row,col] == 0:
                array[row,col] = 255 # White color
            else:
                array[row,col] = 0 # Black color

    return array

def main():
    frames = []
    array = np.full((NROWS, NCOLS),150) # Gray color

    for i in range(NROWS//HITPERCOL):
        array = makeFrame(i,array)
        frames.append(Image.fromarray(np.uint8(array)))

    frames[NROWS//HITPERCOL-1].save('frame.png')
    frames[0].save('animation.gif', format='GIF', append_images=frames[1:], save_all=True, duration=10, loop=0)

if __name__ == '__main__':
    main()
