
#pragma once

#include <etk/types.hpp>
namespace ememory {
	template <class EMEM_UPTR_TYPE>
	class UniquePtr {
		private:
			EMEM_UPTR_TYPE* m_pointer;
			
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr(UniquePtr<EMEM_UPTR_TYPE_2> &) = delete;
			
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr &operator=(UniquePtr<EMEM_UPTR_TYPE_2> &) = delete;
		public:
			UniquePtr() :
			  m_pointer(nullptr) {
				
			}
			UniquePtr(etk::NullPtr) :
			  m_pointer(nullptr) {
				
			}
			explicit UniquePtr(EMEM_UPTR_TYPE* _obj) :
			  m_pointer(_obj)
			{
				
			}
			~UniquePtr() {
				reset();
			}
			UniquePtr& operator=(etk::NullPtr) {
				reset();
				return *this;
			}
			UniquePtr& operator=(UniquePtr _obj) {
				reset();
				m_pointer = etk::move(_obj.release());
				return *this;
			}
			template <class EMEM_UPTR_TYPE_2>
			UniquePtr& operator=(UniquePtr<EMEM_UPTR_TYPE_2> _obj){
				reset();
				m_pointer = etk::move(_obj.m_pointer);
				return *this;
			}
			EMEM_UPTR_TYPE operator*() const{
				return *m_pointer;
			}
			EMEM_UPTR_TYPE *operator->() const{
				return m_pointer;
			}
			EMEM_UPTR_TYPE *get() const{
				return m_pointer;
			}
			EMEM_UPTR_TYPE *release(){
				EMEM_UPTR_TYPE *tmp = m_pointer;
				m_pointer = nullptr;
				return tmp;
			}
			void reset(){
				delete m_pointer;
				m_pointer = nullptr;
			}
			void swap(UniquePtr &_obj){
				etk::swap(m_pointer, _obj.m_pointer);
			}
	};
	
	template<class EMEM_UPTR_TYPE, class... EMEM_UPTR_ARG>
	UniquePtr<EMEM_UPTR_TYPE> makeUniquePtr(EMEM_UPTR_ARG ... _obj) {
		return ememory::UniquePtr<EMEM_UPTR_TYPE>(new EMEM_UPTR_TYPE(_obj...));
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

