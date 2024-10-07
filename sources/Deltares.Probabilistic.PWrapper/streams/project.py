# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of Streams.
#
# Streams is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#
import sys
from ctypes import *
from typing import FrozenSet

from .statistic import *
from .reliability import *
from .sensitivity import *
from . import interface

import inspect

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()


class ZModelContainer:
	def get_model(self):
		return None
	
	def is_dirty(self):
		return False
			
	def update_model(self):
		pass

class ZModel:
	_callback = None
	
	def __init__(self, callback = None):
		ZModel._index = 0;
		ZModel._callback = callback
		self._model = None
		self._is_function = inspect.isfunction(callback)
		self._is_dirty = False

		if self._is_function:
			self._input_parameters = self._get_input_parameters(callback)
			self._output_parameters = self._get_output_parameters(callback)
			self._model_name = callback.__name__
		else:
			self._input_parameters = []
			self._output_parameters = []
			self._model_name = ''
		
	def __dir__(self):
		return ['name',
				'input_parameters',
				'output_parameters']

	def __str__(self):
		return self.name

	@property   
	def name(self):
		return self._model_name

	def _get_input_parameters(self, function):
		variable_names = function.__code__.co_varnames
		parameters = []
		for i in range(function.__code__.co_argcount):
			modelParameter = ModelParameter()
			modelParameter.name = variable_names[i]
			modelParameter.index = i
			parameters.append(modelParameter)
			
		return FrozenList(parameters)
		
	def _get_output_parameters(self, function):
		parameters = []
		source = inspect.getsource(function)
		lines = source.splitlines()
		for line in lines:
			if line.strip().startswith('return'):
				line = line.replace('return', '')
				line = line.strip().split('#')[0]
				line = line.lstrip('[').rstrip(';').rstrip(']')
				words = line.split(',')
				parameters = [word.strip() for word in words]

		for i in range(len(parameters)):
			modelParameter = ModelParameter()
			modelParameter.name = parameters[i]
			modelParameter.index = i
			parameters[i] = modelParameter
		
		return FrozenList(parameters)

	@property   
	def input_parameters(self):
		return self._input_parameters

	@property   
	def output_parameters(self):
		return self._output_parameters

	def _set_callback(self, callback):
		ZModel._callback = callback
		
	def _set_model(self, value):
		self._model = value
		
	def initialize_for_run(self):
		if not self._model is None:
			self._model.initialize_for_run()
	
	def update(self):
		if not self._model is None:
			if self._model.is_dirty():
				self._model.update_model()
				return True
			
		return False
	
	def run(self, values, output_values):
		if self._is_function:
			z = ZModel._callback(*values);
			if type(z) is list or type(z) is tuple:
				for i in range(len(z)):
					output_values[i] = z[i]
			else:
				output_values[0] = z
		else:
			z = ZModel._callback(values, output_values);

class SensitivityProject:

	_project_id = 0
	_zmodel = None

	def __init__(self):
		self._id = interface.Create('sensitivity_project')
		self._callback = interface.CALLBACK(self._performCallBack)
		interface.SetCallBack(self._id, 'model', self._callback)

		self._is_dirty = False
		self._all_variables = {}
		self._variables = FrozenList()
		self._output_parameters = FrozenList()
		self._correlation_matrix = CorrelationMatrix()
		self._settings = SensitivitySettings()
		self._stochast = None
		self._output_correlation_matrix = None

		SensitivityProject._project_id = self._id
		SensitivityProject._zmodel = None

	def __dir__(self):
		return ['variables',
				'correlation_matrix',
				'settings',
				'model',
				'parameter',
				'run',
				'stochast',
				'output_correlation_matrix']

	@property
	def variables(self):
		self._check_model()
		return self._variables

	@property
	def correlation_matrix(self):
		self._check_model()
		return self._correlation_matrix

	@property
	def settings(self):
		self._check_model()
		return self._settings

	@property
	def parameter(self):
		return interface.GetStringValue(self._id, 'parameter')
		
	@parameter.setter
	def parameter(self, value : int):
		interface.SetStringValue(self._id, 'parameter', str(value))

	@property
	def model(self):
		return SensitivityProject._zmodel

	@model.setter
	def model(self, value):
		if inspect.isfunction(value):
			SensitivityProject._zmodel = ZModel(value)
			self._update_model()
		elif isinstance(value, ZModelContainer):
			SensitivityProject._zmodel = value.get_model()
		else:
			raise ValueError('ZModel container expected')
		
	def _check_model(self):
		if not SensitivityProject._zmodel is None:
			if SensitivityProject._zmodel.update():
				self._update_model()
	
	def _update_model(self):
		variables = []
		for input_parameter in SensitivityProject._zmodel.input_parameters:
			var_name = input_parameter.name
			if not var_name in self._all_variables.keys():
				variable = Stochast()
				variable.name = var_name
				variable.mean = input_parameter.default_value
				self._all_variables[var_name] = variable
			else:
				variable = self._all_variables[var_name]
			variables.append(variable)

		self._variables = FrozenList(variables)
		self._correlation_matrix._set_variables(variables)
		self._settings._set_variables(variables)

		self._output_parameters = SensitivityProject._zmodel.output_parameters

		interface.SetArrayIntValue(self._id, 'input_parameters', [input_parameter._id for input_parameter in SensitivityProject._zmodel.input_parameters])
		interface.SetArrayIntValue(self._id, 'output_parameters', [output_parameter._id for output_parameter in SensitivityProject._zmodel.output_parameters])
		interface.SetStringValue(self._id, 'model_name', SensitivityProject._zmodel.name)

	@interface.CALLBACK
	def _performCallBack(values, size, output_values):
		values_list = values[:size]
		SensitivityProject._zmodel.run(values_list, output_values)

	def run(self):
		self._check_model()

		self._stochast = None
		self._stochasts = None
		self._output_correlation_matrix = None

		interface.SetArrayIntValue(self._id, 'variables', [variable._id for variable in self._variables])
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.SetArrayIntValue(self.settings._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self.settings.stochast_settings])

		SensitivityProject._zmodel.initialize_for_run()
		interface.Execute(self._id, 'run')

	@property
	def stochast(self):
		if self._stochast is None:
			stochastId = interface.GetIntValue(self._id, 'sensitivity_stochast')
			if stochastId > 0:
				self._stochast = Stochast(stochastId)

		return self._stochast

	@property   
	def stochasts(self):
		if self._stochasts is None:
			stochasts = []
			stochast_ids = interface.GetArrayIntValue(self._id, 'sensitivity_stochasts')
			for stochast_id in stochast_ids:
				stochasts.append(Stochast(stochast_id))
			self._stochasts = FrozenList(stochasts)
				
		return self._stochasts

	@property   
	def output_correlation_matrix(self):
		if self._output_correlation_matrix is None:
			correlationMatrixId = interface.GetIntValue(self._id, 'output_correlation_matrix')
			if correlationMatrixId > 0:
				self._output_correlation_matrix = CorrelationMatrix(correlationMatrixId)
				self._output_correlation_matrix._update_variables(self.variables.get_list() + self.stochasts.get_list())
				
		return self._output_correlation_matrix

class ReliabilityProject:

	_zmodel = None
	_project_id = 0

	def __init__(self):
		self._id = interface.Create('project')
		self._callback = interface.CALLBACK(self._performCallBack)
		interface.SetCallBack(self._id, 'model', self._callback)

		self._all_variables = {}
		self._variables = FrozenList()
		self._correlation_matrix = CorrelationMatrix()
		self._settings = Settings()
		self._limit_state_function = None
		self._design_point = None
		self._fragility_curve = None

		ReliabilityProject._zmodel = None
		ReliabilityProject._project_id = self._id
  
	def __dir__(self):
		return ['variables',
				'correlation_matrix',
				'limit_state_function',
				'settings',
				'model',
				'run',
				'design_point']

	@property
	def variables(self):
		self._check_model()
		return self._variables

	@property
	def correlation_matrix(self):
		self._check_model()
		return self._correlation_matrix

	@property
	def settings(self):
		self._check_model()
		return self._settings

	@property
	def limit_state_function(self):
		if self._limit_state_function is None:
			lsf_id = interface.GetIntValue(self._id, 'limit_state_function')
			if lsf_id > 0:
				self._limit_state_function = LimitStateFunction(lsf_id)
		return self._limit_state_function

	@property
	def model(self):
		return ReliabilityProject._zmodel

	@model.setter
	def model(self, value):
		if inspect.isfunction(value):
			ReliabilityProject._zmodel = ZModel(value)
			self._update_model()
		elif isinstance(value, ZModelContainer):
			ReliabilityProject._zmodel = value.get_model()
		else:
			raise ValueError('ZModel container expected')
	
	def _check_model(self):
		if not ReliabilityProject._zmodel is None:
			if ReliabilityProject._zmodel.update():
				self._update_model()

	def _update_model(self):
		variables = []
		for input_parameter in ReliabilityProject._zmodel.input_parameters:
			var_name = input_parameter.name
			if not var_name in self._all_variables.keys():
				variable = Stochast()
				variable.name = var_name
				variable.mean = input_parameter.default_value
				self._all_variables[var_name] = variable
			else:
				variable = self._all_variables[var_name]
			variables.append(variable)

		self._variables = FrozenList(variables)
		self._correlation_matrix._set_variables(variables)
		self._settings._set_variables(variables)

		self._output_parameters = ReliabilityProject._zmodel.output_parameters

		interface.SetArrayIntValue(self._id, 'input_parameters', [input_parameter._id for input_parameter in ReliabilityProject._zmodel.input_parameters])
		interface.SetArrayIntValue(self._id, 'output_parameters', [output_parameter._id for output_parameter in ReliabilityProject._zmodel.output_parameters])
		interface.SetStringValue(self._id, 'model_name', ReliabilityProject._zmodel.name)

	@interface.CALLBACK
	def _performCallBack(values, size, output_values):
		values_list = values[:size]
		ReliabilityProject._zmodel.run(values_list, output_values)

	def run(self):
		self._design_point = None
		self._fragility_curve = None

		interface.SetArrayIntValue(self._id, 'variables', [variable._id for variable in self._variables])
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.SetArrayIntValue(self.settings._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self.settings.stochast_settings])

		ReliabilityProject._zmodel.initialize_for_run()
		interface.Execute(self._id, 'run')

	@property
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId, self._variables)

		return self._design_point

	@property
	def fragility_curve(self):
		if self._fragility_curve is None:
			fragilityCurveId = interface.GetIntValue(self._id, 'fragility_curve')
			if fragilityCurveId > 0:
				self._fragility_curve = Stochast(fragilityCurveId)
				ReliabilityProject.update_fragility_value(self._fragility_curve, self._variables)

		return self._fragility_curve

	def update_fragility_curve(stochast: Stochast, variables):
		for fragility_value in stochast.fragility_values:
			if fragility_value.design_point == None:
				id_ = interface.GetIntValue(fragility_value._id, 'design_point')
				if id_ > 0:
					fragility_value.design_point = DesignPoint(id_, variables)


class CombineProject:

	def __init__(self):
		self._id = interface.Create('combine_project')

		self._design_points = CallbackList(self._design_points_changed)
		self._settings = CombineSettings()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point = None

	def __dir__(self):
		return ['design_points',
	            'settings',
	            'design_point']

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

class LengthEffectProject:
	def __init__(self):
		self._id = interface.Create('length_effect_project')
		self._design_point_cross_section = DesignPoint()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point = None

	def __dir__(self):
		return ['design_point_cross_section',
				'correlation_lengths'
				'minimum_failure_length',
				'length',
				'correlation_matrix',
				'design_point']

	@property
	def design_point_cross_section(self):
		return self._design_point_cross_section

	@design_point_cross_section.setter
	def design_point_cross_section(self, value: DesignPoint):
		self._design_point_cross_section = value
		variables = value.get_variables()
		self._correlation_matrix._set_variables(variables)

	@property
	def correlation_matrix(self):
		return self._correlation_matrix

	@property
	def correlation_lengths(self):
		return interface.GetArrayValue(self._id, 'correlation_lengths')

	@correlation_lengths.setter
	def correlation_lengths(self, values):
		interface.SetArrayValue(self._id, 'correlation_lengths', values)

	@property
	def length(self):
		return interface.GetValue(self._id, 'length')

	@length.setter
	def length(self, value: float):
		interface.SetValue(self._id, 'length', value)

	@property
	def minimum_failure_length(self):
		return interface.GetValue(self._id, 'minimum_failure_length')

	@minimum_failure_length.setter
	def minimum_failure_length(self, value: float):
		interface.SetValue(self._id, 'minimum_failure_length', value)

	def run(self):
		self._design_point = None
		interface.SetIntValue(self._id, 'design_point_cross_section', self._design_point_cross_section._id)
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.Execute(self._id, 'run')

	@property
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				variables = self._design_point_cross_section.get_variables()
				self._design_point = DesignPoint(designPointId, variables, self._design_point_cross_section)
		return self._design_point


class ModelParameter:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('model_parameter')
		else:
			self._id = id

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['name',
	            'index',
		        'mean']
	
	@property
	def name(self):
		return interface.GetStringValue(self._id, 'name')
		
	@name.setter
	def name(self, value):
		interface.SetStringValue(self._id, 'name', value)

	@property
	def index(self):
		return interface.GetIntValue(self._id, 'index')
		
	@name.setter
	def index(self, value):
		interface.SetIntValue(self._id, 'index', value)

	@property
	def default_value(self):
		return interface.GetValue(self._id, 'default_value')
		
	@default_value.setter
	def default_value(self, value):
		interface.SetValue(self._id, 'default_value', value)

	def __str__(self):
		return self.name




