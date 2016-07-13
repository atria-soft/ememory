
#pragma once

#include <ememory/debug.h>

namespace ememoryTMP {
	template<typename> class WeakPtr;
	
	class Counter {
		public:
			enum class remove {
				none,
				data,
				counter,
				all
			}
		private:
			int64_t m_counterShared;
			int64_t m_counterWeak;
			mutable std::mutex m_mutex;
		public:
			Counter():
			  m_counterShared(0),
			  m_counterWeak(0),
			  m_element(nullptr) {
				
			}
			int64_t incrementShared() {
				int64_t out;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					if (m_counterShared != 0) {
						m_counterShared++;
					}
					out = m_counterShared;
				}
				return out;
			}
			ememoryTMP::Counter::remove decrementShared() {
				std::unique_lock<std::mutex> lock(m_mutex);
				if (m_counterShared != 0) {
					m_counterShared--;
				} else {
					EMEMORY_ERROR("try to decrement a SharedPtr with no more element in RefCount");
					return ememoryTMP::Counter::remove::none;
				}
				if (m_counterShared == 0) {
					if (m_counterWeak == 0) {
						return ememoryTMP::Counter::remove::all;
					} else {
						return ememoryTMP::Counter::remove::data;
					}
				}
				return ememoryTMP::Counter::remove::none;
			}
			int64_t incrementWeak() {
				int64_t out;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					if (m_counterWeak != 0) {
						m_counterWeak++;
					}
					out = m_counterWeak;
				}
				return out;
			}
			ememoryTMP::Counter::remove decrementWeak() {
				std::unique_lock<std::mutex> lock(m_mutex);
				if (m_counterWeak != 0) {
					m_counterWeak--;
				} else {
					EMEMORY_ERROR("try to decrement a WeakPtr with no more element in RefCount");
					return ememoryTMP::Counter::remove::none;
				}
				if (    m_counterShared == 0
				     && m_counterWeak == 0) {
					return ememoryTMP::Counter::remove::counter;
				}
				return ememoryTMP::Counter::remove::none;
			}
			/*
			void kill() {
				m_element = nullptr;
				if(m_counter == 0) {
					delete this;
				}
			}
			*/
			int64_t getCountWeak() const {
				int64_t out;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					out = m_counterWeak;
				}
				return out;
			}
			int64_t getCountShared() const {
				int64_t out;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					out = m_counterShared;
				}
				return out;
			}
			int64_t getCount() const {
				int64_t out;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					out = m_counterWeak + m_counterShared;
				}
				return out;
			}
	};
	
	
	// limited implementation of actual shared pointer (only 1 instance allowed, can be promoted though)
	template<typename EMEMORY_TYPE>
	class SharedPtr {
		friend class WeakPtr<EMEMORY_TYPE>;
		private:
			EMEMORY_TYPE* m_element;
			ememoryTMP::Counter* m_counter;
		private:
			SharedPtr(EMEMORY_TYPE* _element):
			  m_element(_element),
			  m_counter(nullptr) {
				if (m_element == nullptr) {
					return;
				}
				m_counter = new ememoryTMP::Counter();
				m_counter->incrementShared();
			}
		public:
			SharedPtr():
			  m_element(nullptr),
			  m_counter(nullptr) {
				
			}
			~SharedPtr() {
				reset();
			}
			SharedPtr(const SharedPtr& _obj):
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
			SharedPtr& operator= (const SharedPtr& _obj) {
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
			SharedPtr(SharedPtr&& _obj) {
				m_element = _obj.m_element;
				m_counter = _obj.m_counter;
				_obj.m_element = nullptr;
				_obj.m_counter = nullptr;
			}
			void reset() {
				if(m_counter == nullptr) {
					m_element = nullptr; // in case ...
					return;
				}
				ememoryTMP::Counter::remove rmData = m_counter->decrementShared();
				switch(rmData) {
					case ememoryTMP::Counter::remove::all:
						delete m_counter;
						delete m_element;
						break;
					case ememoryTMP::Counter::remove::data:
						delete m_element;
						break;
					case ememoryTMP::Counter::remove::counter:
						delete m_counter;
						break;
					case ememoryTMP::Counter::remove::none:
						break;
				}
				m_counter = nullptr;
				m_element = nullptr;
			}
			int useCount() const {
				if (m_counter == nullptr) {
					return 0;
				}
				return m_counter->getCountShared();
			}
			bool operator == (std::nullptr_t) {
				return m_counter == nullptr;
			}
			bool operator==(const SharedPtr& _obj) const {
				return m_counter == _obj.m_counter;
			}
			bool operator != (std::nullptr_t) {
				return m_counter != nullptr;
			}
			bool operator!=(const SharedPtr& _obj) const {
				return m_counter != _obj.m_counter;
			}
			const EMEMORY_TYPE* get() const {
				return m_element;
			}
			EMEMORY_TYPE* get() {
				return m_element;
			}
			const EMEMORY_TYPE& operator->() const {
				return *m_element;
			}
			EMEMORY_TYPE& operator->() {
				return *m_element;
			}
			const EMEMORY_TYPE& operator*() const {
				return *m_element;
			}
			EMEMORY_TYPE& operator*() {
				return *m_element;
			}
			void swap(WeakPtr& _obj) {
				EMEMORY_TYPE* tmpE = m_element;
				ememoryTMP::Counter* tmpC = m_counter;
				m_element = _obj.m_element;
				m_counter = _obj.m_counter;
				_obj.m_element = tmpE;
				_obj.m_counter = tmpC;
			}
	};
	
	template<class EMEMORY_TYPE, typename... EMEMORY_ARGS>
	static ememoryTMP::SharedPtr makeShared(EMEMORY_ARGS && ...args) {
		// TODO : Enable shared from this ...
		return ememoryTMP::SharedPtr(new EMEMORY_TYPE(std::forward<EMEMORY_ARGS>(args)...));
	}
	
	template<typename EMEMORY_TYPE>
	class WeakPtr {
		private:
			EMEMORY_TYPE* m_element;
			ememoryTMP::Counter* m_counter;
		public:
			WeakPtr():
			  m_element(nullptr),
			  m_counter(nullptr) {
				
			}
			~WeakPtr() {
				reset();
			}
			WeakPtr(const WeakPtr& _obj):
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
			WeakPtr& operator= (const WeakPtr& _obj) {
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
			WeakPtr(WeakPtr&& _obj) {
				m_element = _obj.m_element;
				m_counter = _obj.m_counter;
				_obj.m_element = nullptr;
				_obj.m_counter = nullptr;
			}
			WeakPtr(const SharedPtr& _obj):
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
			WeakPtr& operator= (const WeakPtr& _obj) {
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
			void reset() {
				if(m_counter == nullptr) {
					m_element = nullptr; // in case ...
					return;
				}
				ememoryTMP::Counter::remove rmData = m_counter->decrementWeak();
				switch(rmData) {
					case ememoryTMP::Counter::remove::all:
						EMEMORY_ERROR("in WeakPtr request remove all (impossible case ...)");
						break;
					case ememoryTMP::Counter::remove::data:
						EMEMORY_ERROR("in WeakPtr request remove all (impossible case ...)");
						break;
					case ememoryTMP::Counter::remove::counter:
						delete m_counter;
						break;
					case ememoryTMP::Counter::remove::none:
						break;
				}
				m_counter = nullptr;
				m_element = nullptr;
			}
			int useCount() const {
				if (m_counter == nullptr) {
					return 0;
				}
				return m_counter->getCountShared();
			}
			bool expired() const {
				if (m_counter == nullptr) {
					return true;
				}
				return m_counter.getCountShared() != 0;
			}
			elemoryTMP::SharedPtr lock() {
				elemoryTMP::SharedPtr out;
				if (m_counter == nullptr) {
					return out;
				}
				int64_t count = m_counter.incrementShared();
				if (count == 0) {
					return out;
				}
				out.m_counter = m_counter;
				out.m_element = m_element;
				return std::move(out);
			}
			bool operator == (const WeakPtr& _obj) {
				return m_counter == _obj.m_counter;
			}
			bool operator == (std::nullptr_t) {
				return m_counter == nullptr;
			}
			void swap(WeakPtr& _obj) {
				EMEMORY_TYPE* tmpE = m_element;
				ememoryTMP::Counter* tmpC = m_counter;
				m_element = _obj.m_element;
				m_counter = _obj.m_counter;
				_obj.m_element = tmpE;
				_obj.m_counter = tmpC;
			}
	};

}

