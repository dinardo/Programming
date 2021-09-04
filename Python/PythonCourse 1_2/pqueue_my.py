class EmptyQueue(Exception):
    pass

class priority_queue():
    def __init__(self):
        self._theQ = [[],[],[],[],[],];

    def add(self, what, prority = 2):
        if not type(prority) == int:
            raise TypeError

        if not (0 <= prority <= 4):
            raise ValueError

        self._theQ[prority].insert(0,what)

    def pop(self):
        for q in self._theQ:
            if q:
                return q.pop()
        raise EmptyQueue

    def __len__(self):
        return sum(map(len,self._theQ))
