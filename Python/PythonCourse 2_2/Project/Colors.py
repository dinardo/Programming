class Colors(object):
    
    def __init__(self):
        self._Cdict = dict({'yellow': (1.0,1.0,0.0),
                            'red':    (1.0,0.0,0.0),
                            'green':  (0.0,1.0,0.0),
                            'blue':   (0.0,0.0,1.0),
                            'white':  (1.0,1.0,1.0),
                            'pink':   (1.0,0.4,0.7),
                            'cyan':   (0.0,1.0,1.0),
                            'gray':   (0.8,0.8,0.8)})

    def setRGB1s(self,color):
        self.color = self._Cdict[self.color]

    def setRGBNames(self,color):
        self.color = color

    def getRGB1s(self):
        return self._Cdict[self.color]

    def getRGBNames(self):
        return self.color
