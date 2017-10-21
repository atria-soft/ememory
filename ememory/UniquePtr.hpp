
#pragma once

#include <etk/types.hpp>
namespace ememory {
	template <class EMEM_UPTR_TYPE>
	class UniquePtr {
		public:
			using deleterCall = etk::Function<void(void* _data)>;
			//private: Do it better ...
		public:
			EMEM_UPTR_TYPE* m_pointer;
			deleterCall m_deleter; //!< Function to call to delete the data pointer
		private:
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr(UniquePtr<EMEM_UPTR_TYPE_2> &) = delete;
			
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr &operator=(UniquePtr<EMEM_UPTR_TYPE_2> &) = delete;
		public:
			UniquePtr() :
			  m_pointer(nullptr),
			  m_deleter(nullptr) {
				
			}
			UniquePtr(etk::NullPtr) :
			  m_pointer(nullptr),
			  m_deleter(nullptr) {
				
			}
			explicit UniquePtr(EMEM_UPTR_TYPE* _obj, deleterCall&& _deleter = [](void* _data) { ETK_DELETE(EMEM_UPTR_TYPE, _data);}) :
			  m_pointer(_obj),
			  m_deleter(_deleter) {
				ETK_MEM_CHECK_POINTER(_obj);
			}
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr(UniquePtr<EMEM_UPTR_TYPE_2>&& _obj) :
			  m_pointer(nullptr),
			  m_deleter(nullptr) {
				etk::swap(_obj.m_pointer, m_pointer);
				etk::swap(_obj.m_deleter, m_deleter);
			}
			~UniquePtr() {
				reset();
			}
			UniquePtr& operator=(etk::NullPtr) {
				reset();
				return *this;
			}
			UniquePtr& operator=(UniquePtr&& _obj) {
				reset();
				etk::swap(_obj.m_pointer, m_pointer);
				etk::swap(_obj.m_deleter, m_deleter);
				return *this;
			}
			/*
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr& operator=(UniquePtr<EMEM_UPTR_TYPE_2>& _obj){
				reset();
				m_pointer = _obj.m_pointer;
				_obj.m_pointer = nullptr;
				return *this;
			}*/
			const EMEM_UPTR_TYPE& operator*() const {
				return *m_pointer;
			}
			EMEM_UPTR_TYPE& operator*() {
				return *m_pointer;
			}
			const EMEM_UPTR_TYPE* operator->() const {
				return m_pointer;
			}
			EMEM_UPTR_TYPE* operator->() {
				return m_pointer;
			}
			EMEM_UPTR_TYPE* get() const{
				return m_pointer;
			}
			EMEM_UPTR_TYPE *release(){
				EMEM_UPTR_TYPE *tmp = m_pointer;
				m_pointer = nullptr;
				return tmp;
			}
			void reset(){
				if (m_deleter != nullptr) {
					m_deleter(m_pointer);
				}
				m_pointer = nullptr;
			}
			void swap(UniquePtr &_obj){
				etk::swap(_obj.m_pointer, m_pointer);
				etk::swap(_obj.m_deleter, m_deleter);
			}
			/**
			 * @brief Check if the UniquePtr have an internal data (not nullptr)
			 * @return true The pointer is not asigned, false otherwise
			 */
			bool operator==(etk::NullPtr) const {
				return m_pointer == nullptr;
			}
			/**
			 * @brief Check if the UniquePtr have not an internal data (equal nullptr)
			 * @return true The pointer is asigned, false otherwise
			 */
			bool operator!=(etk::NullPtr) const {
				return m_pointer != nullptr;
			}
			/*
			template <class EMEM_UPTR_TYPE_2>
			void swap(UniquePtr<EMEM_UPTR_TYPE_2>& _obj) {
				etk::swap(m_pointer, _obj.m_pointer);
			}
			*/
	};
	
	template<class EMEM_UPTR_TYPE, class... EMEM_UPTR_ARG>
	UniquePtr<EMEM_UPTR_TYPE> makeUniquePtr(EMEM_UPTR_ARG ... _obj) {
		return ememory::UniquePtr<EMEM_UPTR_TYPE>(ETK_NEW(EMEM_UPTR_TYPE, _obj...));
	}
	
	template <class EMEM_UPTR_TYPE>
	inline void swap(UniquePtr<EMEM_UPTR_TYPE> &_obj1, UniquePtr<EMEM_UPTR_TYPE> &_obj2) {
		_obj1.swap(_obj2);
	}
	/*
	template <class EMEM_UPTR_TYPE_1, class EMEM_UPTR_DELETER_1, class EMEM_UPTR_TYPE_2, class EMEM_UPTR_DELETER_2>
	bool operator==(const UniquePtr<EMEM_UPTR_TYPE_1, EMEM_UPTR_DELETER_1> &x, const UniquePtr<EMEM_UPTR_TYPE_2, EMEM_UPTR_DELETER_2> &y) {
		return x.get() == y.get();
	}
	template <class EMEM_UPTR_TYPE_1, class EMEM_UPTR_DELETER_1, class EMEM_UPTR_TYPE_2, class EMEM_UPTR_DELETER_2>
	bool operator!=(const UniquePtr<EMEM_UPTR_TYPE_1, EMEM_UPTR_DELETER_1> &x, const UniquePtr<EMEM_UPTR_TYPE_2, EMEM_UPTR_DELETER_2> &y) {
		return x.get() != y.get();
	}
	template <class EMEM_UPTR_TYPE_1, class EMEM_UPTR_DELETER_1, class EMEM_UPTR_TYPE_2, class EMEM_UPTR_DELETER_2>
	bool operator<(const UniquePtr<EMEM_UPTR_TYPE_1, EMEM_UPTR_DELETER_1> &x, const UniquePtr<EMEM_UPTR_TYPE_2, EMEM_UPTR_DELETER_2> &y) {
		return x.get() < y.get();
	}
	template <class EMEM_UPTR_TYPE_1, class EMEM_UPTR_DELETER_1, class EMEM_UPTR_TYPE_2, class EMEM_UPTR_DELETER_2>
	bool operator<=(const UniquePtr<EMEM_UPTR_TYPE_1, EMEM_UPTR_DELETER_1> &x, const UniquePtr<EMEM_UPTR_TYPE_2, EMEM_UPTR_DELETER_2> &y) {
		return x.get() <= y.get();
	}
	template <class EMEM_UPTR_TYPE_1, class EMEM_UPTR_DELETER_1, class EMEM_UPTR_TYPE_2, class EMEM_UPTR_DELETER_2>
	bool operator>(const UniquePtr<EMEM_UPTR_TYPE_1, EMEM_UPTR_DELETER_1> &x, const UniquePtr<EMEM_UPTR_TYPE_2, EMEM_UPTR_DELETER_2> &y) {
		return x.get() > y.get();
	}
	template <class EMEM_UPTR_TYPE_1, class EMEM_UPTR_DELETER_1, class EMEM_UPTR_TYPE_2, class EMEM_UPTR_DELETER_2>
	bool operator>=(const UniquePtr<EMEM_UPTR_TYPE_1, EMEM_UPTR_DELETER_1> &x, const UniquePtr<EMEM_UPTR_TYPE_2, EMEM_UPTR_DELETER_2> &y) {
		return x.get() >= y.get();
	}
	*/
}

