/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ememory/debug.hpp>
#include <ememory/WeakPtr.hpp>

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<    etk::IsSame<EMEMORY_TYPE2, EMEMORY_TYPE>::value
                                 && etk::IsBaseOf<ememory::EnableSharedFromThisBase, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(EMEMORY_TYPE2* _element, deleterCall&& _deleter):
  m_element(_element),
  m_counter(null),
  m_deleter(_deleter) {
	EMEMORY_DBG("Create shared");
	if (m_element == null) {
		return;
	}
	EMEMORY_DBG("Check if the user use the memory allocator or personal system...");
	ETK_MEM_CHECK_POINTER(_element);
	EMEMORY_DBG("    ==> get previous pointer");
	m_counter = m_element->weakFromThis().getCounter();
	if (m_counter != null) {
		m_counter->incrementShared();
	}
	return;
	EMEMORY_ERROR("No counter on a shared ptr class (EnableSharedFromThis ==> this is bad");
}


template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<    etk::IsSame<EMEMORY_TYPE2, EMEMORY_TYPE>::value
                                 && !etk::IsBaseOf<ememory::EnableSharedFromThisBase, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(EMEMORY_TYPE2* _element, deleterCall&& _deleter):
  m_element(_element),
  m_counter(null),
  m_deleter(_deleter) {
	EMEMORY_DBG("Create shared");
	if (m_element == null) {
		return;
	}
	EMEMORY_DBG("Check if the user use the memory allocator or personal system...");
	ETK_MEM_CHECK_POINTER(_element);
	m_counter = ETK_NEW(ememory::Counter, false);
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr():
  m_element(null),
  m_counter(null),
  m_deleter(createDeleter()) {
	EMEMORY_DBG("Create shared");
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(etk::NullPtr):
  m_element(null),
  m_counter(null),
  m_deleter(createDeleter()) {
	EMEMORY_DBG("Create shared");
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(EMEMORY_TYPE* _obj, ememory::Counter* _counter):
  m_element(_obj),
  m_counter(_counter),
  m_deleter(createDeleter()) {
	EMEMORY_DBG("Create shared (from a cast)");
	if (_obj == null) {
		m_counter = null;
		return;
	}
	m_counter->incrementShared();
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::~SharedPtr() {
	EMEMORY_DBG("delete shared");
	reset();
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(const ememory::SharedPtr<EMEMORY_TYPE>& _obj):
  m_element(_obj.m_element),
  m_counter(_obj.m_counter),
  m_deleter(_obj.m_deleter) {
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		m_deleter = null;
		return;
	}
	if (m_counter == null) {
		return;
	}
	m_counter->incrementShared();
};

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>& ememory::SharedPtr<EMEMORY_TYPE>::operator= (const ememory::SharedPtr<EMEMORY_TYPE>& _obj) {
	reset();
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	m_deleter = _obj.m_deleter;
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		m_deleter = null;
		return *this;
	}
	if (m_counter == null) {
		return *this;
	}
	m_counter->incrementShared();
	return *this;
}

template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>& ememory::SharedPtr<EMEMORY_TYPE>::operator= (etk::NullPtr) {
	reset();
	return *this;
}


template<typename EMEMORY_TYPE>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(ememory::SharedPtr<EMEMORY_TYPE>&& _obj) {
	EMEMORY_DBG("move operator ... " << int64_t(_obj.m_counter));
	m_element = _obj.m_element;
	m_counter = _obj.m_counter;
	m_deleter = _obj.m_deleter;
	_obj.m_element = null;
	_obj.m_counter = null;
	_obj.m_deleter = null;
}

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::SharedPtr<EMEMORY_TYPE>::SharedPtr(const ememory::SharedPtr<EMEMORY_TYPE2>& _obj):
  m_element(const_cast<EMEMORY_TYPE2*>(_obj.get())),
  m_counter(const_cast<ememory::Counter*>(_obj.getCounter())),
  m_deleter(createDeleter()) {
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return;
	}
	if (m_counter == null) {
		return;
	}
	m_counter->incrementShared();
}

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2,
         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
                                 , int>::type>
ememory::SharedPtr<EMEMORY_TYPE>& ememory::SharedPtr<EMEMORY_TYPE>::operator= (const SharedPtr<EMEMORY_TYPE2>& _obj) {
	reset();
	m_element = const_cast<EMEMORY_TYPE2*>(_obj.get());
	m_counter = const_cast<ememory::Counter*>(_obj.getCounter());
	m_deleter = createDeleter();
	if (    m_element == null
	     || m_counter == null) {
		m_element = null;
		m_counter = null;
		return *this;
	}
	if (m_counter == null) {
		return *this;
	}
	m_counter->incrementShared();
	return *this;
}

template<typename EMEMORY_TYPE>
void ememory::SharedPtr<EMEMORY_TYPE>::reset() {
	if(m_counter == null) {
		m_element = null; // in case ...
		m_deleter = null;
		return;
	}
	EMEMORY_DBG("reset sharedPtr (start)");
	ememory::Counter::remove rmData = m_counter->decrementShared();
	switch(rmData) {
		case ememory::Counter::remove::all:
			ETK_DELETE(ememory::Counter, m_counter);
			if (m_deleter != null) {
				if (m_element != null) {
					m_deleter((void*)m_element);
				}
			} else {
				EMEMORY_WARNING("Maybe a leak ==> no deleter of the SharedPtr");
			}
			break;
		case ememory::Counter::remove::data:
			if (m_deleter != null) {
				if (m_element != null) {
					m_deleter((void*)m_element);
				}
			} else {
				EMEMORY_WARNING("Maybe a leak ==> no deleter of the SharedPtr");
			}
			break;
		case ememory::Counter::remove::counter:
			ETK_DELETE(ememory::Counter, m_counter);
			break;
		case ememory::Counter::remove::none:
			break;
	}
	m_deleter = null;
	m_counter = null;
	m_element = null;
	EMEMORY_DBG("reset sharedPtr (stop)");
}

template<typename EMEMORY_TYPE>
int64_t ememory::SharedPtr<EMEMORY_TYPE>::useCount() const {
	if (m_counter == null) {
		return 0;
	}
	return m_counter->getCountShared();
}

template<typename EMEMORY_TYPE>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator==(etk::NullPtr) const {
	return m_counter == null;
}

template<typename EMEMORY_TYPE>
template<class EMEMORY_TYPE2>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator==(const SharedPtr<EMEMORY_TYPE2>& _obj) const {
	return m_counter == _obj.getCounter();
}

template<typename EMEMORY_TYPE>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator!=(etk::NullPtr) const {
	return m_counter != null;
}

template<typename EMEMORY_TYPE>
template<typename EMEMORY_TYPE2>
bool ememory::SharedPtr<EMEMORY_TYPE>::operator!=(const SharedPtr<EMEMORY_TYPE2>& _obj) const {
	return m_counter != _obj.getCounter();
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
	etk::swap(_obj.m_element, m_element);
	etk::swap(_obj.m_counter, m_counter);
	etk::swap(_obj.m_deleter, m_deleter);
}






















///////////////////////////////////////////////////////////////////
// void ...
///////////////////////////////////////////////////////////////////
namespace ememory {
	// Void template specification ...
	template<>
	class SharedPtr<void> {
		friend class WeakPtr<void>;
		public:
			using deleterCall = etk::Function<void(void* _data)>;
		private:
			void* m_element;
			ememory::Counter* m_counter;
		public:
			SharedPtr(void* _element);
		public:
			SharedPtr(etk::NullPtr):
			  m_element(null),
			  m_counter(null) {
				EMEMORY_DBG("Create shared<void>");
			}
			SharedPtr():
			  m_element(null),
			  m_counter(null) {
				EMEMORY_DBG("Create shared<void>");
			}
			~SharedPtr() {
				EMEMORY_DBG("delete shared");
				reset();
			}
			SharedPtr(void* _obj, ememory::Counter* _counter):
			  m_element(_obj),
			  m_counter(_counter) {
				EMEMORY_DBG("Create shared (from a cast)");
				if (_obj == null) {
					m_counter = null;
					return;
				}
				m_counter->incrementShared();
			}
			SharedPtr& operator= (etk::NullPtr) {
				reset();
				return *this;
			}
			SharedPtr(SharedPtr<void>&& _obj) {
				m_element = _obj.m_element;
				m_counter = _obj.m_counter;
				_obj.m_element = null;
				_obj.m_counter = null;
			}
			template<class EMEMORY_TYPE2>
			SharedPtr(const SharedPtr<EMEMORY_TYPE2>& _obj):
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
				m_counter->incrementShared();
			}
			template<class EMEMORY_TYPE2>
			SharedPtr& operator= (const SharedPtr<EMEMORY_TYPE2>& _obj) {
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
				m_counter->incrementShared();
				return *this;
			}
		public:
			void reset() {
				if(m_counter == null) {
					m_element = null; // in case ...
					return;
				}
				EMEMORY_DBG("reset sharedPtr (start)");
				ememory::Counter::remove rmData = m_counter->decrementShared();
				switch(rmData) {
					case ememory::Counter::remove::all:
						ETK_DELETE(ememory::Counter, m_counter);
						EMEMORY_WARNING("Maybe a leak ==> no deleter of the SharedPtr<void>");
						break;
					case ememory::Counter::remove::data:
						EMEMORY_WARNING("Maybe a leak ==> no deleter of the SharedPtr<void>");
						break;
					case ememory::Counter::remove::counter:
						ETK_DELETE(ememory::Counter, m_counter);
						break;
					case ememory::Counter::remove::none:
						break;
				}
				m_counter = null;
				m_element = null;
				EMEMORY_DBG("reset sharedPtr (stop)");
			}
			int64_t useCount() const {
				if (m_counter == null) {
					return 0;
				}
				return m_counter->getCountShared();
			}
			bool operator==(etk::NullPtr) const {
				return m_counter == null;
			}
			template<class EMEMORY_TYPE2>
			bool operator==(const SharedPtr<EMEMORY_TYPE2>& _obj) const {
				return m_counter == _obj.m_counter;
			}
			bool operator!=(etk::NullPtr) const {
				return m_counter != null;
			}
			template<class EMEMORY_TYPE2>
			bool operator!=(const SharedPtr<EMEMORY_TYPE2>& _obj) const {
				return m_counter != _obj.m_counter;
			}
			const void* get() const {
				return m_element;
			}
			void* get() {
				return m_element;
			}
			const void* operator->() const {
				return m_element;
			}
			void* operator->() {
				return m_element;
			}
			void swap(SharedPtr<void>& _obj) {
				void* tmpE = m_element;
				ememory::Counter* tmpC = m_counter;
				m_element = _obj.m_element;
				m_counter = _obj.m_counter;
				_obj.m_element = tmpE;
				_obj.m_counter = tmpC;
			}
			ememory::Counter* getCounter() const {
				return m_counter;
			}
			// TODO: unique
			// TODO: bool
	};
}


