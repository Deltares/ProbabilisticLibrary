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
from math import isnan
from enum import Enum

from .utils import FrozenObject, FrozenList, PrintUtils
from . import interface

if not interface.IsLibraryLoaded():
	interface.LoadDefaultLibrary()

class MessageType(Enum):
	"""Enumeration which defines the severity of a message."""
	debug = 'debug'
	info = 'info'
	warning = 'warning'
	error = 'error'
	def __str__(self):
		return str(self.value)

class Message(FrozenObject):
	"""Defines a message, which will be presented to the user.
    A message may be the result of a validation request or an informative message of an algorithm"""

	def __init__(self, id = None):
		if id == None:
			self._id = interface.Create('message')
		else:
			self._id = id
		super()._freeze()

	@classmethod
	def from_message(cls, message_type, message_text):
		message = cls()
		interface.SetStringValue(message._id, 'type', str(message_type))
		interface.SetStringValue(message._id, 'text', message_text)
		return message

	def __del__(self):
		interface.Destroy(self._id)
		
	def __str__(self):
		if self.subject == "":
			return str(self.type) + ': ' + self.text
		else:
			return str(self.type) + ': ' + self.subject + ' => ' + self.text
		
	def __dir__(self):
		return ['type',
		        'subject',
		        'text',
		        'print']

	@property
	def type(self) -> MessageType:
		"""Gets the message type, which indicates the severity of the message"""
		return MessageType[interface.GetStringValue(self._id, 'type')]
		
	@property
	def subject(self) -> str:
		"""Gets the subject, the object to which the message applies"""
		return interface.GetStringValue(self._id, 'subject')

	@property
	def text(self) -> str:
		"""Gets the message text"""
		return interface.GetStringValue(self._id, 'text')

	def print(self):
		"""Prints the message"""
		text = str(self)
		if len(text) > 0:
			text = text[0].capitalize() + text[1:]
		print(text)

class ValidationReport(FrozenObject):
	"""Result of a validation analysis, consists of a number of validation messages"""

	def __init__(self, id = None):
		if id == None:
			self._id = interface.Create('validation_report')
		else:
			self._id = id
		self._messages = None
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['is_valid',
		        'messages',
		        'print']

	def is_valid(self) -> bool:
		"""Indicates whether this validation report should be interpreted as valid or non-valid"""
		return interface.GetBoolValue(self._id, 'is_valid')

	@property   
	def messages(self) -> FrozenList[Message]:
		"""List of all validation messages"""
		if self._messages is None:
			message_ids = interface.GetArrayIdValue(self._id, 'messages')
			messages = []
			for message_id in message_ids:
				message = Message(message_id)
				messages.append(message)
			self._messages = FrozenList(messages)
		return self._messages

	def print(self):
		"""Prints all validation messages or an indication in case of no validation messages"""
		if len(self.messages) == 0:
			print('ok')
		else:
			for message in self.messages:
				message.print()

		
class Evaluation(FrozenObject):
	"""Registers a sample and execution results of a model"""
		
	def __init__(self, id = None):
		if id == None:
			self._id = interface.Create('evaluation')
		else:
			self._id = id
		self._input_values = None	
		self._output_values = None	
		super()._freeze()

	def __del__(self):
		interface.Destroy(self._id)

	def __dir__(self):
		return ['iteration',
				'quantile',
				'z',
				'beta',
				'weight',
				'input_values',
				'output_values',
				'print']
	
	@property   
	def iteration(self) -> int:
		"""Iteration index in a reliability, uncertainty or sensitivity algorithm"""
		return interface.GetIntValue(self._id, 'iteration')
		
	@property   
	def quantile(self) -> float:
		"""Quantile to which the sample belongs in an uncertainty algorithm"""
		return interface.GetValue(self._id, 'quantile')
		
	@property   
	def z(self) -> float:
		"""Z-value, indicating failure or non-failure, used by a reliability algorithm"""
		return interface.GetValue(self._id, 'z')
		
	@property   
	def beta(self) -> float:
		"""Distance of the sample to the origin in u-space"""
		return interface.GetValue(self._id, 'beta')
		
	@property   
	def weight(self) -> float:
		"""Weight of the sample in the reliability or uncertainty algorithm"""
		return interface.GetValue(self._id, 'weight')

	@property   
	def input_values(self) -> FrozenList[float]:
		"""List of input values for the model"""
		if self._input_values is None:
			input_values = interface.GetArrayValue(self._id, 'input_values')
			self._input_values = FrozenList(input_values)
		return self._input_values
		
	@property   
	def output_values(self) -> FrozenList[float]:
		"""List of output values produced by the model"""
		if self._output_values is None:
			output_values = interface.GetArrayValue(self._id, 'output_values')
			self._output_values = FrozenList(output_values)
		return self._output_values

	def print(self, decimals = 4):
		"""Prints the evaluation

        Parameters
        ----------
        decimals : int, optional.
            The number of decimals to print"""

		self._print(0, decimals)

	def _print(self, indent, decimals = 4):
		pre = PrintUtils.get_space_from_indent(indent)
		input_values = ', '.join([f'{v:.{decimals}g}' for v in self.input_values])
		output_values = ', '.join([f'{v:.{decimals}g}' for v in self.output_values])
		if not isnan(self.quantile):
			pre = pre + f'quantile {self.quantile:.{decimals}g}: '
		if isnan(self.z) and len(self.output_values) == 0:
			print(pre + f'[{input_values}]')
		elif isnan(self.z) and len(self.output_values) > 0:
			print(pre + f'[{input_values}] -> [{output_values}]')
		elif not isnan(self.z) and len(self.output_values) == 0:
			print(pre + f'[{input_values}] -> {self.z:.{decimals}g}')
		elif not isnan(self.z) and len(self.output_values) > 0:
			print(pre + f'[{input_values}] -> [{output_values}] -> {self.z:.{decimals}g}')
		
