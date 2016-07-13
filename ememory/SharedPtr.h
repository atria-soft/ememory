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
	template<typename> class WeakPtr;
	// limited implementation of actual shared pointer (only 1 instance allowed, can be promoted though)
	template<typename EMEMORY_TYPE>
	class SharedPtr {
		friend class WeakPtr<EMEMORY_TYPE>;
		private:
			EMEMORY_TYPE* m_element;
			ememory::Counter* m_counter;
		public:
			SharedPtr(EMEMORY_TYPE* _element);
		public:
			SharedPtr();
			~SharedPtr();
			SharedPtr(const SharedPtr& _obj);
			SharedPtr& operator= (const SharedPtr& _obj);
			SharedPtr(SharedPtr&& _obj);
			void reset();
			int64_t useCount() const;
			bool operator == (std::nullptr_t);
			bool operator==(const SharedPtr& _obj) const;
			bool operator != (std::nullptr_t);
			bool operator!=(const SharedPtr& _obj) const;
			const EMEMORY_TYPE* get() const;
			EMEMORY_TYPE* get();
			const EMEMORY_TYPE* operator->() const;
			EMEMORY_TYPE* operator->();
			const EMEMORY_TYPE& operator*() const;
			EMEMORY_TYPE& operator*();
			void swap(SharedPtr& _obj);
	};
}

#include <ememory/details/SharedPtr.hxx>

