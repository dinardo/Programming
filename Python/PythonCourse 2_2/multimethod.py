import inspect

def multimethod(*arg_types):

    def add_method_to_generic_function(method):
        # Disallow adding methods whose argument count does not match
        # the number of types specified in the decorator.
        num_types, num_args = len(inspect.getargspec(method).args), len(arg_types)
        if num_types != num_args:
            raise TypeError("multimethod given %d types and a function with %d arguments. These numbers should be equal." %
                            (num_types, num_args))

        # Make the method aware of its arguments types (useful in
        # cache clearing).
        method._arg_types = arg_types

        name = method.__name__
        # If we do not have a generic function with this name yet,
        # create it.
        if name not in multimethod._generic_functions:
            # Storage space for all the methods in this generic
            # function.
            this_functions_methods = {}

            # The generic function has to find the correct method and
            # call it.
            def new_generic_function(*args):
                argument_types = tuple(map(type,args))
                try:
                    return this_functions_methods[argument_types](*args)
                except KeyError:
                    found = _find_and_cache_matching_method_via_inheritance(argument_types, this_functions_methods)
                    if found:
                        return found(*args)
                    
                    raise TypeError('No matching method found for argument types %s' %
                                    argument_types)

            # The generic function is a closure over its methods
            # table. Allow easy access to the methods table from the
            # outside, to allow adding new methods to it.
            new_generic_function._methods = this_functions_methods
            # Make the 
            new_generic_function.__name__ = name
            # Add the 
            multimethod._generic_functions[name] = new_generic_function

        # Add the new method to the generic function with the same
        # name.
        multimethod._generic_functions[name]._methods[arg_types] = method

        # The method table may contain entries for indirect matches
        # (where subtypes match). If the new method may be a better
        # match than some of the entries in the method table. Make
        # sure it is not shadowed by such outdated entries.
        _clear_cache(multimethod._generic_functions[name]._methods)
        
        return multimethod._generic_functions[name]

    return add_method_to_generic_function

multimethod._generic_functions = {}


def _find_and_cache_matching_method_via_inheritance(argument_types, method_table):
    found = _find_best_matching_signature(
        argument_types,
        _find_all_matching_methods(argument_types, method_table))

    if found:
        found_method = method_table[found]
        method_table[argument_types] = found_method
        return found_method

def _find_all_matching_methods(argument_types, method_table):
    """Finds all methods in METHOD_TABLE which match ARGUMENT_TYPES."""
    return [method_types for method_types in method_table
            if all(map(issubclass, argument_types, method_types))]

def _find_best_matching_signature(argument_types, collection_of_matching_types):
    """Given a set of argument types, and a collection of matching
    supertypes, returns the member of the collection which is the best
    match."""

    # The distance between two types is their separation in the Method
    # Resolution Order of the subtype.

    # For any pair of signatures (sequences of types), the distance
    # between the signatures is made up of the sequence of distances
    # between the types in corresponding positions is the signatures.

    # This sequence is then treated as the digits in a number
    # representing the distance: the first number is the most
    # significant digit, the second is the next most significant,
    # etc. In short, they are compared lexicographically.

    if not collection_of_matching_types:
        return None

    def type_distance(subtype, supertype):
        "Returns how far SUBTYPE is away from SUPERTYPE in MRO."
        return subtype.__mro__.index(supertype)
    
    # The distance between two signatures is the sequence
    #    (distance(sub1,super1), distance(sub2, super2), ... )
    def signature_distance(declared_method_types):
        return map(type_distance, argument_types, declared_method_types)

    # The distances are compared lexicographically: the most
    # signifcant values are those earlier in the sequence.
    return tuple(min(collection_of_matching_types, key=signature_distance))

def _clear_cache(method_table):
    """Removes all entries which are not direct signature matches,
    from METHOD_TABLE."""
    # Not using iterkeys, because modifying dict while iterating over
    # it causes problems.
    for signature, method in method_table.items():
        if signature != method._arg_types:
            del method_table[signature]
