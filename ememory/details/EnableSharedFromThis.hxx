/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ememory/SharedPtr.h>
#include <ememory/WeakPtr.h>

template<typename EMEMORY_TYPE>
ememory::EnableSharedFromThis<EMEMORY_TYPE>::EnableSharedFromThis() :
  m_weakThis(static_cast<EMEMORY_TYPE*>(this)) {
	
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE> ememory::EnableSharedFromThis<EMEMORY_TYPE>::sharedFromThis() {
	return m_weakThis.lock();
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE> ememory::EnableSharedFromThis<EMEMORY_TYPE>::weakFromThis() {
	return m_weakThis;
}

