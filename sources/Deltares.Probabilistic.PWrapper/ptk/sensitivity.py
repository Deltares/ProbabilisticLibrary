import sys
from enum import Enum

from .utils import *
from .statistic import Stochast
from .reliability import StochastSettings, RandomType
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class SensitivityMethod(Enum):
	form = 'form'
	numerical_integration = 'numerical_integration'
	crude_monte_carlo = 'crude_monte_carlo'
	importance_sampling = 'importance_sampling'
	directional_sampling = 'directional_sampling'
	def __str__(self):
		return str(self.value)

class SensitivitySettings:
		  
	def __init__(self):
		self._id = interface.Create('sensitivity_settings')
		self._stochast_settings = FrozenList()

	def __dir__(self):
		return ['sensitivity_method',
	            'random_type',
	            'minimum_samples',
	            'maximum_samples',
	            'maximum_iterations',
	            'minimum_directions',
	            'maximum_directions',
	            'relaxation_factor',
	            'relaxation_loops',
	            'variation_coefficient',
			    'probability_for_convergence'
			    'derive_samples_from_variation_coefficient'
			    'calculate_correlations'
			    'calculate_input_correlations'
	            'stochast_settings']

		
	@property   
	def sensitivity_method(self):
		return SensitivityMethod[interface.GetStringValue(self._id, 'sensitivity_method')]
		
	@sensitivity_method.setter
	def sensitivity_method(self, value : SensitivityMethod):
		interface.SetStringValue(self._id, 'sensitivity_method', str(value))

	@property   
	def random_type(self):
		return RandomType[interface.GetStringValue(self._id, 'random_type')]
		
	@random_type.setter
	def random_type(self, value : RandomType):
		interface.SetStringValue(self._id, 'random_type', str(value))

	@property   
	def minimum_samples(self):
		return interface.GetIntValue(self._id, 'minimum_samples')
		
	@minimum_samples.setter
	def minimum_samples(self, value : int):
		interface.SetIntValue(self._id, 'minimum_samples', value)

	@property   
	def maximum_samples(self):
		return interface.GetIntValue(self._id, 'maximum_samples')
		
	@maximum_samples.setter
	def maximum_samples(self, value : int):
		interface.SetIntValue(self._id, 'maximum_samples', value)

	@property   
	def maximum_iterations(self):
		return interface.GetIntValue(self._id, 'maximum_iterations')
		
	@maximum_iterations.setter
	def maximum_iterations(self, value : int):
		interface.SetIntValue(self._id, 'maximum_iterations', value)

	@property   
	def minimum_directions(self):
		return interface.GetIntValue(self._id, 'minimum_directions')
		
	@minimum_directions.setter
	def minimum_directions(self, value : int):
		interface.SetIntValue(self._id, 'minimum_directions', value)

	@property   
	def maximum_directions(self):
		return interface.GetIntValue(self._id, 'maximum_directions')
		
	@maximum_directions.setter
	def maximum_directions(self, value : int):
		interface.SetIntValue(self._id, 'maximum_directions', value)

	@property   
	def relaxation_factor(self):
		return interface.GetValue(self._id, 'relaxation_factor')
		
	@relaxation_factor.setter
	def relaxation_factor(self, value : float):
		interface.SetValue(self._id, 'relaxation_factor', value)

	@property   
	def variation_coefficient(self):
		return interface.GetValue(self._id, 'variation_coefficient')
		
	@variation_coefficient.setter
	def variation_coefficient(self, value : float):
		interface.SetValue(self._id, 'variation_coefficient', value) 

	@property   
	def probability_for_convergence(self):
		return interface.GetValue(self._id, 'probability_for_convergence')
		
	@probability_for_convergence.setter
	def probability_for_convergence(self, value : float):
		interface.SetValue(self._id, 'probability_for_convergence', value) 

	@property   
	def derive_samples_from_variation_coefficient(self):
		return interface.GetBoolValue(self._id, 'derive_samples_from_variation_coefficient')
		
	@derive_samples_from_variation_coefficient.setter
	def derive_samples_from_variation_coefficient(self, value : bool):
		interface.SetBoolValue(self._id, 'derive_samples_from_variation_coefficient', value)

	@property   
	def calculate_correlations(self):
		return interface.GetBoolValue(self._id, 'calculate_correlations')
		
	@calculate_correlations.setter
	def calculate_correlations(self, value : bool):
		interface.SetBoolValue(self._id, 'calculate_correlations', value) 

	@property   
	def calculate_input_correlations(self):
		return interface.GetBoolValue(self._id, 'calculate_input_correlations')
		
	@calculate_input_correlations.setter
	def calculate_input_correlations(self, value : bool):
		interface.SetBoolValue(self._id, 'calculate_input_correlations', value) 

	@property   
	def stochast_settings(self):
		return self._stochast_settings

	def _set_variables(self, variables):
		new_stochast_settings = []
		for variable in variables:
			stochast_setting = self._stochast_settings[str(variable)]
			if stochast_setting is None:
				stochast_setting = StochastSettings(variable)
			new_stochast_settings.append(stochast_setting)
		self._stochast_settings = FrozenList(new_stochast_settings)
		interface.SetArrayIntValue(self._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self._stochast_settings])
