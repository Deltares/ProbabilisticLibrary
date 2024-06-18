import ctypes
import sys
import os
import time

from pathlib import Path
from ctypes import cdll

paths = []
paths.append('.')
paths.append('../x64/Debug')
paths.append('../x64/Release')

lib_file = 'Deltares.Probabilistic.CWrapper.dll'

lib = None

try:
	for path in paths:
		full_lib_file = os.path.join(path, lib_file)
		if os.path.isfile(full_lib_file):
			lib = cdll.LoadLibrary(full_lib_file)
			break
except:
	message = sys.exc_info()[0]
	print('error: ' + message, flush = True)
	raise

if lib == None:
	raise FileNotFoundError("Could not find " + lib_file)

def Create(object_type):
	object_type_b = bytes(object_type, 'utf-8')
	lib.Create.restype = ctypes.c_int
	return lib.Create(object_type_b)

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

def GetBoolValue(id_, property_):
	lib.GetBoolValue.restype = ctypes.c_bool
	return lib.GetBoolValue(ctypes.c_int(id_), bytes(property_, 'utf-8'))

def SetBoolValue(id_, property_, value_):
	lib.SetBoolValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_bool(value_))

def GetStringValue(id_, property_):
	return lib.GetStringValue(ctypes.c_int(id_), bytes(property_, 'utf-8'))

def SetStringValue(id_, property_, value_):
	lib.SetStringValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), bytes(value_, 'utf-8'))

def SetArrayValue(id_, property_, values_):
	cvalues = (ctypes.c_int * values_.count)(*values_)
	lib.SetArrayValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.POINTER(ctypes.c_int)(cvalues), ctypes.c_int(values_.count))

def GetArgValue(id_, property_, arg_):
	lib.GetArgValue.restype = ctypes.c_double
	return lib.GetArgValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_double(arg_))

def SetArgValue(id_, property_, arg_, value_):
	lib.SetArgValue(ctypes.c_int(id_), bytes(property_, 'utf-8'), ctypes.c_double(arg_), ctypes.c_double(value_))
