# -*- coding: utf-8 -*-

import subprocess
import unittest


class HelloTest(unittest.TestCase):
    def get_output(self, testfile):
        return subprocess.check_output(['./as_exec', 'tests/%s' % (testfile)])

    def test_hello(self):
        output = self.get_output('hello.s')
        self.assertEqual(output, b'Hello\n')

    def test_test(self):
        output = self.get_output('test.s')
        self.assertEqual(output, b'42\n50\n150\n')


if __name__ == '__main__':
    unittest.main()
