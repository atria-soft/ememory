/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <test-debug/debug.hpp>
#include <etk/Vector.hpp>
#include <etest/etest.hpp>
//#include <etk/etk.hpp>


int main(int _argc, const char *_argv[]) {
	//etk::init(_argc, _argv);
	etest::init(_argc, _argv);
	for (int32_t iii=0; iii<_argc ; ++iii) {
		etk::String data = _argv[iii];
		if (    data == "-h"
		     || data == "--help") {
			TEST_PRINT(_argv[0] << " - help : ");
			TEST_PRINT("    " << _argv[0] << " [options]");
			TEST_PRINT("        No optiions ...");
			return -1;
		}
	}
	//etk::initDefaultFolder("esvg-test");
	return RUN_ALL_TESTS();
}
