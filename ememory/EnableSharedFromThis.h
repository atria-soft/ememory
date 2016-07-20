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
	class EnableSharedFromThisBase {};
	template<typename EMEMORY_TYPE>
	class EnableSharedFromThis : public ememory::EnableSharedFromThisBase {
		private:
			mutable ememory::WeakPtr<EMEMORY_TYPE> m_weakThis;
		protected:
			EnableSharedFromThis();
			virtual ~EnableSharedFromThis() = default;
		public:
			ememory::SharedPtr<EMEMORY_TYPE> sharedFromThis();
			const ememory::SharedPtr<EMEMORY_TYPE> sharedFromThis() const;
			ememory::WeakPtr<EMEMORY_TYPE> weakFromThis();
			const ememory::WeakPtr<EMEMORY_TYPE> weakFromThis() const;
	};
}

#include <ememory/details/EnableSharedFromThis.hxx>

