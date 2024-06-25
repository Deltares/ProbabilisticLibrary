from msilib import knownbits
import sys
from ctypes import *

import interface
from statistic import *
from reliability import *

class Project:

	_model = None

	def __init__(self):
		self._id = interface.Create('project')
		self._callback = interface.CALLBACK(self._performCallBack)
		interface.SetCallBack(self._id, 'model', self._callback)

		self._variables = []
		self._correlation_matrix = CorrelationMatrix()
		self._settings = Settings()
		self._design_point = None
		
		_model = None
  
	@property
	def variables(self):
		return self._variables

	@property   
	def correlation_matrix(self):
		return self._correlation_matrix

	@property   
	def settings(self):
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
		interface.SetArrayIntValue(self._id, 'variables', [variable._id for variable in self._variables])
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.Execute(self._id, 'run')

	@property   
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId)
				self._design_point._update_variables(self._variables)
				
		return self._design_point
			
class CombineProject:

	def __init__(self):
		self._id = interface.Create('combine_project')

		self._design_points = []
		self._settings = CombineSettings()
		self._design_point = None
		
		_model = None
  
	@property
	def design_points(self):
		return self._design_points

	@property   
	def settings(self):
		return self._settings

	def run(self):
		_design_point = None
		interface.SetArrayIntValue(self._id, 'design_points', [design_point._id for design_point in self._design_points])
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.Execute(self._id, 'run')
	
	@property   
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId)
				self._design_point._update_contributing_design_points(self._design_points)
		return self._design_point



