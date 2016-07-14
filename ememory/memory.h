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
	static ememory::SharedPtr<EMEMORY_TYPE> makeShared(EMEMORY_ARGS && ..._args) {
		return ememory::SharedPtr<EMEMORY_TYPE>(new EMEMORY_TYPE(std::forward<EMEMORY_ARGS>(_args)...));
	}
	
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> dynamicPointerCast(ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(dynamic_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline const ememory::SharedPtr<EMEMORY_TYPE_CAST> dynamicPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(dynamic_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> staticPointerCast(ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(static_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline const ememory::SharedPtr<EMEMORY_TYPE_CAST> staticPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(static_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> reinterpretPointerCast(ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(reinterpret_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline const ememory::SharedPtr<EMEMORY_TYPE_CAST> reinterpretPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(reinterpret_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
}
