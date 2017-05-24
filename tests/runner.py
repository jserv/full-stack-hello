# -*- coding: utf-8 -*-

import subprocess
import unittest


class TestRunner(unittest.TestCase):
    def get_output(self, testfile):
        return subprocess.check_output(['./as_exec', 'tests/%s' % (testfile)])

    def test_hello(self):
        output = self.get_output('hello.s')
        self.assertEqual(output, b'Hello\n')

    def test_coverage(self):
        output = self.get_output('coverage.s')
        self.assertEqual(output, b'42\n50\n150\n')

    def test_halt(self):
        output = self.get_output('halt.s')
        self.assertEqual(output, b'42\n')


if __name__ == '__main__':
    unittest.main()
