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
from __future__ import annotations
from ctypes import ArgumentError
from enum import Enum
from math import isnan, nan
from .utils import *
from . import interface
import matplotlib.pyplot as plt


if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class ConstantParameterType(Enum):
	deviation = 'deviation'
	variation = 'variation'
	def __str__(self):
		return str(self.value)

class VariableType(Enum):
	properties = 'properties'
	mean_and_deviation = 'mean_and_deviation'
	def __str__(self):
		return str(self.value)

class DistributionType(Enum):
	deterministic = 'deterministic'
	normal = 'normal'
	log_normal = 'log_normal'
	standard_normal = 'standard_normal'
	student_t = 'student_t'
	uniform = 'uniform'
	triangular = 'triangular'
	trapezoidal = 'trapezoidal'
	exponential = 'exponential'
	gumbel = 'gumbel'
	weibull = 'weibull'
	conditional_weibull = 'conditional_weibull'
	frechet = 'frechet'
	generalized_extreme_value = 'generalized_extreme_value'
	rayleigh = 'rayleigh'
	rayleigh_n = 'rayleigh_n'
	pareto = 'pareto'
	generalized_pareto = 'generalized_pareto'
	beta = 'beta'
	gamma = 'gamma'
	bernoulli = 'bernoulli'
	poisson = 'poisson'
	histogram = 'histogram'
	cdf_curve = 'cdf_curve'
	discrete = 'discrete'
	qualitative = 'qualitative'
	composite = 'composite'
	def __str__(self):
		return str(self.value)

class StandardNormal(FrozenObject):
	"""
	Provides conversions between probabilities (p,q), reliability (u) and return time (t)
	"""
	_id_value = 0

	def __dir__(self):
		return ['get_u_from_q',
			'get_u_from_p',
			'get_q_from_u',
			'get_p_from_u',
			'get_t_from_u',
			'get_u_from_t']

	def _id() -> int:
		if StandardNormal._id_value == 0:
			StandardNormal._id_value = interface.Create('standard_normal')
		return StandardNormal._id_value

	def get_u_from_q (q : float) -> float:
		"""
		get u from q (probability of exceedence)
		"""
		return interface.GetArgValue(StandardNormal._id(), 'u_from_q', q)

	def get_u_from_p (p : float) -> float:
		"""
		get u from p (probability of non-exceedence)
		"""
		return interface.GetArgValue(StandardNormal._id(), 'u_from_p', p)

	def get_q_from_u (u : float) -> float:
		"""
		get q (probability of exceedence) from u
		"""
		return interface.GetArgValue(StandardNormal._id(), 'q_from_u', u)

	def get_p_from_u (u : float) -> float:
		"""
		get p (probability of non-exceedence) from u
		"""
		return interface.GetArgValue(StandardNormal._id(), 'p_from_u', u)

	def get_t_from_u (u : float) -> float:
		"""
		get return time from u
		"""
		return interface.GetArgValue(StandardNormal._id(), 't_from_u', u)

	def get_u_from_t (t : float) -> float:
		"""
		get u from return time
		"""
		return interface.GetArgValue(StandardNormal._id(), 'u_from_t', t)

class ProbabilityValue(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('probability_value')
		else:
			self._id = id
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['reliability_index',
		        'probability_of_failure',
		        'probability_of_non_failure',
		        'return_period']

	def __str__(self):
		return str(self.probability_of_non_failure)

	@property
	def reliability_index(self) -> float:
		return interface.GetValue(self._id, 'reliability_index')

	@reliability_index.setter
	def reliability_index(self, value : float):
		interface.SetValue(self._id, 'reliability_index',  value)

	@property
	def probability_of_failure(self) -> float:
		return interface.GetValue(self._id, 'probability_of_failure')

	@probability_of_failure.setter
	def probability_of_failure(self, value : float):
		interface.SetValue(self._id, 'probability_of_failure',  value)

	@property
	def probability_of_non_failure(self) -> float:
		return interface.GetValue(self._id, 'probability_of_non_failure')

	@probability_of_non_failure.setter
	def probability_of_non_failure(self, value : float):
		interface.SetValue(self._id, 'probability_of_non_failure',  value)

	@property
	def return_period(self) -> float:
		return interface.GetValue(self._id, 'return_period')

	@return_period.setter
	def return_period(self, value : float):
		interface.SetValue(self._id, 'return_period',  value)

class Stochast(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('stochast')
		else:
			self._id = id

		self._discrete_values = None
		self._histogram_values = None
		self._fragility_values = None
		self._contributing_stochasts = None
		self._conditional_values = None
		self._conditional_source = None
		self._array_variables = None
		self._temp_source_str = None
		self._variables = FrozenList() # other known variables, to which a reference can exist
		self._synchronizing = False
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['name',
				'distribution',
				'inverted',
				'truncated',
				'mean',
				'deviation',
				'variation',
				'rate',
				'location',
				'scale',
				'shift',
				'shift_b',
				'shape',
				'shape_b',
				'observations',
				'minimum',
				'maximum',
				'discrete_values',
				'histogram_values',
				'fragility_values',
				'contributing_stochasts',
				'copy_from',
				'fit',
				'fit_prior',
				'get_ks_test',
				'get_quantile',
				'get_x_from_u',
				'get_x_from_u_and_source',
				'get_u_from_x',
				'get_pdf',
				'get_cdf',
				'get_special_values',
				'get_series',
				'conditional',
				'conditional_source',
				'conditional_values'
				'design_factor',
				'design_quantile',
				'design_value',
				'is_valid',
				'is_array',
				'array_size',
				'array_variables',
				'print',
				'plot']

	def _set_variables(self, variables):
		self._variables = variables
		if self._temp_source_str != None:
			self.conditional_source = self._temp_source_str
		
	@property
	def name(self) -> str:
		return interface.GetStringValue(self._id, 'name')

	@name.setter
	def name(self, value : str):
		interface.SetStringValue(self._id, 'name', value)

	def __str__(self):
		return self.name

	@property
	def distribution(self) -> DistributionType:
		return DistributionType[interface.GetStringValue(self._id, 'distribution')]

	@distribution.setter
	def distribution(self, value : DistributionType):
		interface.SetStringValue(self._id, 'distribution', str(value))

	@property
	def inverted(self) -> bool:
		return interface.GetBoolValue(self._id, 'inverted')

	@inverted.setter
	def inverted(self, value : bool):
		interface.SetBoolValue(self._id, 'inverted', value)

	@property
	def truncated(self) -> bool:
		return interface.GetBoolValue(self._id, 'truncated')

	@truncated.setter
	def truncated(self, value : bool):
		interface.SetBoolValue(self._id, 'truncated', value)

	@property
	def mean(self) -> float:
		return interface.GetValue(self._id, 'mean')

	@mean.setter
	def mean(self, value : float):
		interface.SetValue(self._id, 'mean', value)

	@property
	def deviation(self) -> float:
		return interface.GetValue(self._id, 'deviation')

	@deviation.setter
	def deviation(self, value : float):
		interface.SetStringValue(self._id, 'constant_parameter', str(ConstantParameterType.deviation))
		interface.SetValue(self._id, 'deviation', value)

	@property
	def variation(self) -> float:
		return interface.GetValue(self._id, 'variation')

	@variation.setter
	def variation(self, value : float):
		interface.SetStringValue(self._id, 'constant_parameter', str(ConstantParameterType.variation))
		interface.SetValue(self._id, 'variation', value)

	@property
	def location(self) -> float:
		return interface.GetValue(self._id, 'location')

	@location.setter
	def location(self, value : float):
		interface.SetValue(self._id, 'location', value)

	@property
	def scale(self) -> float:
		return interface.GetValue(self._id, 'scale')

	@scale.setter
	def scale(self, value : float):
		interface.SetValue(self._id, 'scale', value)

	@property
	def shift(self) -> float:
		return interface.GetValue(self._id, 'shift')

	@shift.setter
	def shift(self, value : float):
		interface.SetValue(self._id, 'shift', value)

	@property
	def shift_b(self) -> float:
		return interface.GetValue(self._id, 'shift_b')

	@shift_b.setter
	def shift_b(self, value):
		interface.SetValue(self._id, 'shift_b', value)

	@property
	def minimum(self) -> float:
		return interface.GetValue(self._id, 'minimum')

	@minimum.setter
	def minimum(self, value : float):
		interface.SetValue(self._id, 'minimum', value)

	@property
	def maximum(self) -> float:
		return interface.GetValue(self._id, 'maximum')

	@maximum.setter
	def maximum(self, value : float):
		interface.SetValue(self._id, 'maximum', value)

	@property
	def shape(self) -> float:
		return interface.GetValue(self._id, 'shape')

	@shape.setter
	def shape(self, value):
		interface.SetValue(self._id, 'shape', value)

	@property
	def shape_b(self) -> float:
		return interface.GetValue(self._id, 'shape_b')

	@shape_b.setter
	def shape_b(self, value : float):
		interface.SetValue(self._id, 'shape_b', value)

	@property
	def rate(self) -> float:
		return interface.GetValue(self._id, 'rate')

	@rate.setter
	def rate(self, value : float):
		interface.SetValue(self._id, 'rate', value)

	@property
	def observations(self) -> int:
		return interface.GetIntValue(self._id, 'observations')

	@observations.setter
	def observations(self, value : int):
		interface.SetIntValue(self._id, 'observations', value)

	@property
	def discrete_values(self) -> list[DiscreteValue]:
		if self._discrete_values is None:
			self._synchronizing = True
			self._discrete_values = CallbackList(self._discrete_values_changed)
			discrete_ids = interface.GetArrayIdValue(self._id, 'discrete_values')
			for discrete_id in discrete_ids:
				self._discrete_values.append(DiscreteValue(discrete_id))
			self._synchronizing = False

		return self._discrete_values

	def _discrete_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'discrete_values', [discrete_value._id for discrete_value in self._discrete_values])

	@property
	def histogram_values(self) -> list[HistogramValue]:
		if self._histogram_values is None:
			self._synchronizing = True
			self._histogram_values = CallbackList(self._histogram_values_changed)
			histogram_ids = interface.GetArrayIdValue(self._id, 'histogram_values')
			for histogram_id in histogram_ids:
				self._histogram_values.append(HistogramValue(histogram_id))
			self._synchronizing = False

		return self._histogram_values

	def _histogram_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'histogram_values', [histogram_value._id for histogram_value in self._histogram_values])

	@property
	def fragility_values(self) -> list[FragilityValue]:
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

	@property
	def contributing_stochasts(self) -> list[ContributingStochast]:
		if self._contributing_stochasts is None:
			self._synchronizing = True
			self._contributing_stochasts = CallbackList(self._contributing_stochasts_changed)
			contributing_stochast_ids = interface.GetArrayIdValue(self._id, 'contributing_stochasts')
			for contributing_stochast_id in contributing_stochast_ids:
				self._contributing_stochasts.append(ContributingStochast(contributing_stochast_id))
			self._synchronizing = False

		return self._contributing_stochasts

	def _contributing_stochasts_changed(self):
		if not self._synchronizing:
			for contributing_stochast in self._contributing_stochasts:
				if contributing_stochast.variable != None and len(contributing_stochast.variable._variables) == 0:
					contributing_stochast.variable._set_variables(self._variables)
			interface.SetArrayIntValue(self._id, 'contributing_stochasts', [contributing_stochast._id for contributing_stochast in self._contributing_stochasts])

	@property
	def conditional(self) -> bool:
		return interface.GetBoolValue(self._id, 'conditional')

	@conditional.setter
	def conditional(self, value : bool):
		interface.SetBoolValue(self._id, 'conditional', value)

	@property
	def conditional_source(self) -> Stochast:
		cs_id = interface.GetIdValue(self._id, 'conditional_source')
		if cs_id > 0:
			if self._conditional_source is None or not self._conditional_source._id == cs_id:
				for var in self._variables:
					if var._id == cs_id:
						self._conditional_source = var
			return self._conditional_source
		else:
			return None

	@conditional_source.setter
	def conditional_source(self, value : str | Stochast):
		if type(value) == str:
			self._temp_source_str = value
			value = self._variables[value]
		if isinstance(value, Stochast):
			interface.SetIntValue(self._id, 'conditional_source', value._id)
			self._temp_source_str = None

	@property
	def conditional_values(self) -> list[ConditionalValue]:
		if self._conditional_values is None:
			self._synchronizing = True
			self._conditional_values = CallbackList(self._conditional_values_changed)
			conditional_value_ids = interface.GetArrayIdValue(self._id, 'conditional_values')
			for conditional_value_id in conditional_value_ids:
				self._conditional_values.append(ConditionalValue(conditional_value_id))
			self._synchronizing = False

		return self._conditional_values

	def _conditional_values_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'conditional_values', [conditional_value._id for conditional_value in self._conditional_values])

	@property
	def array_variables(self) -> list[Stochast]:
		if self._array_variables is None:
			self._synchronizing = True
			self._array_variables = CallbackList(self._array_variables_changed)
			array_variable_ids = interface.GetArrayIdValue(self._id, 'array_variables')
			for array_variable_id in array_variable_ids:
				self._array_variables.append(Stochast(array_variable_id))
			self._synchronizing = False

		return self._array_variables

	def _array_variables_changed(self):
		if not self._synchronizing:
			interface.SetArrayIntValue(self._id, 'array_variables', [array_variable._id for array_variable in self._array_variables])

	@property
	def design_quantile(self) -> float:
		return interface.GetValue(self._id, 'design_quantile')

	@design_quantile.setter
	def design_quantile(self, value : float):
		interface.SetValue(self._id, 'design_quantile', value)

	@property
	def design_factor(self) -> float:
		return interface.GetValue(self._id, 'design_factor')

	@design_factor.setter
	def design_factor(self, value : float):
		interface.SetValue(self._id, 'design_factor',  value)

	@property
	def design_value(self) -> float:
		return interface.GetValue(self._id, 'design_value')

	@design_value.setter
	def design_value(self, value : float):
		interface.SetValue(self._id, 'design_value', value)

	@property
	def is_array(self) -> bool:
		return interface.GetBoolValue(self._id, 'is_array')
		
	@is_array.setter
	def is_array(self, value : bool):
		interface.SetBoolValue(self._id, 'is_array', value)

	@property
	def array_size(self) -> int:
		return interface.GetIntValue(self._id, 'array_size')
		
	@array_size.setter
	def array_size(self, value : int):
		interface.SetIntValue(self._id, 'array_size', value)

	def get_quantile(self, quantile : float) -> float:
		return interface.GetArgValue(self._id, 'quantile', quantile)

	def get_x_from_u(self, u : float) -> float:
		return interface.GetArgValue(self._id, 'x_from_u', u)

	def get_u_from_x(self, x : float) -> float:
		return interface.GetArgValue(self._id, 'u_from_x', x)

	def get_pdf(self, x : float) -> float:
		return interface.GetArgValue(self._id, 'pdf', x)

	def get_cdf(self, x : float) -> float:
		return interface.GetArgValue(self._id, 'cdf', x)

	def get_x_from_u_and_source(self, u : float, x: float) -> float:
		interface.SetArrayValue(self._id, 'u_and_x', [u, x])
		interface.Execute(self._id, 'initialize_conditional_values');
		return interface.GetValue(self._id, 'x_from_u_and_source')

	def fit(self, values, shift = nan):
		"""Fits the stochast parameters from a list of values.

        Parameters
        ----------
        values : list[float]
            The list of values from which is fitted

        shift : float, optional
            If set, the shift value is not fitted, but taken from this value
        """

		if not isnan(shift):
			interface.SetValue(self._id, 'shift_for_fit', shift)
		interface.SetArrayValue(self._id, 'fit', values)
		self._histogram_values = None
		self._discrete_values = None
		self._fragility_values = None

	def can_fit_prior(self) -> bool:
		return interface.GetBoolValue(self._id, 'can_fit_prior')

	def fit_prior(self, prior : str | Stochast, values):
		if type(prior) == str:
			prior = self._variables[prior]

		if not self.can_fit_prior():
			print('Fit with prior is not supported for distribution type ' + str(self.distribution))
		elif self.distribution != prior.distribution:
			print('Fit from prior with another distribution type is not supported')
		else:
			interface.SetIntValue(self._id, 'prior', prior._id)
			interface.SetArrayValue(self._id, 'fit_prior', values)
			self._histogram_values = None
			self._discrete_values = None
			self._fragility_values = None

	def get_ks_test(self, values) -> float:
		interface.SetArrayValue(self._id, 'data', values)
		return interface.GetValue(self._id, 'ks_test')

	def copy_from(self, source):
		if type(source) == str:
			source = self._variables[source]
		if isinstance(source, Stochast):
			interface.SetIntValue(self._id, 'copy_from', source._id)
			self._histogram_values = None
			self._discrete_values = None
			self._fragility_values = None
			self._contributing_stochasts = None
			self._conditional_values = None

	def is_valid(self) -> bool:
		return interface.GetBoolValue(self._id, 'is_valid')

	def print(self, decimals = 4):
		pre = '  '
		if self.name == '':
			print(f'Variable:')
		else:
			print(f'Variable {self.name}:')
		if not self.truncated and not self.inverted:
			print(pre + f'distribution = {self.distribution}')
		elif self.truncated and not self.inverted:
			print(pre + f'distribution = {self.distribution} (truncated)')
		elif not self.truncated and self.inverted:
			print(pre + f'distribution = {self.distribution} (inverted)')
		elif self.truncated and self.inverted:
			print(pre + f'distribution = {self.distribution} (inverted, truncated')
		print('Definition:')
		if self.conditional:
			if self.conditional_source == '' or self.conditional_source == None:
				print(pre + f'conditional x values = [{", ".join([f"{value.x:.{decimals}g}" for value in self.conditional_values])}]')
			else:
				print(pre + f'conditional source = {self.conditional_source}')
				print(pre + f'{self.conditional_source} = [{", ".join([f"{value.x:.{decimals}g}" for value in self.conditional_values])}]')
			for prop in ['mean', 'deviation', 'location', 'scale', 'minimum', 'shift', 'shift_b', 'maximum', 'shape', 'shape_b', 'observations']:
				if interface.GetBoolValue(self._id, 'is_used_' + prop):
					if prop == 'observations':
						values = [interface.GetIntValue(value._id, prop) for value in self.conditional_values]
					else:
						values = [interface.GetValue(value._id, prop) for value in self.conditional_values]
					if len(values) > 0 and not isnan(values[0]):
						print(pre + f'{prop} = [{", ".join([f"{value:.{decimals}g}" for value in values])}]')
		else:
			for prop in ['location', 'scale', 'minimum', 'shift', 'shift_b', 'maximum', 'shape', 'shape_b', 'observations']:
				if interface.GetBoolValue(self._id, 'is_used_' + prop):
					if prop == 'observations':
						print(pre + f'{prop} = {interface.GetIntValue(self._id, prop)}')
					else:
						print(pre + f'{prop} = {interface.GetValue(self._id, prop):.{decimals}g}')
			if self.distribution == DistributionType.histogram:
				for value in self.histogram_values:
					print(pre + f'amount[{value.lower_bound:.{decimals}g}, {value.upper_bound:.{decimals}g}] = {value.amount:.{decimals}g}')
			elif self.distribution == DistributionType.cdf_curve:
				for value in self.fragility_values:
					print(pre + f'beta[{value.x:.{decimals}g}] = {value.reliability_index:.{decimals}g}')
			elif self.distribution == DistributionType.discrete or self.distribution == DistributionType.qualitative:
				for value in self.discrete_values:
					print(pre + f'amount[{value.x:.{decimals}g}] = {value.amount:.{decimals}g}')
			if self.design_quantile != 0.5 or self.design_factor != 1.0:
				print(pre + f'design_quantile = {self.design_quantile:.{decimals}g}')
				print(pre + f'design_factor = {self.design_factor:.{decimals}g}')
			print('Derived values:')
			print(pre + f'mean = {self.mean:.{decimals}g}')
			print(pre + f'deviation = {self.deviation:.{decimals}g}')
			print(pre + f'variation = {self.variation:.{decimals}g}')
			if self.design_quantile != 0.5 or self.design_factor != 1.0:
				print(pre + f'design_value = {self.design_value:.{decimals}g}')

	def get_special_values(self) -> list[float]:
		return interface.GetArrayValue(self._id, 'special_values')

	def get_series(self, xmin : float = None, xmax : float = None, number_of_points : int = None) -> list[float]:

		"""Gets a list of x values which is useful for plotting.

        The list of x values will be generated between a minimum and maximum with equal distance.
        The distance is based on the number points. The minimum and maximum are included always.
        The list of values will be extended with x-values to plot discontinuity points properly.

        Parameters
        ----------
        xmin : float, optional
            The minimum x value (default is None, a proper minimum value will be used)

        xmax : float, optional
            The maximum x value (default is None, a proper maximum value will be used)

        number_of_points : int, optional
            The number of points to be generated (default is None)

        """

		import numpy as np

		limit_special_values_min = xmin != None
		limit_special_values_max = xmax != None

		if xmin is None:
			xmin = self.get_x_from_u(0) - 4 * (self.get_x_from_u(0) - self.get_x_from_u(-1))
		if xmax is None:
			xmax = self.get_x_from_u(0) + 4 * (self.get_x_from_u(1) - self.get_x_from_u(0))

		xmin, xmax = NumericUtils.order(xmin, xmax)
		xmin, xmax = NumericUtils.make_different(xmin, xmax)

		if number_of_points is None or number_of_points < 0:
			number_of_points = 1000

        # ignore too few points, the minimum and maximum values are always included
		if number_of_points <= 2:
			values = [xmin, xmax]
		else:
			increment = (xmax - xmin) / (number_of_points - 1)
            # add increment to include maximum
			values = np.arange(xmin, xmax + increment, increment).tolist()
		add_values = self.get_special_values()
		if limit_special_values_min:
			add_values = [x for x in add_values if x >= xmin]
		if limit_special_values_max:
			add_values = [x for x in add_values if x <= xmax]
		values.extend(add_values)
		values.sort()

		return values

	def plot(self, xmin : float = None, xmax : float = None):

		self.get_plot(xmin, xmax).show()

	def get_plot(self, xmin : float = None, xmax : float = None) -> plt:

		if not self.is_valid():
			print('Variable definition is not valid, plot can not be made.')
			return

		if self.conditional:
			self._get_plot_conditional(xmin, xmax)
		else:
			self._get_plot(xmin, xmax)

		return plt

	def _get_plot(self, xmin : float = None, xmax : float = None):

		values = self.get_series(xmin, xmax)

		pdf = [self.get_pdf(x) for x in values]
		cdf = [self.get_cdf(x) for x in values]

		plt.close()
    
		fig, ax1 = plt.subplots()
		color = "tab:blue"
		if self.name == '':
			ax1.set_xlabel("value [x]")
		else:
			ax1.set_xlabel(f"{self.name} [x]")
		ax1.set_ylabel("pdf [-]", color=color)
		ax1.plot(values, pdf, label = '_pdf')
		ax1.tick_params(axis="y", labelcolor=color)
		ax2 = ax1.twinx()
		color = "tab:red"
		ax2.set_ylabel("cdf [-]", color=color)
		ax2.plot(values, cdf, "r--", label="_cdf")
		ax2.tick_params(axis="y", labelcolor=color)

	def _get_plot_conditional(self, xmin : float = None, xmax : float = None):

		if not self.is_valid():
			print('Variable definition is not valid, plot can not be made.')
			return

		import numpy as np
		import matplotlib.pyplot as plt

		if xmin is None:
			xmin = min([value.x for value in self.conditional_values])

		if xmax is None:
			xmax = max([value.x for value in self.conditional_values])

		xmin, xmax = NumericUtils.order(xmin, xmax)
		xmin, xmax = NumericUtils.make_different(xmin, xmax)

		values = np.arange(xmin, xmax, (xmax - xmin) / 100).tolist()
		for value in self.conditional_values:
			values.append(value.x)
		values.sort()

		u_low = StandardNormal.get_u_from_p(0.05)
		u_high = StandardNormal.get_u_from_p(0.95)
		median_values = [self.get_x_from_u_and_source(0, x) for x in values]
		low_values = [self.get_x_from_u_and_source(u_low, x) for x in values]
		high_values = [self.get_x_from_u_and_source(u_high, x) for x in values]
    
		plt.close()

		fig, ax1 = plt.subplots()
		color = "tab:blue"
		if self.conditional_source == None:
			ax1.set_xlabel("source [x]")
		else:
			ax1.set_xlabel(f"{self.conditional_source.name} [x]")
		if self.name == '':
			ax1.set_ylabel("value [x]")
		else:
			ax1.set_ylabel(f"{self.name} [x]")
		ax1.plot(values, median_values, label="50 %")
		ax1.plot(values, low_values, "b--", label="5 %")
		ax1.plot(values, high_values, "b--", label="95 %")
		ax1.tick_params(axis="y", labelcolor=color)

		plt.grid()
		plt.legend()

class DiscreteValue(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('discrete_value')
		else:
			self._id = id
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['x',
	            'amount']

	def create(x : float, amount : float):
		discreteValue = DiscreteValue()
		discreteValue.x = x
		discreteValue.amount = amount
		return discreteValue

	@property
	def x(self) -> float:
		return interface.GetValue(self._id, 'x')

	@x.setter
	def x(self, value : float):
		interface.SetValue(self._id, 'x',  value)

	@property
	def amount(self) -> float:
		return interface.GetValue(self._id, 'amount')

	@amount.setter
	def amount(self, value : float):
		interface.SetValue(self._id, 'amount',  value)

class FragilityValue(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('fragility_value')
		else:
			self._id = id
		self._design_point = None
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['x',
			'reliability_index',
			'probability_of_failure',
			'probability_of_non_failure',
			'return_period',
			'design_point']

	def create(x: float, reliability_index :float):
		fragilityValue = FragilityValue()
		fragilityValue.x = x
		fragilityValue.reliability_index = reliability_index
		return fragilityValue

	@property
	def x(self) -> float:
		return interface.GetValue(self._id, 'x')

	@x.setter
	def x(self, value : float):
		interface.SetValue(self._id, 'x',  value)

	@property
	def reliability_index(self) -> float:
		return interface.GetValue(self._id, 'reliability_index')

	@reliability_index.setter
	def reliability_index(self, value : float):
		interface.SetValue(self._id, 'reliability_index',  value)

	@property
	def probability_of_failure(self) -> float:
		return interface.GetValue(self._id, 'probability_of_failure')

	@probability_of_failure.setter
	def probability_of_failure(self, value : float):
		interface.SetValue(self._id, 'probability_of_failure',  value)

	@property
	def probability_of_non_failure(self) -> float:
		return interface.GetValue(self._id, 'probability_of_non_failure')

	@probability_of_non_failure.setter
	def probability_of_non_failure(self, value : float):
		interface.SetValue(self._id, 'probability_of_non_failure',  value)

	@property
	def return_period(self) -> float:
		return interface.GetValue(self._id, 'return_period')

	@return_period.setter
	def return_period(self, value : float):
		interface.SetValue(self._id, 'return_period',  value)

	@property
	def design_point(self) -> DesignPoint:
		return self._design_point

	@design_point.setter
	def design_point(self, value):
		self._design_point = value
		if self._design_point is not None:
			interface.SetIntValue(self._id, 'design_point',  self._design_point._id)

class HistogramValue(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('histogram_value')
		else:
			self._id = id
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['lower_bound',
		        'upper_bound',
		        'amount']

	def create(lower_bound : float, upper_bound : float, amount : float):
		histogramValue = HistogramValue();
		histogramValue.lower_bound = lower_bound
		histogramValue.upper_bound = upper_bound
		histogramValue.amount = amount
		return histogramValue

	@property
	def lower_bound(self) -> float:
		return interface.GetValue(self._id, 'lower_bound')

	@lower_bound.setter
	def lower_bound(self, value : float):
		interface.SetValue(self._id, 'lower_bound',  value)

	@property
	def upper_bound(self) -> float:
		return interface.GetValue(self._id, 'upper_bound')

	@upper_bound.setter
	def upper_bound(self, value : float):
		interface.SetValue(self._id, 'upper_bound',  value)

	@property
	def amount(self) -> float:
		return interface.GetValue(self._id, 'amount')

	@amount.setter
	def amount(self, value : float):
		interface.SetValue(self._id, 'amount',  value)

class ContributingStochast(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('contributing_stochast')
		else:
			self._id = id
		self._stochast = None
		self._variable = None
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['probability',
		        'variable']

	def create(probability : float, variable : Stochast):
		contributingStochast = ContributingStochast();
		contributingStochast.probability = probability
		contributingStochast.variable = variable
		return contributingStochast

	@property
	def probability(self) -> float:
		return interface.GetValue(self._id, 'probability')

	@probability.setter
	def probability(self, value : float):
		interface.SetValue(self._id, 'probability',  value)

	@property
	def variable(self) -> Stochast:
		if self._variable is None:
			id_ = interface.GetIdValue(self._id, 'variable')
			if id_ > 0:
				self._variable = Stochast(id_)
		return self._variable

	@variable.setter
	def variable(self, value : Stochast):
		self._variable = value
		if not self._variable is None:
			interface.SetIntValue(self._id, 'variable',  self._variable._id)

class ConditionalValue(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('conditional_value')
		else:
			self._id = id
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['x',
	            'mean',
	            'deviation',
				'location',
				'scale',
				'shift',
				'shift_b',
				'shape',
				'shape_b',
				'observations',
				'minimum',
				'maximum']

	@property
	def x(self) -> float:
		return interface.GetValue(self._id, 'x')

	@x.setter
	def x(self, value : float):
		interface.SetValue(self._id, 'x',  value)

	@property
	def mean(self) -> float:
		return interface.GetValue(self._id, 'mean')

	@mean.setter
	def mean(self, value : float):
		interface.SetValue(self._id, 'mean', value)

	@property
	def deviation(self) -> float:
		return interface.GetValue(self._id, 'deviation')

	@deviation.setter
	def deviation(self, value : float):
		interface.SetValue(self._id, 'deviation', value)

	@property
	def location(self) -> float:
		return interface.GetValue(self._id, 'location')

	@location.setter
	def location(self, value : float):
		interface.SetValue(self._id, 'location', value)

	@property
	def scale(self) -> float:
		return interface.GetValue(self._id, 'scale')

	@scale.setter
	def scale(self, value : float):
		interface.SetValue(self._id, 'scale', value)

	@property
	def shift(self) -> float:
		return interface.GetValue(self._id, 'shift')

	@shift.setter
	def shift(self, value : float):
		interface.SetValue(self._id, 'shift', value)

	@property
	def shift_b(self) -> float:
		return interface.GetValue(self._id, 'shift_b')

	@shift_b.setter
	def shift_b(self, value):
		interface.SetValue(self._id, 'shift_b', value)

	@property
	def minimum(self) -> float:
		return interface.GetValue(self._id, 'minimum')

	@minimum.setter
	def minimum(self, value : float):
		interface.SetValue(self._id, 'minimum', value)

	@property
	def maximum(self) -> float:
		return interface.GetValue(self._id, 'maximum')

	@maximum.setter
	def maximum(self, value : float):
		interface.SetValue(self._id, 'maximum', value)

	@property
	def shape(self) -> float:
		return interface.GetValue(self._id, 'shape')

	@shape.setter
	def shape(self, value):
		interface.SetValue(self._id, 'shape', value)

	@property
	def shape_b(self) -> float:
		return interface.GetValue(self._id, 'shape_b')

	@shape_b.setter
	def shape_b(self, value : float):
		interface.SetValue(self._id, 'shape_b', value)

	@property
	def observations(self) -> int:
		return interface.GetIntValue(self._id, 'observations')

	@observations.setter
	def observations(self, value : int):
		interface.SetIntValue(self._id, 'observations', value)

class CorrelationMatrix(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('correlation_matrix')
		else:
			self._id = id
		self._variables = FrozenList()
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	@property
	def variables(self) -> list[Stochast]:
		return self._variables

	def _set_variables(self, variables):
		self._variables = FrozenList(variables)
		interface.SetArrayIntValue(self._id, 'variables', [variable._id for variable in self._variables])

	def _update_variables(self, known_variables):
		update_variables = []
		stochast_ids = interface.GetArrayIdValue(self._id, 'variables')
		for stochast_id in stochast_ids:
			found = False
			for known_variable in known_variables:
				if not found and known_variable._id == stochast_id:
					update_variables.append(known_variable)
					found = True
			if not found:
				update_variables.append(Stochast(stochast_id))
		self._variables = FrozenList(update_variables)

	def __getitem__(self, stochasts) -> float:
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

class SelfCorrelationMatrix(FrozenObject):

	def __init__(self):
		self._id = interface.Create('self_correlation_matrix')
		self._variables = None
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def _set_variables(self, variables):
		self._variables = FrozenList(variables)

	def __getitem__(self, stochast : str | Stochast) -> float:
		stochast_obj = stochast
		if isinstance(stochast_obj, str):
			stochast_obj = self._variables[str(stochast_obj)]

		return interface.GetIntArgValue(self._id, stochast_obj._id, 'rho')

	def __setitem__(self, stochast : str | Stochast, value : float):
		stochast_obj = stochast
		if isinstance(stochast_obj, str):
			stochast_obj = self._variables[str(stochast_obj)]

		interface.SetIntArgValue(self._id, stochast_obj._id, 'rho', value)

class Scenario(FrozenObject):

	def __init__(self, id = None):
		if id is None:
			self._id = interface.Create('scenario')
		else:
			self._id = id
		super()._freeze()

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['name',
		        'probability']

	def __str__(self):
		return str(self.name)

	@property
	def name(self) -> str:
		return interface.GetStringValue(self._id, 'name')

	@name.setter
	def name(self, value : str):
		interface.SetStringValue(self._id, 'name', value)

	@property
	def probability(self) -> float:
		return interface.GetValue(self._id, 'probability')

	@probability.setter
	def probability(self, value : float):
		interface.SetValue(self._id, 'probability',  value)


