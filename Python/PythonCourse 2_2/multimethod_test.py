from pytest import raises
from multimethod import multimethod

def test_simple_multiple_dispatch_should_work():

    class Rock(object):     pass
    class Paper(object):    pass
    class Scissors(object): pass

    @multimethod(Rock, Paper)
    def winner(l,r): return 'right'

    @multimethod(Paper, Scissors)
    def winner(l,r): return 'right'

    @multimethod(Scissors, Rock)
    def winner(l,r): return 'right'

    @multimethod(Paper, Rock)
    def winner(l,r): return 'left'

    @multimethod(Scissors, Paper)
    def winner(l,r): return 'left'

    @multimethod(Rock, Scissors)
    def winner(l,r): return 'left'

    rock, paper, scissors = Rock(), Paper(), Scissors()

    assert winner(rock,     paper)     == 'right'
    assert winner(paper,    scissors)  == 'right'
    assert winner(scissors, rock)      == 'right'
    assert winner(paper,    rock)      == 'left'
    assert winner(scissors, paper)     == 'left'
    assert winner(rock,     scissors)  == 'left'

def test_multimethod_should_raise_TypeError_if_given_function_with_wrong_number_of_parameters():
    with raises(TypeError):
        @multimethod(int)
        def foo(one, two):
            pass

    with raises(TypeError):
        @multimethod(float, str)
        def bar(one):
            pass

def test_multimethod_should_raise_TypeError_if_no_method_matching_argument_types_exists():
    class A(object): pass

    @multimethod(A,int)
    def baz(a,b): pass

    with raises(TypeError):
        baz(1,2)
        
def test_multimethods_should_respect_inheritance():

    class A(object): pass
    class B(A):      pass

    class MyInt(int): pass

    @multimethod(A, int)
    def zot(a,b): return A,int

    assert zot(B(), MyInt()) == (A, int)

    # After adding a better matching method to the generic function,
    # the new method should be found in response to the same request
    @multimethod(B,int)
    def zot(a,b): return (B, int)

    assert zot(B(), MyInt()) == (B, int)


# To Do

# Remove generic function

# Docstrings ?

# Removal of Methods ?

# Call next method ?
