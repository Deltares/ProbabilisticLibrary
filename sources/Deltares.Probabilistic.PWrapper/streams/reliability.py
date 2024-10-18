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
from enum import Enum

from .utils import *
from .statistic import Stochast
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class ReliabilityMethod(Enum):
	form = 'form'
	numerical_integration = 'numerical_integration'
	crude_monte_carlo = 'crude_monte_carlo'
	importance_sampling = 'importance_sampling'
	adaptive_importance_sampling = 'adaptive_importance_sampling'
	directional_sampling = 'directional_sampling'
	subset_simulation = 'subset_simulation'
	numerical_bisection = 'numerical_bisection'
	latin_hypercube = 'latin_hypercube'
	def __str__(self):
		return str(self.value)

class DesignPointMethod(Enum):
	nearest_to_mean = 'nearest_to_mean'
	center_of_gravity = 'center_of_gravity'
	center_of_angles = 'center_of_angles'
	def __str__(self):
		return str(self.value)

class StartMethod(Enum):
	fixed_value = 'fixed_value'
	one = 'one'
	ray_search = 'ray_search'
	sphere_search = 'sphere_search'
	sensitivity_search = 'sensitivity_search'
	def __str__(self):
		return str(self.value)

class RandomType(Enum):
	mersenne_twister = 'mersenne_twister'
	george_marsaglia = 'george_marsaglia'
	modified_knuth_subtractive = 'modified_knuth_subtractive'
	def __str__(self):
		return str(self.value)

class SampleMethod(Enum):
	markov_chain = 'markov_chain'
	adaptive_conditional = 'adaptive_conditional'
	def __str__(self):
		return str(self.value)

class CombinerMethod(Enum):
	hohenbichler = 'hohenbichler'
	importance_sampling = 'importance_sampling'
	directional_sampling = 'directional_sampling'
	def __str__(self):
		return str(self.value)

class CombineType(Enum):
	series = 'series'
	parallel = 'parallel'
	def __str__(self):
		return str(self.value)

class MessageType(Enum):
	debug = 'debug'
	info = 'info'
	warning = 'warning'
	error = 'error'
	def __str__(self):
		return str(self.value)

class Settings:

	def __init__(self):
		self._id = interface.Create('settings')
		self._stochast_settings = FrozenList()

	def __dir__(self):
		return ['reliability_method',
				'design_point_method',
				'start_method',
				'all_quadrants',
				'max_steps_sphere_search',
				'random_type',
				'sample_method'
				'minimum_samples',
				'maximum_samples',
				'maximum_iterations',
				'minimum_directions',
				'maximum_directions',
				'epsilon_beta',
				'relaxation_factor',
				'relaxation_loops',
				'minimum_variance_loops',
				'maximum_variance_loops',
				'variation_coefficient',
				'fraction_failed',
				'stochast_settings']

	@property
	def reliability_method(self):
		return ReliabilityMethod[interface.GetStringValue(self._id, 'reliability_method')]

	@reliability_method.setter
	def reliability_method(self, value : ReliabilityMethod):
		interface.SetStringValue(self._id, 'reliability_method', str(value))

	@property
	def design_point_method(self):
		return DesignPointMethod[interface.GetStringValue(self._id, 'design_point_method')]
		
	@design_point_method.setter
	def design_point_method(self, value : DesignPointMethod):
		interface.SetStringValue(self._id, 'design_point_method', str(value))

	@property
	def start_method(self):
		return StartMethod[interface.GetStringValue(self._id, 'start_method')]

	@start_method.setter
	def start_method(self, value : StartMethod):
		interface.SetStringValue(self._id, 'start_method', str(value))

	@property
	def all_quadrants(self):
		return interface.GetBoolValue(self._id, 'all_quadrants')

	@all_quadrants.setter
	def all_quadrants(self, value : bool):
		interface.SetBoolValue(self._id, 'all_quadrants', value)

	@property
	def max_steps_sphere_search(self):
		return interface.GetIntValue(self._id, 'max_steps_sphere_search')

	@max_steps_sphere_search.setter
	def max_steps_sphere_search(self, value : int):
		interface.SetIntValue(self._id, 'max_steps_sphere_search', value)

	@property
	def random_type(self):
		return RandomType[interface.GetStringValue(self._id, 'random_type')]

	@random_type.setter
	def random_type(self, value : RandomType):
		interface.SetStringValue(self._id, 'random_type', str(value))

	@property
	def sample_method(self):
		return interface.GetStringValue(self._id, 'sample_method')

	@sample_method.setter
	def sample_method(self, value : SampleMethod):
		interface.SetStringValue(self._id, 'sample_method', str(value))

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
	def minimum_iterations(self):
		return interface.GetIntValue(self._id, 'minimum_iterations')

	@minimum_iterations.setter
	def minimum_iterations(self, value : int):
		interface.SetIntValue(self._id, 'minimum_iterations', value)

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
	def epsilon_beta(self):
		return interface.GetValue(self._id, 'epsilon_beta')

	@epsilon_beta.setter
	def epsilon_beta(self, value : float):
		interface.SetValue(self._id, 'epsilon_beta', value)

	@property
	def relaxation_factor(self):
		return interface.GetValue(self._id, 'relaxation_factor')

	@relaxation_factor.setter
	def relaxation_factor(self, value : float):
		interface.SetValue(self._id, 'relaxation_factor', value)

	@property
	def relaxation_loops(self):
		return interface.GetIntValue(self._id, 'relaxation_loops')
		
	@relaxation_loops.setter
	def relaxation_loops(self, value):
		interface.SetIntValue(self._id, 'relaxation_loops', value)

	@property
	def maximum_variance_loops(self):
		return interface.GetIntValue(self._id, 'maximum_variance_loops')
		
	@maximum_variance_loops.setter
	def maximum_variance_loops(self, value : int):
		interface.SetIntValue(self._id, 'maximum_variance_loops', value)

	@property
	def minimum_variance_loops(self):
		return interface.GetIntValue(self._id, 'minimum_variance_loops')
		
	@minimum_variance_loops.setter
	def minimum_variance_loops(self, value : int):
		interface.SetIntValue(self._id, 'minimum_variance_loops', value)

	@property
	def variation_coefficient(self):
		return interface.GetValue(self._id, 'variation_coefficient')
		
	@variation_coefficient.setter
	def variation_coefficient(self, value : float):
		interface.SetValue(self._id, 'variation_coefficient', value)

	@property
	def fraction_failed(self):
		return interface.GetValue(self._id, 'fraction_failed')
		
	@fraction_failed.setter
	def fraction_failed(self, value : float):
		interface.SetValue(self._id, 'fraction_failed', value)

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

	
class StochastSettings:
		
	def __init__(self, variable):
		self._id = interface.Create('stochast_settings')
		self._variable = variable
		if not variable is None:
			interface.SetIntValue(self._id, 'variable', self._variable._id)

	def __dir__(self):
		return ['min_value',
				'max_value',
				'start_value',
				'variance_factor',
				'is_initialization_allowed',
				'is_variance_allowed',
				'intervals']
		
	@property
	def variable(self):
		if self._variable is None:
			id_ = interface.GetIntValue(self._id, 'variable')
			if id_ > 0:
				self._variable = Stochast(id_)
		return self._variable
		
	def __str__(self):
		if self._variable is None:
			return ''
		else:
			return self._variable.name

	@property
	def min_value(self):
		return interface.GetValue(self._id, 'min_value')
		
	@min_value.setter
	def min_value(self, value : float):
		interface.SetValue(self._id, 'min_value', value)

	@property
	def max_value(self):
		return interface.GetValue(self._id, 'max_value')
		
	@max_value.setter
	def max_value(self, value : float):
		interface.SetValue(self._id, 'max_value', value)

	@property
	def start_value(self):
		return interface.GetValue(self._id, 'start_value')
		
	@start_value.setter
	def start_value(self, value : float):
		interface.SetValue(self._id, 'start_value', value)

	@property
	def intervals(self):
		return interface.GetIntValue(self._id, 'intervals')
		
	@intervals.setter
	def intervals(self, value : int):
		interface.SetIntValue(self._id, 'intervals', value)

	@property
	def variance_factor(self):
		return interface.GetValue(self._id, 'variance_factor')
		
	@variance_factor.setter
	def variance_factor(self, value : float):
		interface.SetValue(self._id, 'variance_factor', value)

	@property
	def is_initialization_allowed(self):
		return interface.GetBoolValue(self._id, 'is_initialization_allowed')
		
	@is_initialization_allowed.setter
	def is_initialization_allowed(self, value : bool):
		interface.SetBoolValue(self._id, 'is_initialization_allowed', value)

	@property
	def is_variance_allowed(self):
		return interface.GetBoolValue(self._id, 'is_variance_allowed')
		
	@is_variance_allowed.setter
	def is_variance_allowed(self, value: bool):
		interface.SetBoolValue(self._id, 'is_variance_allowed', value)

class CompareType(Enum):
	less_than = 'less_than'
	greater_than = 'greater_than'
	def __str__(self):
		return str(self.value)

class LimitStateFunction:
		
	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('limit_state_function')
		else:
			self._id = id

	def __dir__(self):
		return ['parameter',
		        'compare_type',
		        'critical_value']
		
	def __str__(self):
		return self.parameter + ' ' + str(self.compare_type) + ' ' + str(self.critical_value)

	@property
	def parameter(self):
		return interface.GetStringValue(self._id, 'parameter')
		
	@parameter.setter
	def parameter(self, value):
		interface.SetStringValue(self._id, 'parameter', str(value))

	@property
	def compare_type(self):
		return CompareType[interface.GetStringValue(self._id, 'compare_type')]
		
	@compare_type.setter
	def compare_type(self, value : CompareType):
		interface.SetStringValue(self._id, 'compare_type', str(value))

	@property
	def critical_value(self):
		if interface.GetBoolValue('use_compare_parameter'):
			return interface.GetStringValue(self._id, 'compare_parameter')
		else:
			return interface.GetValue(self._id, 'critical_value')
		
	@critical_value.setter
	def critical_value(self, value):
		if type(value) is float or type(value) is int:
			interface.SetBoolValue(self._id, 'use_compare_parameter', False)
			interface.SetValue(self._id, 'critical_value', value)
		else:
			interface.SetBoolValue(self._id, 'use_compare_parameter', True)
			interface.SetStringValue(self._id, 'compare_parameter', str(value))
		

class DesignPoint:

	def __init__(self, id = None, known_variables = None, known_design_points = None):
		if id is None:
			self._id = interface.Create('design_point')
		else:
			self._id = id

		self._alphas = None
		self._contributing_design_points = None
		self._messages = None
		self._known_variables = known_variables
		self._known_design_points = known_design_points
		
	def __dir__(self):
		return ['identifier',
				'reliability_index',
				'probability_failure',
				'alphas',
				'contributing_design_points',
				'convergence',
				'is_converged',
				'total_directions',
				'total_iterations',
				'total_model_runs',
				'messages']
		
	@property
	def identifier(self):
		return interface.GetStringValue(self._id, 'identifier')
		
	@identifier.setter
	def identifier(self, value : str):
		interface.SetStringValue(self._id, 'identifier', value)

	@property
	def reliability_index(self):
		return interface.GetValue(self._id, 'reliability_index')

	# testing method
	def _set_reliability_index(self, reliability_index_value):
		interface.SetValue(self._id, 'reliability_index', reliability_index_value)

	# testing method
	def _add_alpha(self, variable, alpha_value):
		alpha = Alpha();
		alpha._set_alpha(variable, alpha_value, self.reliability_index);
		self.alphas.append(alpha)

		values = [a._id for a in self._alphas]
		interface.SetArrayIntValue(self._id, 'alphas', values)

	@property
	def probability_failure(self):
		return interface.GetValue(self._id, 'probability_failure')
		
	@property
	def convergence(self):
		return interface.GetValue(self._id, 'convergence')
		
	@property
	def is_converged(self):
		return interface.GetBoolValue(self._id, 'is_converged')
		
	@property
	def total_directions(self):
		return interface.GetIntValue(self._id, 'total_directions')
		
	@property
	def total_iterations(self):
		return interface.GetIntValue(self._id, 'total_iterations')
		
	@property
	def total_model_runs(self):
		return interface.GetIntValue(self._id, 'total_model_runs')
		
	@property
	def alphas(self):
		if self._alphas is None:
			self._alphas = []
			alpha_ids = interface.GetArrayIntValue(self._id, 'alphas')
			for alpha_id in alpha_ids:
				self._alphas.append(Alpha(alpha_id, self._known_variables))
				
		return self._alphas
	
	@property
	def contributing_design_points(self):
		if self._contributing_design_points is None:
			self._contributing_design_points = []
			design_point_ids = interface.GetArrayIntValue(self._id, 'contributing_design_points')
			for design_point_id in design_point_ids:
				if design_point_id > 0:
					added = False
					if not self._known_design_points is None:
						for design_point in self._known_design_points:
							if design_point._id == design_point_id:
								self._contributing_design_points.append(design_point)
								added = True

					if not added:
						self._contributing_design_points.append(DesignPoint(design_point_id, self._known_variables, self._known_design_points))
				
		return self._contributing_design_points

	@property
	def messages(self):
		if self._messages is None:
			self._messages = []
			message_ids = interface.GetArrayIntValue(self._id, 'messages')
			for message_id in message_ids:
				self._messages.append(Message(message_id))
				
		return self._messages
	
	def get_variables(self):
		variables = []
		for alpha in self.alphas:
			variables.append(alpha.variable)
		for contributing_design_point in self.contributing_design_points:
			variables.extend(contributing_design_point.get_variables())
		return frozenset(variables)
		
class Alpha:

	def __init__(self, id = None, known_variables = None):
		if id is None:
			self._id = interface.Create('alpha')
		else:
			self._id = id
			
		self._variable = None
		self._known_variables = known_variables

	def __dir__(self):
		return ['variable',
				'alpha',
				'alpha_correlated',
				'influence_factor',
				'x']

	@property
	def variable(self):
		if self._variable is None:
			variable_id = interface.GetIntValue(self._id, 'variable')
			if variable_id > 0:
				if not self._known_variables is None:
					for variable in self._known_variables:
						if variable._id == variable_id:
							self._variable = variable

				if self._variable is None:
					self._variable = Stochast(variable_id);
				
		return self._variable

	def __str__(self):
		if self.variable is None:
			return ''
		else:
			return self._variable.name

	# internal method		
	def _set_variable(self, variable):
		self._variable = variable

	# testing method
	def _set_alpha(self, variable, alpha_value, beta):
		self._variable = variable
		interface.SetIntValue(self._id, 'variable', variable._id)
		interface.SetValue(self._id, 'alpha', alpha_value)

		u = - beta * alpha_value
		interface.SetValue(self._id, 'u',- beta * alpha_value)
		interface.SetValue(self._id, 'x', variable.get_x_from_u(u))

	@property
	def alpha(self):
		return interface.GetValue(self._id, 'alpha')
		
	@property
	def alpha_correlated(self):
		return interface.GetValue(self._id, 'alpha_correlated')
		
	@property
	def influence_factor(self):
		return interface.GetValue(self._id, 'influence_factor')
		
	@property
	def u(self):
		return interface.GetValue(self._id, 'u')

	@property
	def x(self):
		return interface.GetValue(self._id, 'x')


class CombineSettings:

	def __init__(self):
		self._id = interface.Create('combine_settings')
		
	def __dir__(self):
		return ['combiner_method',
				'combine_type']

	@property
	def combiner_method(self):
		return CombinerMethod[interface.GetStringValue(self._id, 'combiner_method')]
		
	@combiner_method.setter
	def combiner_method(self, value : CombinerMethod):
		interface.SetStringValue(self._id, 'combiner_method', str(value))

	@property
	def combine_type(self):
		return CombineType[interface.GetStringValue(self._id, 'combine_type')]
		
	@combine_type.setter
	def combine_type(self, value : CombineType):
		interface.SetStringValue(self._id, 'combine_type', str(value))


class Message:

	def __init__(self, id = None):
		if id == None:
			self._id = interface.Create('message')
		else:
			self._id = id

	def __del__(self):
		interface.Destroy(self._id)
		
	@property
	def type(self):
		return MessageType[interface.GetStringValue(self._id, 'type')]
		
	@property
	def text(self):
		return interface.GetStringValue(self._id, 'text')
		
