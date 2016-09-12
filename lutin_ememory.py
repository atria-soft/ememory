#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol memory basic interface (have a thread safe shared_ptr with constness)"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
		'ememory/debug.cpp',
		'ememory/Counter.cpp'
		])
	
	my_module.add_header_file([
		'ememory/debug.h',
		'ememory/memory.h',
		'ememory/Counter.h',
		'ememory/SharedPtr.h',
		'ememory/WeakPtr.h',
		'ememory/EnableSharedFromThis.h',
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
	
	my_module.add_path(tools.get_current_path(__file__))
	return my_module


