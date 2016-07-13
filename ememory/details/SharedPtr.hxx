/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <mutex>
#include <ememory/debug.h>
#include <ememory/WeakPtr.h>

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(EMEMORY_TYPE* _element):
  m_element(_element),
  m_counter(nullptr) {
	EMEMORY_VERBOSE("new shared");
	if (m_element == nullptr) {
		return;
	}
	// check if the clas does not refer itself ... to get his own sharedPtr
	ememory::EnableSharedFromThis<EMEMORY_TYPE> upperClass = static_cast<ememory::EnableSharedFromThis<EMEMORY_TYPE>*>(m_element);
	if (upperClass != nullptr) {
		m_counter = upperClass->m_weakThis;
		if (m_counter != nullptr) {
			m_counter->incrementShared();
		}
	} else {
		m_counter = new ememory::Counter(false);
	}
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(EMEMORY_TYPE* _element):
  m_element(_element),
  m_counter(nullptr) {
	EMEMORY_VERBOSE("new shared");
	if (m_element == nullptr) {
		return;
	}
	m_counter = new ememory::Counter(false);
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr():
  m_element(nullptr),
  m_counter(nullptr) {
	EMEMORY_VERBOSE("new shared");
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::~SharedPtr() {
	EMEMORY_VERBOSE("delete shared");
	reset();
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(const ememory::SharedPtr<EMEMORY_TYPE>& _obj):
  m_element(_obj.m_element),
  m_counter(_obj.m_counter) {
	if (    m_element == nullptr
	     || m_counter == nullptr) {
		m_element = nullptr;
		m_counter = nullptr;
		return;
	}
	if (m_counter == nullptr) {
		return;
	}
	m_counter->incrementShared();
};

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>& ememory::SharedPtr<EMEMORY_TYPE>::operator= (const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	if (    m_element == nullptr
	     || m_counter == nullptr) {
		m_element = nullptr;
		m_counter = nullptr;
		return *this;
	}
	if (m_counter == nullptr) {
		return *this;
	}
	m_counter->incrementShared();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(ememory::SharedPtr<EMEMORY_TYPE>&& _obj) {
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	_obj.m_element = nullptr;
	_obj.m_counter = nullptr;
}

template<typename EMEMORY_TYPE>
void ememory::SharedPtr<EMEMORY_TYPE>::reset() {
	if(m_counter == nullptr) {
		m_element = nullptr; // in case ...
		return;
	}
	EMEMORY_VERBOSE("reset sharedPtr (start)");
	ememory::Counter::remove rmData = m_counter->decrementShared();
	switch(rmData) {
		case ememory::Counter::remove::all:
			delete m_counter;
			delete m_element;
			break;
		case ememory::Counter::remove::data:
			delete m_element;
			break;
		case ememory::Counter::remove::counter:
			delete m_counter;
			break;
		case ememory::Counter::remove::none:
			break;
	}
	m_counter = nullptr;
	m_element = nullptr;
	EMEMORY_VERBOSE("reset sharedPtr (stop)");
}

template<typename EMEMORY_TYPE>
int64_t ememory::SharedPtr<EMEMORY_TYPE>::useCount() const {
	if (m_counter == nullptr) {
		return 0;
	}
	return m_counter->getCountShared();
}

template<typename EMEMORY_TYPE>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator == (std::nullptr_t) {
	return m_counter == nullptr;
}

template<typename EMEMORY_TYPE>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator==(const SharedPtr& _obj) const {
	return m_counter == _obj.m_counter;
}

template<typename EMEMORY_TYPE>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator != (std::nullptr_t) {
	return m_counter != nullptr;
}

template<typename EMEMORY_TYPE>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator!=(const SharedPtr& _obj) const {
	return m_counter != _obj.m_counter;
}

template<typename EMEMORY_TYPE>
const EMEMORY_TYPE* ememory::SharedPtr<EMEMORY_TYPE>::get() const {
	return m_element;
}

template<typename EMEMORY_TYPE>
EMEMORY_TYPE* ememory::SharedPtr<EMEMORY_TYPE>::get() {
	return m_element;
}

template<typename EMEMORY_TYPE>
const EMEMORY_TYPE* ememory::SharedPtr<EMEMORY_TYPE>::operator->() const {
	return m_element;
}

template<typename EMEMORY_TYPE>
EMEMORY_TYPE* ememory::SharedPtr<EMEMORY_TYPE>::operator->() {
	return m_element;
}

template<typename EMEMORY_TYPE>
const EMEMORY_TYPE& ememory::SharedPtr<EMEMORY_TYPE>::operator*() const {
	return *m_element;
}

template<typename EMEMORY_TYPE>
EMEMORY_TYPE& ememory::SharedPtr<EMEMORY_TYPE>::operator*() {
	return *m_element;
}

template<typename EMEMORY_TYPE>
void ememory::SharedPtr<EMEMORY_TYPE>::swap(SharedPtr& _obj) {
	EMEMORY_TYPE* tmpE = m_element;
	ememory::Counter* tmpC = m_counter;
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	_obj.m_element = tmpE;
	_obj.m_counter = tmpC;
}


