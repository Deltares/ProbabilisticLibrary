from ctypes import ArgumentError
from .utils import *
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class StandardNormal:
	_id = interface.Create('standard_normal')
	
	def get_u_from_q (q : float):
		return interface.GetArgValue(StandardNormal._id, 'u_from_q', q)

	def get_u_from_p (p : float):
		return interface.GetArgValue(StandardNormal._id, 'u_from_p', p)

	def get_q_from_u (u : float):
		return interface.GetArgValue(StandardNormal._id, 'q_from_u', u)

	def get_p_from_u (u : float):
		return interface.GetArgValue(StandardNormal._id, 'p_from_u', u)


class Stochast:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('stochast')
		else:
			self._id = id

		self._discrete_values = None
		self._histogram_values = None
		self._fragility_values = None
		self._contributing_stochasts = None
		self._synchronizing = False

	def __del__(self):
		interface.Destroy(self._id)

	@property
	def name(self):
		return interface.GetStringValue(self._id, 'name')
		
	@name.setter
	def name(self, value):
		interface.SetStringValue(self._id, 'name', value)

	def __str__(self):
		return self.name

	@property   
	def distribution(self):
		return interface.GetStringValue(self._id, 'distribution')
		
	@distribution.setter
	def distribution(self, value):
		interface.SetStringValue(self._id, 'distribution', value)

	@property   
	def inverted(self):
		return interface.GetBoolValue(self._id, 'inverted')
		
	@inverted.setter
	def inverted(self, value):
		interface.SetBoolValue(self._id, 'inverted', value)

	@property   
	def truncated(self):
		return interface.GetBoolValue(self._id, 'truncated')
		
	@truncated.setter
	def truncated(self, value):
		interface.SetBoolValue(self._id, 'truncated', value)

	@property   
	def mean(self):
		return interface.GetValue(self._id, 'mean')
		
	@mean.setter
	def mean(self, value):
		interface.SetValue(self._id, 'mean', value)

	@property   
	def deviation(self):
		return interface.GetValue(self._id, 'deviation')
		
	@deviation.setter
	def deviation(self, value):
		interface.SetValue(self._id, 'deviation', value)
		
	@property   
	def variation(self):
		return interface.GetValue(self._id, 'variation')
		
	@variation.setter
	def variation(self, value):
		interface.SetValue(self._id, 'variation', value)

	@property   
	def location(self):
		return interface.GetValue(self._id, 'location')
		
	@location.setter
	def location(self, value):
		interface.SetValue(self._id, 'location', value)

	@property   
	def scale(self):
		return interface.GetValue(self._id, 'scale')
		
	@scale.setter
	def scale(self, value):
		interface.SetValue(self._id, 'scale', value)

	@property   
	def shift(self):
		return interface.GetValue(self._id, 'shift')
		
	@shift.setter
	def shift(self, value):
		interface.SetValue(self._id, 'shift', value)

	@property   
	def shift_b(self):
		return interface.GetValue(self._id, 'shift_b')
		
	@shift_b.setter
	def shift_b(self, value):
		interface.SetValue(self._id, 'shift_b', value)

	@property   
	def minimum(self):
		return interface.GetValue(self._id, 'minimum')
		
	@minimum.setter
	def minimum(self, value):
		interface.SetValue(self._id, 'minimum', value)

	@property   
	def maximum(self):
		return interface.GetValue(self._id, 'maximum')
		
	@maximum.setter
	def maximum(self, value):
		interface.SetValue(self._id, 'maximum', value)

	@property   
	def shape(self):
		return interface.GetValue(self._id, 'shape')
		
	@shape.setter
	def shape(self, value):
		interface.SetValue(self._id, 'shape', value)

	@property   
	def shape_b(self):
		return interface.GetValue(self._id, 'shape_b')
		
	@shape_b.setter
	def shape_b(self, value):
		interface.SetValue(self._id, 'shape_b', value)

	@property   
	def rate(self):
		return interface.GetValue(self._id, 'rate')
		
	@rate.setter
	def rate(self, value):
		interface.SetValue(self._id, 'rate', value)

	@property   
	def observations(self):
		return interface.GetIntValue(self._id, 'observations')
		
	@observations.setter
	def observations(self, value):
		interface.SetIntValue(self._id, 'observations', value)

	@property   
	def discrete_values(self):
		if self._discrete_values is None:
			self._synchronizing = True
			self._discrete_values = CallbackList(self._discrete_values_changed)
			discrete_ids = interface.GetArrayIntValue(self._id, 'discrete_values')
			for discrete_id in discrete_ids:
				self._discrete_values.append(DiscreteValue(discrete_id))
			self._synchronizing = False
			
		return self._discrete_values

	def _discrete_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'discrete_values', [discrete_value._id for discrete_value in self._discrete_values])

	@property   
	def histogram_values(self):
		if self._histogram_values is None:
			self._synchronizing = True
			self._histogram_values = CallbackList(self._histogram_values_changed)
			histogram_ids = interface.GetArrayIntValue(self._id, 'histogram_values')
			for histogram_id in histogram_ids:
				self._histogram_values.append(HistogramValue(histogram_id))
			self._synchronizing = False

		return self._histogram_values

	def _histogram_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'histogram_values', [histogram_value._id for histogram_value in self._histogram_values])

	@property   
	def fragility_values(self):
		if self._fragility_values is None:
			self._synchronizing = True
			self._fragility_values = CallbackList(self._fragility_values_changed)
			fragility_ids = interface.GetArrayIntValue(self._id, 'fragility_values')
			for fragility_id in fragility_ids:
				self._fragility_values.append(FragilityValue(fragility_id))
			self._synchronizing = False

		return self._fragility_values

	def _fragility_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'fragility_values', [fragility_value._id for fragility_value in self._fragility_values])

	@property   
	def contributing_stochasts(self):
		if self._contributing_stochasts is None:
			self._synchronizing = True
			self._contributing_stochasts = CallbackList(self._contributing_stochasts_changed)
			contributing_stochast_ids = interface.GetArrayIntValue(self._id, 'contributing_stochasts')
			for contributing_stochast_id in contributing_stochast_ids:
				self._contributing_stochasts.append(ContributingStochast(contributing_stochast_id))
			self._synchronizing = False

		return self._contributing_stochasts

	def _contributing_stochasts_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'contributing_stochasts', [contributing_stochast._id for contributing_stochast in self._contributing_stochasts])

	@property   
	def design_fraction(self):
		return interface.GetValue(self._id, 'design_fraction')
		
	@design_fraction.setter
	def design_fraction(self, value):
		interface.SetValue(self._id, 'design_fraction', value)

	@property   
	def design_factor(self):
		return interface.GetValue(self._id, 'design_factor')
		
	@design_factor.setter
	def design_factor(self, value):
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

	def fit(self, values):
		interface.SetArrayValue(self._id, 'fit', values)
		self._histogram_values = None
		self._discrete_values = None
		self._fragility_values = None
		

class DiscreteValue:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('discrete_value')
		else:
			self._id = id
		
	def __del__(self):
		interface.Destroy(self._id)

	def create(x : float, amount : float):
		discreteValue = DiscreteValue()
		discreteValue.x = x
		discreteValue.amount = amount
		return discreteValue
  
	@property   
	def x(self):
		return interface.GetValue(self._id, 'x')
		
	@x.setter
	def x(self, value):
		interface.SetValue(self._id, 'x',  value)
		
	@property   
	def amount(self):
		return interface.GetValue(self._id, 'amount')
		
	@amount.setter
	def amount(self, value):
		interface.SetValue(self._id, 'amount',  value)

class FragilityValue:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('fragility_value')
		else:
			self._id = id
		self._design_point = None
		
	def __del__(self):
		interface.Destroy(self._id)
	    
	def create(x: float, reliability_index :float):
		fragilityValue = FragilityValue()
		fragilityValue.x = x
		fragilityValue.reliability_index = reliability_index
		return fragilityValue

	@property   
	def x(self):
		return interface.GetValue(self._id, 'x')
		
	@x.setter
	def x(self, value):
		interface.SetValue(self._id, 'x',  value)
		
	@property   
	def reliability_index(self):
		return interface.GetValue(self._id, 'reliability_index')
		
	@reliability_index.setter
	def reliability_index(self, value):
		interface.SetValue(self._id, 'reliability_index',  value)

	@property   
	def probability_of_failure(self):
		return interface.GetValue(self._id, 'probability_of_failure')
		
	@probability_of_failure.setter
	def probability_of_failure(self, value):
		interface.SetValue(self._id, 'probability_of_failure',  value)
		
	@property   
	def probability_of_non_failure(self):
		return interface.GetValue(self._id, 'probability_of_non_failure')
		
	@probability_of_non_failure.setter
	def probability_of_non_failure(self, value):
		interface.SetValue(self._id, 'probability_of_non_failure',  value)
		
	@property   
	def return_period(self):
		return interface.GetValue(self._id, 'return_period')
		
	@return_period.setter
	def return_period(self, value):
		interface.SetValue(self._id, 'return_period',  value)

	@property   
	def design_point(self):
		return self._design_point

	@design_point.setter
	def design_point(self, value):
		self._design_point = value

class HistogramValue:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('histogram_value')
		else:
			self._id = id
  
	def __del__(self):
		interface.Destroy(self._id)

	def create(lower_bound : float, upper_bound : float, amount : float):
		histogramValue = HistogramValue();
		histogramValue.lower_bound = lower_bound
		histogramValue.upper_bound = upper_bound
		histogramValue.amount = amount
		return histogramValue
  
	@property   
	def lower_bound(self):
		return interface.GetValue(self._id, 'lower_bound')
		
	@lower_bound.setter
	def lower_bound(self, value):
		interface.SetValue(self._id, 'lower_bound',  value)
		
	@property   
	def upper_bound(self):
		return interface.GetValue(self._id, 'upper_bound')
		
	@upper_bound.setter
	def upper_bound(self, value):
		interface.SetValue(self._id, 'upper_bound',  value)
		
	@property   
	def amount(self):
		return interface.GetValue(self._id, 'amount')
		
	@amount.setter
	def amount(self, value):
		interface.SetValue(self._id, 'amount',  value)


class ContributingStochast:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('contributing_stochast')
		else:
			self._id = id
		self._stochast = None
  
	def __del__(self):
		interface.Destroy(self._id)

	def create(probability : float, variable : Stochast):
		contributingStochast = ContributingStochast();
		contributingStochast.probability = probability
		contributingStochast.variable = variable
		return contributingStochast
  
	@property   
	def probability(self):
		return interface.GetValue(self._id, 'property')
		
	@probability.setter
	def probability(self, value):
		interface.SetValue(self._id, 'probability',  value)
		
	@property   
	def variable(self):
		if self._variable is None:
			id_ = interface.GetIntValue(self._id, 'variable')
			if id_ > 0:
				self._variable = Stochast(id_)
		return self._variable
		
	@variable.setter
	def variable(self, value):
		self._variable = value
		if not self._variable is None:
			interface.SetIntValue(self._id, 'variable',  self._variable._id)

class CorrelationMatrix:

	def __init__(self):
		self._id = interface.Create('correlation_matrix')
		self._variables = FrozenList()

	def __del__(self):
	    interface.Destroy(self._id)

	@property   
	def variables(self):
		return self._variables

	def _set_variables(self, variables):
		self._variables = FrozenList(variables)
		interface.SetArrayIntValue(self._id, 'variables', [variable._id for variable in self._variables])

	def __getitem__(self, stochasts):
		if not isinstance(stochasts, tuple) or not len(stochasts) == 2:
			raise ArgumentError('Expected 2 arguments')

		stochast_list = []
		for i in range(len(stochasts)):
			if isinstance(stochasts[i], str):
				stochast_list.append(self._variables[stochasts[i]])
			else:
				stochast_list.append(stochasts[i])

		return interface.GetIndexedIndexedValue(self._id, 'correlation', stochast_list[0]._id, stochast_list[1]._id)

	def __setitem__(self, stochasts, value):
		if not isinstance(stochasts, tuple) or not len(stochasts) == 2:
			raise ArgumentError('Expected 2 arguments')

		stochast_list = []
		for i in range(len(stochasts)):
			if isinstance(stochasts[i], str):
				stochast_list.append(self._variables[stochasts[i]])
			else:
				stochast_list.append(stochasts[i])

		interface.SetIndexedIndexedValue(self._id, 'correlation', stochast_list[0]._id, stochast_list[1]._id, value)
  
class SelfCorrelationMatrix:

	def __init__(self):
		self._id = interface.Create('self_correlation_matrix')

	def __del__(self):
		interface.Destroy(self._id)

	def _set_variables(self, variables):
		self._variables = FrozenList(variables)

	def __getitem__(self, stochast):
		stochast_obj = stochast
		if isinstance(stochast_obj, str):
			stochast_obj = self._variables[str(stochast_obj)]

		return interface.GetIntArgValue(self._id, stochast_obj._id, 'rho')

	def __setitem__(self, stochast, value):
		stochast_obj = stochast
		if isinstance(stochast_obj, str):
			stochast_obj = self._variables[str(stochast_obj)]

		interface.SetIntArgValue(self._id, stochast_obj._id, 'rho', value)
  


