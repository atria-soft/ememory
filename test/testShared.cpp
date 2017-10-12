/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etest/etest.hpp>
#include <ememory/memory.hpp>
#include <test-debug/debug.hpp>

#include "main.hpp"

TEST(TestShared, createAndDestroy) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	EXPECT_EQ(data == nullptr, false);
	EXPECT_EQ(data != nullptr, true);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(data == nullptr, true);
	EXPECT_EQ(data != nullptr, false);
}

TEST(TestShared, copy) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	ememory::SharedPtr<etk::String> dataCopy = data;
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

TEST(TestShared, swap) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	ememory::SharedPtr<etk::String> dataCopy;
	EXPECT_EQ(dataCopy.useCount(), 0);
	dataCopy.swap(data);
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataCopy.useCount(), 1);
	EXPECT_EQ(*dataCopy, "coucou");
}

TEST(TestShared, callOperator) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data->size(), 6);
}

static void functionCallRef(etk::String& _data) {
	_data = "plop";
}

TEST(TestShared, callOperatorStar) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data->size(), 6);
	EXPECT_EQ(*data, "coucou");
	*data = "ragout";
	EXPECT_EQ(data->size(), 6);
	EXPECT_EQ(*data, "ragout");
	functionCallRef(*data);
	EXPECT_EQ(data->size(), 4);
	EXPECT_EQ(*data, "plop");
}

TEST(TestShared, setInVoid) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	ememory::SharedPtr<void> dataVoid(data);
	ememory::SharedPtr<void> dataVoid2;
	dataVoid2 = data;
	EXPECT_EQ(data.useCount(), 3);
}




static uint32_t isDestroy = 0;

class testContructDestruct {
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

class testContructDestruct2 : public ememory::EnableSharedFromThis<testContructDestruct2> {
	private:
		uint32_t m_addValue;
	public:
		testContructDestruct2(uint32_t _addValue):
		  m_addValue(_addValue) {
			isDestroy += m_addValue;
			TEST_DEBUG("Create class " << m_addValue);
		}
		testContructDestruct2(testContructDestruct2&& _obj):
		  m_addValue(_obj.m_addValue) {
			_obj.m_addValue = 0;
			TEST_DEBUG("move contruction " << m_addValue);
		}
		virtual ~testContructDestruct2() {
			if (m_addValue == 0) {
				TEST_DEBUG("Remove class (after move)");
				return;
			}
			TEST_DEBUG("Remove Class " << m_addValue);
			isDestroy -= m_addValue;
		}
		testContructDestruct2& operator= (testContructDestruct2&& _obj) {
			TEST_DEBUG("move operator " << m_addValue);
			if (this != &_obj) {
				etk::swap(m_addValue, _obj.m_addValue);
			}
			return *this;
		}
};

TEST(TestShared, destroyElementAtTheCorectMoment) {
	isDestroy = 0;
	{
		etk::Vector<ememory::SharedPtr<testContructDestruct>> list;
		list.pushBack(ememory::makeShared<testContructDestruct>(55));
		EXPECT_EQ(list.size(), 1);
		EXPECT_EQ(isDestroy, 55);
		auto it = list.erase(list.begin());
		EXPECT_EQ(isDestroy, 0);
		EXPECT_EQ(list.size(), 0);
		EXPECT_EQ(it, list.end());
	}
	EXPECT_EQ(isDestroy, 0);
}

TEST(TestShared, destroyElementAtTheCorectMoment_2) {
	isDestroy = 0;
	{
		etk::Vector<ememory::SharedPtr<testContructDestruct>> list;
		list.pushBack(ememory::makeShared<testContructDestruct>(4));
		list.pushBack(ememory::makeShared<testContructDestruct>(30));
		list.pushBack(ememory::makeShared<testContructDestruct>(1000));
		list.pushBack(ememory::makeShared<testContructDestruct>(200));
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

TEST(TestShared, destroyElementAtTheCorectMoment_3) {
	isDestroy = 0;
	{
		etk::Vector<ememory::SharedPtr<testContructDestruct2>> list;
		list.pushBack(ememory::makeShared<testContructDestruct2>(55));
		EXPECT_EQ(list.size(), 1);
		EXPECT_EQ(isDestroy, 55);
		auto it = list.erase(list.begin());
		EXPECT_EQ(isDestroy, 0);
		EXPECT_EQ(list.size(), 0);
		EXPECT_EQ(it, list.end());
	}
	EXPECT_EQ(isDestroy, 0);
}

TEST(TestShared, destroyElementAtTheCorectMoment_4) {
	isDestroy = 0;
	{
		etk::Vector<ememory::SharedPtr<testContructDestruct2>> list;
		list.pushBack(ememory::makeShared<testContructDestruct2>(4));
		list.pushBack(ememory::makeShared<testContructDestruct2>(30));
		list.pushBack(ememory::makeShared<testContructDestruct2>(1000));
		list.pushBack(ememory::makeShared<testContructDestruct2>(200));
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
