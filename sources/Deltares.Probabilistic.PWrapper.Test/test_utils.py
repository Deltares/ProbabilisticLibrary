# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of the Probabilistic Library.
#
# The Probabilistic Library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#
import unittest
import sys

from probabilistic_library import *

margin = 0.02

class test_object (FrozenObject):
	def __init__(self, name, value):
		self.name = name
		self.value = value
		super()._freeze()
	def __str__(self):
		return str(self.name)

class Test_utils(unittest.TestCase):

    def test_frozen_list(self):
        t1 = test_object('a', 1)
        t2 = test_object('b', 2)
        t3 = test_object('c', 3)
        t4 = test_object('d', 4)
        t5 = test_object('e', 5)

        fl = FrozenList([t1, t2, t3, t4, t5])

        self.assertEqual(2, fl[1].value)
        self.assertEqual(3, fl['c'].value)
        self.assertEqual(None, fl['f'])

        slc = fl[2:4]
        self.assertEqual(2, len(slc))
        self.assertEqual(3, slc[0].value)
        self.assertEqual(4, slc['d'].value)
        self.assertEqual(None, slc['e'])

if __name__ == '__main__':
    unittest.main()
