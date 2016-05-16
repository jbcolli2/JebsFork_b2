# This file is part of Bertini 2.
# 
# python/test/tracker_test.py is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# python/test/tracker_test.py is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with python/test/tracker_test.py.  If not, see <http://www.gnu.org/licenses/>.
# 
#  Copyright(C) 2016 by Bertini2 Development Team
# 
#  See <http://www.gnu.org/licenses/> for a copy of the license, 
#  as well as COPYING.  Bertini2 is provided with permitted 
#  additional terms in the b2/licenses/ directory.

#  individual authors of this file include:
# 
#   Daniel Brake
#   University of Notre Dame
# 


__author__ = 'ofloveandhate'

from pybertini import *
import unittest
import numpy as np
import pdb


class TrackerTest(unittest.TestCase):
    def setUp(self):
        self.toldbl = 1e-15;
        self.x = Variable("x");
        self.y = Variable("y");
        self.t = Variable("t");
        #
        self.f = Function(self.x*self.y);
        self.g = Function(pow(self.x,2)*self.y - self.a*self.z*self.x);
        #
        self.sys = System();
        #
        self.vars = VariableGroup();
        #
        self.vars.append(self.x);
        self.vars.append(self.y);
        #
        self.sys.add_variable_group(self.vars);
        #
        self.sys.add_path_variable(self.t);
        self.sys.add_function(pow(x,2) + (1-t)*x - 1);
        self.sys.add_function(pow(y,2) + (1-t)*x*y - 2);
        #
        self.start_time = mpfr(1);
        self.end_time = mpfr(0);
        #
        self.start_point = VectorXmp(2);
        self.start_point[0] = mpfr(1);
        self.start_point[1] = sqrt(mpfr(2));
        #
        #
        self.end_point = VectorXmp();
        #
        self.tracker = AMPTracker(self.sys);
        #
        #
        #BOOST_CHECK(abs(end_point(0)-mpfr("6.180339887498949e-01")) < 1e-5);
        #BOOST_CHECK(abs(end_point(1)-mpfr("1.138564265110173e+00")) < 1e-5);