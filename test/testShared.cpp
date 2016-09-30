/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <gtest/gtest.h>
#include <ememory/memory.hpp>
#include "main.hpp"

TEST(TestShared, createAndDestroy) {
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
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
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	ememory::SharedPtr<std::string> dataCopy = data;
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
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(*data, "coucou");
	ememory::SharedPtr<std::string> dataCopy;
	EXPECT_EQ(dataCopy.useCount(), 0);
	dataCopy.swap(data);
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataCopy.useCount(), 1);
	EXPECT_EQ(*dataCopy, "coucou");
}

TEST(TestShared, callOperator) {
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
	EXPECT_EQ(data->size(), 6);
}

static void functionCallRef(std::string& _data) {
	_data = "plop";
}

TEST(TestShared, callOperatorStar) {
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
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
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
	ememory::SharedPtr<void> dataVoid(data);
	ememory::SharedPtr<void> dataVoid2;
	dataVoid2 = data;
	EXPECT_EQ(data.useCount(), 3);
}


