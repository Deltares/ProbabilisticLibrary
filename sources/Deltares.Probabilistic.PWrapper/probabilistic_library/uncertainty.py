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
This module contains all uncertainty related functionality.

The entry point for performing an uncertainty analysis is `probabilistic_library.project.UncertaintyProject`. A model
can be attached to an uncertainty project, then stochastic variables, correlation matrix and settings are provided. When
the uncertainty project is run, an uncertainty result is generated, which contains a stochastic variable, which resembles
the variation of the output of the model.

```mermaid
classDiagram
    class ModelProject{
        +model ZModel
        +variables list[Stochast]
        +correlation_matrix CorrelationMatrix
    }
    class UncertaintyProject{
        +parameter ModelParameter
        +settings UncertaintySettings
        +results list[UncertaintyResult]
        +result UncertaintyResult
        +run()
    }
    class Stochast{}
    class CorrelationMatrix{}
    class ProbabilityValue{}
    class UncertaintySettings{
        +method UncertaintyMethod
        +quantiles list[ProbabilityValue]
    }
    class StochastSettings{
        +variable Stochast
    }
    class UncertaintyResult{
        +parameter ModelParameter
        +variable Stochast
        +realizations list[Evaluation]
    }
    class Evaluation{}

    UncertaintyProject <|-- ModelProject
    Stochast "*" <-- ModelProject
    CorrelationMatrix <-- ModelProject
    UncertaintySettings <-- UncertaintyProject
    StochastSettings "*" <-- UncertaintySettings
    ProbabilityValue "*" <-- UncertaintySettings
    Stochast <-- StochastSettings
    UncertaintyResult "*" <-- UncertaintyProject
    Stochast <-- UncertaintyResult
    Evaluation "*" <-- UncertaintyResult
```
"""

import sys
from math import isnan
from enum import Enum
import matplotlib.pyplot as plt

from .utils import FrozenObject, FrozenList, CallbackList
from .logging import Evaluation, Message, ValidationReport
from .statistic import Stochast, ProbabilityValue
from .reliability import StochastSettings, GradientType
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class UncertaintyMethod(Enum):
	"""Enumeration which defines the algorithm to perform an uncertainty analysis"""
	form = 'form'
	fosm = 'fosm'
	numerical_integration = 'numerical_integration'
	crude_monte_carlo = 'crude_monte_carlo'
	importance_sampling = 'importance_sampling'
	directional_sampling = 'directional_sampling'
	def __str__(self):
		return str(self.value)

class UncertaintySettings(FrozenObject):
	"""Settings of an uncertainty algorithm

    Settings of all uncertainty algorithms are combined in this class. Often settings only apply to
    a selected number of algorithms. Settings per variable are listed in `stochast_settings`.

    The settings are divided into the following categories:

    Algorithm settings: Settings which are used to control the algorithm, such as the number of allowed
    samples and the convergence criterion.

    Runtime settings: Settings to control how model executions take place and which additional output to produce.
    These settings do not influence the calculated uncertainty."""

	def __init__(self):
		self._id = interface.Create('uncertainty_settings')
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
		        'save_messages',
		        'reuse_calculations'
		        'uncertainty_method',
				'is_repeatable_random',
				'random_seed',
	            'minimum_samples',
	            'maximum_samples',
	            'maximum_iterations',
	            'minimum_directions',
	            'maximum_directions',
	            'minimum_u',
	            'maximum_u',
	            'step_size',
		        'gradient_type',
	            'global_step_size',
	            'variation_coefficient',
			    'probability_for_convergence',
			    'derive_samples_from_variation_coefficient',
			    'calculate_correlations',
			    'calculate_input_correlations',
			    'quantiles',
	            'stochast_settings',
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
		"""Indicates whether messages generated by the uncertainty analysis will be saved
        If saved, messages will be part of the design point"""
		return interface.GetBoolValue(self._id, 'save_messages')

	@save_messages.setter
	def save_messages(self, value : bool):
		interface.SetBoolValue(self._id, 'save_messages', value)

	@property
	def reuse_calculations(self) -> bool:
		"""Indicates whether prior model results will be reused by the uncertainty analysis.

        This will speed up calculations when several analyses are performed, for which the same realizations
        will have to be executed, for example a Crude Monte Carlo analysis for different output parameters.
        But when a modification to the model is made, which is beyond the scope of the  model definition,
        this leads to undesired results"""

		return interface.GetBoolValue(self._id, 'reuse_calculations')

	@reuse_calculations.setter
	def reuse_calculations(self, value : bool):
		interface.SetBoolValue(self._id, 'reuse_calculations', value)

	@property
	def uncertainty_method(self) -> UncertaintyMethod:
		"""Defines the uncertainty algorithm"""
		return UncertaintyMethod[interface.GetStringValue(self._id, 'uncertainty_method')]
		
	@uncertainty_method.setter
	def uncertainty_method(self, value : UncertaintyMethod):
		interface.SetStringValue(self._id, 'uncertainty_method', str(value))

	@property
	def is_repeatable_random(self) -> bool:
		"""Indicates whether in each run the same random samples will be generated"""
		return interface.GetBoolValue(self._id, 'is_repeatable_random')
		
	@is_repeatable_random.setter
	def is_repeatable_random(self, value : bool):
		interface.SetBoolValue(self._id, 'is_repeatable_random', value)

	@property
	def random_seed(self) -> int:
		"""Seed number for the random generator"""
		return interface.GetIntValue(self._id, 'random_seed')
		
	@random_seed.setter
	def random_seed(self, value : int):
		interface.SetIntValue(self._id, 'random_seed', value)

	@property
	def minimum_samples(self) -> int:
		"""The minimum number of samples to be used"""
		return interface.GetIntValue(self._id, 'minimum_samples')
		
	@minimum_samples.setter
	def minimum_samples(self, value : int):
		interface.SetIntValue(self._id, 'minimum_samples', value)

	@property
	def maximum_samples(self) -> int:
		"""The maximum number of samples to be used"""
		return interface.GetIntValue(self._id, 'maximum_samples')
		
	@maximum_samples.setter
	def maximum_samples(self, value : int):
		interface.SetIntValue(self._id, 'maximum_samples', value)

	@property
	def maximum_iterations(self) -> int:
		"""The maximum number of iterations to be used, only for FORM"""
		return interface.GetIntValue(self._id, 'maximum_iterations')
		
	@maximum_iterations.setter
	def maximum_iterations(self, value : int):
		interface.SetIntValue(self._id, 'maximum_iterations', value)

	@property
	def minimum_directions(self) -> int:
		"""The minimum number of directions to be used, only for directional sampling"""
		return interface.GetIntValue(self._id, 'minimum_directions')
		
	@minimum_directions.setter
	def minimum_directions(self, value : int):
		interface.SetIntValue(self._id, 'minimum_directions', value)

	@property
	def maximum_directions(self) -> int:
		"""The maximum number of directions to be used, only for directional sampling"""
		return interface.GetIntValue(self._id, 'maximum_directions')
		
	@maximum_directions.setter
	def maximum_directions(self, value : int):
		interface.SetIntValue(self._id, 'maximum_directions', value)

	@property
	def minimum_u(self) -> float:
		"""The minimum u-value to be applied to a stochastic variable, used for FORM"""
		return interface.GetValue(self._id, 'minimum_u')
		
	@minimum_u.setter
	def minimum_u(self, value : float):
		interface.SetValue(self._id, 'minimum_u', value)

	@property
	def maximum_u(self) -> float:
		"""The maximum u-value to be applied to a stochastic variable, used for FORM"""
		return interface.GetValue(self._id, 'maximum_u')
		
	@maximum_u.setter
	def maximum_u(self, value : float):
		interface.SetValue(self._id, 'maximum_u', value)

	@property
	def global_step_size(self) -> float:
		"""Step size between fragility values, which is the result if a FORM calculation"""
		return interface.GetValue(self._id, 'global_step_size')
		
	@global_step_size.setter
	def global_step_size(self, value : float):
		interface.SetValue(self._id, 'global_step_size', value)

	@property
	def step_size(self) -> float:
		"""Step size in calculating the model gradient, used by FORM"""
		return interface.GetValue(self._id, 'step_size')
		
	@step_size.setter
	def step_size(self, value : float):
		interface.SetValue(self._id, 'step_size', value)

	@property
	def gradient_type(self) -> GradientType:
		"""Method to determine the gradient of a model"""
		return GradientType[interface.GetStringValue(self._id, 'gradient_type')]
		
	@gradient_type.setter
	def gradient_type(self, value : GradientType):
		interface.SetStringValue(self._id, 'gradient_type', str(value))

	@property
	def variance_factor(self) -> float:
		"""Variance factor, used by importance sampling"""
		return interface.GetValue(self._id, 'variance_factor')
		
	@variance_factor.setter
	def variance_factor(self, value : float):
		interface.SetValue(self._id, 'variance_factor', value)

	@property
	def variation_coefficient(self) -> float:
		"""Convergence criterion, used by Monte Carlo family algorithms"""
		return interface.GetValue(self._id, 'variation_coefficient')
		
	@variation_coefficient.setter
	def variation_coefficient(self, value : float):
		interface.SetValue(self._id, 'variation_coefficient', value)

	@property
	def probability_for_convergence(self) -> float:
		"""Probability to which the convergence applies

        The convergence is calculated as if it were a reliability analysis. The reliability analysis is tuned
        in such a way (by modification of the failure definition) that it produces the given probability."""
        
		return interface.GetValue(self._id, 'probability_for_convergence')
		
	@probability_for_convergence.setter
	def probability_for_convergence(self, value : float):
		interface.SetValue(self._id, 'probability_for_convergence', value)

	@property
	def derive_samples_from_variation_coefficient(self) -> bool:
		"""Indicates that the number of samples is derived from the variation coefficient and the probability of
        failure (only for Crude Monte Carlo)"""
		return interface.GetBoolValue(self._id, 'derive_samples_from_variation_coefficient')
		
	@derive_samples_from_variation_coefficient.setter
	def derive_samples_from_variation_coefficient(self, value : bool):
		interface.SetBoolValue(self._id, 'derive_samples_from_variation_coefficient', value)

	@property   
	def calculate_correlations(self) -> bool:
		"""Indicates that correlations between output parameters must be calculated"""
		return interface.GetBoolValue(self._id, 'calculate_correlations')
		
	@calculate_correlations.setter
	def calculate_correlations(self, value : bool):
		interface.SetBoolValue(self._id, 'calculate_correlations', value) 

	@property   
	def calculate_input_correlations(self) -> bool:
		"""Indicates that correlations between output parameters and input parameters must be calculated"""
		return interface.GetBoolValue(self._id, 'calculate_input_correlations')
		
	@calculate_input_correlations.setter
	def calculate_input_correlations(self, value : bool):
		interface.SetBoolValue(self._id, 'calculate_input_correlations', value) 

	@property   
	def stochast_settings(self) -> list[StochastSettings]:
		"""List of settings specified per stochastic variable"""
		return self._stochast_settings

	@property
	def quantiles(self) -> list[ProbabilityValue]:
		"""List of quantiles of the output parameter, which must be calculated"""
		if self._quantiles is None:
			self._synchronizing = True
			self._quantiles = CallbackList(self._quantiles_changed)
			quantile_ids = interface.GetArrayIntValue(self._id, 'quantiles')
			for quantile_id in quantile_ids:
				self._quantiles.append(ProbabilityValue(quantile_id))
			self._synchronizing = False

		return self._quantiles

	def _quantiles_changed(self):
		if not self._synchronizing:
			# replace floats by ProbabilityValue
			self._synchronizing = True
			for i in range(len(self._quantiles)):
				if type(self._quantiles[i]) == int or type(self._quantiles[i]) == float:
					val = self._quantiles[i]
					self._quantiles[i] = ProbabilityValue()
					self._quantiles[i].probability_of_non_failure = val
			self._synchronizing = False

			interface.SetArrayIntValue(self._id, 'quantiles', [quantile._id for quantile in self._quantiles])

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

class UncertaintyResult(FrozenObject):
	"""Contains the result of an uncertainty analysis for a specific output parameter

    An uncertainty analysis results in a stochastic variable. The characteristics of this stochastic variable describe the
    uncertainty of the output parameter. Depending on the uncertainty algorithm different distributions may be used."""

	def __init__(self, id : int, variables : list[Stochast]):
		self._id = id
		self._variable = None
		self._messages = None
		self._realizations = None
		self._quantile_realizations = None
		self._variables = FrozenList(variables)
		super()._freeze()
		
	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['identifier',
		        'variable',
		        'quantile_realizations',
		        'realizations',
		        'messages',
		        'print',
		        'plot',
		        'plot_realizations',
		        'get_plot',
		        'get_plot_realizations']
		
	def __str__(self):
		return self.identifier

	@property
	def identifier(self) -> str:
		"""Identification of the output parameter"""
		return interface.GetStringValue(self._id, 'identifier')

	@property
	def variable(self) -> Stochast:
		"""Stochastic variable describing the uncertainty of the output parameter"""
		if self._variable is None:
			variable_id = interface.GetIdValue(self._id, 'variable')
			if variable_id > 0:
				self._variable = Stochast(variable_id);
				
		return self._variable

	@property
	def realizations(self) -> list[Evaluation]:
		"""List of samples calculated by the uncertainty algorithm. Depends on the setting `UncertaintySettings.save_realizations` whether
        this list is provided"""
		if self._realizations is None:
			realizations = []
			realization_ids = interface.GetArrayIdValue(self._id, 'evaluations')
			for realization_id in realization_ids:
				realizations.append(Evaluation(realization_id))
			self._realizations = FrozenList(realizations)
				
		return self._realizations
	
	@property
	def quantile_realizations(self) -> list[Evaluation]:
		"""List of samples corresponding with the list of quantiles in the uncertainty settings.
       The samples in this list are the closest samples to the given quantiles"""
		if self._quantile_realizations is None:
			quantile_realizations = []
			quantile_realization_ids = interface.GetArrayIdValue(self._id, 'quantile_evaluations')
			for realization_id in quantile_realization_ids:
				quantile_realizations.append(Evaluation(realization_id))
			self._quantile_realizations = FrozenList(quantile_realizations)
				
		return self._quantile_realizations
	
	@property
	def messages(self) -> list[Message]:
		"""List of messages generated by the reliability algorithm. Depends on the setting `UncertaintySettings.save_messages` whether this
        list is provided"""
		if self._messages is None:
			messages = []
			message_ids = interface.GetArrayIdValue(self._id, 'messages')
			for message_id in message_ids:
				messages.append(Message(message_id))
			self._messages = FrozenList(messages)
				
		return self._messages

	def print(self, decimals=4):
		"""Prints the resulting stochastic variable and quantile samples.

        Parameters
        ----------
        decimals : int, optional
            The number of decimals to print"""
		self.variable.print(decimals)
		if len(self.quantile_realizations) > 0:
			print('Quantiles:')
			for quantile in self.quantile_realizations:
				quantile._print(1, decimals)

	def plot(self, xmin : float = None, xmax : float = None):
		"""Shows a plot of the resulting stochastic variable"""
		self.get_plot(xmin, xmax).show()

	def get_plot(self, xmin : float = None, xmax : float = None) -> plt:
		"""Gets a plot object of the resulting stochastic variable"""

		vplot = self.variable.get_plot(xmin, xmax)

		plot_legend = False
		for ii in range(len(self.quantile_realizations)):
			vplot.axvline(x=self.quantile_realizations[ii].output_values[0], color="green", linestyle="--", label=f"{self.quantile_realizations[ii].quantile:.4g}-quantile")
			plot_legend = True

		if plot_legend:
			vplot.legend()

		return vplot

	def plot_realizations(self, var_x : str | Stochast = None, var_y : str | Stochast = None):
		"""Shows a scatter-plot of realizations performed by the uncertainty analysis. The x-y coordinates
        correspond with realization input values. Only available when `UncertaintySettings.save_realizations`
        was set in the `UncertaintySettings`.

        Parameters
        ----------
        var_x : str | Stochast, optional
            The stochastic variable to use for the x-axis, if omitted the first variable is used

        var_y : str | Stochast, optional
            The stochastic variable to use for the y-axis, if omitted the second variable is used"""

		self.get_plot_realizations(var_x, var_y).show()

	def get_plot_realizations(self, var_x : str | Stochast = None, var_y : str | Stochast = None) -> plt:
		"""Gets a plot object of a scatter-plot of realizations performed by the uncertainty analysis. The
        x-y coordinates correspond with realization input values. Only available when `UncertaintySettings.save_realizations`
        was set in the `UncertaintySettings`.

        Parameters
        ----------
        var_x : str | Stochast, optional
            The stochastic variable to use for the x-axis, if omitted the first variable is used.

        var_y : str | Stochast, optional
            The stochastic variable to use for the y-axis, if omitted the second variable is used"""

		if len(self.realizations) == 0:
			print ("No realizations were saved, run again with settings.save_realizations = True")
			return plt

		if len(self._variables) < 2:
			print ("Not enough variables to plot realizations")
			return plt

		plt.close()

		# the first two variables
		index = [0, 1]

		if var_x is not None:
			index[0] = self._variables.index(var_x)
		if var_y is not None:
			index[1] = self._variables.index(var_y)

		if index[0] < 0 or index[1] < 0:
			print ("Variables could not be found")
			return

		x_values = [realization.input_values[index[0]] for realization in self.realizations]
		y_values = [realization.input_values[index[1]] for realization in self.realizations]

		# plot realizations
		plt.figure()
		plt.grid(True)    
		plt.scatter(x_values, y_values, color='b')

		x_values = [realization.input_values[index[0]] for realization in self.quantile_realizations]
		y_values = [realization.input_values[index[1]] for realization in self.quantile_realizations]

		plt.scatter(x_values, y_values, color='black')

		plt.xlabel(self._variables[index[0]].name)
		plt.ylabel(self._variables[index[1]].name)

		plt.title('Realizations', fontsize=14)

		return plt
