class NotFound(Exception):
    pass

def binary_search(search, ordSet, start=None, stop=None):
    if start == None and stop == None:
        start = 0
        stop = len(ordSet)-1

    where = (stop+start)/2
    if start > stop:
        return None
    else:
        val = ordSet[where]
        if search == val:
            return where
        else:
            if search < val:
                if where == stop:
                    where -= 1
                return binary_search(search, ordSet, start, where)
            else:
                if where == start:
                    where += 1
                return binary_search(search, ordSet, where, stop)

#a = range(10)
#print binary_search(9,a)
#print map(binary_search,a,[a]*10)

class AccessCountingListS(list):
    def __init__(self,*args):
        list.__init__(self,*args)
        self.reset_counters()

    def __getitem__(self, indx):
        self.nread += 1
        return list.__getitem__(self,indx)

    def __setitem__(self, indx, val):
        self.nwrite += 1
        return list.__setitem__(self,indx, val)

    @property
    def reads(self):
        return self.nread

    @property
    def writes(self):
        return self.nwrite

    def reset_counters(self):
        self.nread = 0
        self.nwrite = 0

AccessCountingListA = 0
