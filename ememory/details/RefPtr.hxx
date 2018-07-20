/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ememory/debug.hpp>
#include <ememory/RefPtr.hpp>

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<    etk::IsSame<EMEMORY_TYPE2, EMEMORY_TYPE>::value
                                 && etk::IsBaseOf<ememory::RefCounter, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr(EMEMORY_TYPE2* _element):
  m_element(_element) {
	EMEMORY_DBG("Create shared");
	if (m_element == null) {
		return;
	}
	EMEMORY_DBG("Check if the user use the memory allocator or personal system...");
	ETK_MEM_CHECK_POINTER(_element);
	EMEMORY_DBG("    ==> get previous pointer");
	m_element->refKeep();
	return;
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE> ememory::RefPtr<EMEMORY_TYPE>::create(EMEMORY_TYPE* _obj) {
	EMEMORY_DBG("Create shared");
	ememory::RefPtr<EMEMORY_TYPE> tmp{_obj};
	_obj->refRelease();
	return tmp;
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr():
  m_element(null) {
	EMEMORY_DBG("Create shared");
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr(etk::NullPtr):
  m_element(null) {
	EMEMORY_DBG("Create shared");
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr(EMEMORY_TYPE* _obj):
  m_element(_obj) {
	EMEMORY_DBG("Create shared (from a cast)");
	if (m_element == null) {
		return;
	}
	m_element->refKeep();
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>::~RefPtr() {
	EMEMORY_DBG("delete shared");
	reset();
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr(const ememory::RefPtr<EMEMORY_TYPE>& _obj):
  m_element(_obj.m_element) {
	if (m_element == null) {
		return;
	}
	m_element->refKeep();
};

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>& ememory::RefPtr<EMEMORY_TYPE>::operator= (const ememory::RefPtr<EMEMORY_TYPE>& _obj) {
	reset();
	m_element = _obj.m_element;
	if (m_element == null) {
		return *this;
	}
	m_element->refKeep();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>& ememory::RefPtr<EMEMORY_TYPE>::operator= (etk::NullPtr) {
	reset();
	return *this;
}


template<typename EMEMORY_TYPE>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr(ememory::RefPtr<EMEMORY_TYPE>&& _obj) {
	m_element = _obj.m_element;
	_obj.m_element = null;
}

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::RefPtr<EMEMORY_TYPE>::RefPtr(const ememory::RefPtr<EMEMORY_TYPE2>& _obj):
  m_element(const_cast<EMEMORY_TYPE2*>(_obj.get())) {
	if (m_element == null) {
		return;
	}
	m_element->refKeep();
}

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::RefPtr<EMEMORY_TYPE>& ememory::RefPtr<EMEMORY_TYPE>::operator= (const RefPtr<EMEMORY_TYPE2>& _obj) {
	reset();
	m_element = const_cast<EMEMORY_TYPE2*>(_obj.get());
	if (m_element == null) {
		return *this;
	}
	m_element->refKeep();
	return *this;
}

template<typename EMEMORY_TYPE>
void ememory::RefPtr<EMEMORY_TYPE>::reset() {
	if(m_element == null) {
		return;
	}
	EMEMORY_DBG("reset RefPtr (start)");
	m_element->refRelease();
	m_element = null;
	EMEMORY_DBG("reset RefPtr (stop)");
}

template<typename EMEMORY_TYPE>
int ememory::RefPtr<EMEMORY_TYPE>::useCount() const {
	if (m_element == null) {
		return 0;
	}
	return m_element->getRefCount();
}

template<typename EMEMORY_TYPE>
bool ememory::RefPtr<EMEMORY_TYPE>::operator==(etk::NullPtr) const {
	return m_element == null;
}

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2>
bool ememory::RefPtr<EMEMORY_TYPE>::operator==(const RefPtr<EMEMORY_TYPE2>& _obj) const {
	return m_element == _obj.get();
}

template<typename EMEMORY_TYPE>
bool ememory::RefPtr<EMEMORY_TYPE>::operator!=(etk::NullPtr) const {
	return m_element != null;
}

template<typename EMEMORY_TYPE>
template<typename EMEMORY_TYPE2>
bool ememory::RefPtr<EMEMORY_TYPE>::operator!=(const RefPtr<EMEMORY_TYPE2>& _obj) const {
	return m_element != _obj.get();
}

template<typename EMEMORY_TYPE>
const EMEMORY_TYPE* ememory::RefPtr<EMEMORY_TYPE>::get() const {
	return m_element;
}

template<typename EMEMORY_TYPE>
EMEMORY_TYPE* ememory::RefPtr<EMEMORY_TYPE>::get() {
	return m_element;
}

template<typename EMEMORY_TYPE>
const EMEMORY_TYPE* ememory::RefPtr<EMEMORY_TYPE>::operator->() const {
	return m_element;
}

template<typename EMEMORY_TYPE>
EMEMORY_TYPE* ememory::RefPtr<EMEMORY_TYPE>::operator->() {
	return m_element;
}

template<typename EMEMORY_TYPE>
const EMEMORY_TYPE& ememory::RefPtr<EMEMORY_TYPE>::operator*() const {
	return *m_element;
}

template<typename EMEMORY_TYPE>
EMEMORY_TYPE& ememory::RefPtr<EMEMORY_TYPE>::operator*() {
	return *m_element;
}


template<typename EMEMORY_TYPE>
void ememory::RefPtr<EMEMORY_TYPE>::swap(RefPtr& _obj) {
	etk::swap(_obj.m_element, m_element);
}

