from pytest import mark, raises
from Vector import Vector, Vector2D, Vector3D

component_access_test_data = (
    "Vector names data data_new".split(),
    ((Vector2D, 'xy',  ( 1,2),   (3,4)),
     (Vector3D, 'xyz', (-2,7,3), (4,3,8)),
     (Vector('abcd'), 'abcd', (8,6,5,2), (1,7,8,7))))

@mark.parametrize(*component_access_test_data)
def test_should_be_able_to_access_vector_components_by_name(Vector, names, data, data_new):
    v = Vector(*data)
    for name, datum in zip(names, data):
        assert getattr(v, name) == datum
    
@mark.parametrize(*component_access_test_data)
def test_should_be_able_to_set_components_by_name(Vector, names, data, data_new):
    v = Vector(*data)
    for name, datum, new_datum in zip(names, data, data_new):
        assert getattr(v, name) != new_datum
        setattr(v, name, new_datum)
        assert getattr(v, name) == new_datum

@mark.parametrize(*component_access_test_data)
def test_should_be_able_to_access_components_by_number(Vector, names, data, data_new):
    v = Vector(*data)
    for dim, datum in enumerate(data):
        assert v[dim] == datum

@mark.parametrize(*component_access_test_data)
def test_should_be_able_to_set_components_by_number(Vector, names, data, data_new):
    v = Vector(*data)
    for dim, (datum, new_datum) in enumerate(zip(data, data_new)):
        assert v[dim] != new_datum
        v[dim] = new_datum
        assert v[dim] == new_datum

vector_binary_op_test_data = (
    "Vector names data_l data_r".split(), 
    ((Vector2D, 'xy',         (1, 2),      (3,4)),
     (Vector3D, 'xyz',        (5.1,6.2,1.0),   (7.3,8.4,2.0)),
     (Vector('xyzt'), 'xyzt', (5.1,2,4,2), (1,3,9,2.8))))

@mark.parametrize(*vector_binary_op_test_data)
def test_should_be_able_to_add_vectors(Vector, names, data_l, data_r):
    v = Vector(*data_l) + Vector(*data_r)
    for name, datum_l, datum_r in zip(names, data_l, data_r):
        assert getattr(v,name) == datum_l + datum_r

@mark.parametrize(*vector_binary_op_test_data)
def test_should_be_able_to_add_vectors_in_place(Vector, names, data_l, data_r):
    v =  Vector(*data_l)
    v += Vector(*data_r)
    for name, datum_l, datum_r in zip(names, data_l, data_r):
        assert getattr(v,name) == datum_l + datum_r

vector_scalar_binary_op_test_data = (
    "Vector names data s".split(),
    ((Vector2D, 'xy', (1, 2), 3),
     (Vector3D, 'xyz', (4, 5,5), 6.3),
     (Vector('pqrst'), 'pqrst', (8,7,6,5,4), 2.5)))

@mark.parametrize(*vector_scalar_binary_op_test_data)
def test_should_be_able_to_multiply_vector_by_scalar_on_right(Vector, names, data, s):
    v = Vector(*data) * s
    for name, datum in zip(names, data):
        assert getattr(v, name) == datum * s

@mark.parametrize(*vector_scalar_binary_op_test_data)
def test_should_be_able_to_multiply_vector_by_scalar_on_left(Vector, names, data, s):
    v = s * Vector(*data) 
    for name, datum in zip(names, data):
        assert getattr(v, name) == datum * s

@mark.parametrize(('vector', 'other'),
                  ((Vector2D(1,2), None),
                   (Vector3D(1,2,3), Vector3D(1,3,4)),
                   (Vector('ab')(1,2), __builtins__)))
def test_Vector_multiplication_should_raise_TypeError_for_inappropriate_type(vector, other):
    with raises(TypeError):
        vector * other
    with raises(TypeError):
        other * vector

@mark.parametrize(("Vector", "data"),
                  ((Vector2D, (1,)),
                   (Vector2D, (1,2,3)),
                   (Vector('abc'), (1,2)),
                   (Vector('abcde'), range(10))))
def test_constructor_should_raise_TypeError_on_wrong_number_of_args(Vector, data):
    with raises(TypeError):
        Vector(*data)

@mark.parametrize("l r result".split(),
                  ((Vector2D(1,2), Vector2D(1,2), True),
                   (Vector2D(1,2), Vector2D(2,1), False),
                   (Vector3D(2,3,5), Vector3D(2,3,5), True),
                   (Vector3D(2,3,5), Vector3D(2,3,4), False),
                   (Vector('abcd')(9,8,7,6), Vector('abcd')(9,8,7,6), True)))
def test_vector_equality_operator_should_work(l,r, result):
    assert (l == r) == result
