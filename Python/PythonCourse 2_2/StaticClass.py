class C(object):

    def normal(*args):
        return args

    @staticmethod
    def static(*args):
        return args

    @classmethod
    def class_(*args):
        return args

print C().normal()
print C.normal()
print C.static()
