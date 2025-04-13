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
from enum import Enum

from .utils import *
from .statistic import Stochast, ProbabilityValue
from .reliability import StochastSettings, RandomType, GradientType, Evaluation, Message
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class SensitivityMethod(Enum):
	form = 'form'
	fosm = 'fosm'
	numerical_integration = 'numerical_integration'
	crude_monte_carlo = 'crude_monte_carlo'
	importance_sampling = 'importance_sampling'
	directional_sampling = 'directional_sampling'
	def __str__(self):
		return str(self.value)

class SensitivitySettings(FrozenObject):

	def __init__(self):
		self._id = interface.Create('sensitivity_settings')
		self._stochast_settings = FrozenList()
		self._quantiles = None
		self._synchronizing = False
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['max_parallel_processes',
		        'save_realizations',
		        'save_convergence',
		        'save_messages',
		        'sensitivity_method',
	            'random_type',
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
	            'stochast_settings']

		
	@property
	def max_parallel_processes(self) -> int:
		return interface.GetIntValue(self._id, 'max_parallel_processes')

	@max_parallel_processes.setter
	def max_parallel_processes(self, value : int):
		interface.SetIntValue(self._id, 'max_parallel_processes', value)

	@property
	def save_realizations(self) -> bool:
		return interface.GetBoolValue(self._id, 'save_realizations')

	@save_realizations.setter
	def save_realizations(self, value : bool):
		interface.SetBoolValue(self._id, 'save_realizations', value)

	@property
	def save_convergence(self) -> bool:
		return interface.GetBoolValue(self._id, 'save_convergence')

	@save_convergence.setter
	def save_convergence(self, value : bool):
		interface.SetBoolValue(self._id, 'save_convergence', value)

	@property
	def save_messages(self) -> bool:
		return interface.GetBoolValue(self._id, 'save_messages')

	@save_messages.setter
	def save_messages(self, value : bool):
		interface.SetBoolValue(self._id, 'save_messages', value)

	@property
	def sensitivity_method(self) -> SensitivityMethod:
		return SensitivityMethod[interface.GetStringValue(self._id, 'sensitivity_method')]
		
	@sensitivity_method.setter
	def sensitivity_method(self, value : SensitivityMethod):
		interface.SetStringValue(self._id, 'sensitivity_method', str(value))

	@property
	def random_type(self) -> RandomType:
		return RandomType[interface.GetStringValue(self._id, 'random_type')]
		
	@random_type.setter
	def random_type(self, value : RandomType):
		interface.SetStringValue(self._id, 'random_type', str(value))

	@property
	def is_repeatable_random(self) -> bool:
		return interface.GetBoolValue(self._id, 'is_repeatable_random')
		
	@is_repeatable_random.setter
	def is_repeatable_random(self, value : bool):
		interface.SetBoolValue(self._id, 'is_repeatable_random', value)

	@property
	def random_seed(self) -> int:
		return interface.GetIntValue(self._id, 'random_seed')
		
	@random_seed.setter
	def random_seed(self, value : int):
		interface.SetIntValue(self._id, 'random_seed', value)

	@property
	def minimum_samples(self) -> int:
		return interface.GetIntValue(self._id, 'minimum_samples')
		
	@minimum_samples.setter
	def minimum_samples(self, value : int):
		interface.SetIntValue(self._id, 'minimum_samples', value)

	@property
	def maximum_samples(self) -> int:
		return interface.GetIntValue(self._id, 'maximum_samples')
		
	@maximum_samples.setter
	def maximum_samples(self, value : int):
		interface.SetIntValue(self._id, 'maximum_samples', value)

	@property
	def maximum_iterations(self) -> int:
		return interface.GetIntValue(self._id, 'maximum_iterations')
		
	@maximum_iterations.setter
	def maximum_iterations(self, value : int):
		interface.SetIntValue(self._id, 'maximum_iterations', value)

	@property
	def minimum_directions(self) -> int:
		return interface.GetIntValue(self._id, 'minimum_directions')
		
	@minimum_directions.setter
	def minimum_directions(self, value : int):
		interface.SetIntValue(self._id, 'minimum_directions', value)

	@property
	def maximum_directions(self) -> int:
		return interface.GetIntValue(self._id, 'maximum_directions')
		
	@maximum_directions.setter
	def maximum_directions(self, value : int):
		interface.SetIntValue(self._id, 'maximum_directions', value)

	@property
	def minimum_u(self) -> float:
		return interface.GetValue(self._id, 'minimum_u')
		
	@minimum_u.setter
	def minimum_u(self, value : float):
		interface.SetValue(self._id, 'minimum_u', value)

	@property
	def maximum_u(self) -> float:
		return interface.GetValue(self._id, 'maximum_u')
		
	@maximum_u.setter
	def maximum_u(self, value : float):
		interface.SetValue(self._id, 'maximum_u', value)

	@property
	def global_step_size(self) -> float:
		return interface.GetValue(self._id, 'global_step_size')
		
	@global_step_size.setter
	def global_step_size(self, value : float):
		interface.SetValue(self._id, 'global_step_size', value)

	@property
	def step_size(self) -> float:
		return interface.GetValue(self._id, 'step_size')
		
	@step_size.setter
	def step_size(self, value : float):
		interface.SetValue(self._id, 'step_size', value)

	@property
	def gradient_type(self) -> GradientType:
		return GradientType[interface.GetStringValue(self._id, 'gradient_type')]
		
	@gradient_type.setter
	def gradient_type(self, value : GradientType):
		interface.SetStringValue(self._id, 'gradient_type', str(value))

	@property
	def relaxation_factor(self) -> float:
		return interface.GetValue(self._id, 'relaxation_factor')
		
	@relaxation_factor.setter
	def relaxation_factor(self, value : float):
		interface.SetValue(self._id, 'relaxation_factor', value)

	@property
	def variance_factor(self) -> float:
		return interface.GetValue(self._id, 'variance_factor')
		
	@variance_factor.setter
	def variance_factor(self, value : float):
		interface.SetValue(self._id, 'variance_factor', value)

	@property
	def variation_coefficient(self) -> float:
		return interface.GetValue(self._id, 'variation_coefficient')
		
	@variation_coefficient.setter
	def variation_coefficient(self, value : float):
		interface.SetValue(self._id, 'variation_coefficient', value)

	@property
	def probability_for_convergence(self) -> float:
		return interface.GetValue(self._id, 'probability_for_convergence')
		
	@probability_for_convergence.setter
	def probability_for_convergence(self, value : float):
		interface.SetValue(self._id, 'probability_for_convergence', value)

	@property
	def derive_samples_from_variation_coefficient(self) -> bool:
		return interface.GetBoolValue(self._id, 'derive_samples_from_variation_coefficient')
		
	@derive_samples_from_variation_coefficient.setter
	def derive_samples_from_variation_coefficient(self, value : bool):
		interface.SetBoolValue(self._id, 'derive_samples_from_variation_coefficient', value)

	@property   
	def calculate_correlations(self) -> bool:
		return interface.GetBoolValue(self._id, 'calculate_correlations')
		
	@calculate_correlations.setter
	def calculate_correlations(self, value : bool):
		interface.SetBoolValue(self._id, 'calculate_correlations', value) 

	@property   
	def calculate_input_correlations(self) -> bool:
		return interface.GetBoolValue(self._id, 'calculate_input_correlations')
		
	@calculate_input_correlations.setter
	def calculate_input_correlations(self, value : bool):
		interface.SetBoolValue(self._id, 'calculate_input_correlations', value) 

	@property   
	def stochast_settings(self) -> list[StochastSettings]:
		return self._stochast_settings

	@property
	def quantiles(self) -> list[ProbabilityValue]:
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

	def _set_variables(self, variables):
		new_stochast_settings = []
		for variable in variables:
			stochast_setting = self._stochast_settings[str(variable)]
			if stochast_setting is None:
				stochast_setting = StochastSettings(variable)
			new_stochast_settings.append(stochast_setting)
		self._stochast_settings = FrozenList(new_stochast_settings)
		interface.SetArrayIntValue(self._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self._stochast_settings])

class SensitivityResult(FrozenObject):

	def __init__(self, id):
		self._id = id
		self._variable = None
		self._messages = None
		self._realizations = None
		self._quantile_realizations = None
		super()._freeze()
		
	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['identifier',
				'variable',
				'quantile_realizations',
				'realizations',
				'messages']
		
	def __str__(self):
		return self.identifier

	@property
	def identifier(self) -> str:
		return interface.GetStringValue(self._id, 'identifier')

	@property
	def variable(self) -> Stochast:
		if self._variable is None:
			variable_id = interface.GetIdValue(self._id, 'variable')
			if variable_id > 0:
				self._variable = Stochast(variable_id);
				
		return self._variable

	@property
	def realizations(self) -> list[Evaluation]:
		if self._realizations is None:
			realizations = []
			realization_ids = interface.GetArrayIdValue(self._id, 'evaluations')
			for realization_id in realization_ids:
				realizations.append(Evaluation(realization_id))
			self._realizations = FrozenList(realizations)
				
		return self._realizations
	
	@property
	def quantile_realizations(self) -> list[Evaluation]:
		if self._quantile_realizations is None:
			quantile_realizations = []
			quantile_realization_ids = interface.GetArrayIdValue(self._id, 'quantile_evaluations')
			for realization_id in quantile_realization_ids:
				quantile_realizations.append(Evaluation(realization_id))
			self._quantile_realizations = FrozenList(quantile_realizations)
				
		return self._quantile_realizations
	
	@property
	def messages(self) -> list[Message]:
		if self._messages is None:
			messages = []
			message_ids = interface.GetArrayIdValue(self._id, 'messages')
			for message_id in message_ids:
				messages.append(Message(message_id))
			self._messages = FrozenList(messages)
				
		return self._messages	
