/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ememory/RefPtr.hpp>


namespace ememory {
	/**
	 * @brief Use the element as a refcounted element
	 */
	class RefCounter {
		#ifndef ETK_MEMORY_CHECKER
			protected:
		#else
			public:
		#endif
			RefCounter();
			// Virtualize destructor in private to prevent user ot remove it without permition
			virtual ~RefCounter();
		private:
			int32_t m_refCount = 1;
			#ifdef DEBUG
				int32_t m_uid;
			#endif
		public:
			/**
			 * @brief Keep a copy of this reference-counted element.
			 */
			void refKeep();
			/**
			 * @brief Relese a copy of this reference-counted element.
			 */
			void refRelease();
			/**
			 * @brief Get the number of time the object is required.
			 */
			int32_t getRefCount() const;
			/**
			 * @brief Get the currect class SharedPtr
			 * @return Request SharedPtr
			 */
			//ememory::RefPtr<EMEMORY_TYPE> refFromThis();
			/**
			 * @brief Get the currect class SharedPtr
			 * @return Request const SharedPtr
			 */
			//const ememory::RefPtr<EMEMORY_TYPE> refFromThis() const;
		protected:
			/**
			 * @brief get the RAW pointer value of this element (for debug only)
			 */
			uint64_t getRawPointer() const;
	};
	#ifdef DEBUG
		void resetDebugRefCounter();
	#endif
}

#include <ememory/details/EnableSharedFromThis.hxx>

