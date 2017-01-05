/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <mutex>
#include <ememory/debug.hpp>
#include <ememory/Counter.hpp>

namespace ememory {
	template<typename> class SharedPtr;
	template<typename> class EnableSharedFromThis;
	template<typename EMEMORY_TYPE>
	/**
	 * @brief WeakPtr is an interface that lose the data pointer when all SharedPtr as been released
	 */
	class WeakPtr {
		friend class EnableSharedFromThis<EMEMORY_TYPE>;
		friend class SharedPtr<EMEMORY_TYPE>;
		private:
			EMEMORY_TYPE* m_element; //!< Pointer on the Data
			ememory::Counter* m_counter; //!< Pointer on the counter
		public:
			/**
			 * @brief Empty Contructor
			 */
			WeakPtr();
			/**
			 * @brief nullptr contructor
			 */
			WeakPtr(std::nullptr_t);
		private:
			/**
			 * @brief Contructor of the Weak Ptr (specific for EnableSharedFromThis)
			 * @param[in] _element Pointer on data
			 */
			WeakPtr(EMEMORY_TYPE* _element); // this is only for enable shared from this ...
		public:
			~WeakPtr();
			/**
			 * @brief Copy contuctor
			 * @param[in] _obj Object to copy
			 */
			WeakPtr(const WeakPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief Const copy asignement
			 * @param[in] _obj Object to copy
			 * @return Reference on this
			 */
			WeakPtr<EMEMORY_TYPE>& operator= (const WeakPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief Copy contructor
			 * @param[in] _obj Object to copy
			 */
			WeakPtr(WeakPtr&& _obj);
			/**
			 * @brief Const copy asignement
			 * @param[in] _obj Object to copy
			 */
			WeakPtr(const SharedPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief Const copy asignement
			 * @param[in] _obj Object to copy
			 * @return Reference on this
			 */
			WeakPtr<EMEMORY_TYPE>& operator= (const SharedPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief nullptr asignement
			 * @return Reference on this
			 */
			WeakPtr<EMEMORY_TYPE>& operator= (std::nullptr_t);
			/**
			 * @brief Copy contuctor of herited WeakPtr
			 * @param[in] _obj Object to copy
			 */
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<    std::is_void<EMEMORY_TYPE>::value
			                                  && !std::is_void<EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			WeakPtr(const SharedPtr<EMEMORY_TYPE2>& _obj);
			/**
			 * @brief Const copy asignement of herited WeakPtr
			 * @param[in] _obj Object to copy
			 * @return Reference on this
			 */
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<    std::is_void<EMEMORY_TYPE>::value
			                                  && !std::is_void<EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			WeakPtr<EMEMORY_TYPE>& operator= (const SharedPtr<EMEMORY_TYPE2>& _obj);
		public:
			/*
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			WeakPtr(const WeakPtr<EMEMORY_TYPE2>& _obj);
			template<class EMEMORY_TYPE2,
			         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
			                                 , int>::type = 0>
			WeakPtr& operator= (const WeakPtr<EMEMORY_TYPE2>& _obj);
			*/
		public:
			/**
			 * @brief Reset the WeakPtr ==> Remove data if needed
			 */
			void reset();
			/**
			 * @brief Get the number of conencted SharedPtr
			 * @return Number of SharedPtr on this data
			 */
			int useCount() const;
			/**
			 * @brief Get the status of the pointer
			 * @return true if the Pointer is lockable
			 * @note The status chan change in an other thread release the SharedPtr
			 */
			bool expired() const;
			/**
			 * @brief Lock in SharedPtr the current Weak pointer
			 * @return created SharedPtr
			 */
			ememory::SharedPtr<EMEMORY_TYPE> lock();
			/**
			 * @brief Check if two WeakPtr are the same data
			 * @param[in] _obj Object to compare
			 * @return true The Object have the same pointer reference, false otherwise
			 */
			bool operator==(const WeakPtr& _obj);
			/**
			 * @brief Check if the WeakPtr have an internal data (not nullptr)
			 * @return true The pointer is not asigned, false otherwise
			 */
			bool operator==(std::nullptr_t) const;
			/**
			 * @brief Check if two WeakPtr are different data
			 * @param[in] _obj Object to compare
			 * @return true The Object have NOT the same pointer reference, false otherwise
			 */
			bool operator!=(const WeakPtr& _obj);
			/**
			 * @brief Check if the SharedPtr have NOT an internal data (nullptr)
			 * @return true The pointer is asigned, false otherwise
			 */
			bool operator!=(std::nullptr_t) const;
			/**
			 * @brief Swap 2 Object inside the WeakPtr
			 * @param[in] _obj Object to swap with
			 */
			void swap(WeakPtr& _obj);
			/**
			 * @brief Get Counter pointer
			 * @return Pointer on the counter
			 */
			ememory::Counter* getCounter() const {
				return m_counter;
			}
	};
}

#include <ememory/details/WeakPtr.hxx>