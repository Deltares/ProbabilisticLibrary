class EventList(list):

	def __init__(self, callback):

		for method in ['append', 'insert', 'extend', 'remove', '__add__', '__iadd__']:
			def code_added(self, *args, **kwargs):
				getattr(super(EventList, self), method)(*args, **kwargs)
				callback()

		setattr(EventList, method, code_added)
