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
#include <ememory/EnableSharedFromThis.h>

namespace ememory {
	template<typename> class WeakPtr;
	template<typename> class EnableSharedFromThis;
	// limited implementation of actual shared pointer (only 1 instance allowed, can be promoted though)
	template<typename EMEMORY_TYPE>
	class SharedPtr {
		friend class WeakPtr<EMEMORY_TYPE>;
		private:
			EMEMORY_TYPE* m_element;
			ememory::Counter* m_counter;
		public:
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
			                                  && std::is_base_of<ememory::EnableSharedFromThis<EMEMORY_TYPE2>, EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			SharedPtr(EMEMORY_TYPE2* _element);
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
			                                  && !std::is_base_of<ememory::EnableSharedFromThis<EMEMORY_TYPE2>, EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			SharedPtr(EMEMORY_TYPE2* _element);
		public:
			SharedPtr(std::nullptr_t);
			SharedPtr();
			~SharedPtr();
			SharedPtr(EMEMORY_TYPE* _obj, ememory::Counter* _counter);
			SharedPtr(const SharedPtr<EMEMORY_TYPE>& _obj);
			SharedPtr& operator= (const SharedPtr<EMEMORY_TYPE>& _obj);
			SharedPtr& operator= (std::nullptr_t);
			SharedPtr(SharedPtr<EMEMORY_TYPE>&& _obj);
		public:
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			SharedPtr(const SharedPtr<EMEMORY_TYPE2>& _obj);
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			SharedPtr& operator= (const SharedPtr<EMEMORY_TYPE2>& _obj);
		public:
			void reset();
			int64_t useCount() const;
			bool operator == (std::nullptr_t) const;
			bool operator==(const SharedPtr<EMEMORY_TYPE>& _obj) const;
			bool operator != (std::nullptr_t) const;
			bool operator!=(const SharedPtr<EMEMORY_TYPE>& _obj) const;
			const EMEMORY_TYPE* get() const;
			EMEMORY_TYPE* get();
			template<class EMEMORY_TYPE2 = EMEMORY_TYPE,
			         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
			                                  && !std::is_same<EMEMORY_TYPE2, void>::value
			                                 , int>::type = 0>
			const EMEMORY_TYPE* operator->() const;
			template<class EMEMORY_TYPE2 = EMEMORY_TYPE,
			         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
			                                  && !std::is_same<EMEMORY_TYPE2, void>::value
			                                 , int>::type = 0>
			EMEMORY_TYPE* operator->();
			template<class EMEMORY_TYPE2 = EMEMORY_TYPE,
			         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
			                                  && !std::is_same<EMEMORY_TYPE2, void>::value
			                                 , int>::type = 0>
			const EMEMORY_TYPE& operator*() const;
			template<class EMEMORY_TYPE2 = EMEMORY_TYPE,
			         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
			                                  && !std::is_same<EMEMORY_TYPE2, void>::value
			                                 , int>::type = 0>
			EMEMORY_TYPE& operator*();
			void swap(SharedPtr<EMEMORY_TYPE>& _obj);
			ememory::Counter* getCounter() const {
				return m_counter;
			}
	};
}

#include <ememory/details/SharedPtr.hxx>
