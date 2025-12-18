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
"""
This module contains all the basic functionality for a project.

A project forms the foundation for performing analyses, such as reliability, uncertainty or
sensitivity analyses. In addition to reliability analyses, additional functionalities are
available to combine or upscale reliability results.

A model can be assigned to a project, which can be either a Python script or a PTK model
(the ptk wheel is required for this). When a model is assigned, stochastic variables and
a correlation matrix are automatically generated.

```mermaid
classDiagram
    class ModelProject{
        +model ZModel
        +variables list[Stochast]
        +correlation_matrix CorrelationMatrix
    }
    class Stochast{}
    class CorrelationMatrix{}
    class ZModel{
        +name : string
        +input_parameters : list[ModelParameter]
        +output_parameters : list[ModelParameter]
        ~run()
        ~run_multiple()
    }
    class ZModelContainer{
        +get_model() : ZModel
    }
    class ModelParameter{
        +name : string
    }

    ZModel <-- ModelProject
    ZModel "get_model" <-- ZModelContainer
    ModelParameter "*, input, output " <-- ModelProject
    Stochast <-- ModelProject
    CorrelationMatrix <-- ModelProject
```
"""


from __future__ import annotations
import sys
from multiprocessing import Pool, cpu_count
from typing import FrozenSet
from types import FunctionType, MethodType
from enum import Enum

from .statistic import Stochast, DistributionType, CorrelationMatrix, CopulaCorrelation, SelfCorrelationMatrix, Scenario, CorrelationType
from .reliability import DesignPoint, ReliabilityMethod, Settings, CombineSettings, ExcludingCombineSettings, LimitStateFunction
from .sensitivity import SensitivityResult, SensitivityValue, SensitivitySettings, SensitivityMethod
from .uncertainty import UncertaintyResult, UncertaintySettings, UncertaintyMethod
from .logging import Evaluation, Message, ValidationReport
from .utils import FrozenObject, FrozenList, CallbackList
from . import interface

import inspect

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class ZModel(FrozenObject):
	"""Wrapper around a python function or method.

    This class is created by a `ModelProject` when its model is set.

    The ZModel provides additional information about its input and output parameters.
    This is based on reflection when the ZModel is created."""

	_callback = None
	_multiple_callback = None
	
	def __init__(self, callback = None, output_parameter_size = 1):
		self._model = None
		source_code = None
		if isinstance(callback, str):
			source_code = callback
			try:
				dynamic_code = compile(callback, '<string>', 'exec')
				code_object = dynamic_code.co_consts[0]
				callback = FunctionType(code_object, globals(), code_object.co_name)
			except Exception:
				callback = source_code # so that _is_function will be False
		self._is_function = inspect.isfunction(callback) or inspect.ismethod(callback)
		self._is_dirty = False
		self._has_arrays = False
		self._array_sizes = None
		self._pool = None
		self._max_processes = 1
		self._project = None
		self._project_id = 0
		self._z_values_size = 0
		ZModel._index = 0;
		ZModel._callback = callback

		if self._is_function:
			self._model_name = callback.__name__
			self._input_parameters = self._get_input_parameters(callback)
			if source_code != None:
				self._output_parameters = self._get_output_parameters(source_code, output_parameter_size)
			else:
				self._output_parameters = self._get_output_parameters(callback, output_parameter_size)
		else:
			self._model_name = ''
			self._input_parameters = []
			self._output_parameters = []

		super()._freeze()

	def __dir__(self):
		return ['name',
				'input_parameters',
				'output_parameters',
 				'print']

	def __del__(self):
		try:
			if not self._pool is None:
				self._pool.close()
		except Exception as err:
			print(f"Unexpected {err=}, {type(err)=}")

	def __str__(self):
		return self.name

	@property
	def name(self) -> str:
		"""Name of the model"""
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
		if isinstance(function, str):
			source = function
		else:
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

	def is_model_valid(self) -> bool:
		"""Indicates whether the model is valid"""
		if self._is_function:
			return True
		elif not self._model is None:
			return self._model.is_model_valid()
		else:
			return False

	@property
	def input_parameters(self) -> list[ModelParameter]:
		"""List of input parameters"""
		return self._input_parameters

	@property
	def output_parameters(self) -> list[ModelParameter]:
		"""List of output parameters"""
		return self._output_parameters

	def _set_callback(self, callback):
		ZModel._callback = callback
		
	def _set_multiple_callback(self, multiple_callback):
		ZModel._multiple_callback = multiple_callback
		
	def _set_model(self, value):
		self._model = value
		
	def set_max_processes(self, value : int):
		"""Sets the maximum number of parallel processes"""
		self._max_processes = value
		if not self._model is None:
			self._model.set_max_processes(value)

	def initialize_for_run(self):
		"""Method to be called before the first run (this method is used internally by `ModelProject`)"""
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
		"""Updates input and output parameters (this method is used internally by `ModelProject`)"""
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
		"""Performs the execution of multiple samples by the model (used internally)"""
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
		"""Performs the execution of a sample by the model (used internally)"""
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

	def print(self):
		"""Prints the model with all input and output parameters"""
		pre = '  '
		if not self.name == '':
			print(f'Model {self.name}:')
		print('Input parameters:')
		for input_parameter in self.input_parameters:
			if input_parameter.is_array:
				print(pre + f'{input_parameter.name}[{input_parameter.array_size}]')
			else: 
				print(pre + f'{input_parameter.name}')
		print('Output parameters:')
		for output_parameter in self.output_parameters:
			if output_parameter.is_array:
				print(pre + f'{output_parameter.name}[{output_parameter.array_size}]')
			else: 
				print(pre + f'{output_parameter.name}')

class ZModelContainer:
	"""Wrapper of a `ZModel`, used internally by ptk.whl to assign a PTK model to a `ModelProject`"""

	def get_model(self) -> ZModel|None:
		"""Gets a ZModel"""
		return None

	def is_model_valid(self) -> bool:
		"""Indicates whether the model is valid"""
		return True

	def is_dirty(self):
		return False

	def update_model(self):
		pass

class ModelParameter(FrozenObject):
	"""Input or output parameter of a model

    A model parameter is part of a `ZModel`, as one of its input or output parameters. It is generated when a
    `ZModel` is created. By reflection and type hints as much as possible information about the parameter is
    provided."""

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('model_parameter')
		else:
			self._id = id
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['name',
				'index',
				'default_value',
				'is_array',
				'array_size']
	
	@property
	def name(self) -> str:
		"""Name of the parameter"""
		return interface.GetStringValue(self._id, 'name')
		
	@name.setter
	def name(self, value):
		interface.SetStringValue(self._id, 'name', value)

	@property
	def index(self) -> int:
		"""Sequence number of the parameter in the list of input or output parameters of a `ZModel`"""
		return interface.GetIntValue(self._id, 'index')
		
	@index.setter
	def index(self, value : int):
		interface.SetIntValue(self._id, 'index', value)

	@property
	def default_value(self) -> float:
		"""Default value of the parameter"""
		return interface.GetValue(self._id, 'default_value')
		
	@default_value.setter
	def default_value(self, value : float):
		interface.SetValue(self._id, 'default_value', value)

	@property
	def is_array(self) -> bool:
		"""Indicates whether the parameter is an array"""
		return interface.GetBoolValue(self._id, 'is_array')
		
	@is_array.setter
	def is_array(self, value : bool):
		interface.SetBoolValue(self._id, 'is_array', value)

	@property
	def array_size(self) -> int:
		"""Array size, in case the parameter is an array"""
		return interface.GetIntValue(self._id, 'array_size')

	@array_size.setter
	def array_size(self, value : int):
		interface.SetIntValue(self._id, 'array_size', value)

	def __str__(self):
		return self.name

class ModelProject(FrozenObject):
	"""Base class for projects, which contain a model

    When a model is set to `model`, the model input parameters and output parameters are updated. Based on
    these parameters, variables, correlation matrix and settings and settings per variable are generated in
    this class."""

	_project_id = 0
	_zmodel = None

	def __init__(self):

		self._known_variables = []
		self._variables = FrozenList()
		self._correlation_matrix = CorrelationMatrix()
		self._copulas = None
		self._output_parameters = FrozenList()
		self._settings = None
		self._model = None
		self._correlation_type = CorrelationType.gaussian
		# do not freeze, this will be done by inheritors

	def _initialize_callbacks(self, project_id):

		ModelProject._project_id = project_id
		self._project_id = project_id
		self._callback = interface.CALLBACK(self._performCallBack)
		self._multiple_callback = interface.MULTIPLE_CALLBACK(self._perform_multiple_callback)

		interface.SetCallBack(project_id, 'model', self._callback)
		interface.SetMultipleCallBack(project_id, 'model', self._multiple_callback)
		interface.SetBoolValue(project_id, 'callback_assigned', False)

	def _set_settings(self, settings):
		self._settings = settings

	@interface.CALLBACK
	def _performCallBack(values, size, output_values):
		sample = _Sample(values[:size], output_values)
		ModelProject._zmodel.run(sample)

	@interface.MULTIPLE_CALLBACK
	def _perform_multiple_callback(sample_count, values, input_size, output_values):
		samples = []
		for i in range(sample_count):
			samples.append(_Sample(values[i][:input_size], output_values[i]))
		ModelProject._zmodel.run_multiple(samples)

	def is_valid(self) -> bool:
		"""Indicates whether the settings are valid"""
		self._update()
		return interface.GetBoolValue(self._id, 'is_valid')

	def validate(self):
		"""Prints the validity of the settings"""
		self._update()
		id_ = interface.GetIdValue(self._id, 'validate')
		if id_ > 0:
			validation_report = ValidationReport(id_)
			validation_report.print()

	@property
	def variables(self) -> list[Stochast]:
		"""List of variables based on the input parameters of the model"""
		self._check_model()
		return self._variables

	@property
	def correlation_type(self) -> CorrelationType:
		return self._correlation_type

	@correlation_type.setter
	def correlation_type(self, value):
		# if the type changes, create an new correlation class for the type now in use, and make the other inaccessable
		if (value != self._correlation_type):
			if (value == CorrelationType.gaussian):
				self._correlation_matrix = CorrelationMatrix()
				self._correlation_matrix._set_variables(self._copulas._variables)
				self._copulas = None
			else:
				self._copulas = CopulaCorrelation()
				self._copulas._set_variables(self._correlation_matrix._variables)
				self._correlation_matrix = None
			self._correlation_type = value

	@property
	def correlation_matrix(self) -> CorrelationMatrix:
		"""Correlation matrix based on the input parameters of the model"""
		self._check_model()
		return self._correlation_matrix

	@property
	def copulas(self) -> CopulaCorrelation:
		"""Correlation copula based on the input parameters of the model"""
		self._check_model()
		return self._copulas

	@property
	def model(self) -> ZModel:
		"""Method which serves as a model. A model is a function which calculates real world
        results based on real world input data (or is an academic function). It often relates
        to physical processes and is deterministic (it does not use uncertainty)

        When a model is set, it accepts a python function or python class method. Alternatively,
        a string defining a function is accepted too. The model should accept a number of input
        values (floats) or array of input values and returns a single value (float), an array of
        floats or a tuple of floats.

        When set, the function/method/string is wrapped in a `ZModel`. The ZModel has information
        about its input and output parameters (derived from the function signature). When the model
        is retrieved, the ZModel object is returned.

        It is also possible to set a `ZModelContainer`, which can provide a `ZModel`. PTK models
        are set with a `ZModelContainer` (using ptk.whl)"""

		if not self._model is None:
			self._model._project = self
		return self._model

	@model.setter
	def model(self, value : FunctionType | MethodType | str | ZModel | ZModelContainer):
		if isinstance(value, tuple):
			output_parameter_size = value[1]
			value = value[0]
		else:
			output_parameter_size = 1

		if inspect.isfunction(value) or inspect.ismethod(value) or isinstance(value, str):
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

		interface.SetBoolValue(self._project_id, 'callback_assigned', self._model.is_model_valid())
		
	def _check_model(self):
		if not self._model is None:
			if self._model.update():
				self._update_model()
	
	def _share(self, shared_project):
		id1 = self._id
		id2 = shared_project._id
		self._known_variables.extend(shared_project.variables)
		interface.SetIntValue(self._id, 'share_project', shared_project._id)

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

		if (self._correlation_type == CorrelationType.gaussian):
			self._correlation_matrix._set_variables(variables)
		else:
			self._copulas._set_variables(variables)
		self._settings._set_variables(variables)
		for var in self._variables:
			var._set_variables(self._variables)

		self._output_parameters = self._model.output_parameters

	def _update(self):
		self._check_model()

		if (self._correlation_type == CorrelationType.gaussian):
			interface.SetIntValue(self._project_id, 'correlation_matrix', self._correlation_matrix._id)
		else:
			interface.SetIntValue(self._project_id, 'copula_correlation', self._copulas._id)
		interface.SetIntValue(self._project_id, 'settings', self._settings._id)
		if hasattr(self.settings, 'stochast_settings'):
			interface.SetArrayIntValue(self.settings._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self.settings.stochast_settings])
		if self._model != None:
			if hasattr(self.settings, 'max_parallel_processes'):
				self._model.set_max_processes(self.settings.max_parallel_processes)
			self._model.initialize_for_run()
		ModelProject._zmodel = self._model

	def _run(self):
		if (self.is_valid()):
			interface.Execute(self._project_id, 'run')
		else:
			# print the validation messages
			self.validate()

class RunValuesType(Enum):
	"""Enumeration which defines which value to use from a stochastic variable"""
	median_values = 'median_values'
	mean_values = 'mean_values'
	design_values = 'design_values'
	def __str__(self):
		return str(self.value)

class _Sample(FrozenObject):
	"""Defines a sample, used internally"""
	def __init__(self, input_values, output_values):
		self.input_values = input_values
		self.output_values = output_values
		super()._freeze()

class RunProjectSettings(FrozenObject):
	"""Settings for a model run"""

	def __init__(self):
		self._id = interface.Create('run_project_settings')
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['run_values_type',
		        'reuse_calculations',
		        'validate',
		        'is_valid']
		
	@property
	def run_values_type(self) -> RunValuesType:
		"""Defines which value to extract from the stochastic variables"""
		return RunValuesType[interface.GetStringValue(self._id, 'run_values_type')]

	@run_values_type.setter
	def run_values_type(self, value : RunValuesType):
		interface.SetStringValue(self._id, 'run_values_type', str(value))

	@property
	def reuse_calculations(self) -> bool:
		"""Indicates whether previous model results will be reused by the model run."""
		return interface.GetBoolValue(self._id, 'reuse_calculations')

	@reuse_calculations.setter
	def reuse_calculations(self, value : bool):
		interface.SetBoolValue(self._id, 'reuse_calculations', value)

	def is_valid(self) -> bool:
		"""Indicates whether the settings are valid"""
		return interface.GetBoolValue(self._id, 'is_valid')

	def validate(self):
		"""Prints the validity of the settings"""
		id_ = interface.GetIdValue(self._id, 'validate')
		if id_ > 0:
			validation_report = ValidationReport(id_)
			validation_report.print()

	def _set_variables(self, variables):
		pass

class RunProject(ModelProject):
	"""Project for a running a model. This is the main entry point for running a model.

    This class is based on the `ModelProject` class. The model to use is defined in
    this class in `ModelProject.model`. When a model is set, variables and settings
    are generated.

    To run the model, use the `run` method. This results are stored in `realization`.

    ```mermaid
    classDiagram
        class ModelProject{
            +model ZModel
            +variables list[Stochast]
            +correlation_matrix CorrelationMatrix
        }
        class RunProject{
            +settings RunProjectSettings
            +realization Evalution
            +run()
        }
        class RunProjectSettings{}
        class Stochast{}
        class CorrelationMatrix{}
        class Evaluation{}
        RunProject <|-- ModelProject
        Stochast <-- ModelProject
        CorrelationMatrix <-- ModelProject
        RunProjectSettings <-- RunProject
        Evaluation <-- RunProject
    ```
    """

	def __init__(self):
		super().__init__()
		self._id = interface.Create('run_project')
		self._realization = None
		self._initialize_callbacks(self._id)
		self._set_settings(RunProjectSettings())
		super()._freeze()

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

	@property
	def settings(self) -> RunProjectSettings:
		"""Settings for running a model"""
		self._check_model()
		return self._settings

	def run(self):
		"""Performs the model run and puts the result in `realization`"""
		self._realization = None
		self._run()

	@property
	def realization(self) -> Evaluation:
		"""Realization of the performed model run"""
		if self._realization is None:
			realizationId = interface.GetIdValue(self._id, 'realization')
			if realizationId > 0:
				self._realization = Evaluation(realizationId)

		return self._realization

class SensitivityProject(ModelProject):
	"""Project for a sensitivity analysis. This is the main entry point for performing a sensitivity analysis.

    This class is based on the `ModelProject` class. The model to use is defined in this class in `ModelProject.model`.
    When a model is set, variables, correlation matrix and settings and settings per variable are generated.

    To run a sensitivity analysis, use the `run` method. This results are stored in a list of `results`, where each
    result corresponds with an output parameter. A shortcut is provided in `result`, which corresponds with the
    first output parameter."""

	def __init__(self):
		super().__init__()
		self._id = interface.Create('sensitivity_project')

		self._result = None
		self._results = None

		self._initialize_callbacks(self._id)
		self._set_settings(SensitivitySettings())
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['variables',
				'correlation_matrix',
				'settings',
				'model',
				'parameter',
				'run',
				'result',
				'results',
				'validate',
				'is_valid',
				'total_model_runs']

	@property
	def parameter(self) -> str:
		"""Output parameter for which the sensitivity analysis should be performed, if left blank it will
        be performed for all output parameters"""
		return interface.GetStringValue(self._id, 'parameter')
		
	@parameter.setter
	def parameter(self, value : str | ModelParameter):
		interface.SetStringValue(self._id, 'parameter', str(value))

	@property
	def settings(self) -> SensitivitySettings :
		"""Settings for the sensitivity algorithm"""
		self._check_model()
		return self._settings

	def run(self):
		"""Performs the sensitivity analysis
        Results will be stored in `results`, for the first output `parameter` in `result`."""
		self._result = None
		self._results = None

		self._run()

	@property
	def result(self) -> SensitivityResult:
		"""Sensitivity result corresponding with the output parameters"""
		if self._result is None:
			result_id = interface.GetIdValue(self._id, 'result')
			if result_id > 0:
				self._result = SensitivityResult(result_id)

		return self._result

	@property
	def results(self) -> list[SensitivityResult]:
		"""List of sensitivity results, corresponding with the output parameters"""
		if self._results is None:
			results = []
			result_ids = interface.GetArrayIdValue(self._id, 'results')
			for result_id in result_ids:
				result = SensitivityResult(result_id)
				result._set_variables(self.variables)
				results.append(result)
			self._results = FrozenList(results)
				
		return self._results

	@property
	def total_model_runs(self) -> int:
		"""Total model runs performed by the last `run`"""
		return interface.GetIntValue(self._id, 'total_model_runs')

class UncertaintyProject(ModelProject):
	"""Project for an uncertainty analysis. This is the main entry point for performing an uncertainty analysis.

    This class is based on the `ModelProject` class. The model to use is defined in this class in 'ModelProject.model'.
    When a model is set, variables, correlation matrix and settings and settings per variable are generated.

    The uncertainty analysis calculates the uncertainty for the output parameter selected in 'parameter'. If left blank,
    uncertainty analyses are performed for each output parameter.

    To run an uncertainty analysis, use the `run` method. This results in a list of `results`. Each result contains the
    uncertainty for a certain output parameter. Part of the result is a stochast, which contains the distribution
    of the output parameter. A shortcut to the stochasts is available via `stochasts`. The first output parameter
    result can also be accessed by `result` and `stochast`.

    If correlation between output parameters is requested via the `settings`, the resulting correlation matrix can be found
    in `output_correlation_matrix`."""

	def __init__(self):
		super().__init__()
		self._id = interface.Create('uncertainty_project')

		self._stochast = None
		self._stochasts = None
		self._result = None
		self._results = None
		self._output_correlation_matrix = None

		self._initialize_callbacks(self._id)
		self._set_settings(UncertaintySettings())
		super()._freeze()

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
				'stochasts',
				'result',
				'results',
				'output_correlation_matrix',
				'validate',
				'is_valid',
				'total_model_runs']

	@property
	def parameter(self) -> str:
		"""Output parameter for which the uncertainty analysis should be performed, if left blank it will
        be performed for all output parameters"""
		return interface.GetStringValue(self._id, 'parameter')
		
	@parameter.setter
	def parameter(self, value : str | ModelParameter):
		interface.SetStringValue(self._id, 'parameter', str(value))

	@property
	def settings(self) -> UncertaintySettings :
		"""Settings for the uncertainty algorithm"""
		self._check_model()
		return self._settings

	def run(self):
		"""Performs the uncertainty analysis
        Results will be stored in `results`, for the first output `parameter` in `result`. The stochasts in
        the results are available directly in `stochasts` and `stochast`"""

		self._stochast = None
		self._stochasts = None
		self._result = None
		self._results = None
		self._output_correlation_matrix = None

		self._run()

	@property
	def stochast(self) -> Stochast:
		"""Stochast distribution of the first output `parameter`"""
		if self._stochast is None:
			if not self.result is None:
				self._stochast = self.result.variable
		return self._stochast

	@property
	def stochasts(self) -> list[Stochast]:
		"""List of probability distributions, corresponding with the output parameters"""
		if self._stochasts is None:
			stochasts = []
			for result in self.results:
				if not result is None:
					stochasts.append(result.variable)
				else:
					stochasts.append(None)
			self._stochasts = FrozenList(stochasts)
				
		return self._stochasts

	@property
	def result(self) -> UncertaintyResult:
		"""Uncertainty result of the first output parameter"""
		if self._result is None:
			result_id = interface.GetIdValue(self._id, 'uncertainty_result')
			if result_id > 0:
				self._result = UncertaintyResult(result_id, self.variables)

		return self._result

	@property
	def results(self) -> list[UncertaintyResult]:
		"""List of uncertainty results, corresponding with the output parameters"""
		if self._results is None:
			results = []
			result_ids = interface.GetArrayIdValue(self._id, 'uncertainty_results')
			for result_id in result_ids:
				results.append(UncertaintyResult(result_id, self.variables))
			self._results = FrozenList(results)
				
		return self._results

	@property
	def output_correlation_matrix(self) -> CorrelationMatrix:
		"""Correlation matrix between output parameters and possibly with input parameters
        Depends on settings `UncertaintySettings.calculate_correlations` and
        `UncertaintySettings.calculate_input_correlations` whether this correlation matrix is generated"""

		if self._output_correlation_matrix is None:
			correlationMatrixId = interface.GetIdValue(self._id, 'output_correlation_matrix')
			if correlationMatrixId > 0:
				self._output_correlation_matrix = CorrelationMatrix(correlationMatrixId)
				self._output_correlation_matrix._update_variables(self.variables.get_list() + self.stochasts.get_list())
				
		return self._output_correlation_matrix

	@property
	def total_model_runs(self) -> int:
		"""Total model runs performed by the last `run`"""
		return interface.GetIntValue(self._id, 'total_model_runs')

class ReliabilityProject(ModelProject):
	"""Project for reliability analysis. This is the main entry point for performing a reliability analysis.

    This class is based on the `ModelProject` class. The model to use is defined in this class in `ModelProject.model`.
    When a model is set, variables, correlation matrix and settings and settings per variable are generated.

    To run a reliability analysis, use the `run` method. This results in a `design_point`, where the reliability
    index, alpha values and, if specified in the settings, an overview of performed realizations and generated messages."""

	def __init__(self):
		super().__init__()
		self._id = interface.Create('project')

		self._limit_state_function = None
		self._design_point = None
		self._fragility_curve = None
		self._initialized = False

		self._initialize_callbacks(self._id)
		self._set_settings(Settings())
		super()._freeze()
        
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
	def settings(self) -> Settings:
		"""Settings for the algorithm performing the reliability analysis"""
		self._check_model()
		return self._settings

	@property
	def limit_state_function(self) -> LimitStateFunction:
		"""Defines the transformation of the model output to a z-value
        By default, the first value in the model output is used as z-value"""

		if self._limit_state_function is None:
			lsf_id = interface.GetIdValue(self._id, 'limit_state_function')
			if lsf_id > 0:
				self._limit_state_function = LimitStateFunction(lsf_id)
		return self._limit_state_function

	def run(self):
		"""Performs the reliability analysis
        Results in a `design_point`, which is part of this project. When failed, the `design_point` is empty."""

		self._design_point = None
		self._fragility_curve = None
		self._initialized = False
		self._run()

	@property
	def design_point(self) -> DesignPoint:
		"""The resulting design point of a reliability analysis, invoked by `run`
        Is empty when the run failed"""
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
	def total_model_runs(self) -> int:
		"""Total model runs performed by the last `run`"""
		return interface.GetIntValue(self._id, 'total_model_runs')

class CombineProject(FrozenObject):
	"""Project for combining design points. This is the main entry point for performing combining design points.

    The design points to be combined should be added to the list of `design_points`. 

    To run the combination, use the `run` method. This results in a `design_point`, where the
    reliability index reflects the combined reliability index. The original `design_points` are
    added to the `probabilistic_library.reliability.DesignPoint.contributing_design_points` of the
    resulting design point.

    ```mermaid
    classDiagram
        class CombineProject{
            +design_points list[DesignPoint]
            +design_point DesignPoint
            +settings CombineSettings
            +run()
        }

        class Stochast{}
        class CombineSettings{
            +combine_type CombineType
            +combine_method CombineMethod
        }
        class CorrelationMatrix{}
        class SelfCorrelationMatrix{}
        class Stochast{}

        DesignPoint "*, input" <-- CombineProject
        DesignPoint "result" <-- CombineProject
        CombineSettings <-- CombineProject
        CorrelationMatrix <-- CombineProject
        SelfCorrelationMatrix <-- CombineProject
        Stochast "*" <-- CorrelationMatrix
        Stochast "*" <-- SelfCorrelationMatrix
    ```
    """

	def __init__(self):
		self._id = interface.Create('combine_project')

		self._design_points = CallbackList(self._design_points_changed)
		self._settings = CombineSettings()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point_correlation_matrix = CorrelationMatrix()
		self._design_point = None
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['design_points',
				'settings',
				'correlation_matrix',
				'run',
				'design_point_correlation_matrix',
				'design_point']

	def _design_points_changed(self):
		variables = []
		for design_point in self._design_points:
			variables.extend(design_point.get_variables())
		self._correlation_matrix._set_variables(variables)
		self._design_point_correlation_matrix._set_variables(variables)

	@property
	def design_points(self) -> list[DesignPoint]:
		"""List of design points to be combined"""
		return self._design_points

	@property
	def settings(self) -> CombineSettings:
		"""Settings for the combination algorithm"""
		return self._settings

	@property
	def correlation_matrix(self) -> SelfCorrelationMatrix:
		"""Auto correlation matrix, holds correlations between same named variables in the `design_points`"""
		return self._correlation_matrix

	def _update(self):
		interface.SetArrayIntValue(self._id, 'design_points', [design_point._id for design_point in self._design_points])
		interface.SetIntValue(self._id, 'settings', self._settings._id)
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.SetIntValue(self._id, 'design_point_correlation_matrix', self._design_point_correlation_matrix._id)

	def is_valid(self) -> bool:
		"""Indicates whether the combine project is valid"""
		self._update()
		return interface.GetBoolValue(self._id, 'is_valid')

	def validate(self):
		"""Prints the validity of the combine project"""
		self._update()
		id_ = interface.GetIdValue(self._id, 'validate')
		if id_ > 0:
			validation_report = ValidationReport(id_)
			validation_report.print()

	def run(self):
		"""Performs the combination of the design point.
        Results in a `design_point`, which is part of this project. When failed, the `design_point` is empty."""
		self._design_point = None
		# update performed by is_valid
		if (self.is_valid()):
			interface.Execute(self._id, 'run')
		else:
			print('run not executed, input is not valid')

	@property
	def design_point(self) -> DesignPoint:
		"""The resulting combined design point, invoked by `run`
        Is empty when the run failed. The original `design_points` are added to the
        `probabilistic_library.reliability.DesignPoint.contributing_design_point`
        """
		if self._design_point is None:
			designPointId = interface.GetIdValue(self._id, 'design_point')
			if designPointId > 0:
				variables = []
				for design_point in self._design_points:
					variables.extend(design_point.get_variables())
				self._design_point = DesignPoint(designPointId, variables, self._design_points)
		return self._design_point

class ExcludingCombineProject(FrozenObject):
	"""Project for combining design points exclusively or, otherwise stated, design points calculated for a
    scenario. This is the main entry point for this operation.

    Excluding design points refer to design points generated for a scenario. Each scenario has a
    probability too, but scenarios are mutually exclusive. Probabilities of scenarios should add up to 1.

    The design points to be combined should be added to the list of `design_points`. The list of
    `scenarios` should correspond with the list of `design_points`.

    To run the combination, use the `run` method. This results in a `design_point`, where the
    reliability index reflects the combined reliability index. The original `design_points` are
    added to the `probabilistic_library.reliability.DesignPoint.contributing_design_points` of the resulting design point.

    ```mermaid
    classDiagram
        class ExcludingCombineProject{
            +design_points list[DesignPoint]
            +scenarios list[Scenario]
            +settings ExcludingCombineSettings
            +design_point DesignPoint
            +run()
        }

        class DesignPoint{}
        class Scenario{}
        class ExcludingCombineSettings{}

        Scenario "*" <-- ExcludingCombineProject
        DesignPoint "*, input" <-- ExcludingCombineProject
        DesignPoint "result" <-- ExcludingCombineProject
        ExcludingCombineSettings <-- ExcludingCombineProject
    ```
    """

	def __init__(self):
		self._id = interface.Create('excluding_combine_project')

		self._design_points = CallbackList(self._design_points_changed)
		self._scenarios = CallbackList(self._scenarios_changed)
		self._settings = ExcludingCombineSettings()
		self._design_point = None
		self._synchronizing = False
		self._dirty = True

		interface.SetIntValue(self._id, 'settings', self._settings._id)
		super()._freeze()

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
	def design_points(self) -> list[DesignPoint]:
		"""List of design points to be combined"""
		return self._design_points

	@property
	def scenarios(self) -> list[Scenario]:
		"""List of scenarios corresponding with design points to be combined"""
		return self._scenarios

	@property
	def settings(self) -> ExcludingCombineSettings:
		"""Settings for the combine algorithm"""
		return self._settings

	def is_valid(self) -> bool:
		"""Indicates whether the excluding combine project is valid"""
		self._update()
		return interface.GetBoolValue(self._id, 'is_valid')

	def validate(self):
		"""Prints the validity of the excluding combine project"""
		self._update()
		id_ = interface.GetIdValue(self._id, 'validate')
		if id_ > 0:
			validation_report = ValidationReport(id_)
			validation_report.print()

	def run(self):
		"""Performs the excluding combination of the design point.
        Results in a design point, which is part of this project. When failed, the `design_point` is empty."""
		self._update()
		self._design_point = None
		if (self.is_valid()):
			interface.Execute(self._id, 'run')
		else:
			# print validation messages
			self.validate()

	@property
	def design_point(self):
		"""The resulting excluding combined design point, invoked by `run`.
        Is empty when the run failed. The original `design_points` are added to the `contributing_design_points`"""
		if self._design_point is None:
			design_point_id = interface.GetIdValue(self._id, 'design_point')
			if design_point_id > 0:
				variables = []
				for design_point in self._design_points:
					variables.extend(design_point.get_variables())
				self._design_point = DesignPoint(design_point_id, variables, self._design_points)
		return self._design_point

class LengthEffectProject(FrozenObject):
	"""Project for applying the length effect to a design point, also known as upscaling in space. This is
    the main entry point for applying the length effect.

    When a design point is valid for a certain section or cross section, it can be useful to make it
    applicable to a longer section. This operation takes into account the length effect or upscaling.
    Each input variable is valid for a certain length. These lengths are stored in the `correlation_lengths`
    list. The length effect applied to the design point results in a design point valid for the requested `length`.

    To run the combination, use the `LengthEffectProject.run` method. This results in a `design_point`, where the
    reliability index reflects the length effect applied reliability index. The original
    `LengthEffectProject.design_point_cross_section` is added to the
    `probabilistic_library.reliability.DesignPoint.contributing_design_points`  of the resulting design point.

    ```mermaid
    classDiagram
        class LengthEffectProject{
            +design_point_cross_section DesignPoint
            +design_point DesignPoint
            +run()
        }

        class Stochast{}
        class SelfCorrelationMatrix{}
        class Stochast{}

        DesignPoint "input" <-- LengthEffectProject
        DesignPoint "result" <-- LengthEffectProject
        SelfCorrelationMatrix <-- LengthEffectProject
        Stochast "*" <-- SelfCorrelationMatrix
    ```
    """

	def __init__(self):
		self._id = interface.Create('length_effect_project')
		self._design_point_cross_section = DesignPoint()
		self._correlation_matrix = SelfCorrelationMatrix()
		self._design_point = None
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['design_point_cross_section',
				'correlation_lengths'
				'length',
				'correlation_matrix',
				'run',
				'design_point']

	@property
	def design_point_cross_section(self) -> DesignPoint:
		"""The design point to which the length effect will be applied"""
		return self._design_point_cross_section

	@design_point_cross_section.setter
	def design_point_cross_section(self, value: DesignPoint):
		self._design_point_cross_section = value
		variables = value.get_variables()
		self._correlation_matrix._set_variables(variables)

	@property
	def correlation_matrix(self) -> SelfCorrelationMatrix:
		"""Auto correlation matrix, holds correlations of variables in the `design_point_cross_section`"""
		return self._correlation_matrix

	@property
	def correlation_lengths(self) -> list[float]:
		"""List of lengths of the input variables, should match with the input variables"""
		return interface.GetArrayValue(self._id, 'correlation_lengths')

	@correlation_lengths.setter
	def correlation_lengths(self, values : list[float]):
		interface.SetArrayValue(self._id, 'correlation_lengths', values)

	@property
	def length(self) -> float:
		"""Length to be applied to the design point. The resulting design point will be applicable to this length"""
		return interface.GetValue(self._id, 'length')

	@length.setter
	def length(self, value: float):
		interface.SetValue(self._id, 'length', value)

	def run(self):
		"""Applies the length effect to the `design_point_cross_section`.
        Results is a design point, which is part of this project. When failed, the `design_point` is empty."""
		self._design_point = None
		interface.SetIntValue(self._id, 'design_point_cross_section', self._design_point_cross_section._id)
		interface.SetIntValue(self._id, 'correlation_matrix', self._correlation_matrix._id)
		interface.Execute(self._id, 'run')

	@property
	def design_point(self) -> DesignPoint:
		"""The length effect applied design point, invoked by `run`
        Is empty when the run failed. The original `design_point_cross_section` is added to the `DesignPoint.contributing_design_point`"""
		if self._design_point is None:
			designPointId = interface.GetIdValue(self._id, 'design_point')
			if designPointId > 0:
				variables = self._design_point_cross_section.get_variables()
				self._design_point = DesignPoint(designPointId, variables, self._design_point_cross_section)
		return self._design_point

