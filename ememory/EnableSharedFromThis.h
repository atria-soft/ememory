/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <mutex>
#include <ememory/SharedPtr.h>
#include <ememory/WeakPtr.h>


namespace ememory {
	template<typename EMEMORY_TYPE>
	class EnableSharedFromThis {
		friend class ememory::SharedPtr<EMEMORY_TYPE>;
		private:
			ememory::WeakPtr<EMEMORY_TYPE> m_weakThis;
		protected:
			EnableSharedFromThis();
			virtual ~EnableSharedFromThis() = default;
		public:
			ememory::SharedPtr<EMEMORY_TYPE> sharedFromThis();
			ememory::WeakPtr<EMEMORY_TYPE> weakFromThis();
	};
}

#include <ememory/details/EnableSharedFromThis.hxx>

