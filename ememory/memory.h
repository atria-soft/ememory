/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <mutex>
#include <ememory/SharedPtr.h>
#include <ememory/WeakPtr.h>
#include <ememory/EnableSharedFromThis.h>

namespace ememory {
	template<class EMEMORY_TYPE, typename... EMEMORY_ARGS>
	static ememory::SharedPtr<EMEMORY_TYPE> makeShared(EMEMORY_ARGS && ...args) {
		return ememory::SharedPtr<EMEMORY_TYPE>(new EMEMORY_TYPE(std::forward<EMEMORY_ARGS>(args)...));
	}
}
