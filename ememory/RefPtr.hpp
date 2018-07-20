/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ememory/debug.hpp>
#include <ememory/RefCounter.hpp>

namespace ememory {
	class RefCounter;
	/**
	 * @brief ememory::Ref is a smart pointer on RefCounted element.
	 * this is the good way to use a RefCounter object to not missed to remove one of them.
	 *
	 * @note this is smaller than SharedPtr in memory impact and code done, then it is faster.
	 */
	template<typename EMEMORY_TYPE>
	class RefPtr {
		private:
			EMEMORY_TYPE* m_element = null; //!< Pointer on the Data
		public:
			#ifndef PARSE_DOXYGEN
				template<class EMEMORY_TYPE2,
				         typename etk::EnableIf<    etk::IsSame<EMEMORY_TYPE2, EMEMORY_TYPE>::value
				                                  && etk::IsBaseOf<ememory::RefCounter, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				RefPtr(EMEMORY_TYPE2* _element);
			#else
				/**
				 * @brief Contructor whith the pointer of data
				 * @param[in] _element allocated data (RefPtr will remove it)
				 */
				RefPtr(EMEMORY_TYPE2* _element);
			#endif
		public:
			/**
			 * @brief Contructor on null
			 */
			RefPtr(etk::NullPtr);
			/**
			 * @brief Contructor empty
			 */
			RefPtr();
			/**
			 * @brief creator (to use wher create a new object (using nuw or ...)
			 * @param[in] _obj Pointer on the Data
			 */
			static inline RefPtr create(EMEMORY_TYPE* _obj);
			/**
			 * @brief Contructor (API for casting)
			 * @param[in] _obj Pointer on the Data
			 */
			RefPtr(EMEMORY_TYPE* _obj);
			/**
			 * @brief copy Contructor
			 * @param[in] _obj RefPtr to copy
			 */
			RefPtr(const RefPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief copy Contructor
			 * @param[in] _obj RefPtr to copy
			 */
			RefPtr(RefPtr<EMEMORY_TYPE>&& _obj);
			/**
			 * @brief Destructor
			 */
			~RefPtr();
			/**
			 * @brief Asignement operator
			 * @param[in] _obj RefPtr to copy
			 * @return Reference on this
			 */
			RefPtr& operator= (const RefPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief Asignement operator (asign null)
			 * @return Reference on this
			 */
			RefPtr& operator= (etk::NullPtr);
		public:
			#ifndef PARSE_DOXYGEN
				template<class EMEMORY_TYPE2,
				         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				RefPtr(const RefPtr<EMEMORY_TYPE2>& _obj);
				template<class EMEMORY_TYPE2,
				         typename etk::EnableIf<  etk::IsBaseOf<EMEMORY_TYPE, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				RefPtr& operator= (const RefPtr<EMEMORY_TYPE2>& _obj);
			#endif
		public:
			/**
			 * @brief Reset the RefPtr ==> Remove data if needed
			 */
			void reset();
			/**
			 * @brief Get the number of conencted RefPtr
			 * @return Number of RefPtr on this data
			 */
			int useCount() const;
			/**
			 * @brief Check if the RefPtr have an internal data (not null)
			 * @return true The pointer is not asigned, false otherwise
			 */
			bool operator==(etk::NullPtr) const;
			/**
			 * @brief Check if two RefPtr are the same data (maybe not the same cast)
			 * @param[in] _obj Object to compare
			 * @return true The Object have the same pointer reference, false otherwise
			 */
			template<class EMEMORY_TYPE2>
			bool operator==(const RefPtr<EMEMORY_TYPE2>& _obj) const;
			/**
			 * @brief Check if the RefPtr have NOT an internal data (null)
			 * @return true The pointer is asigned, false otherwise
			 */
			bool operator!=(etk::NullPtr) const;
			/**
			 * @brief Check if two RefPtr are NOT the same data (maybe not the same cast)
			 * @param[in] _obj Object to compare
			 * @return true The Object have NOT the same pointer reference, false otherwise
			 */
			template<class EMEMORY_TYPE2>
			bool operator!=(const RefPtr<EMEMORY_TYPE2>& _obj) const;
			/**
			 * @brief Get a const pointer on the data
			 * @return Data const pointer
			 */
			const EMEMORY_TYPE* get() const;
			/**
			 * @brief Get a pointer on the data
			 * @return Data pointer
			 */
			EMEMORY_TYPE* get();
			/**
			 * @brief Const dereferences the stored pointer.
			 * @return Const pointer on the Data
			 */
			const EMEMORY_TYPE* operator->() const;
			/**
			 * @brief Dereferences the stored pointer.
			 * @return Pointer on the Data
			 */
			EMEMORY_TYPE* operator->();
			/**
			 * @brief Get a const reference on the data
			 * @return Data const reference
			 */
			const EMEMORY_TYPE& operator*() const;
			/**
			 * @brief Get a reference on the data
			 * @return Data reference
			 */
			EMEMORY_TYPE& operator*();
			/**
			 * @brief Swap 2 Object inside the RefPtr
			 * @param[in] _obj Object to swap with
			 */
			void swap(RefPtr<EMEMORY_TYPE>& _obj);
			// TODO: unique
			// TODO: bool
	};
}

#include <ememory/details/RefPtr.hxx>

