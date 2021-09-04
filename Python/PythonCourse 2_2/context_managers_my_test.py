def testSample():
    assert True

def test_range_returns_list():
    assert type(range(2)) is list

def test_range_1_arg_return_consecutive_ints():
    assert range(4) == [0,1,2,3]
    assert range(2) == [0,1]

def test_2_arg_should_start_at_first_end_before_second():
    assert range(2,5) == [2,3,4]
    assert range(3,7) == [3,4,5,6]

from math import sqrt
from pytest import raises
def test_sqrt_without_integers1():
    try:
        sqrt('not integer')
    except TypeError:
        assert True
    except Exception:
        assert False
    else:
        assert False

def check_exception(ExpectedExceptionType,callable,*args,**kwds):
    try:
        callable(*args,**kwds)
    except ExpectedExceptionType:
        assert True
    except Exception:
        assert False
    else:
        assert False

# Example of "context manager"
class myRaises(object):

    def __init__(self,ExpectedExceptionType):
        self._Exception_spec = ExpectedExceptionType

    def __enter__(self):
        pass

    # Passaggio dei parametri e` fisso
    def __exit__(self,ExType,ExInst,TraceBack):
        if ExType is self._Exception_spec or ExType in self._Exception_spec:
            assert True
        else:
            assert False
        return True

# Easy way to construct "context managers"
from contextlib import contextmanager
@contextmanager
def myRaises2(E_spec):
    try:
        # Here code called by the "contextmanager" decorator when "with" is first called
        yield # Here the output if needed
        # Here code called by the "contextmanager" decorator when "with" ends
    except E_spec:
        assert True
    except Exception:
        assert False
    else:
        assert False

def test_sqrt_without_integers2():
    # They are all equivalent
    # pytest.raises is a "context manager"
    check_exception(TypeError,sqrt,'Not integer')
    
    raises(TypeError,sqrt,'Not integer')

    raises(TypeError,"sqrt('Not integer')")

    with raises(TypeError):
        sqrt('Not integer')

    with myRaises(TypeError):
        sqrt('Not integer')

    with myRaises2(TypeError):
        sqrt('Not integer')
