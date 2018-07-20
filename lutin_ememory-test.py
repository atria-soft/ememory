#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "BINARY"

def get_sub_type():
	return "TEST"

def get_desc():
	return "e-memory test-unit"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def configure(target, my_module):
	my_module.add_src_file([
	    'test/main.cpp',
	    'test/testUnique.cpp',
	    'test/testRef.cpp',
	    'test/testShared.cpp',
	    'test/testWeak.cpp',
	    'test/testEnableSharedFromThis.cpp',
	    'test/testCasts.cpp'
	    ])
	my_module.add_depend([
	    'cxx',
	    'ememory',
	    'test-debug',
	    'etest',
	    ])
	return True

