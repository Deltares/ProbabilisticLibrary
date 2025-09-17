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
from math import isnan
from enum import Enum
import matplotlib.pyplot as plt

from .utils import *
from .statistic import Stochast, ProbabilityValue
from .reliability import StochastSettings, RandomType, GradientType, Evaluation, Message
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class SensitivityMethod(Enum):
	single_variation = 'single_variation'
	sobol = 'sobol'
	def __str__(self):
		return str(self.value)

class SensitivitySettings(FrozenObject):

	def __init__(self):
		self._id = interface.Create('sensitivity_settings')
		self._stochast_settings = FrozenList()
		self._quantiles = None
		self._synchronizing = False
		super()._freeze()
		interface.SetBoolValue(self._id, 'use_openmp_in_reliability', False)

	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['sensitivity_method',
		        'low_value',
		        'high_value',
		        'iterations']
		
	@property
	def save_realizations(self) -> bool:
		return interface.GetBoolValue(self._id, 'save_realizations')

	@save_realizations.setter
	def save_realizations(self, value : bool):
		interface.SetBoolValue(self._id, 'save_realizations', value)

	@property
	def save_messages(self) -> bool:
		return interface.GetBoolValue(self._id, 'save_messages')

	@save_messages.setter
	def save_messages(self, value : bool):
		interface.SetBoolValue(self._id, 'save_messages', value)

	@property
	def reuse_calculations(self) -> bool:
		return interface.GetBoolValue(self._id, 'reuse_calculations')

	@reuse_calculations.setter
	def reuse_calculations(self, value : bool):
		interface.SetBoolValue(self._id, 'reuse_calculations', value)

	@property
	def sensitivity_method(self) -> SensitivityMethod:
		return SensitivityMethod[interface.GetStringValue(self._id, 'sensitivity_method')]
		
	@sensitivity_method.setter
	def sensitivity_method(self, value : SensitivityMethod):
		interface.SetStringValue(self._id, 'sensitivity_method', str(value))

	@property
	def iterations(self) -> int:
		return interface.GetIntValue(self._id, 'iterations')

	@iterations.setter
	def iterations(self, value : int):
		interface.SetIntValue(self._id, 'iterations', value)

	@property
	def u_low(self) -> float:
		return interface.GetValue(self._id, 'u_low')
		
	@u_low.setter
	def u_low(self, value : float):
		interface.SetValue(self._id, 'u_low', value)

	@property
	def u_high(self) -> float:
		return interface.GetValue(self._id, 'u_high')
		
	@u_high.setter
	def u_high(self, value : float):
		interface.SetValue(self._id, 'u_high', value)

	def _set_variables(self, variables):
		new_stochast_settings = []
		for variable in variables:
			stochast_setting = self._stochast_settings[str(variable)]
			if stochast_setting is None:
				stochast_setting = StochastSettings(variable)
			new_stochast_settings.append(stochast_setting)
		self._stochast_settings = FrozenList(new_stochast_settings)
		interface.SetArrayIntValue(self._id, 'stochast_settings', [stochast_setting._id for stochast_setting in self._stochast_settings])

class SensitivityValue(FrozenObject):

	def __init__(self, id):
		self._id = id
		self._variable = None
		super()._freeze()
		
	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['variable',
				'low',
				'medium',
				'high',
				'first_order_index',
				'total_index',
				'print']
		
	def __str__(self):
		return self.variable.name

	@property
	def variable(self) -> Stochast:
		if self._variable is None:
			variable_id = interface.GetIdValue(self._id, 'variable')
			if variable_id > 0:
				self._variable = Stochast(variable_id);
				
		return self._variable

	@property
	def low(self) -> float:
		return interface.GetValue(self._id, 'low')

	@property
	def medium(self) -> float:
		return interface.GetValue(self._id, 'medium')

	@property
	def high(self) -> float:
		return interface.GetValue(self._id, 'high')

	@property
	def first_order_index(self) -> float:
		return interface.GetValue(self._id, 'first_order_index')

	@property
	def total_index(self) -> float:
		return interface.GetValue(self._id, 'total_index')

class SensitivityResult(FrozenObject):

	def __init__(self, id):
		self._id = id
		self._variable = None
		self._values = None
		self._messages = None
		self._realizations = None
		super()._freeze()
		
	def __del__(self):
		try:
			interface.Destroy(self._id)
		except:
			pass

	def __dir__(self):
		return ['identifier',
				'variable',
				'realizations',
				'messages',
				'print',
				'plot']
		
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
	def values(self) -> list[SensitivityValue]:
		if self._values is None:
			sens_values = []
			sens_value_ids = interface.GetArrayIdValue(self._id, 'sensitivity_values')
			for sens_value_id in sens_value_ids:
				sens_values.append(SensitivityValue(sens_value_id))
			self._values = FrozenList(sens_values)
				
		return self._values
	
	@property
	def messages(self) -> list[Message]:
		if self._messages is None:
			messages = []
			message_ids = interface.GetArrayIdValue(self._id, 'messages')
			for message_id in message_ids:
				messages.append(Message(message_id))
			self._messages = FrozenList(messages)
				
		return self._messages

	def print(self, decimals=4):
		self.variable.print(decimals)
		if len(self.quantile_realizations) > 0:
			print('Quantiles:')
			for quantile in self.quantile_realizations:
				quantile._print(1, decimals)

	def plot(self, xmin : float = None, xmax : float = None):
		self.get_plot(xmin, xmax).show()

	def get_plot(self, xmin : float = None, xmax : float = None) -> plt:

		vplot = self.variable.get_plot(xmin, xmax)

		plot_legend = False
		for ii in range(len(self.quantile_realizations)):
			vplot.axvline(x=self.quantile_realizations[ii].output_values[0], color="green", linestyle="--", label=f"{self.quantile_realizations[ii].quantile:.4g}-quantile")
			plot_legend = True

		if plot_legend:
			vplot.legend()

		return vplot




