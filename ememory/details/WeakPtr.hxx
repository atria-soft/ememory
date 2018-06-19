/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ethread/Mutex.hpp>
#include <ememory/debug.hpp>
#include <ememory/Counter.hpp>

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr():
  m_element(null),
  m_counter(null) {
	
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(etk::NullPtr):
  m_element(null),
  m_counter(null) {
	
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(EMEMORY_TYPE* _element):
  m_element(_element),
  m_counter(null) {
	EMEMORY_DBG("New weak");
	if (m_element == null) {
		return;
	}
	m_counter = ETK_NEW(ememory::Counter, true);
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::~WeakPtr() {
	reset();
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(const ememory::WeakPtr<EMEMORY_TYPE>& _obj):
  m_element(_obj.m_element),
  m_counter(_obj.m_counter) {
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return;
	}
	if (m_counter == null) {
		return;
	}
	m_counter->incrementWeak();
};

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (const ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	reset();
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return *this;
	}
	if (m_counter == null) {
		return *this;
	}
	m_counter->incrementWeak();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (etk::NullPtr) {
	reset();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(ememory::WeakPtr<EMEMORY_TYPE>&& _obj) {
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	_obj.m_element = null;
	_obj.m_counter = null;
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(const ememory::SharedPtr<EMEMORY_TYPE>& _obj):
  m_element(_obj.m_element),
  m_counter(_obj.m_counter) {
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return;
	}
	if (m_counter == null) {
		return;
	}
	m_counter->incrementWeak();
}


template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
	reset();
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return *this;
	}
	if (m_counter == null) {
		return *this;
	}
	m_counter->incrementWeak();
	return *this;
}


template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<    etk::IsVoid<EMEMORY_TYPE>::value
                                  && !etk::IsVoid<EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(const ememory::SharedPtr<EMEMORY_TYPE2>& _obj):
  m_element((void*)_obj.get()),
  m_counter(_obj.getCounter()) {
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return;
	}
	if (m_counter == null) {
		return;
	}
	m_counter->incrementWeak();
}


template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<    etk::IsVoid<EMEMORY_TYPE>::value
                                  && !etk::IsVoid<EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (const ememory::SharedPtr<EMEMORY_TYPE2>& _obj) {
	reset();
	m_element = (void*)_obj.get();
	m_counter = _obj.getCounter();
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return *this;
	}
	if (m_counter == null) {
		return *this;
	}
	m_counter->incrementWeak();
	return *this;
}


/*
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
WeakPtr(const WeakPtr<EMEMORY_TYPE2>& _obj):
  m_element(_obj.m_element),
  m_counter(_obj.getCounter()) {
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return;
	}
	if (m_counter == null) {
		return;
	}
	m_counter->incrementWeak();
	
}
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
WeakPtr& operator= (const WeakPtr<EMEMORY_TYPE2>& _obj) {
	reset();
	m_element = _obj.m_element;
	m_counter = _obj.getCounter();
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return *this;
	}
	if (m_counter == null) {
		return *this;
	}
	m_counter->incrementWeak();
	return *this;
}
*/
template<typename EMEMORY_TYPE>
void ememory::WeakPtr<EMEMORY_TYPE>::reset() {
	if(m_counter == null) {
		m_element = null; // in case ...
		return;
	}
	ememory::Counter::remove rmData = m_counter->decrementWeak();
	switch(rmData) {
		case ememory::Counter::remove::all:
			EMEMORY_ERROR("in WeakPtr request remove all (impossible case ...)");
			break;
		case ememory::Counter::remove::data:
			EMEMORY_ERROR("in WeakPtr request remove all (impossible case ...)");
			break;
		case ememory::Counter::remove::counter:
			ETK_DELETE(ememory::Counter, m_counter);
			break;
		case ememory::Counter::remove::none:
			break;
	}
	m_counter = null;
	m_element = null;
}

template<typename EMEMORY_TYPE>
int ememory::WeakPtr<EMEMORY_TYPE>::useCount() const {
	if (m_counter == null) {
		return 0;
	}
	return m_counter->getCountShared();
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::expired() const {
	if (m_counter == null) {
		return true;
	}
	return m_counter->getCountShared() == 0;
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE> ememory::WeakPtr<EMEMORY_TYPE>::lock() {
	ememory::SharedPtr<EMEMORY_TYPE> out;
	if (m_counter == null) {
		return out;
	}
	int64_t count = m_counter->incrementShared(true);
	if (count == 0) {
		return out;
	}
	out.m_counter = m_counter;
	out.m_element = m_element;
	return etk::move(out);
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator==(const ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	return m_counter == _obj.m_counter;
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator==(etk::NullPtr) const {
	return m_counter == null;
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator!=(const ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	return m_counter != _obj.m_counter;
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator!=(etk::NullPtr) const {
	return m_counter != null;
}

template<typename EMEMORY_TYPE>
void ememory::WeakPtr<EMEMORY_TYPE>::swap(ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	etk::swap(_obj.m_element, m_element);
	etk::swap(_obj.m_counter, m_counter);
}

