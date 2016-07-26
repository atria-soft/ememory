/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <mutex>
#include <ememory/debug.h>
#include <ememory/Counter.h>

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr():
  m_element(nullptr),
  m_counter(nullptr) {
	
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(std::nullptr_t):
  m_element(nullptr),
  m_counter(nullptr) {
	
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(EMEMORY_TYPE* _element):
  m_element(_element),
  m_counter(nullptr) {
	EMEMORY_DBG("new weak");
	if (m_element == nullptr) {
		return;
	}
	m_counter = new ememory::Counter(true);
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::~WeakPtr() {
	reset();
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(const ememory::WeakPtr<EMEMORY_TYPE>& _obj):
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
	m_counter->incrementWeak();
};

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (const ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	reset();
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
	m_counter->incrementWeak();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (std::nullptr_t) {
	reset();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(ememory::WeakPtr<EMEMORY_TYPE>&& _obj) {
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	_obj.m_element = nullptr;
	_obj.m_counter = nullptr;
}

template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(const ememory::SharedPtr<EMEMORY_TYPE>& _obj):
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
	m_counter->incrementWeak();
}


template<typename EMEMORY_TYPE>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
	reset();
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
	m_counter->incrementWeak();
	return *this;
}


template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename std::enable_if<    std::is_void<EMEMORY_TYPE>::value
                                  && !std::is_void<EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::WeakPtr<EMEMORY_TYPE>::WeakPtr(const ememory::SharedPtr<EMEMORY_TYPE2>& _obj):
  m_element((void*)_obj.get()),
  m_counter(_obj.getCounter()) {
	if (    m_element == nullptr
	     || m_counter == nullptr) {
		m_element = nullptr;
		m_counter = nullptr;
		return;
	}
	if (m_counter == nullptr) {
		return;
	}
	m_counter->incrementWeak();
}


template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename std::enable_if<    std::is_void<EMEMORY_TYPE>::value
                                  && !std::is_void<EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::WeakPtr<EMEMORY_TYPE>& ememory::WeakPtr<EMEMORY_TYPE>::operator= (const ememory::SharedPtr<EMEMORY_TYPE2>& _obj) {
	reset();
	m_element = (void*)_obj.get();
	m_counter = _obj.getCounter();
	if (    m_element == nullptr
	     || m_counter == nullptr) {
		m_element = nullptr;
		m_counter = nullptr;
		return *this;
	}
	if (m_counter == nullptr) {
		return *this;
	}
	m_counter->incrementWeak();
	return *this;
}


/*
template<class EMEMORY_TYPE2,
         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
WeakPtr(const WeakPtr<EMEMORY_TYPE2>& _obj):
  m_element(_obj.m_element),
  m_counter(_obj.getCounter()) {
	if (    m_element == nullptr
	     || m_counter == nullptr) {
		m_element = nullptr;
		m_counter = nullptr;
		return;
	}
	if (m_counter == nullptr) {
		return;
	}
	m_counter->incrementWeak();
	
}
template<class EMEMORY_TYPE2,
         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
WeakPtr& operator= (const WeakPtr<EMEMORY_TYPE2>& _obj) {
	reset();
	m_element = _obj.m_element;
	m_counter = _obj.getCounter();
	if (    m_element == nullptr
	     || m_counter == nullptr) {
		m_element = nullptr;
		m_counter = nullptr;
		return *this;
	}
	if (m_counter == nullptr) {
		return *this;
	}
	m_counter->incrementWeak();
	return *this;
}
*/
template<typename EMEMORY_TYPE>
void ememory::WeakPtr<EMEMORY_TYPE>::reset() {
	if(m_counter == nullptr) {
		m_element = nullptr; // in case ...
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
			delete m_counter;
			break;
		case ememory::Counter::remove::none:
			break;
	}
	m_counter = nullptr;
	m_element = nullptr;
}

template<typename EMEMORY_TYPE>
int ememory::WeakPtr<EMEMORY_TYPE>::useCount() const {
	if (m_counter == nullptr) {
		return 0;
	}
	return m_counter->getCountShared();
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::expired() const {
	if (m_counter == nullptr) {
		return true;
	}
	return m_counter->getCountShared() == 0;
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE> ememory::WeakPtr<EMEMORY_TYPE>::lock() {
	ememory::SharedPtr<EMEMORY_TYPE> out;
	if (m_counter == nullptr) {
		return out;
	}
	int64_t count = m_counter->incrementShared(true);
	if (count == 0) {
		return out;
	}
	out.m_counter = m_counter;
	out.m_element = m_element;
	return std::move(out);
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator==(const ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	return m_counter == _obj.m_counter;
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator==(std::nullptr_t) const {
	return m_counter == nullptr;
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator!=(const ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	return m_counter != _obj.m_counter;
}

template<typename EMEMORY_TYPE>
bool ememory::WeakPtr<EMEMORY_TYPE>::operator!=(std::nullptr_t) const {
	return m_counter != nullptr;
}

template<typename EMEMORY_TYPE>
void ememory::WeakPtr<EMEMORY_TYPE>::swap(ememory::WeakPtr<EMEMORY_TYPE>& _obj) {
	EMEMORY_TYPE* tmpE = m_element;
	ememory::Counter* tmpC = m_counter;
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	_obj.m_element = tmpE;
	_obj.m_counter = tmpC;
}

