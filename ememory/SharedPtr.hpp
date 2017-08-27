/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Vector.hpp>
#include <mutex>
#include <ememory/debug.hpp>
#include <ememory/Counter.hpp>
#include <ememory/EnableSharedFromThis.hpp>

namespace ememory {
	template<typename> class WeakPtr;
	template<typename> class EnableSharedFromThis;
	using deleterCall = std::function<void(void* _data)>;
	/**
	 * @brief ememory::SharedPtr is a smart pointer that retains shared ownership of an object through a pointer.
	 * Several SharedPtr objects may own the same object. The object is destroyed and its memory deallocated when
	 * either of the following happens:
	 *     - the last remaining SharedPtr owning the object is destroyed;
	 *     - the last remaining SharedPtr owning the object is assigned another pointer via operator= or reset().
	 * The object is destroyed using delete-expression or a custom deleter that is supplied to shared_ptr during construction.
	 * 
	 * A SharedPtr can share ownership of an object while storing a pointer to another object.
	 * This feature can be used to point to member objects while owning the object they belong to.
	 * The stored pointer is the one accessed by get(), the dereference and the comparison operators.
	 * The managed pointer is the one passed to the deleter when use count reaches zero.
	 *
	 * @note A big difference whith this wrapper is the constness sharing. When your sharedPtr is shared, the internal pointer is shared too.
	 */
	template<typename EMEMORY_TYPE>
	class SharedPtr {
		friend class WeakPtr<EMEMORY_TYPE>;
		private:
			EMEMORY_TYPE* m_element; //!< Pointer on the Data
			ememory::Counter* m_counter; //!< Pointer on the counter
			deleterCall m_deleter; //!< Function to call to delete the data pointer
			/**
			 * @brief Create the function to remove the pointer of the data
			 * @return deleter function (need access to a voind data access)
			 */
			deleterCall createDeleter() const {
				return [](void* _data) { delete((EMEMORY_TYPE*)_data);};
			}
		public:
			#ifndef PARSE_DOXYGEN
				template<class EMEMORY_TYPE2,
				         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
				                                  && std::is_base_of<ememory::EnableSharedFromThisBase, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				SharedPtr(EMEMORY_TYPE2* _element);
				template<class EMEMORY_TYPE2,
				         typename std::enable_if<    std::is_same<EMEMORY_TYPE2, EMEMORY_TYPE>::value
				                                  && !std::is_base_of<ememory::EnableSharedFromThisBase, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				SharedPtr(EMEMORY_TYPE2* _element);
			#else
				/**
				 * @brief Contructor whith the pointer of data
				 * @param[in] _element allocated data (SharedPtr will remove it)
				 */
				SharedPtr(EMEMORY_TYPE2* _element);
			#endif
		public:
			/**
			 * @brief Contructor on nullptr
			 */
			SharedPtr(std::nullptr_t);
			/**
			 * @brief Contructor empty
			 */
			SharedPtr();
			/**
			 * @brief Contructor (API for casting)
			 * @param[in] _obj Pointer on the Data
			 * @param[in] _counter Pointer on the counter
			 */
			SharedPtr(EMEMORY_TYPE* _obj, ememory::Counter* _counter);
			/**
			 * @brief copy Contructor
			 * @param[in] _obj SharedPtr to copy
			 */
			SharedPtr(const SharedPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief copy Contructor
			 * @param[in] _obj SharedPtr to copy
			 */
			SharedPtr(SharedPtr<EMEMORY_TYPE>&& _obj);
			/**
			 * @brief Destructor
			 */
			~SharedPtr();
			/**
			 * @brief Asignement operator
			 * @param[in] _obj SharedPtr to copy
			 * @return Reference on this
			 */
			SharedPtr& operator= (const SharedPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief Asignement operator (asign nullptr)
			 * @return Reference on this
			 */
			SharedPtr& operator= (std::nullptr_t);
		public:
			#ifndef PARSE_DOXYGEN
				template<class EMEMORY_TYPE2,
				         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				SharedPtr(const SharedPtr<EMEMORY_TYPE2>& _obj);
				template<class EMEMORY_TYPE2,
				         typename std::enable_if<  std::is_base_of<EMEMORY_TYPE, EMEMORY_TYPE2>::value
				                                 , int>::type = 0>
				SharedPtr& operator= (const SharedPtr<EMEMORY_TYPE2>& _obj);
			#endif
		public:
			/**
			 * @brief Reset the SharedPtr ==> Remove data if needed
			 */
			void reset();
			/**
			 * @brief Get the number of conencted SharedPtr
			 * @return Number of SharedPtr on this data
			 */
			int64_t useCount() const;
			/**
			 * @brief Check if the SharedPtr have an internal data (not nullptr)
			 * @return true The pointer is not asigned, false otherwise
			 */
			bool operator==(std::nullptr_t) const;
			/**
			 * @brief Check if two SharedPtr are the same data (maybe not the same cast)
			 * @param[in] _obj Object to compare
			 * @return true The Object have the same pointer reference, false otherwise
			 */
			template<class EMEMORY_TYPE2>
			bool operator==(const SharedPtr<EMEMORY_TYPE2>& _obj) const;
			/**
			 * @brief Check if the SharedPtr have NOT an internal data (nullptr)
			 * @return true The pointer is asigned, false otherwise
			 */
			bool operator!=(std::nullptr_t) const;
			/**
			 * @brief Check if two SharedPtr are NOT the same data (maybe not the same cast)
			 * @param[in] _obj Object to compare
			 * @return true The Object have NOT the same pointer reference, false otherwise
			 */
			template<class EMEMORY_TYPE2>
			bool operator!=(const SharedPtr<EMEMORY_TYPE2>& _obj) const;
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
			 * @brief Swap 2 Object inside the SharedPtr
			 * @param[in] _obj Object to swap with
			 */
			void swap(SharedPtr<EMEMORY_TYPE>& _obj);
			/**
			 * @brief Get Counter pointer
			 * @return Pointer on the counter
			 */
			ememory::Counter* getCounter() const {
				return m_counter;
			}
			/**
			 * @brief Get deleter function of the data pointer
			 * @return deleter function
			 */
			deleterCall getDeleter() const {
				return m_deleter;
			}
			// TODO: unique
			// TODO: bool
	};
}

#include <ememory/details/SharedPtr.hxx>

