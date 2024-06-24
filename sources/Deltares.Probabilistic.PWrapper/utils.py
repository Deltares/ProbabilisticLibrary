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

	def extend(self, items):
		super().extend(items)
		self._callback()