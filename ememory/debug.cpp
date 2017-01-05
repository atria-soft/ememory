/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */

#include <ememory/debug.hpp>

int32_t ememory::getLogId() {
	static int32_t g_val = elog::registerInstance("ememory");
	return g_val;
}

