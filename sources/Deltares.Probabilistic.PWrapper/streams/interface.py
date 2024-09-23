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
from ast import Pass
import ctypes
import sys
import os
import time

from pathlib import Path
from ctypes import cdll
from ctypes import *

CALLBACK = CFUNCTYPE(ctypes.c_double, POINTER(ctypes.c_double), ctypes.c_int)
EMPTY_CALLBACK = CFUNCTYPE(ctypes.c_void_p)

def LoadLibrary(lib_full_path):
	if os.path.isfile(lib_full_path):
		try:
			global lib
			lib = cdll.LoadLibrary(lib_full_path)
		except:
			message = sys.exc_info()[0]
			print('error: ' + message, flush = True)
			raise
	if lib == None:
		raise FileNotFoundError("Could not find " + lib_full_path)

def IsLibraryLoaded():
	return 'lib' in globals() and not lib is None

def LoadDefaultLibrary():
	dir_path = os.path.dirname(os.path.realpath(__file__))
	if (sys.platform.startswith("linux")):
		lib_file = 'libDeltares.Probabilistic.CWrapper.so'
	else:
		lib_file = 'Deltares.Probabilistic.CWrapper.dll'
	lib_full_path = os.path.join(dir_path, 'bin', lib_file);
	LoadLibrary(lib_full_path)

def Create(object_type):
	try:
		object_type_b = bytes(object_type, 'utf-8')
		lib.Create.restype = ctypes.c_int
		return lib.Create(object_type_b)
	except:
		message = sys.exc_info()[0]
		print('error: ' + message, flush = True)
		raise

def Destroy(id_):
	lib.Destroy(ctypes.c_int(id_))

def GetValue(id_, property_):
	lib.GetValue.restype = ctypes.c_double
	return lib.GetValue(ctypes.c_int(id_), bytes(property_, 'utf-8'))

def SetValue(id_, property_, value_):
	lib.SetValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_double(value_))

def GetIntValue(id_, property_):
	lib.GetIntValue.restype = ctypes.c_int
	return lib.GetIntValue(ctypes.c_int(id_), bytes(property_, 'utf-8'))

def SetIntValue(id_, property_, value_):
	lib.SetIntValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(value_))

def GetIntArgValue(id_, arg_, property_):
	lib.GetIntArgValue.restype = ctypes.c_double
	return lib.GetIntArgValue(ctypes.c_int(id_), ctypes.c_int(arg_), bytes(property_, 'utf-8'))

def SetIntArgValue(id_, arg_, property_, value_):
	lib.SetIntArgValue(ctypes.c_int(id_), ctypes.c_int(arg_), bytes(property_, 'utf-8'), ctypes.c_double(value_))

def GetBoolValue(id_, property_):
	lib.GetBoolValue.restype = ctypes.c_bool
	return lib.GetBoolValue(ctypes.c_int(id_), bytes(property_, 'utf-8'))

def SetBoolValue(id_, property_, value_):
	lib.SetBoolValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_bool(value_))

def GetStringValue(id_, property_):

	lib.GetStringLength.restype = ctypes.c_int
	size = lib.GetStringLength(ctypes.c_int(id_), bytes(property_, 'utf-8'))

	result = ctypes.create_string_buffer(size+1)
	lib.GetStringValue.restype = ctypes.c_void_p
	lib.GetStringValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), result, ctypes.c_size_t(sizeof(result)))
	result_str = result.value.decode()
	return result_str

def SetStringValue(id_, property_, value_):
	lib.SetStringValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), bytes(value_, 'utf-8'))

def SetArrayValue(id_, property_, values_):
	cvalues = (ctypes.c_double * len(values_))(*values_)
	lib.SetArrayValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.POINTER(ctypes.c_double)(cvalues), ctypes.c_uint(len(values_)))

def GetArrayValue(id_, property_):

	count_property = property_ + '_count'
	count = GetIntValue(id_, count_property)

	lib.GetIndexedValue.restype = ctypes.c_double

	values = []
	for i in range(count):
		value = lib.GetIndexedValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(i))
		values.append(value)

	return values

def GetArrayIntValue(id_, property_):

	count_property = property_ + '_count'
	count = GetIntValue(id_, count_property)

	lib.GetIndexedIntValue.restype = ctypes.c_int

	values = []
	for i in range(count):
		value = lib.GetIndexedIntValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(i))
		values.append(value)

	return values

def SetArrayIntValue(id_, property_, values_):
	cvalues = (ctypes.c_int * len(values_))(*values_)
	lib.SetArrayIntValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.POINTER(ctypes.c_int)(cvalues), ctypes.c_uint(len(values_)))

def GetArgValue(id_, property_, arg_):
	lib.GetArgValue.restype = ctypes.c_double
	return lib.GetArgValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_double(arg_))

def SetArgValue(id_, property_, arg_, value_):
	lib.SetArgValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_double(arg_), ctypes.c_double(value_))

def GetIndexedValue(id_, property_, index_):
	lib.GetIndexedValue.restype = ctypes.c_double
	return lib.GetIndexedValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(index_))

def SetIndexedValue(id_, property_, index_, value_):
	lib.SetIndexedValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(index_), ctypes.c_double(value_))

def GetIndexedIndexedValue(id_, property_, index1_, index2_):
	lib.GetIndexedIndexedValue.restype = ctypes.c_double
	return lib.GetIndexedIndexedValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(index1_), ctypes.c_int(index2_))

def SetIndexedIndexedValue(id_, property_, index1_, index2_, value_):
	lib.SetIndexedIndexedValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(index1_), ctypes.c_int(index2_), ctypes.c_double(value_))

def GetIntArgValue(id_, property_, arg_):
	lib.GetIntArgValue.restype = ctypes.c_int
	return lib.GetArgValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_int(arg_))

def SetCallBack(id_, property_, callBack_):
	try:
		lib.SetCallBack(ctypes.c_int(id_), bytes(property_, 'utf-8'), callBack_)
	except:
		message = sys.exc_info()[0]
		print('error: ' + str(message), flush = True)
		raise

def SetEmptyCallBack(id_, property_, callBack_):
	try:
		lib.SetInitializeCallBack(ctypes.c_int(id_), bytes(property_, 'utf-8'), callBack_)
	except:
		message = sys.exc_info()[0]
		print('error: ' + str(message), flush = True)
		raise

def Execute(id_, method_):
	lib.Execute(ctypes.c_int(id_), bytes(method_, 'utf-8'))
