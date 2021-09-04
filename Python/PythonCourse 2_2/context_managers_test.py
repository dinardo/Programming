from pytest import mark

################################################################################
# A context manager for timing code. Note: do not use this for serious
# timings: the overhead will be too large when the timed duration is
# very short. For serious timings use the timeit module.

           ############## Test ##############

from time import sleep

@mark.parametrize('seconds_to_sleep', range(1,4))
def test_timer_should_report_approximately_correct_times(seconds_to_sleep):
    with timer() as t:
        sleep(seconds_to_sleep)
    assert abs(t.time - seconds_to_sleep) < 0.01

import sys

from contextlib import contextmanager

      ############## Implementation ##############

from time import time

class Timer_result(object):
    pass

@contextmanager
def timer():
    start = time()
    result = Timer_result()
    yield result
    result.time = time() - start

################################################################################    
################################################################################
################################################################################
# A Context manager for temporarily redirecting stdout, stderr and stdin

           ############## Tests ##############

from StringIO import StringIO

def test_stream_redirect_should_be_able_to_unpack_into_3_separate_values():
    """Checks that the as-target of the with statement can be unpacked
    into the 3 objects. These 3 objects should be the bindings of
    sys.stdout, sys.stderr and sys.stdin which are in effect within
    the with block."""
    stdout_text = 'stdout'
    stderr_text = 'stderr'
    stdin_text  = 'stdin\n'
    with redirected_std_streams(StringIO(), StringIO(), StringIO(stdin_text)) as (out, err, in_):
        print stdout_text
        print >> sys.stderr, stderr_text
        assert raw_input() == stdin_text[:-1]
    assert out.getvalue() == stdout_text + '\n'
    assert err.getvalue() == stderr_text + '\n'

def test_stream_redirect_attribute_names_should_be_out_err_in_():
    """Checks that the as-target of the with statement has three
    attributes, out, err and in_. These should be the bindings or
    sys.stdout, sys.stderr and sys.stdin which are in effect within
    the with block. """
    stdout_text = 'stdout'
    stderr_text = 'stderr'
    stdin_text  = 'stdin\n'
    with redirected_std_streams(StringIO(), StringIO(), StringIO(stdin_text)) as streams:
        print stdout_text
        print >> sys.stderr, stderr_text
        assert raw_input() == stdin_text[:-1]
    assert streams.out.getvalue() == stdout_text + '\n'
    assert streams.err.getvalue() == stderr_text + '\n'

def test_nested_stream_redirection_should_work():
    """Checks that two nested stream redirection context managers
    restore the intermediate and original bindings as expected."""
    print "0 before"
    with redirected_std_streams(StringIO()) as (out, err, in_):
        print "1 before"
        with redirected_std_streams(StringIO()) as streams:
            print "inside"
        print "1 after"
    print "0 after"

    assert out.getvalue() == "1 before\n1 after\n"
    assert streams.out.getvalue() == "inside\n"

def test_stream_redirection_should_reset_streams_when_exception_raised():
    """Checks that redirected streams are correctly restored even when
    an exception is raised in the with block."""
    with redirected_std_streams(StringIO()) as (outer, _, _):
        try:
            with redirected_std_streams(StringIO()) as (inner, _, _):
                print "inner"
                raise Exception
                print "never happens"
        except Exception:
            pass
        print "outer"
    assert inner.getvalue() == "inner\n"
    assert outer.getvalue() == "outer\n"

def test_should_be_able_to_redirect_just_one_stream_by_keyword_argument():
    """Checks that a single one of the 3 standard streams can be
    redirected with a keyword argument, leaving the other two alone."""
    with redirected_std_streams(StringIO(), StringIO(), StringIO('stdin before\nstdin after\n')) as (out1, err1, in1):
        print >> sys.stderr, "stderr before"
        with redirected_std_streams(stderr=StringIO()) as (out2, err2, in2):
            print >> sys.stderr, "inner stderr"
        print >> sys.stderr, "stderr after"
    assert out1 is out2
    assert in1  is in2
    assert err2 is not err1
    assert err1.getvalue() == "stderr before\nstderr after\n"
    assert err2.getvalue() == "inner stderr\n"


      ############## Implementation ##############

from collections import namedtuple

Streams = namedtuple("Streams", 'out err in_')

@contextmanager
def redirected_std_streams(stdout=None, stderr=None, stdin=None):

    def set_sys(streams):
        sys.stdout, sys.stderr, sys.stdin = streams

    new =     stdout,      stderr,      stdin
    old = sys.stdout,  sys.stderr,  sys.stdin

    current = Streams(*(new or old for new, old in zip(new, old)))
    set_sys(current)

    try:
        yield current
    finally:
        set_sys(old)
