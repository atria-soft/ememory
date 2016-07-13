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

namespace ememory {
	template<typename> class SharedPtr;
	template<typename> class EnableSharedFromThis;
	template<typename EMEMORY_TYPE>
	class WeakPtr {
		friend class EnableSharedFromThis<EMEMORY_TYPE>;
		private:
			EMEMORY_TYPE* m_element;
			ememory::Counter* m_counter;
		public:
			WeakPtr();
		private:
			WeakPtr(EMEMORY_TYPE* _element); // this is only for enable shared from this ...
		public:
			~WeakPtr();
			WeakPtr(const WeakPtr<EMEMORY_TYPE>& _obj);
			WeakPtr<EMEMORY_TYPE>& operator= (const WeakPtr<EMEMORY_TYPE>& _obj);
			WeakPtr(WeakPtr&& _obj);
			WeakPtr(const SharedPtr<EMEMORY_TYPE>& _obj);
			WeakPtr<EMEMORY_TYPE>& operator= (const SharedPtr<EMEMORY_TYPE>& _obj);
			void reset();
			int useCount() const;
			bool expired() const;
			ememory::SharedPtr<EMEMORY_TYPE> lock();
			bool operator == (const WeakPtr& _obj);
			bool operator == (std::nullptr_t);
			void swap(WeakPtr& _obj);
	};
}

#include <ememory/details/WeakPtr.hxx>