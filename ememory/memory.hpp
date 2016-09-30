/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <mutex>
#include <ememory/SharedPtr.hpp>
#include <ememory/WeakPtr.hpp>
#include <ememory/EnableSharedFromThis.hpp>

/**
 * @brief Ememory is a namespace to represent the @code #include <memory> @endcode part ==> simple memory access abstraction
 */
namespace ememory {
	/**
	 * @brief Create a SharedPtr with specific arguments
	 * @param[in] _args Multiples argument to add in the EMEMORY_TYPE public constructor.
	 * @return the requested created SharedPtr
	 */
	template<class EMEMORY_TYPE, typename... EMEMORY_ARGS>
	static ememory::SharedPtr<EMEMORY_TYPE> makeShared(EMEMORY_ARGS && ..._args) {
		return ememory::SharedPtr<EMEMORY_TYPE>(new EMEMORY_TYPE(std::forward<EMEMORY_ARGS>(_args)...));
	}
	/**
	 * @brief Cast in Dynamic the input SharedPtr into an other type like dynamic_cast on pointer
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> dynamicPointerCast(ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(dynamic_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	/**
	 * @brief CONST Cast in Dynamic the input SharedPtr into an other type like dynamic_cast on pointer
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline const ememory::SharedPtr<EMEMORY_TYPE_CAST> dynamicPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(dynamic_cast<EMEMORY_TYPE_CAST*>(const_cast<EMEMORY_TYPE*>(_obj.get())), _obj.getCounter());
	}
	/**
	 * @brief Cast in static the input SharedPtr into an other type like static_cast on pointer
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> staticPointerCast(ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(static_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	/**
	 * @brief CONST Cast in static the input SharedPtr into an other type like static_cast on pointer
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline const ememory::SharedPtr<EMEMORY_TYPE_CAST> staticPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(static_cast<EMEMORY_TYPE_CAST*>(const_cast<EMEMORY_TYPE*>(_obj.get())), _obj.getCounter());
	}
	/**
	 * @brief Cast in reinterpret the input SharedPtr into an other type like reinterpret_cast on pointer
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> reinterpretPointerCast(ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(reinterpret_cast<EMEMORY_TYPE_CAST*>(_obj.get()), _obj.getCounter());
	}
	/**
	 * @brief CONST Cast in reinterpret the input SharedPtr into an other type like reinterpret_cast on pointer
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline const ememory::SharedPtr<EMEMORY_TYPE_CAST> reinterpretPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(reinterpret_cast<EMEMORY_TYPE_CAST*>(const_cast<EMEMORY_TYPE*>(_obj.get())), _obj.getCounter());
	}
	/**
	 * @brief Cast in const the input SharedPtr into an other type like const_cast on pointer (remove constness)
	 * @param[in] _obj Object To cast
	 * @return Casted Object
	 */
	template<class EMEMORY_TYPE_CAST, class EMEMORY_TYPE>
	inline ememory::SharedPtr<EMEMORY_TYPE_CAST> constPointerCast(const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
		return ememory::SharedPtr<EMEMORY_TYPE_CAST>(const_cast<EMEMORY_TYPE*>(_obj.get()), _obj.getCounter());
	}
}
