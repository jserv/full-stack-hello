# -*- coding: utf-8 -*-

import os
import glob
import subprocess
import unittest


testfile = map(lambda f: f.strip('.s'), glob.glob('tests/*.s'))


def get_expect_output(testfile):
    with open('%s.expected' % testfile, 'rb') as f:
        return f.read()


def get_exec_output(testfile):
    return subprocess.check_output(['./as_exec', '%s.s' % testfile])


def with_metaclass(meta, *bases):
    """Create a base class with a metaclass."""
    # This requires a bit of explanation: the basic idea is to make a dummy
    # metaclass for one level of class instantiation that replaces itself with
    # the actual metaclass.
    class metaclass(meta):

        def __new__(cls, name, this_bases, d):
            return meta(name, bases, d)
    return type.__new__(metaclass, 'temporary_class', (), {})


class TestRunnerMeta(type):

    def __new__(mcs, name, bases, dict):
        def gen_test(got, expected):
            def test(self):
                self.assertEqual(got, expected)
            return test

        for f in testfile:
            test_name = 'test_%s' % (os.path.basename(f))
            got = get_exec_output(f)
            expected = get_expect_output(f)
            dict[test_name] = gen_test(got, expected)

        return type.__new__(mcs, name, bases, dict)


class TestRunner(with_metaclass(TestRunnerMeta, unittest.TestCase)):
    pass


if __name__ == '__main__':
    unittest.main()
