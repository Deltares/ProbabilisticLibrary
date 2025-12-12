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
import sys
from typing import Iterator, TypeVar, Generic

T = TypeVar('T')

class CallbackList(list):
	"""List which invokes a callback after each list modification"""

	def __init__(self, callback):
		"""Constructor

        Parameters
        ----------
        callback : method
            Method to invoke after each list modification"""

		self._callback = callback

	def __setitem__(self, index, item):
		super().__setitem__(index, item)
		self._callback()
		
	def clear(self):
		super().clear()
		self._callback()
		
	def append(self, item):
		super().append(item)
		self._callback()
		
	def insert(self, index, item):
		super().insert(index, item)
		self._callback()
		
	def remove(self, item):
		super().remove(item)
		self._callback()

	def pop(self, item):
		item = super().pop(item)
		self._callback()
		return item

	def extend(self, items):
		super().extend(items)
		self._callback()

class FrozenList(Generic[T]):
	"""Read-only list, items can also be retrieved by their string representation"""

	def __init__(self, initial_list = None):
		"""Constructor

        Parameters
        ----------
        initial_list : list, optional
            List which will be wrapped as a read-only list by this class"""

		self._list : list[T] = []
		self._dict : dict[str, T] = {}
		if not initial_list is None:
			self._list.extend(initial_list)
			for item in self._list:
				self._dict[str(item)] = item

	def __getitem__(self, index) -> T:
		if isinstance(index, int):
			return self._list[index]
		elif isinstance(index, slice):
			return FrozenList(self._list[index])
		else:
			if not isinstance(index, str):
				index = str(index)
			if index in self._dict.keys():
				return self._dict[index]
			else:
				return None

	def __iter__(self) -> Iterator[T]:
		return self._list.__iter__()

	def __next__(self) -> T:
		self._list.__next__()

	def __len__(self) -> int:
		return len(self._list)

	def __str__(self) -> str:
		return str(self._list)

	def index(self, item, start = 0, stop = sys.maxsize) -> int:
		"""Gets the index of an item

        Parameters
        ----------
        item : obj
            Item to be found
        start : int, optional
            Start index
        stop : int, optional
            Stop index"""

		if isinstance(item, str):
			item = self[item]
		if item != None:
			return self._list.index(item, start, stop)
		else:
			return -1

	def count(self) -> int:
		"""Gets the number of items in the list"""
		return self._list.count()

	def get_list(self) -> list[T]:
		"""Gets a copy of the list with full access"""
		getlist = []
		getlist.extend(self._list)
		return getlist

class FrozenObject:
	"""Object to which no members can be added. If members are added, an exception occurs."""
	def __setattr__(self, key, value):
		if hasattr(self, '_frozen'):
			if key in self.__dir__() or hasattr(self, key):
				super.__setattr__(self, key, value)
			else:
				raise ValueError(key + ' does not exist')
		else:
			super.__setattr__(self, key, value)

	def __dir__(self):
		return []

	def _freeze(self):
		self._frozen = True

class PrintUtils:
	"""Utilities for printing"""
	def get_space_from_indent(indent : int) -> str:
		"""Converts indentation level to spaces"""
		indent_str = ''
		for i in range(indent):
			indent_str += '  '
		return indent_str

class NumericUtils:
	"""Numeric utilities"""
	def order (value1 : float, value2 : float) -> tuple[float, float]:
		"""Returns floats ordered"""
		if value1 > value2:
			return value2, value1
		else:
			return value1, value2

	def make_different(value1 : float, value2 : float) -> tuple[float, float]:
		"""Guarantees that values are different"""
		if value1 == value2:
			diff = abs(value1) / 10
			if diff == 0:
				diff = 1
			value1 = value1 - diff
			value2 = value2 + diff
		return value1, value2
