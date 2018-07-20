/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etest/etest.hpp>
#include <ememory/memory.hpp>
#include <test-debug/debug.hpp>

#include "main.hpp"

namespace {
	class MyTestString:
	  public ememory::RefCounter,
	  public etk::String {
		public:
			MyTestString() = default;
			MyTestString(const etk::String& _value):
			  etk::String(_value) {
				
			}
			MyTestString& operator = (const etk::String& _value) {
				etk::String::operator=(_value);
				return *this;
			}
	};
}

TEST(TestRef, createAndDestroy) {
	ememory::RefPtr<MyTestString> data = ememory::makeRef<MyTestString>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	EXPECT_EQ(data == null, false);
	EXPECT_EQ(data != null, true);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(data == null, true);
	EXPECT_EQ(data != null, false);
}

TEST(TestRef, createAndDestroy_2) {
	ememory::RefPtr<MyTestString> data = ememory::RefPtr<MyTestString>::create(ETK_NEW(MyTestString, "coucou"));
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	EXPECT_EQ(data == null, false);
	EXPECT_EQ(data != null, true);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(data == null, true);
	EXPECT_EQ(data != null, false);
}

TEST(TestRef, createAndDestroy_3) {
	ememory::RefPtr<MyTestString> data = ememory::RefPtr<MyTestString>::create(new MyTestString("coucou"));
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	EXPECT_EQ(data == null, false);
	EXPECT_EQ(data != null, true);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(data == null, true);
	EXPECT_EQ(data != null, false);
}

TEST(TestRef, copy) {
	ememory::RefPtr<MyTestString> data = ememory::makeRef<MyTestString>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	ememory::RefPtr<MyTestString> dataCopy = data;
	EXPECT_EQ(data.useCount(), 2);
	EXPECT_EQ(data == dataCopy, true);
	EXPECT_EQ(data != dataCopy, false);
	data.reset();
	EXPECT_EQ(data == dataCopy, false);
	EXPECT_EQ(data != dataCopy, true);
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataCopy.useCount(), 1);
	EXPECT_EQ(*dataCopy, "coucou");
	dataCopy.reset();
	EXPECT_EQ(dataCopy.useCount(), 0);
	dataCopy.reset();
	EXPECT_EQ(dataCopy.useCount(), 0);
}

TEST(TestRef, swap) {
	ememory::RefPtr<MyTestString> data = ememory::makeRef<MyTestString>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	ememory::RefPtr<MyTestString> dataCopy;
	EXPECT_EQ(dataCopy.useCount(), 0);
	dataCopy.swap(data);
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataCopy.useCount(), 1);
	EXPECT_EQ(*dataCopy, "coucou");
}

TEST(TestRef, callOperator) {
	ememory::RefPtr<MyTestString> data = ememory::makeRef<MyTestString>("coucou");
	EXPECT_EQ(data->size(), 6);
}

static void functionCallRef(etk::String& _data) {
	_data = "plop";
}

TEST(TestRef, callOperatorStar) {
	ememory::RefPtr<MyTestString> data = ememory::makeRef<MyTestString>("coucou");
	EXPECT_EQ(data->size(), 6);
	EXPECT_EQ(*data, "coucou");
	*data = "ragout";
	EXPECT_EQ(data->size(), 6);
	EXPECT_EQ(*data, "ragout");
	functionCallRef(*data);
	EXPECT_EQ(data->size(), 4);
	EXPECT_EQ(*data, "plop");
}

namespace {
	class basicClass : public ememory::RefCounter {
		
	};
	
	class heritedClass : public basicClass {
		
	};
}

TEST(TestRef, heritage) {
	ememory::RefPtr<heritedClass> data = ememory::makeRef<heritedClass>();
	ememory::RefPtr<basicClass> data2 = data;
	ememory::RefPtr<basicClass> data3(data);
	EXPECT_NE(data, null);
	EXPECT_NE(data2, null);
	EXPECT_NE(data3, null);
}




static uint32_t isDestroy = 0;
namespace {
	class testContructDestruct : public ememory::RefCounter {
		private:
			uint32_t m_addValue;
		public:
			testContructDestruct(uint32_t _addValue):
			  m_addValue(_addValue) {
				isDestroy += m_addValue;
				TEST_DEBUG("Create class " << m_addValue);
			}
			testContructDestruct(testContructDestruct&& _obj):
			  m_addValue(_obj.m_addValue) {
				_obj.m_addValue = 0;
				TEST_DEBUG("move contruction " << m_addValue);
			}
			virtual ~testContructDestruct() {
				if (m_addValue == 0) {
					TEST_DEBUG("Remove class (after move)");
					return;
				}
				TEST_DEBUG("Remove Class " << m_addValue);
				isDestroy -= m_addValue;
			}
			testContructDestruct& operator= (testContructDestruct&& _obj) {
				TEST_DEBUG("move operator " << m_addValue);
				if (this != &_obj) {
					etk::swap(m_addValue, _obj.m_addValue);
				}
				return *this;
			}
	};
}

TEST(TestRef, destroyElementAtTheCorectMoment) {
	isDestroy = 0;
	{
		etk::Vector<ememory::RefPtr<testContructDestruct>> list;
		list.pushBack(ememory::makeRef<testContructDestruct>(55));
		EXPECT_EQ(list.size(), 1);
		EXPECT_EQ(isDestroy, 55);
		auto it = list.erase(list.begin());
		EXPECT_EQ(isDestroy, 0);
		EXPECT_EQ(list.size(), 0);
		EXPECT_EQ(it, list.end());
	}
	EXPECT_EQ(isDestroy, 0);
}

TEST(TestRef, destroyElementAtTheCorectMoment_2) {
	isDestroy = 0;
	{
		etk::Vector<ememory::RefPtr<testContructDestruct>> list;
		list.pushBack(ememory::makeRef<testContructDestruct>(4));
		list.pushBack(ememory::makeRef<testContructDestruct>(30));
		list.pushBack(ememory::makeRef<testContructDestruct>(1000));
		list.pushBack(ememory::makeRef<testContructDestruct>(200));
		EXPECT_EQ(list.size(), 4);
		EXPECT_EQ(isDestroy, 1234);
		auto it = list.erase(list.begin());
		EXPECT_EQ(list.size(), 3);
		EXPECT_EQ(isDestroy, 1230);
		it = list.erase(list.begin()+1);
		EXPECT_EQ(isDestroy, 230);
		EXPECT_EQ(list.size(), 2);
	}
	EXPECT_EQ(isDestroy, 0);
}
