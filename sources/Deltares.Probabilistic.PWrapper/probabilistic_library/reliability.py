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
from .statistic import Stochast, FragilityValue
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
	cobyla_reliability = 'cobyla_reliability'
	form_then_directional_sampling = 'form_then_directional_sampling'
	directional_sampling_then_form = 'directional_sampling_then_form'
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
	def __str__(self):
		return str(self.value)

class GradientType(Enum):
	single = 'single'
	double = 'double'
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

class ExcludingCombinerMethod(Enum):
	weighted_sum = 'weighted_sum'
	hohenbichler = 'hohenbichler_excluding'
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

	def __init__(self, id_ = None):
		if id_ is None:
			self._id = interface.Create('settings')
		else:
			self._id = id_
		self._stochast_settings = FrozenList()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['max_parallel_processes',
				'save_realizations',
				'save_convergence',
				'save_messages',
				'reliability_method',
				'design_point_method',
				'start_method',
				'all_quadrants',
				'max_steps_sphere_search',
				'random_type',
				'is_repeatable_random',
				'random_seed',
				'sample_method'
				'minimum_samples',
				'maximum_samples',
				'minimum_iterations',
				'maximum_iterations',
				'minimum_directions',
				'maximum_directions',
				'epsilon_beta',
				'step_size',
				'gradient_type',
				'relaxation_factor',
				'relaxation_loops',
				'minimum_variance_loops',
				'maximum_variance_loops',
				'variation_coefficient',
				'fraction_failed',
				'stochast_settings']

	@property
	def max_parallel_processes(self):
		return interface.GetIntValue(self._id, 'max_parallel_processes')

	@max_parallel_processes.setter
	def max_parallel_processes(self, value : int):
		interface.SetIntValue(self._id, 'max_parallel_processes', value)

	@property
	def save_realizations(self):
		return interface.GetBoolValue(self._id, 'save_realizations')

	@save_realizations.setter
	def save_realizations(self, value : bool):
		interface.SetBoolValue(self._id, 'save_realizations', value)

	@property
	def save_convergence(self):
		return interface.GetBoolValue(self._id, 'save_convergence')

	@save_convergence.setter
	def save_convergence(self, value : bool):
		interface.SetBoolValue(self._id, 'save_convergence', value)

	@property
	def save_messages(self):
		return interface.GetBoolValue(self._id, 'save_messages')

	@save_messages.setter
	def save_messages(self, value : bool):
		interface.SetBoolValue(self._id, 'save_messages', value)

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
	def is_repeatable_random(self):
		return interface.GetBoolValue(self._id, 'is_repeatable_random')
		
	@is_repeatable_random.setter
	def is_repeatable_random(self, value : bool):
		interface.SetBoolValue(self._id, 'is_repeatable_random', value)

	@property
	def random_seed(self):
		return interface.GetIntValue(self._id, 'random_seed')
		
	@random_seed.setter
	def random_seed(self, value : int):
		interface.SetIntValue(self._id, 'random_seed', value)

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
	def step_size(self):
		return interface.GetValue(self._id, 'step_size')
		
	@step_size.setter
	def step_size(self, value : float):
		interface.SetValue(self._id, 'step_size', value)

	@property
	def gradient_type(self):
		return GradientType[interface.GetStringValue(self._id, 'gradient_type')]
		
	@gradient_type.setter
	def gradient_type(self, value : GradientType):
		interface.SetStringValue(self._id, 'gradient_type', str(value))

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

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['min_value',
				'max_value',
				'start_value',
				'variance_factor',
				'is_initialization_allowed',
				'is_variance_allowed',
				'intervals']
		
	def __str__(self):
		if self._variable is None:
			return ''
		else:
			return self._variable.name

	@property
	def variable(self):
		if self._variable is None:
			id_ = interface.GetIdValue(self._id, 'variable')
			if id_ > 0:
				self._variable = Stochast(id_)
		return self._variable

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

	def __del__(self):
		interface.Destroy(self._id)

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

# abstract class for design point ids
class DesignPointIds:

	def __init__(self):
		pass # empty method as it is in a abstract class

class DesignPoint:

	def __init__(self, id = None, known_variables = None, known_design_points = None):
		if id is None:
			self._id = interface.Create('design_point')
		else:
			self._id = id

		self._alphas = None
		self._contributing_design_points = None
		self._messages = None
		self._realizations = None
		self._ids = None
		self._known_variables = known_variables
		self._known_design_points = known_design_points
		
	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['identifier',
				'ids',
				'reliability_index',
				'probability_failure',
				'alphas',
				'contributing_design_points',
				'convergence',
				'is_converged',
				'total_directions',
				'total_iterations',
				'total_model_runs',
				'realizations',
				'messages']
		
	def __str__(self):
		return self.identifier

	@property
	def identifier(self):
		return interface.GetStringValue(self._id, 'identifier')
		
	@identifier.setter
	def identifier(self, value : str):
		interface.SetStringValue(self._id, 'identifier', value)

	@property
	def ids(self):
		return self._ids
		
	@ids.setter
	def ids(self, value : DesignPointIds):
		self._ids = value
		interface.SetIntValue(self._id, 'ids', value._id)

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

		alphas = []
		alphas.extend(self.alphas)
		alphas.append(alpha)
		self._alphas = FrozenList(alphas)
		
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
			alphas = []
			alpha_ids = interface.GetArrayIdValue(self._id, 'alphas')
			for alpha_id in alpha_ids:
				alphas.append(Alpha(alpha_id, self._known_variables))
			self._alphas = FrozenList(alphas)
		return self._alphas
	
	@property
	def contributing_design_points(self):
		if self._contributing_design_points is None:
			contributing_design_points = []
			design_point_ids = interface.GetArrayIdValue(self._id, 'contributing_design_points')
			for design_point_id in design_point_ids:
				if design_point_id > 0:
					added = False
					if not self._known_design_points is None:
						for design_point in self._known_design_points:
							if design_point._id == design_point_id:
								contributing_design_points.append(design_point)
								added = True

					if not added:
						contributing_design_points.append(DesignPoint(design_point_id, self._known_variables, self._known_design_points))
			self._contributing_design_points = FrozenList(contributing_design_points)
				
		return self._contributing_design_points

	@property
	def realizations(self):
		if self._realizations is None:
			realizations = []
			realization_ids = interface.GetArrayIdValue(self._id, 'evaluations')
			for realization_id in realization_ids:
				realizations.append(Evaluation(realization_id))
			self._realizations = FrozenList(realizations)
				
		return self._realizations
	
	@property
	def messages(self):
		if self._messages is None:
			messages = []
			message_ids = interface.GetArrayIdValue(self._id, 'messages')
			for message_id in message_ids:
				messages.append(Message(message_id))
			self._messages = FrozenList(messages)
				
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

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['variable',
				'identifier',
				'alpha',
				'alpha_correlated',
				'influence_factor',
				'index',
				'x',
				'u']

	@property
	def variable(self):
		if self._variable is None:
			variable_id = interface.GetIdValue(self._id, 'variable')
			if variable_id > 0:
				if not self._known_variables is None:
					for variable in self._known_variables:
						if variable._id == variable_id:
							self._variable = variable

				if self._variable is None:
					self._variable = Stochast(variable_id);
				
		return self._variable

	def __str__(self):
		return self.identifier

	@property
	def identifier(self):
		return interface.GetStringValue(self._id, 'identifier')

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
	def index(self):
		return interface.GetIntValue(self._id, 'index')

	@property
	def u(self):
		return interface.GetValue(self._id, 'u')

	@property
	def x(self):
		return interface.GetValue(self._id, 'x')


class FragilityCurve:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('fragility_curve')
		else:
			self._id = id

		self._fragility_values = None

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['name',
				'mean',
				'deviation',
				'variation',
				'fragility_values',
				'copy_from',
				'get_quantile',
				'get_x_from_u',
				'get_u_from_x',
				'get_pdf',
				'get_cdf',
				'get_special_values',
				'integrate']

	@property
	def name(self):
		return interface.GetStringValue(self._id, 'name')

	@name.setter
	def name(self, value : str):
		interface.SetStringValue(self._id, 'name', value)

	def __str__(self):
		return self.name

	@property
	def mean(self):
		return interface.GetValue(self._id, 'mean')

	@property
	def deviation(self):
		return interface.GetValue(self._id, 'deviation')

	@property
	def variation(self):
		return interface.GetValue(self._id, 'variation')

	@property
	def fragility_values(self):
		if self._fragility_values is None:
			self._synchronizing = True
			self._fragility_values = CallbackList(self._fragility_values_changed)
			fragility_ids = interface.GetArrayIdValue(self._id, 'fragility_values')
			for fragility_id in fragility_ids:
				self._fragility_values.append(FragilityValue(fragility_id))
			self._synchronizing = False

		return self._fragility_values

	def _fragility_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'fragility_values', [fragility_value._id for fragility_value in self._fragility_values])

	def get_quantile(self, quantile : float):
		return interface.GetArgValue(self._id, 'quantile', quantile)

	def get_x_from_u(self, u : float):
		return interface.GetArgValue(self._id, 'x_from_u', u)

	def get_u_from_x(self, x : float):
		return interface.GetArgValue(self._id, 'u_from_x', x)

	def get_pdf(self, x : float):
		return interface.GetArgValue(self._id, 'pdf', x)

	def get_cdf(self, x : float):
		return interface.GetArgValue(self._id, 'cdf', x)

	def get_special_values(self):
		return interface.GetArrayValue(self._id, 'special_values')

	def copy_from(self, source):
		if source is FragilityCurve:
			interface.SetIntValue(self._id, 'copy_from', source._id)
			self._fragility_values = None

	def integrate(self, integrand : Stochast):
		project = FragilityCurveProject()
		project.integrand = integrand
		project.fragility_curve = self
		project.run()
		return project.design_point

class FragilityCurveProject:

	def __init__(self):
		self._id = interface.Create('fragility_curve_project')
		self._integrand = None
		self._fragility_curve = None
		self._design_point = None

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['integrand',
				'fragility_curve',
				'design_point']

	@property
	def integrand(self):
		return self._integrand

	@integrand.setter
	def integrand(self, value : Stochast):
		self._integrand = value
		interface.SetIntValue(self._id, 'integrand', value._id)

	@property
	def fragility_curve(self):
		return self._fragility_curve

	@fragility_curve.setter
	def fragility_curve(self, value : FragilityCurve):
		self._fragility_curve = value
		interface.SetIntValue(self._id, 'fragility_curve', value._id)

	@property
	def design_point(self):
		return self._design_point

	def run(self):
		self._design_point = None
		interface.Execute(self._id, 'run')
		design_point_id = interface.GetIdValue(self._id, 'design_point')
		if design_point_id > 0:
			known_variables = self._get_variables()
			self._design_point = DesignPoint(design_point_id, known_variables)

	def _get_variables(self):
		variables = []
		variables.append(self.integrand)
		variables.append(self.fragility_curve)
		for fragility_value in self.fragility_curve.fragility_values:
			if isinstance(fragility_value.design_point, DesignPoint):
				for alpha in fragility_value.design_point.alphas:
					if alpha.variable is not None and alpha.variable not in variables:
						variables.append(alpha.variable)
						for array_variable in alpha.variable.array_variables:
							if array_variable not in variables:
								variables.append(array_variable)
		return variables


class CombineSettings:

	def __init__(self):
		self._id = interface.Create('combine_settings')
		
	def __del__(self):
		interface.Destroy(self._id)

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

class ExcludingCombineSettings:

	def __init__(self):
		self._id = interface.Create('excluding_combine_settings')
		
	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['combiner_method']

	@property
	def combiner_method(self):
		return ExcludingCombinerMethod[interface.GetStringValue(self._id, 'combiner_method')]
		
	@combiner_method.setter
	def combiner_method(self, value : ExcludingCombinerMethod):
		interface.SetStringValue(self._id, 'combiner_method', str(value))

class Message:

	def __init__(self, id = None):
		if id == None:
			self._id = interface.Create('message')
		else:
			self._id = id

	@classmethod
	def from_message(cls, message_type, message_text):
		message = cls()
		interface.SetStringValue(message._id, 'type', str(message_type))
		interface.SetStringValue(message._id, 'text', message_text)
		return message

	def __del__(self):
		interface.Destroy(self._id)
		
	def __str__(self):
		return str(self.type) + ': ' + self.text
		
	@property
	def type(self):
		return MessageType[interface.GetStringValue(self._id, 'type')]
		
	@property
	def text(self):
		return interface.GetStringValue(self._id, 'text')

		
class Evaluation:
		
	def __init__(self, id = None):
		if id == None:
			self._id = interface.Create('evaluation')
		else:
			self._id = id
		self._input_values = None	
		self._output_values = None	

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['iteration',
				'z',
				'beta',
				'weight',
				'input_values',
				'output_values']
	
	@property   
	def iteration(self):
		return interface.GetIntValue(self._id, 'iteration')
		
	@property   
	def z(self):
		return interface.GetValue(self._id, 'z')
		
	@property   
	def beta(self):
		return interface.GetValue(self._id, 'beta')
		
	@property   
	def weight(self):
		return interface.GetValue(self._id, 'weight')

	@property   
	def input_values(self):
		if self._input_values is None:
			input_values = interface.GetArrayValue(self._id, 'input_values')
			self._input_values = FrozenList(input_values)
		return self._input_values
		
	@property   
	def output_values(self):
		if self._output_values is None:
			output_values = interface.GetArrayValue(self._id, 'output_values')
			self._output_values = FrozenList(output_values)
		return self._output_values
		

				
