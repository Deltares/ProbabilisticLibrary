import sys

from .utils import *
from .statistic import Stochast
from . import interface

class Settings:
		  
	def __init__(self):
		self._id = interface.Create('settings')
		self._stochast_settings = FrozenList()
		
	@property   
	def reliability_method(self):
		return interface.GetStringValue(self._id, 'reliability_method')
		
	@reliability_method.setter
	def reliability_method(self, value):
		interface.SetStringValue(self._id, 'reliability_method', value)

	@property   
	def design_point_method(self):
		return interface.GetStringValue(self._id, 'design_point_method')
		
	@design_point_method.setter
	def design_point_method(self, value):
		interface.SetStringValue(self._id, 'design_point_method', value)

	@property   
	def start_method(self):
		return interface.GetStringValue(self._id, 'start_method')
		
	@start_method.setter
	def start_method(self, value):
		interface.SetStringValue(self._id, 'start_method', value)

	@property   
	def random_type(self):
		return interface.GetStringValue(self._id, 'random_type')
		
	@random_type.setter
	def random_type(self, value):
		interface.SetStringValue(self._id, 'random_type', value)

	@property   
	def minimum_samples(self):
		return interface.GetIntValue(self._id, 'minimum_samples')
		
	@minimum_samples.setter
	def minimum_samples(self, value):
		interface.SetIntValue(self._id, 'minimum_samples', value)

	@property   
	def maximum_samples(self):
		return interface.GetIntValue(self._id, 'maximum_samples')
		
	@maximum_samples.setter
	def maximum_samples(self, value):
		interface.SetIntValue(self._id, 'maximum_samples', value)

	@property   
	def maximum_iterations(self):
		return interface.GetIntValue(self._id, 'maximum_iterations')
		
	@maximum_iterations.setter
	def maximum_iterations(self, value):
		interface.SetIntValue(self._id, 'maximum_iterations', value)

	@property   
	def minimum_directions(self):
		return interface.GetIntValue(self._id, 'minimum_directions')
		
	@minimum_directions.setter
	def minimum_directions(self, value):
		interface.SetIntValue(self._id, 'minimum_directions', value)

	@property   
	def maximum_directions(self):
		return interface.GetIntValue(self._id, 'maximum_directions')
		
	@maximum_directions.setter
	def maximum_directions(self, value):
		interface.SetIntValue(self._id, 'maximum_directions', value)

	@property   
	def relaxation_factor(self):
		return interface.GetValue(self._id, 'relaxation_factor')
		
	@relaxation_factor.setter
	def relaxation_factor(self, value):
		interface.SetValue(self._id, 'relaxation_factor', value)

	@property   
	def relaxation_loops(self):
		return interface.GetValue(self._id, 'relaxation_loops')
		
	@relaxation_loops.setter
	def relaxation_loops(self, value):
		interface.SetIntValue(self._id, 'relaxation_loops', value)

	@property   
	def maximum_variance_loops(self):
		return interface.GetIntValue(self._id, 'maximum_variance_loops')
		
	@maximum_variance_loops.setter
	def maximum_variance_loops(self, value):
		interface.SetIntValue(self._id, 'maximum_variance_loops', value)

	@property   
	def minimum_variance_loops(self):
		return interface.GetIntValue(self._id, 'minimum_variance_loops')
		
	@minimum_variance_loops.setter
	def minimum_variance_loops(self, value):
		interface.SetIntValue(self._id, 'minimum_variance_loops', value)

	@property   
	def variation_coefficient(self):
		return interface.GetValue(self._id, 'variation_coefficient')
		
	@variation_coefficient.setter
	def variation_coefficient(self, value):
		interface.SetValue(self._id, 'variation_coefficient', value)

	@property   
	def fraction_failed(self):
		return interface.GetValue(self._id, 'fraction_failed')
		
	@fraction_failed.setter
	def fraction_failed(self, value):
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
	def min_value(self, value):
		interface.SetValue(self._id, 'min_value', value)

	@property   
	def max_value(self):
		return interface.GetValue(self._id, 'max_value')
		
	@max_value.setter
	def max_value(self, value):
		interface.SetValue(self._id, 'max_value', value)

	@property   
	def start_value(self):
		return interface.GetValue(self._id, 'start_value')
		
	@start_value.setter
	def start_value(self, value):
		interface.SetValue(self._id, 'start_value', value)

	@property   
	def intervals(self):
		return interface.GetIntValue(self._id, 'intervals')
		
	@intervals.setter
	def intervals(self, value):
		interface.SetIntValue(self._id, 'intervals', value)

	@property   
	def variance_factor(self):
		return interface.GetValue(self._id, 'variance_factor')
		
	@variance_factor.setter
	def variance_factor(self, value):
		interface.SetValue(self._id, 'variance_factor', value)

	@property   
	def is_initialization_allowed(self):
		return interface.GetBoolValue(self._id, 'is_initialization_allowed')
		
	@is_initialization_allowed.setter
	def is_initialization_allowed(self, value):
		interface.SetBoolValue(self._id, 'is_initialization_allowed', value)

	@property   
	def is_variance_allowed(self):
		return interface.GetBoolValue(self._id, 'is_variance_allowed')
		
	@is_variance_allowed.setter
	def is_variance_allowed(self, value):
		interface.SetBoolValue(self._id, 'is_variance_allowed', value)


		

class DesignPoint:

	def __init__(self, id = None, known_variables = None, known_design_points = None):
		if id is None:
			self._id = interface.Create('design_point')
		else:
			self._id = id

		self._alphas = None
		self._contributing_design_points = None
		self._known_variables = known_variables
		self._known_design_points = known_design_points
		
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
		
	@property   
	def combiner_method(self):
		return interface.GetStringValue(self._id, 'combiner_method')
		
	@combiner_method.setter
	def combiner_method(self, value):
		interface.SetStringValue(self._id, 'combiner_method', value)

	@property   
	def combine_type(self):
		return interface.GetStringValue(self._id, 'combine_type')
		
	@combine_type.setter
	def combine_type(self, value):
		interface.SetStringValue(self._id, 'combine_type', value)


