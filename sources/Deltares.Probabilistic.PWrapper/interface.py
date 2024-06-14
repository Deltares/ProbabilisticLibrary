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
	lib.Create.restype = ctypes.c_void_p
	return lib.Create(object_type_b)

def Destroy(id_):
	lib.Destroy(ctypes.c_void_p(id_))

def GetValue(id_, property_):
	lib.GetValue.restype = ctypes.c_double
	return lib.GetValue(ctypes.c_void_p(id_), bytes(property_, 'utf-8'))

def SetValue(id_, property_, value_):
	lib.SetValue(ctypes.c_void_p(id_), bytes(property_, 'utf-8'), ctypes.c_double(value_))

def GetStringValue(id_, property_):
	return lib.GetStringValue(ctypes.c_void_p(id_), bytes(property_, 'utf-8'))

def SetStringValue(id_, property_, value_):
	lib.SetStringValue(ctypes.c_void_p(id_), bytes(property_, 'utf-8'), bytes(value_, 'utf-8'))

def GetArgValue(id_, property_, arg_):
	lib.GetArgValue.restype = ctypes.c_double
	return lib.GetArgValue(ctypes.c_void_p(id_), bytes(property_, 'utf-8'), ctypes.c_double(arg_))

def SetArgValue(id_, property_, arg_, value_):
	lib.SetArgValue(ctypes.c_void_p(id_), bytes(property_, 'utf-8'), ctypes.c_double(arg_), ctypes.c_double(value_))
