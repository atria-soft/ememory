#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


def get_type():
	return "BINARY"

def get_sub_type():
	return "TEST"

def get_desc():
	return "eSVG test-unit"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'test/main.cpp',
		'test/testShared.cpp',
		'test/testWeak.cpp',
		'test/testEnableSharedFromThis.cpp',
		'test/testCasts.cpp'
		])
	my_module.add_module_depend(['ememory', 'gtest', 'test-debug'])
	return my_module

