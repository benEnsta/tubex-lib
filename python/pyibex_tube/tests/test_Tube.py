#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Tube.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================

import unittest
import pyibex
from pyibex import *
from pyibex_tube import *
# from pyibex_thickset import *
import numpy as np

class TestTube(unittest.TestCase):

  def setUp(self):
    tube = Tube(Interval(0.,46.), 1.);
    tube.set(Interval(4,8), 0);
    tube.set(Interval(2,7), 1);
    tube.set(Interval(1,6), 2);
    tube.set(Interval(-4,4), 3.2); # double
    tube.set(Interval(-7,-1), 4);
    tube.set(Interval(-9,-5), 5.9); # double
    tube.set(Interval(-10,-6), 6);
    tube.set(Interval(-11,-7), 7);
    tube.set(Interval(-10,-6), 8);
    tube.set(Interval(-9,-4), 9);
    tube.set(Interval(-8,-5), 10);
    tube.set(Interval(-7,-4), 11);
    tube.set(Interval(-6,-2), 12);
    tube.set(Interval(-5,-1), 13);
    tube.set(Interval(-5,3), 14); # to be updated
    tube.set(Interval(-2,4), 15);
    tube.set(Interval(0,6), 16);
    tube.set(Interval(2,7), 17);
    tube.set(Interval(4,8), 18);
    tube.set(Interval(6,9), 19);
    tube.set(Interval(7,10), 20);
    tube.set(Interval(8,11), 21);
    tube.set(Interval(9,12), 22);
    tube.set(Interval(8,13), 23);
    tube.set(Interval(7,12), 24);
    tube.set(Interval(5,11), 25);
    tube.set(Interval(3,10), 26);
    tube.set(Interval(4,9), 27);
    tube.set(Interval(5,8), 28);
    tube.set(Interval(4,7), 29);
    tube.set(Interval(3,6), 30);
    tube.set(Interval(3,5), 31);
    tube.set(Interval(2,5), 32);
    tube.set(Interval(2,5), 33);
    tube.set(Interval(1,5), 34);
    tube.set(Interval(2,4), 35);
    tube.set(Interval(1,4), 36);
    tube.set(Interval(0,4), 37);
    tube.set(Interval(-1,3), 38);
    tube.set(Interval(-1,3), 39);
    tube.set(Interval(-1,4), 40);
    tube.set(Interval(0,5), 41);
    tube.set(Interval(1,6), 42);
    tube.set(Interval(0,5), 43);
    tube.set(Interval(-1,4), 44);
    tube.set(Interval(-1,3), 45);
    self.tube = tube


  def test_Init(self):
    tube = Tube(Interval(0.,46.), 1.);
    tube.set(Interval(4,8), 0);
    tube.set(Interval(2,7), 1);
    tube.set(Interval(1,6), 2);
    tube.set(Interval(-4,4), 3.2); # double
    tube.set(Interval(-7,-1), 4);
    tube.set(Interval(-9,-5), 5.9); # double
    tube.set(Interval(-10,-6), 6);
    tube.set(Interval(-11,-7), 7);
    tube.set(Interval(-10,-6), 8);
    tube.set(Interval(-9,-4), 9);
    tube.set(Interval(-8,-5), 10);
    tube.set(Interval(-7,-4), 11);
    tube.set(Interval(-6,-2), 12);
    tube.set(Interval(-5,-1), 13);
    tube.set(Interval(-5,3), 14); # to be updated
    tube.set(Interval(-2,4), 15);
    tube.set(Interval(0,6), 16);
    tube.set(Interval(2,7), 17);
    tube.set(Interval(4,8), 18);
    tube.set(Interval(6,9), 19);
    tube.set(Interval(7,10), 20);
    tube.set(Interval(8,11), 21);
    tube.set(Interval(9,12), 22);
    tube.set(Interval(8,13), 23);
    tube.set(Interval(7,12), 24);
    tube.set(Interval(5,11), 25);
    tube.set(Interval(3,10), 26);
    tube.set(Interval(4,9), 27);
    tube.set(Interval(5,8), 28);
    tube.set(Interval(4,7), 29);
    tube.set(Interval(3,6), 30);
    tube.set(Interval(3,5), 31);
    tube.set(Interval(2,5), 32);
    tube.set(Interval(2,5), 33);
    tube.set(Interval(1,5), 34);
    tube.set(Interval(2,4), 35);
    tube.set(Interval(1,4), 36);
    tube.set(Interval(0,4), 37);
    tube.set(Interval(-1,3), 38);
    tube.set(Interval(-1,3), 39);
    tube.set(Interval(-1,4), 40);
    tube.set(Interval(0,5), 41);
    tube.set(Interval(1,6), 42);
    tube.set(Interval(0,5), 43);
    tube.set(Interval(-1,4), 44);
    tube.set(Interval(-1,3), 45);


  def test_Serialize(self):
    self.tube.serialize("/tmp/x.tube")

    t2 = Tube("/tmp/x.tube")
    self.assertTrue(self.tube == t2)





if __name__ == '__main__':

  unittest.main()
