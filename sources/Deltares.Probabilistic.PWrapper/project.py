import sys
from ctypes import *

import interface
from stochast import *
from reliability import *

class Project:

	_model = None

	def __init__(self):
		self._id = interface.Create('project')
		self._callback = interface.CALLBACK(self._performCallBack)
		interface.SetCallBack(self._id, 'model', self._callback)

		self._variables = []
		self._settings = None
		self._design_point = None
		
		_model = None
  
	@property
	def variables(self):
		if self._variables is None:
			self._variables = []
		return self._variables

	@property   
	def settings(self):
		if self._settings is None:
			self._settings = Settings()
		return self._settings

	@property   
	def model(self):
		return Project._model
		
	@model.setter
	def model(self, value):
		Project._model = value

	@interface.CALLBACK
	def _performCallBack(values, size):
		values_list = values[:size]
		return Project._model(values_list);

	def run(self):
		_design_point = None
		interface.SetArrayValue(self._id, 'variables', [variable._id for variable in self.variables])
		interface.SetIntValue(self._id, 'settings', self.settings._id)
		interface.Execute(self._id, 'run')

	@property   
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId)
		return self._design_point


