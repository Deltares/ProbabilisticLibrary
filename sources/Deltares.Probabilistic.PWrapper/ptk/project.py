import sys
from ctypes import *
from typing import FrozenSet

from .statistic import *
from .reliability import *
from . import interface

class ReliabilityProject:

	_model = None

	def __init__(self):
		self._id = interface.Create('project')
		self._callback = interface.CALLBACK(self._performCallBack)
		interface.SetCallBack(self._id, 'model', self._callback)

		self._all_variables = {}
		self._variables = FrozenList()
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
		return ReliabilityProject._model
		
	@model.setter
	def model(self, value):
		ReliabilityProject._model = value
		
		variable_names = value.__code__.co_varnames

		variables = []
		for var_name in variable_names[:value.__code__.co_argcount]:
			if not var_name in self._all_variables.keys():
				variable = Stochast()
				variable.name = var_name
				self._all_variables[var_name] = variable
			else:
				variable = self._all_variables[var_name]
			variables.append(variable)
			
		self._variables = FrozenList(variables)
		self._correlation_matrix._set_variables(variables)
		self._settings._set_variables(variables)

	@interface.CALLBACK
	def _performCallBack(values, size):
		values_list = values[:size]
		z = ReliabilityProject._model(*values_list);
		return z

	def run(self):
		self._design_point = None
		interface.SetArrayIntValue(self._id, 'variables', [variable._id for variable in self._variables])
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.SetArrayIntValue(self.settings._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self.settings.stochast_settings])
		interface.Execute(self._id, 'run')

	@property   
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId, self._variables)
				
		return self._design_point
			
class CombineProject:

	def __init__(self):
		self._id = interface.Create('combine_project')

		self._design_points = CallbackList(self._design_points_changed)
		self._settings = CombineSettings()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point = None
		
		_model = None

	def _design_points_changed(self):
		variables = []
		for design_point in self._design_points:
			variables.extend(design_point.get_variables())
		self._correlation_matrix._set_variables(variables)
  
	@property
	def design_points(self):
		return self._design_points

	@property   
	def settings(self):
		return self._settings

	@property   
	def correlation_matrix(self):
		return self._correlation_matrix

	def run(self):
		self._design_point = None
		interface.SetArrayIntValue(self._id, 'design_points', [design_point._id for design_point in self._design_points])
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.Execute(self._id, 'run')
	
	@property   
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				variables = []
				for design_point in self._design_points:
					variables.extend(design_point.get_variables())
				self._design_point = DesignPoint(designPointId, variables, self._design_points)
		return self._design_point





