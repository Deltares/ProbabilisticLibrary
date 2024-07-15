import sys

class CallbackList(list):

	def __init__(self, callback):
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

class FrozenList():
	def __init__(self, initial_list = None):
		self._list = []
		self._dict = {}
		if not initial_list is None:
			self._list.extend(initial_list)
			for item in self._list:
				self._dict[str(item)] = item

	def __getitem__(self, index):
		if isinstance(index, str):
			if index in self._dict.keys():
				return self._dict[str(index)]
			else:
				return None
		else:
			return self._list[index]

	def __iter__(self):
		return self._list.__iter__()

	def __next__(self):
		self._list.__next__()

	def __len__(self):
		return len(self._list)

	def __str__(self):
		return str(self._list)

	def count(self):
		return self._list.count()


		
