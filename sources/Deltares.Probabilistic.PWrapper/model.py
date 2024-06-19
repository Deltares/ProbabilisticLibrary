import sys
import os
import time
import inspect
from ctypes import *

import interface

class EventList(list):

	def __init__(self, callback):

		for method in ['append', 'insert', 'extend', 'remove', '__add__', '__iadd__']:
			def code_added(self, *args, **kwargs):
				getattr(super(EventList, self), method)(*args, **kwargs)
				callback()

		setattr(EventList, method, code_added)

class Project:

	_model = None

	def __init__(self):
		try:
			self._id = interface.Create('project')
			self._callback = interface.CALLBACK(self._performCallBack)
			interface.SetCallBack(self._id, 'model', self._callback)
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise

		self._variables = []
		self._settings = None
		self._design_point = None
		
		_model = None
  
	@property
	def variables(self):
		if self._variables is None:
			self._variables = []
		return self._variables

	@property   
	def settings(self):
		if self._settings is None:
			self._settings = Settings()
		return self._settings

	@property   
	def model(self):
		return Project._model
		
	@model.setter
	def model(self, value):
		Project._model = value

	@interface.CALLBACK
	def _performCallBack(values, size):
		values_list = values[:size]
		return Project._model(values_list);

	def run(self):
		_design_point = None
		interface.SetArrayValue(self._id, 'variables', [variable._id for variable in self.variables])
		interface.SetIntValue(self._id, 'settings', self.settings._id)
		interface.Execute(self._id, 'run')

	@property   
	def design_point(self):
		if self._design_point is None:
			designPointId = interface.GetIntValue(self._id, 'design_point')
			if designPointId > 0:
				self._design_point = DesignPoint(designPointId)
		return self._design_point


class Stochast:

	def __init__(self, id = None):
		try:
			if id is None:
				self._id = interface.Create('stochast')
			else:
				self._id = id
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise

		self._clear_values()

	def __del__(self):
	    interface.Destroy(self._id)

	@property   
	def name(self):
		return self._name
		
	@property   
	def fullname(self):
		return self._fullname
		
	@property   
	def submodel(self):
		return self._submodel
		
	def _clear_values(self):
		self._distribution = None
		self._inverted = None
		self._truncated = None
		self._mean = None
		self._deviation = None
		self._variation = None
		self._shift = None
		self._shift_b = None
		self._minimum = None
		self._maximum = None
		self._rate = None
		self._shape = None
		self._shape_b = None
		self._scale = None
		self._location = None
		self._observations = None
		self._design_fraction = None
		self._design_factor = None
		self._discrete_values = EventList(self._discrete_values_changed)
		self._histogram_values = EventList(self._histogram_values_changed)
		self._fragility_values = EventList(self._fragility_values_changed)

	def _clear_derived_values(self):
		self._distribution = None
		self._mean = None
		self._deviation = None
		self._variation = None

	def clear(self):
		interface.ClearVariable(self._fullname)
		self._clear_values()

	@property   
	def distribution(self):
		if self._distribution is None:
			self._distribution = interface.GetStringValue(self._id, 'distribution')
		return self._distribution
		
	@distribution.setter
	def distribution(self, value):
		self._clear_values();
		self._distribution = value
		interface.SetStringValue(self._id, 'distribution', value)

	@property   
	def inverted(self):
		if self._inverted is None:
			self._inverted = interface.GetBoolValue(self._id, 'inverted')
		return self._inverted
		
	@inverted.setter
	def inverted(self, value):
		self._inverted = value
		self._clear_derived_values();
		interface.SetBoolValue(self._id, 'inverted', value)

	@property   
	def truncated(self):
		if self._truncated is None:
			self._truncated = interface.GetBoolValue(self._id, 'truncated')
		return self._truncated
		
	@truncated.setter
	def truncated(self, value):
		self._truncated = value
		self._clear_derived_values();
		interface.SetBoolValue(self._id, 'truncated', value)

	@property   
	def mean(self):
		if self._mean is None:
			self._mean = interface.GetValue(self._id, 'mean')
		return self._mean
		
	@mean.setter
	def mean(self, value):
		self._clear_values();
		interface.SetValue(self._id, 'mean', value)

	@property   
	def deviation(self):
		if self._deviation is None:
			self._deviation = interface.GetValue(self._id, 'deviation')
		return self._deviation
		
	@deviation.setter
	def deviation(self, value):
		self._clear_values();
		interface.SetValue(self._id, 'deviation', value)
		
	@property   
	def variation(self):
		if self._variation is None:
			self._variation = interface.GetValue(self._id, 'variation')
		return self._variation
		
	@variation.setter
	def variation(self, value):
		self._clear_values();
		interface.SetValue(self._id, 'variation', value)

	@property   
	def location(self):
		if self._location is None:
			self._location = interface.GetValue(self._id, 'location')
		return self._location
		
	@location.setter
	def location(self, value):
		self._location = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'location', value)

	@property   
	def scale(self):
		if self._scale is None:
			self._scale = interface.GetValue(self._id, 'scale')
		return self._scale
		
	@scale.setter
	def scale(self, value):
		self._scale = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'scale', value)

	@property   
	def shift(self):
		if self._shift is None:
			self._shift = interface.GetValue(self._id, 'shift')
		return self._shift
		
	@shift.setter
	def shift(self, value):
		self._shift = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'shift', value)

	@property   
	def shift_b(self):
		if self._shift_b is None:
			self._shift_b = interface.GetValue(self._id, 'shift_b')
		return self._shift_b
		
	@shift_b.setter
	def shift_b(self, value):
		self._shift_b = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'shift_b', value)

	@property   
	def minimum(self):
		if self._minimum is None:
			self._minimum = interface.GetValue(self._id, 'minimum')
		return self._minimum
		
	@minimum.setter
	def minimum(self, value):
		self._minimum = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'minimum', value)

	@property   
	def maximum(self):
		if self._maximum is None:
			self._maximum = interface.GetValue(self._id, 'maximum')
		return self._maximum
		
	@maximum.setter
	def maximum(self, value):
		self._maximum = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'maximum', value)

	@property   
	def shape(self):
		if self._shape is None:
			self._shape = interface.GetValue(self._id, 'shape')
		return self._shape
		
	@shape.setter
	def shape(self, value):
		self._shape = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'shape', value)

	@property   
	def shape_b(self):
		if self._shape_b is None:
			self._shape_b = interface.GetValue(self._id, 'shape_b')
		return self._shape_b
		
	@shape_b.setter
	def shape_b(self, value):
		self._shape_b = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'shape_b', value)

	@property   
	def rate(self):
		if self._rate is None:
			self._rate = interface.GetValue(self._id, 'rate')
		return self._rate
		
	@rate.setter
	def rate(self, value):
		self._rate = value
		interface.SetValue(self._id, 'rate', value)

	@property   
	def observations(self):
		if self._observations is None:
			self._observations = interface.GetValue(self._id, 'observations')
		return self._observations
		
	@observations.setter
	def observations(self, value):
		self._observations = value
		self._clear_derived_values();
		interface.SetValue(self._id, 'observations', value)

	@property   
	def discrete_values(self):
		return self._discrete_values

	def _discrete_values_changed(self):
		self._clear_derived_values()
		values = [discrete_value._id for discrete_value in self._discrete_values]
		interface.SetArrayValue(self._id, 'discrete_values', values)

	@property   
	def histogram_values(self):
		return self._histogram_values

	def _histogram_values_changed(self):
		self._clear_derived_values()
		values = [histogram_value._id for histogram_value in self._histogram_values]
		interface.SetArrayValue(self._id, 'histogram_values', values)

	@property   
	def fragility_values(self):
		return self._fragility_values

	def _fragility_values_changed(self):
		self._clear_derived_values()
		values = [fragility_value._id for fragility_value in self._fragility_values]
		interface.SetArrayValue(self._id, 'fragility_values', values)

	@property   
	def design_fraction(self):
		if self._design_fraction is None:
			self._design_fraction = interface.GetValue(self._id, 'design_fraction')
		return self._design_fraction
		
	@design_fraction.setter
	def design_fraction(self, value):
		self._design_fraction = value
		interface.SetValue(self._id, 'design_fraction', value)

	@property   
	def design_factor(self):
		if self._design_factor is None:
			self._design_factor = interface.GetValue(self._id, 'design_factor')
		return self._design_factor
		
	@design_factor.setter
	def design_factor(self, value):
		self._design_factor = value
		interface.SetValue(self._id, 'design_factor',  value)
		
	@property   
	def design_value(self):
		return interface.GetValue(self._id, 'design_value')
		
	@design_value.setter
	def design_value(self, value):
		interface.SetValue(self._id, 'design_value', value)

	def get_quantile(self, quantile):
		return interface.GetArgValue(self._id, 'quantile', quantile)
		
	def get_x_from_u(self, u):
		return interface.GetArgValue(self._id, 'x_from_u', u)
		
	def get_u_from_x(self, x):
		return interface.GetArgValue(self._id, 'u_from_x', x)
		
	def get_correlation(self, other):
		if type(other) is Stochast:
			other = other.name
		return interface.GetCorrelation(self._fullname, other)
		  
	def set_correlation(self, other, correlation):
		if type(other) is Stochast:
			other = other.name
		interface.SetCorrelation(self._fullname, other, correlation)


class DiscreteValue:

	def __init__(self):
		try:
			self._id = interface.Create('discrete_value')
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise

		self._x = None
		self._amount = None
  
	@property   
	def x(self):
		if self._x is None:
			self._x = interface.GetValue(self._id, 'x')
		return self._x
		
	@x.setter
	def x(self, value):
		self._x = value
		interface.SetValue(self._id, 'x',  value)
		
	@property   
	def amount(self):
		if self._amount is None:
			self._amount = interface.GetValue(self._id, 'amount')
		return self._amount
		
	@amount.setter
	def amount(self, value):
		self._amount = value
		interface.SetValue(self._id, 'amount',  value)

class FragilityValue:

	def __init__(self):
		try:
			self._id = interface.Create('fragility_value')
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise

		self._x = None
		self._clear_values()

		
	def _clear_values(self):
		self._reliability_index = None
		self._probability_of_failure = None
		self._probability_of_non_failure = None
		self._return_period = None
  
	@property   
	def x(self):
		if self._x is None:
			self._x = interface.GetValue(self._id, 'x')
		return self._x
		
	@x.setter
	def x(self, value):
		self._x = value
		interface.SetValue(self._id, 'x',  value)
		
	@property   
	def reliability_index(self):
		if self._reliability_index is None:
			self._reliability_index = interface.GetValue(self._id, 'reliability_index')
		return self._reliability_index
		
	@reliability_index.setter
	def reliability_index(self, value):
		self._clear_values()
		self._reliability_index = value
		interface.SetValue(self._id, 'reliability_index',  value)

	@property   
	def probability_of_failure(self):
		if self._probability_of_failure is None:
			self._probability_of_failure = interface.GetValue(self._id, 'probability_of_failure')
		return self._probability_of_failure
		
	@probability_of_failure.setter
	def probability_of_failure(self, value):
		self._clear_values()
		self._probability_of_failure = value
		interface.SetValue(self._id, 'probability_of_failure',  value)
		
	@property   
	def probability_of_non_failure(self):
		if self._probability_of_non_failure is None:
			self._probability_of_non_failure = interface.GetValue(self._id, 'probability_of_non_failure')
		return self._probability_of_non_failure
		
	@probability_of_non_failure.setter
	def probability_of_non_failure(self, value):
		self._clear_values()
		self._probability_of_non_failure = value
		interface.SetValue(self._id, 'probability_of_non_failure',  value)
		
	@property   
	def return_period(self):
		if self._return_period is None:
			self._return_period = interface.GetValue(self._id, 'return_period')
		return self._return_period
		
	@return_period.setter
	def return_period(self, value):
		self._clear_values()
		self._return_period = value
		interface.SetValue(self._id, 'return_period',  value)

class HistogramValue:

	def __init__(self):
		try:
			self._id = interface.Create('histogram_value')
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise

		self._lower_bound = None
		self._upper_bound = None
		self._amount = None
  
	@property   
	def lower_bound(self):
		if self._lower_bound is None:
			self._lower_bound = interface.GetValue(self._id, 'lower_bound')
		return self._lower_bound
		
	@lower_bound.setter
	def lower_bound(self, value):
		self._lower_bound = value
		interface.SetValue(self._id, 'lower_bound',  value)
		
	@property   
	def upper_bound(self):
		if self._upper_bound is None:
			self._upper_bound = interface.GetValue(self._id, 'upper_bound')
		return self._upper_bound
		
	@upper_bound.setter
	def upper_bound(self, value):
		self._upper_bound = value
		interface.SetValue(self._id, 'upper_bound',  value)
		
	@property   
	def amount(self):
		if self._amount is None:
			self._amount = interface.GetValue(self._id, 'amount')
		return self._amount
		
	@amount.setter
	def amount(self, value):
		self._amount = value
		interface.SetValue(self._id, 'amount',  value)

class Settings:
		  
	def __init__(self):
		try:
			self._id = interface.Create('settings')
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise

		self._clear_values()

	def _clear_values(self):
		self._reliability_method = None
		self._design_point_method = None
		self._start_method = None
		self._minimum_samples = None
		self._maximum_samples = None
		self._maximum_iterations = None
		self._minimum_directions = None
		self._maximum_directions = None
		self._relaxation_factor = None
		self._relaxation_loops = None
		self._variation_coefficient = None
		self._intervals = None
		self._variance_factor = None
		self._start_value = None
		self._variance_loops = None
		self._min_variance_loops = None
		self._fraction_failed = None
		
	def set_fragility_values(self, values):
		toolkit.SetFragilityValues(values)
		
	@property   
	def reliability_method(self):
		if self._reliability_method is None:
			self._reliability_method= interface.GetStringValue(self._id, 'reliability_method')
		return self._reliability_method
		
	@reliability_method.setter
	def reliability_method(self, value):
		self._reliability_method = value
		interface.SetStringValue(self._id, 'reliability_method', value)

	@property   
	def design_point_method(self):
		if self._design_point_method is None:
			self._design_point_method= interface.GetStringValue(self._id, 'design_point_method')
		return self._design_point_method
		
	@design_point_method.setter
	def design_point_method(self, value):
		self._design_point_method = value
		interface.SetStringValue(self._id, 'design_point_method', value)

	@property   
	def start_method(self):
		if self._start_method is None:
			self._start_method = interface.GetStringValue(self._id, 'start_method')
		return self._start_method
		
	@start_method.setter
	def start_method(self, value):
		self._start_method = value
		interface.SetStringValue(self._id, 'start_method', value)

	@property   
	def minimum_samples(self):
		if self._minimum_samples is None:
			self._minimum_samples = interface.GetIntValue(self._id, 'minimum_samples')
		return self._minimum_samples
		
	@minimum_samples.setter
	def minimum_samples(self, value):
		self._minimum_samples = value
		interface.SetIntValue(self._id, 'minimum_samples', value)

	@property   
	def maximum_samples(self):
		if self._maximum_samples is None:
			self._maximum_samples = interface.GetIntValue(self._id, 'maximum_samples')
		return self._maximum_samples
		
	@maximum_samples.setter
	def maximum_samples(self, value):
		self._maximum_samples = value
		interface.SetIntValue(self._id, 'maximum_samples', value)

	@property   
	def maximum_iterations(self):
		if self._maximum_iterations is None:
			self._maximum_iterations = interface.GetIntValue(self._id, 'maximum_iterations')
		return self._maximum_iterations
		
	@maximum_iterations.setter
	def maximum_iterations(self, value):
		self._maximum_iterations = value
		interface.SetIntValue(self._id, 'maximum_iterations', value)

	@property   
	def minimum_directions(self):
		if self._minimum_directions is None:
			self._minimum_directions = interface.GetIntValue(self._id, 'minimum_directions')
		return self._minimum_directions
		
	@minimum_directions.setter
	def minimum_directions(self, value):
		self._minimum_directions = value
		interface.SetIntValue(self._id, 'minimum_directions', value)

	@property   
	def maximum_directions(self):
		if self._maximum_directions is None:
			self._maximum_directions = interface.GetIntValue(self._id, 'maximum_directions')
		return self._maximum_directions
		
	@maximum_directions.setter
	def maximum_directions(self, value):
		self._maximum_directions = value
		interface.SetIntValue(self._id, 'maximum_directions', value)

	@property   
	def relaxation_factor(self):
		if self._relaxation_factor is None:
			self._relaxation_factor = interface.GetValue(self._id, 'relaxation_factor')
		return self._relaxation_factor
		
	@relaxation_factor.setter
	def relaxation_factor(self, value):
		self._relaxation_factor = value
		interface.SetValue(self._id, 'relaxation_factor', value)

	@property   
	def relaxation_loops(self):
		if self._relaxation_loops is None:
			self._relaxation_loops = interface.GetValue(self._id, 'relaxation_loops')
		return self._relaxation_loops
		
	@relaxation_loops.setter
	def relaxation_loops(self, value):
		self._relaxation_loops = value
		interface.SetIntValue(self._id, 'relaxation_loops', value)

	@property   
	def maximum_variance_loops(self):
		if self._maximum_variance_loops is None:
			self._maximum_variance_loops = interface.GetIntValue(self._id, 'maximum_variance_loops')
		return self._maximum_variance_loops
		
	@maximum_variance_loops.setter
	def maximum_variance_loops(self, value):
		self._maximum_variance_loops = value
		interface.SetIntValue(self._id, 'maximum_variance_loops', value)

	@property   
	def minimum_variance_loops(self):
		if self._minimum_variance_loops is None:
			self._minimum_variance_loops = interface.GetIntValue(self._id, 'minimum_variance_loops')
		return self._minimum_variance_loops
		
	@minimum_variance_loops.setter
	def _minimum_variance_loops(self, value):
		self._minimum_variance_loops = value
		interface.SetIntValue(self._id, 'minimum_variance_loops', value)

	@property   
	def variation_coefficient(self):
		if self._variation_coefficient is None:
			self._variation_coefficient = interface.GetValue(self._id, 'variation_coefficient')
		return self._variation_coefficient
		
	@variation_coefficient.setter
	def variation_coefficient(self, value):
		self._variation_coefficient = value
		interface.SetValue(self._id, 'variation_coefficient', value)

	@property   
	def fraction_failed(self):
		if self._fraction_failed is None:
			self._fraction_failed = interface.GetValue(self._id, 'fraction_failed')
		return self._fraction_failed
		
	@fraction_failed.setter
	def fraction_failed(self, value):
		self._fraction_failed = value
		interface.SetValue(self._id, 'fraction_failed', value)

	def get_variable_settings(self, stochast):
		if type(stochast) is Stochast:
			stochast = stochast.name
		return StochastSettings(stochast)
		
class StochastSettings:
		
	def __init__(self, name):
		self._name = name
		self._clear_values()

	def _clear_values(self):
		self._start_value = None
		
	@property   
	def name(self):
		return self._name
		
	@property   
	def start_value(self):
		if self._start_value is None:
			self._start_value = toolkit.GetStartValue(self._name)
		return self._start_value
		
	@start_value.setter
	def start_value(self, value):
		self._start_value = value
		toolkit.SetStartValue(self._name, value)

class DesignPoint:

	def __init__(self, id):
		self._id = id
		self._clear_values()

	def _clear_values(self):
		self._reliability_index = None
		self._probability_failure = None
		self._convergence = None
		self._alphas = None
		self._contributing_design_points = None
		self._realizations = None
		
	@property   
	def reliability_index(self):
		if self._reliability_index is None:
			self._reliability_index = interface.GetValue(self._id, 'reliability_index')
		return self._reliability_index
		
	@property   
	def probability_failure(self):
		if self._probability_failure is None:
			self._probability_failure = interface.GetValue(self._id, 'probability_index')
		return self._probability_failure
		
	@property   
	def convergence(self):
		if self._convergence is None:
			self._convergence = interface.GetValue(self._id, 'convergence')
		return self._convergence
		
	@property   
	def alphas(self):
		if self._alphas is None:
			self._alphas = []
			alpha_ids = interface.GetArrayValue(self._id, 'alphas')
			for alpha_id in alpha_ids:
				self._alphas.append(Alpha(alpha_id))
				
		return self._alphas
	
	@property   
	def contributing_design_points(self):
		if self._contributing_design_points is None:
			self._contributing_design_points = []
			design_point_ids = interface.GetArrayValue(self._id, 'contributing_design_points')
			for design_point_id in design_point_ids:
				self._contributing_design_points.append(DesignPoint(design_point_id))
				
		return self._contributing_design_points
	
		
class Alpha:

	def __init__(self, id):
		self._id = id
		self._clear_values()

	def _clear_values(self):
		self._alpha = None
		self._x = None
		self._variable = None
		
	@property   
	def variable(self):
		if self._variable is None:
			variable_id = interface.GetIntValue(self._id, 'variable')
			self._variable = Stochast(variable_id);
		return self._variable
		
	@property   
	def alpha(self):
		if self._alpha is None:
			self._alpha = interface.GetValue(self._id, 'alpha')
		return self._alpha
		
	@property   
	def x(self):
		if self._x is None:
			self._x = interface.GetValue(self._id, 'x')
		return self._x


