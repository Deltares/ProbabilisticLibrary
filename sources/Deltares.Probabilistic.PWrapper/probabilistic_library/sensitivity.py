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
This module contains all sensitivity related functionality.

The entry point for performing a sensitivity analysis is `probabilistic_library.project.SensitivityProject`. A model can be attached to a sensitivity project,
then stochastic variables, correlation matrix and settings are provided. When the sensitivity project is run, a sensitivity
result is generated.

```mermaid
classDiagram
    class ModelProject{
        +model ZModel
        +variables list[Stochast]
        +correlation_matrix CorrelationMatrix
    }
    class SensitivityProject{
        +parameter ModelParameter
        +settings SensitivitySettings
        +results list[SensitivityResult]
        +result SensitivityResult
        +run()
    }
    class Stochast{}
    class CorrelationMatrix{}
    class SensitivitySettings{
        +method SensitivityMethod
    }
    class StochastSettings{
        +variable Stochast
    }
    class SensitivityResult{
        +parameter ModelParameter
        +realizations list[Evaluation]
    }
    class Evaluation{}

    SensitivityProject <|-- ModelProject
    Stochast "*" <-- ModelProject
    CorrelationMatrix <-- ModelProject
    SensitivitySettings <-- SensitivityProject
    StochastSettings "*" <-- SensitivitySettings
    Stochast <-- StochastSettings
    SensitivityResult "*" <-- SensitivityProject
    Evaluation "*" <-- SensitivityResult
```
"""

import sys
from math import isnan
from enum import Enum
import matplotlib.pyplot as plt
import numpy as np

from .utils import FrozenObject, FrozenList, PrintUtils
from .statistic import Stochast, ProbabilityValue
from .reliability import StochastSettings, GradientType
from .logging import Evaluation, Message, ValidationReport
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class SensitivityMethod(Enum):
	"""Enumeration which defines the algorithm to perform a sensitivity analysis"""
	single_variation = 'single_variation'
	sobol = 'sobol'
	def __str__(self):
		return str(self.value)

class SensitivitySettings(FrozenObject):
	"""Settings of a sensitivity algorithm

    Settings of all sensitivity algorithms are combined in this class. Often settings only apply to
    a selected number of algorithms. Settings per variable are listed in `stochast_settings`.

    The settings are divided into the following categories:

    Algorithm settings: Settings which are used to control the algorithm, such as variations of the input values.

    Runtime settings: Settings to control how model executions take place and which additional output to produce.
    These settings do not influence the calculated sensitivity."""

	def __init__(self):
		self._id = interface.Create('sensitivity_settings')
		self._stochast_settings = FrozenList()
		self._quantiles = None
		self._synchronizing = False
		super()._freeze()
		interface.SetBoolValue(self._id, 'use_openmp_in_reliability', False)

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['max_parallel_processes',
		        'save_realizations',
		        'save_convergence',
		        'save_messages',
		        'reuse_calculations',
		        'sensitivity_method',
		        'low_value',
		        'high_value',
		        'iterations',
		        'is_valid',
		        'validate']
		
	@property
	def max_parallel_processes(self) -> int:
		"""The number of parallel executions of model evaluations"""
		return interface.GetIntValue(self._id, 'max_parallel_processes')

	@max_parallel_processes.setter
	def max_parallel_processes(self, value : int):
		interface.SetIntValue(self._id, 'max_parallel_processes', value)

	@property
	def save_realizations(self) -> bool:
		"""Indicates whether samples should be saved
        If saved, the samples will be part of the design point"""
		return interface.GetBoolValue(self._id, 'save_realizations')

	@save_realizations.setter
	def save_realizations(self, value : bool):
		interface.SetBoolValue(self._id, 'save_realizations', value)

	@property
	def save_messages(self) -> bool:
		"""Indicates whether messages generated by the reliability analysis will be saved
        If saved, messages will be part of the design point"""
		return interface.GetBoolValue(self._id, 'save_messages')

	@save_messages.setter
	def save_messages(self, value : bool):
		interface.SetBoolValue(self._id, 'save_messages', value)

	@property
	def reuse_calculations(self) -> bool:
		"""Indicates whether prior model results will be reused in the sensitivity analysis.

        This will speed up calculations when several analyses are performed, for which the same realizations
        will have to be executed, for example when calculating the sensitivity of several output parameters. But
        when a modification to the model is made, which is beyond the scope of the model definition, this
        leads to undesired results"""

		return interface.GetBoolValue(self._id, 'reuse_calculations')

	@reuse_calculations.setter
	def reuse_calculations(self, value : bool):
		interface.SetBoolValue(self._id, 'reuse_calculations', value)

	@property
	def sensitivity_method(self) -> SensitivityMethod:
		"""Defines the sensitivity algorithm"""
		return SensitivityMethod[interface.GetStringValue(self._id, 'sensitivity_method')]
		
	@sensitivity_method.setter
	def sensitivity_method(self, value : SensitivityMethod):
		interface.SetStringValue(self._id, 'sensitivity_method', str(value))

	@property
	def iterations(self) -> int:
		"""Number of iterations to apply"""
		return interface.GetIntValue(self._id, 'iterations')

	@iterations.setter
	def iterations(self, value : int):
		interface.SetIntValue(self._id, 'iterations', value)

	@property
	def low_value(self) -> float:
		"""The low value (defined as probability) a variable can be assigned to"""
		return interface.GetValue(self._id, 'low_value')
		
	@low_value.setter
	def low_value(self, value : float):
		interface.SetValue(self._id, 'low_value', value)

	@property
	def high_value(self) -> float:
		"""The high value (defined as probability) a variable can be assigned to"""
		return interface.GetValue(self._id, 'high_value')
		
	@high_value.setter
	def high_value(self, value : float):
		interface.SetValue(self._id, 'high_value', value)

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
		new_stochast_settings = []
		for variable in variables:
			stochast_setting = self._stochast_settings[str(variable)]
			if stochast_setting is None:
				stochast_setting = StochastSettings(variable)
			new_stochast_settings.append(stochast_setting)
		self._stochast_settings = FrozenList(new_stochast_settings)
		interface.SetArrayIntValue(self._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self._stochast_settings])

class SensitivityValue(FrozenObject):
	"""Contains the result of a sensitivity analysis for a specific input variable

    Several sensitivity values (one for each input variable) are listed in a sensitivity result, which
    contains the sensitivity results for a specific output parameter of a model."""

	def __init__(self, id, known_variables = None):
		self._id = id
		self._variable = None
		self._known_variables = known_variables
		super()._freeze()
		
	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['variable',
				'low',
				'medium',
				'high',
				'first_order_index',
				'total_index',
				'print']
		
	def __str__(self):
		return self.variable.name

	@property
	def variable(self) -> Stochast:
		"""The (input) stochastic variable to which these results apply"""
		if self._variable is None:
			variable_id = interface.GetIdValue(self._id, 'variable')
			if variable_id > 0:
				self._variable = self._get_variable_by_id(variable_id)
				
		return self._variable

	def _get_variable_by_id(self, variable_id) -> Stochast:
		if self._known_variables is not None:
			for variable in self._known_variables:
				if variable._id == variable_id:
					return variable
		return Stochast(variable_id)

	@property
	def low(self) -> float:
		"""The value the output parameter gets when the input variable is set to its low value"""
		return interface.GetValue(self._id, 'low')

	@property
	def medium(self) -> float:
		"""The value the output parameter gets when the input variable is set to its median value"""
		return interface.GetValue(self._id, 'medium')

	@property
	def high(self) -> float:
		"""The value the output parameter gets when the input variable is set to its high value"""
		return interface.GetValue(self._id, 'high')

	@property
	def first_order_index(self) -> float:
		"""The first order index of the output parameter due to variations of the input variable"""
		return interface.GetValue(self._id, 'first_order_index')

	@property
	def total_index(self) -> float:
		"""The total index of the output parameter due to variations of the input variable"""
		return interface.GetValue(self._id, 'total_index')

	def print(self, decimals=4):
		"""Prints the sensitivity value

        Parameters
        ----------
        decimals : int, optional
            The number of decimals to print"""

		self._print(0, decimals)

	def _print(self, indent = 0, decimals=4):
		pre = PrintUtils.get_space_from_indent(indent)
		if not isnan(self.low):
			print(pre + f'{self.variable.name}: low = {self.low:.{decimals}g}, medium = {self.medium:.{decimals}g}, high = {self.high:.{decimals}g}')
		elif not isnan(self.total_index):
			print(pre + f'{self.variable.name}: first order index = {self.first_order_index:.{decimals}g}, total index = {self.total_index:.{decimals}g}')


class SensitivityResult(FrozenObject):
	"""Contains the result of a sensitivity analysis for a specific output parameter

    A sensitivity result contains values for each input variable, listed in 'values'."""

	def __init__(self, id):
		self._id = id
		self._values = None
		self._messages = None
		self._realizations = None
		self._known_variables = None
		super()._freeze()
		
	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['identifier',
				'realizations',
				'messages',
				'print',
				'plot']
		
	def __str__(self):
		return self.identifier

	@property
	def identifier(self) -> str:
		"""Identification of the output parameter"""
		return interface.GetStringValue(self._id, 'identifier')

	@property
	def values(self) -> list[SensitivityValue]:
		"""List of sensitivity values per input variable. The values in this list contain the sensitivity results."""
		if self._values is None:
			sens_values = []
			sens_value_ids = interface.GetArrayIdValue(self._id, 'values')
			for sens_value_id in sens_value_ids:
				sens_values.append(SensitivityValue(sens_value_id, self._known_variables))
			self._values = FrozenList(sens_values)
				
		return self._values

	@property
	def realizations(self) -> list[Evaluation]:
		"""List of samples calculated by the reliability algorithm. Depends on the setting 'save_realizations' whether
        this list is provided"""
		if self._realizations is None:
			realizations = []
			realization_ids = interface.GetArrayIdValue(self._id, 'evaluations')
			for realization_id in realization_ids:
				realizations.append(Evaluation(realization_id))
			self._realizations = FrozenList(realizations)
				
		return self._realizations
	
	@property
	def messages(self) -> list[Message]:
		"""List of messages generated by the reliability algorithm. Depends on the setting 'save_messages' whether this
        list is provided"""
		if self._messages is None:
			messages = []
			message_ids = interface.GetArrayIdValue(self._id, 'messages')
			for message_id in message_ids:
				messages.append(Message(message_id))
			self._messages = FrozenList(messages)
				
		return self._messages

	def _set_variables(self, variables):
		self._known_variables = FrozenList(variables)

	def print(self, decimals=4):
		"""Prints the sensitivity result, including sensitivity values for each input variable.

        Parameters
        ----------
        decimals : int, optional.
            The number of decimals to print"""

		print('Parameter: ' + self.identifier)
		if len(self.values) > 0:
			print('Values:')
			for value in self.values:
				value._print(1, decimals)

	def plot(self):
		"""Shows a plot of the sensitivity in the form of a bar-chart"""
		self.get_plot().show()

	def get_plot(self) -> plt:
		"""Gets a plot object of the sensitivity in the form of a bar-chart"""

		plt.subplot()

		low_values = [value.low for value in self.values if not isnan(value.low)]
		medium_values = [value.medium for value in self.values if not isnan(value.medium)]
		high_values = [value.high for value in self.values if not isnan(value.high)]
		first_values = [value.first_order_index for value in self.values if not isnan(value.first_order_index)]
		total_values = [value.total_index for value in self.values if not isnan(value.total_index)]

		meaningful_values_count = sum(len(values) > 0 for values in [low_values, medium_values, high_values, first_values, total_values])

		bar_width = 1.0 / (meaningful_values_count + 1)

		x = np.arange(len(self.values)) 

		if len(low_values) > 0:
			plt.bar(x, low_values, color ='b', width = bar_width, label ='low')
			x = [x + bar_width for x in x] 

		if len(medium_values) > 0:
			plt.bar(x, medium_values, color ='r', width = bar_width, label ='medium')
			x = [x + bar_width for x in x] 

		if len(high_values) > 0:
			plt.bar(x, high_values, color ='g', width = bar_width, label ='high')
			x = [x + bar_width for x in x] 

		if len(first_values) > 0:
			plt.bar(x, first_values, color ='y', width = bar_width, label ='first order')
			x = [x + bar_width for x in x] 

		if len(total_values) > 0:
			plt.bar(x, total_values, color ='m', width = bar_width, label ='total')
			x = [x + bar_width for x in x] 

		plt.xlabel('variables') 
		plt.ylabel(self.identifier)

		increment = (meaningful_values_count - 1) / 2 * bar_width
		plt.xticks([r + increment for r in range(len(self.values))], [value.variable.name for value in self.values])

		plt.legend()

		return plt
