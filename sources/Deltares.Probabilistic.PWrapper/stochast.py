from utils import *
import interface

class Stochast:

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('stochast')
		else:
			self._id = id

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
		self._id = interface.Create('discrete_value')
  
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

	def __init__(self):
		self._id = interface.Create('fragility_value')
		
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

class HistogramValue:

	def __init__(self):
		self._id = interface.Create('histogram_value')
  
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
