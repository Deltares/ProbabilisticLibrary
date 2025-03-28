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
from ctypes import *
from multiprocessing import Pool, cpu_count
from typing import FrozenSet

from .statistic import *
from .reliability import *
from .sensitivity import *
from . import interface

import inspect

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class Sample:
	def __init__(self, input_values, output_values):
		self.input_values = input_values
		self.output_values = output_values

class ZModelContainer:
	def get_model(self):
		return None

	def is_valid(self):
		return True

	def validate(self):
		return FrozenList() 

	def is_dirty(self):
		return False

	def update_model(self):
		pass

class ZModel:
	_callback = None
	_multiple_callback = None
	
	def __init__(self, callback = None, output_parameter_size = 1):
		ZModel._index = 0;
		ZModel._callback = callback
		self._model = None
		self._is_function = inspect.isfunction(callback) or inspect.ismethod(callback)
		self._is_dirty = False
		self._has_arrays = False
		self._array_sizes = None
		self._pool = None
		self._max_processes = 1
		self._project_id = 0

		if self._is_function:
			self._input_parameters = self._get_input_parameters(callback)
			self._output_parameters = self._get_output_parameters(callback, output_parameter_size)
			self._model_name = callback.__name__
		else:
			self._input_parameters = []
			self._output_parameters = []
			self._model_name = ''

	def __dir__(self):
		return ['name',
				'input_parameters',
				'output_parameters']

	def __del__(self):
		if not self._pool is None:
			self._pool.close()

	def __str__(self):
		return self.name

	@property
	def name(self):
		return self._model_name

	def _get_input_parameters(self, function):
		sig = inspect.signature(function)
		parameters = []
		index = 0
		for name, param in sig.parameters.items():
			modelParameter = ModelParameter()
			modelParameter.name = name
			if param.default != param.empty:
				modelParameter.default_value = param.default
			if param.annotation != param.empty:
				modelParameter.is_array = '[' in str(param)
			modelParameter.index = index
			parameters.append(modelParameter)
			index += 1

		return FrozenList(parameters)

	def _get_output_parameters(self, function, output_parameter_size = 1):
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
			if (len(parameters) == 1 and output_parameter_size > 1):
				parameters[i].is_array = True
				parameters[i].array_size = output_parameter_size

		return FrozenList(parameters)

	def validate(self):
		if self._is_function:
			return FrozenList()
		elif not self._model is None:
			return self._model.validate()
		else:
			return FrozenList([Message.from_message(MessageType.error, 'No model provided')])

	def is_valid(self):
		if self._is_function:
			return True
		elif not self._model is None:
			return self._model.is_valid()
		else:
			return False

	@property
	def input_parameters(self):
		return self._input_parameters

	@property
	def output_parameters(self):
		return self._output_parameters

	def _set_callback(self, callback):
		ZModel._callback = callback
		
	def _set_multiple_callback(self, multiple_callback):
		ZModel._multiple_callback = multiple_callback
		
	def _set_model(self, value):
		self._model = value
		
	def set_max_processes(self, value):
		self._max_processes = value
		if not self._model is None:
			self._model.set_max_processes(value)

	def initialize_for_run(self):
		self._has_arrays = False
		for parameter in self.input_parameters:
			if parameter.is_array:
				self._has_arrays = True

		if self._has_arrays:
			self._array_sizes = []
			for parameter in self.input_parameters:
				if parameter.is_array:
					self._array_sizes.append(parameter.array_size)
				else:
					self._array_sizes.append(-1)

		self._z_values_size = 0
		for parameter in self.output_parameters:
			if parameter.is_array:
				self._z_values_size += parameter.array_size
			else:
				self._z_values_size += 1

		if not self._model is None:
			self._model.initialize_for_run()

		if self._is_function:
			if self._max_processes > 1:
				self._pool = Pool(self._max_processes)
			elif self._max_processes < 1:
				self._pool = Pool()
			else:
				self._pool = None
	
	def update(self):
		if not self._model is None:
			if self._model.is_dirty():
				self._model.update_model()
				return True
			
		return False

	def _get_args(self, values):
		args = []
		index = 0;
		for i in range(len(self._array_sizes)):
			if self._array_sizes[i] == -1:
				args.append(values[index])
				index += 1
			else:
				arg_array = []
				for j in range(self._array_sizes[i]):
					arg_array.append(values[index])
					index += 1
				args.append(arg_array)
		return args

	def run_multiple(self, samples):
		if self._is_function and self._pool is None:
			for sample in samples:
				self.run(sample)
		elif self._is_function and not self._pool is None:
			results = {}
			for sample in samples:
				sample_input = self._get_input(sample)
				results[sample] = self._pool.apply_async(func=ZModel._callback, args=(*sample_input,))
			for sample in samples:
				z = results[sample].get()
				self._assign_output(sample, z)
		else:
			ZModel._multiple_callback(samples)

	def run(self, sample):
		if self._is_function:
			sample_input = self._get_input(sample)
			z = ZModel._callback(*sample_input)
			self._assign_output(sample, z)
		else:
			z = ZModel._callback(sample);

	def _get_input(self, sample):
		if self._has_arrays:
			return self._get_args(sample.input_values)
		else:
			return sample.input_values

	def _assign_output(self, sample, z):
		if type(z) is list or type(z) is tuple:
			for i in range(self._z_values_size):
				sample.output_values[i] = z[i]
		else:
			sample.output_values[0] = z

	def _run_callback(sample_input):
		return ZModel._callback(*sample_input)

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
				'mean',
				'is_array',
				'array_size']
	
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

	@property
	def is_array(self):
		return interface.GetBoolValue(self._id, 'is_array')
		
	@is_array.setter
	def is_array(self, value):
		interface.SetBoolValue(self._id, 'is_array', value)

	@property
	def array_size(self):
		return interface.GetIntValue(self._id, 'array_size')

	@array_size.setter
	def array_size(self, value):
		interface.SetIntValue(self._id, 'array_size', value)

	def __str__(self):
		return self.name

class ModelProject:
	_project_id = 0
	_zmodel = None

	def __init__(self):

		self._known_variables = []
		self._variables = FrozenList()
		self._correlation_matrix = CorrelationMatrix()
		self._output_parameters = FrozenList()
		self._settings = None
		self._model = None

	def _initialize_callbacks(self, project_id):

		ModelProject._project_id = project_id
		self._project_id = project_id
		self._callback = interface.CALLBACK(self._performCallBack)
		self._multiple_callback = interface.MULTIPLE_CALLBACK(self._perform_multiple_callback)

		interface.SetCallBack(project_id, 'model', self._callback)
		interface.SetMultipleCallBack(project_id, 'model', self._multiple_callback)

	def _set_settings(self, settings):
		self._settings = settings

	@interface.CALLBACK
	def _performCallBack(values, size, output_values):
		sample = Sample(values[:size], output_values)
		ModelProject._zmodel.run(sample)

	@interface.MULTIPLE_CALLBACK
	def _perform_multiple_callback(sample_count, values, input_size, output_values):
		samples = []
		for i in range(sample_count):
			samples.append(Sample(values[i][:input_size], output_values[i]))
		ModelProject._zmodel.run_multiple(samples)

	def validate(self):
		if not self._model is None:
			return self._model.validate()
		else:
			return FrozenList([Message.from_message(MessageType.error, 'No model provided')])

	def is_valid(self):
		if not self._model is None:
			return self._model.is_valid()
		else:
			return False

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
	def model(self):
		if not self._model is None:
			self._model._project = self
		return self._model

	@model.setter
	def model(self, value):
		if isinstance(value, tuple):
			output_parameter_size = value[1]
			value = value[0]
		else:
			output_parameter_size = 1

		if inspect.isfunction(value) or inspect.ismethod(value):
			self._model = ZModel(value, output_parameter_size)
			self._update_model()
		elif isinstance(value, ZModel):
			self._model = value
			self._share(value._project)
			self._update_model()
		elif isinstance(value, ZModelContainer):
			self._model = value.get_model()
		else:
			raise ValueError('ZModel container expected')
		
	def _check_model(self):
		if not self._model is None:
			if self._model.update():
				self._update_model()
	
	def _share(self, shared_project):
		self._known_variables.extend(shared_project.variables)
		interface.SetIntValue(self._project_id, 'share_project', shared_project._id)

	def _update_model(self):
		interface.SetArrayIntValue(self._project_id, 'input_parameters', [input_parameter._id for input_parameter in self._model.input_parameters])
		interface.SetArrayIntValue(self._project_id, 'output_parameters', [output_parameter._id for output_parameter in self._model.output_parameters])
		interface.SetStringValue(self._project_id, 'model_name', self._model.name)

		variables = []
		variable_ids = interface.GetArrayIdValue(self._project_id, 'stochasts')
		for variable_id in variable_ids:
			variable = None
			for known_variable in self._known_variables:
				if known_variable._id == variable_id:
					variable = known_variable
			if variable is None:
				variable = Stochast(variable_id)
				self._known_variables.append(variable)
			variables.append(variable)
		self._variables = FrozenList(variables)

		self._correlation_matrix._set_variables(variables)
		self._settings._set_variables(variables)
		for var in self._variables:
			var._set_variables(self._variables)

		self._output_parameters = self._model.output_parameters

	def _run(self):
		self._check_model()

		interface.SetIntValue(self._project_id, 'correlation_matrix', self._correlation_matrix._id)
		interface.SetIntValue(self._project_id, 'settings', self._settings._id)
		if hasattr(self.settings, 'stochast_settings'):
			interface.SetArrayIntValue(self.settings._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self.settings.stochast_settings])
		if hasattr(self.settings, 'stochast_settings'):
			self._model.set_max_processes(self.settings.max_parallel_processes)
		self._model.initialize_for_run()
		ModelProject._zmodel = self._model
		interface.Execute(self._project_id, 'run')

class RunValuesType(Enum):
	median_values = 'median_values'
	mean_values = 'mean_values'
	design_values = 'design_values'
	def __str__(self):
		return str(self.value)

class RunProjectSettings:

	def __init__(self):
		self._id = interface.Create('run_project_settings')

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['run_values_type']
		
	@property
	def run_values_type(self) -> RunValuesType:
		return RunValuesType[interface.GetStringValue(self._id, 'run_values_type')]

	@run_values_type.setter
	def run_values_type(self, value : RunValuesType):
		interface.SetStringValue(self._id, 'run_values_type', str(value))

	def _set_variables(self, variables):
		pass

class RunProject(ModelProject):

	def __init__(self):
		super().__init__()
		self._id = interface.Create('run_project')
		self._realization = None
		self._initialize_callbacks(self._id)
		self._set_settings(RunProjectSettings())

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['variables',
				'correlation_matrix',
				'settings',
				'model',
				'run',
				'realization',
				'validate',
				'is_valid']

	def run(self):
		self._realization = None
		self._run()

	@property
	def realization(self):
		if self._realization is None:
			realizationId = interface.GetIdValue(self._id, 'realization')
			if realizationId > 0:
				self._realization = Evaluation(realizationId)

		return self._realization

class SensitivityProject(ModelProject):

	def __init__(self):
		super().__init__()
		self._id = interface.Create('sensitivity_project')

		self._stochast = None
		self._output_correlation_matrix = None

		self._initialize_callbacks(self._id)
		self._set_settings(SensitivitySettings())

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['variables',
				'correlation_matrix',
				'settings',
				'model',
				'parameter',
				'run',
				'stochast',
				'output_correlation_matrix',
				'validate',
				'is_valid',
				'total_model_runs']

	@property
	def parameter(self):
		return interface.GetStringValue(self._id, 'parameter')
		
	@parameter.setter
	def parameter(self, value : str):
		interface.SetStringValue(self._id, 'parameter', str(value))

	def run(self):
		self._stochast = None
		self._stochasts = None
		self._output_correlation_matrix = None

		self._run()

	@property
	def stochast(self):
		if self._stochast is None:
			stochastId = interface.GetIdValue(self._id, 'sensitivity_stochast')
			if stochastId > 0:
				self._stochast = Stochast(stochastId)

		return self._stochast

	@property
	def stochasts(self):
		if self._stochasts is None:
			stochasts = []
			stochast_ids = interface.GetArrayIdValue(self._id, 'sensitivity_stochasts')
			for stochast_id in stochast_ids:
				stochasts.append(Stochast(stochast_id))
			self._stochasts = FrozenList(stochasts)
				
		return self._stochasts

	@property
	def output_correlation_matrix(self):
		if self._output_correlation_matrix is None:
			correlationMatrixId = interface.GetIdValue(self._id, 'output_correlation_matrix')
			if correlationMatrixId > 0:
				self._output_correlation_matrix = CorrelationMatrix(correlationMatrixId)
				self._output_correlation_matrix._update_variables(self.variables.get_list() + self.stochasts.get_list())
				
		return self._output_correlation_matrix

	@property
	def total_model_runs(self):
		return interface.GetIntValue(self._id, 'total_model_runs')

class ReliabilityProject(ModelProject):

	def __init__(self):
		super().__init__()
		self._id = interface.Create('project')

		self._limit_state_function = None
		self._design_point = None
		self._fragility_curve = None

		self._initialize_callbacks(self._id)
		self._set_settings(Settings())
        
	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['variables',
				'correlation_matrix',
				'limit_state_function',
				'settings',
				'model',
				'run',
				'design_point',
				'validate',
				'is_valid',
				'total_model_runs']

	@property
	def limit_state_function(self):
		if self._limit_state_function is None:
			lsf_id = interface.GetIdValue(self._id, 'limit_state_function')
			if lsf_id > 0:
				self._limit_state_function = LimitStateFunction(lsf_id)
		return self._limit_state_function

	def run(self):
		self._design_point = None
		self._fragility_curve = None
		self._initialized = False

		self._run()

	@property
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIdValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId, self._get_variables())

		return self._design_point

	def _get_variables(self):
		variables = []
		variables.extend(self.variables)
		for variable in variables:
			for array_variable in variable.array_variables:
				if array_variable not in variables:
					variables.append(array_variable)
		return variables

	@property
	def fragility_curve(self):
		if self._fragility_curve is None:
			fragilityCurveId = interface.GetIntValue(self._id, 'fragility_curve')
			if fragilityCurveId > 0:
				self._fragility_curve = Stochast(fragilityCurveId)
				ReliabilityProject.update_fragility_value(self._fragility_curve, self.variables)

		return self._fragility_curve

	def update_fragility_curve(stochast: Stochast, variables):
		for fragility_value in stochast.fragility_values:
			if fragility_value.design_point == None:
				id_ = interface.GetIdValue(fragility_value._id, 'design_point')
				if id_ > 0:
					fragility_value.design_point = DesignPoint(id_, variables)

	@property
	def total_model_runs(self):
		return interface.GetIntValue(self._id, 'total_model_runs')

class CombineProject:

	def __init__(self):
		self._id = interface.Create('combine_project')

		self._design_points = CallbackList(self._design_points_changed)
		self._settings = CombineSettings()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point_correlation_matrix = CorrelationMatrix()
		self._design_point = None

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['design_points',
				'settings',
				'correlation_matrix',
				'design_point_correlation_matrix',
				'design_point']

	def _design_points_changed(self):
		variables = []
		for design_point in self._design_points:
			variables.extend(design_point.get_variables())
		self._correlation_matrix._set_variables(variables)
		self._design_point_correlation_matrix._set_variables(variables)

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
		interface.SetIntValue(self._id, 'design_point_correlation_matrix', self._design_point_correlation_matrix._id)
		interface.Execute(self._id, 'run')

	@property
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIdValue(self._id, 'design_point')
			if designPointId > 0:
				variables = []
				for design_point in self._design_points:
					variables.extend(design_point.get_variables())
				self._design_point = DesignPoint(designPointId, variables, self._design_points)
		return self._design_point

class ExcludingCombineProject:

	def __init__(self):
		self._id = interface.Create('excluding_combine_project')

		self._design_points = CallbackList(self._design_points_changed)
		self._scenarios = CallbackList(self._scenarios_changed)
		self._settings = ExcludingCombineSettings()
		self._design_point = None
		self._synchronizing = False
		self._dirty = True

		interface.SetIntValue(self._id, 'settings', self._settings._id)

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['design_points',
				'scenarios',
				'settings',
				'is_valid',
				'validate',
				'run',
				'design_point']

	def _design_points_changed(self):
		if not self._synchronizing:
			self._dirty = True

	def _scenarios_changed(self):
		if not self._synchronizing:
			# replace floats by Scenario
			self._synchronizing = True
			for i in range(len(self._scenarios)):
				if isinstance(self._scenarios[i], int) or isinstance(self._scenarios[i], float):
					val = self._scenarios[i]
					self._scenarios[i] = Scenario()
					self._scenarios[i].probability = val
			self._synchronizing = False
			self._dirty = True

	def _update(self):
		if self._dirty:
			interface.SetArrayIntValue(self._id, 'design_points', [design_point._id for design_point in self._design_points])
			interface.SetArrayIntValue(self._id, 'scenarios', [scenario._id for scenario in self._scenarios])
			self._dirty = False

	@property
	def design_points(self):
		return self._design_points

	@property
	def scenarios(self):
		return self._scenarios

	@property
	def settings(self):
		return self._settings

	def is_valid(self):
		self._update()
		return interface.GetBoolValue(self._id, 'is_valid')

	def validate(self):
		self._update()
		interface.Execute(self._id, 'validate')
		messages = []
		message_ids = interface.GetArrayIdValue(self._id, 'validation_messages')
		for message_id in message_ids:
			messages.append(Message(message_id))
		interface.Execute(self._id, 'clear_validate')
		return FrozenList(messages) 

	def run(self):
		self._update()
		self._design_point = None
		interface.Execute(self._id, 'run')

	@property
	def design_point(self):
		if self._design_point is None:
			design_point_id = interface.GetIdValue(self._id, 'design_point')
			if design_point_id > 0:
				variables = []
				for design_point in self._design_points:
					variables.extend(design_point.get_variables())
				self._design_point = DesignPoint(design_point_id, variables, self._design_points)
		return self._design_point

class LengthEffectProject:
	def __init__(self):
		self._id = interface.Create('length_effect_project')
		self._design_point_cross_section = DesignPoint()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point = None

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['design_point_cross_section',
				'correlation_lengths'
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

	def run(self):
		self._design_point = None
		interface.SetIntValue(self._id, 'design_point_cross_section', self._design_point_cross_section._id)
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.Execute(self._id, 'run')

	@property
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIdValue(self._id, 'design_point')
			if designPointId > 0:
				variables = self._design_point_cross_section.get_variables()
				self._design_point = DesignPoint(designPointId, variables, self._design_point_cross_section)
		return self._design_point

