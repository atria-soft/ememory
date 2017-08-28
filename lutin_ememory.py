#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol memory basic interface (have a thread safe shared_ptr with constness)"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
		'ememory/debug.cpp',
		'ememory/Counter.cpp'
		])
	
	my_module.add_header_file([
		'ememory/debug.hpp',
		'ememory/memory.hpp',
		'ememory/Counter.hpp',
		'ememory/SharedPtr.hpp',
		'ememory/UniquePtr.hpp',
		'ememory/WeakPtr.hpp',
		'ememory/EnableSharedFromThis.hpp',
		'ememory/details/memory.hxx',
		'ememory/details/SharedPtr.hxx',
		'ememory/details/WeakPtr.hxx',
		'ememory/details/EnableSharedFromThis.hxx'
		])
	
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	# add dependency of the generic C++ library:
	my_module.add_depend('cxx')
	my_module.add_optionnal_depend('elog')
	
	my_module.add_path(".")
	return True


