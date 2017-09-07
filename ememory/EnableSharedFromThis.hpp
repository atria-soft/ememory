/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Vector.hpp>
#include <ethread/Mutex.hpp>
#include <ememory/SharedPtr.hpp>
#include <ememory/WeakPtr.hpp>


namespace ememory {
	/**
	 * @brief Basic handle to be simplify at the basic mode of the onject recognition for internal tamplate filter (empty implementation)
	 */
	class EnableSharedFromThisBase {};
	/**
	 * @brief Enable the acces of the self sharedPtr inside an object (note: not availlable in contructor and destructor)
	 */
	template<typename EMEMORY_TYPE>
	class EnableSharedFromThis : public ememory::EnableSharedFromThisBase {
		private:
			mutable ememory::WeakPtr<EMEMORY_TYPE> m_weakThis; //!< Weak pointer reference of itself.
		protected:
			/**
			 * @brief Contructor
			 */
			EnableSharedFromThis();
			/**
			 * @brief Virual destructor (simply virtualyse the destructor)
			 */
			virtual ~EnableSharedFromThis() = default;
		public:
			/**
			 * @brief Get the currect class SharedPtr
			 * @return Request SharedPtr
			 */
			ememory::SharedPtr<EMEMORY_TYPE> sharedFromThis();
			/**
			 * @brief Get the currect class SharedPtr
			 * @return Request const SharedPtr
			 */
			const ememory::SharedPtr<EMEMORY_TYPE> sharedFromThis() const;
			/**
			 * @brief Get the currect class WeakPtr
			 * @return Request WeakPtr
			 */
			ememory::WeakPtr<EMEMORY_TYPE> weakFromThis();
			/**
			 * @brief Get the currect class WeakPtr
			 * @return Request const WeakPtr
			 */
			const ememory::WeakPtr<EMEMORY_TYPE> weakFromThis() const;
	};
}

#include <ememory/details/EnableSharedFromThis.hxx>

