/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <gtest/gtest.h>
#include <ememory/memory.hpp>
#include "main.hpp"

TEST(TestWeak, createAndDestroy) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<etk::String> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
}

TEST(TestWeak, retrive) {
	ememory::SharedPtr<etk::String> data = ememory::makeShared<etk::String>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<etk::String> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	ememory::SharedPtr<etk::String> dataRetrive = dataWeak.lock();
	EXPECT_EQ(data.useCount(), 2);
	EXPECT_EQ(dataWeak.useCount(), 2);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataRetrive.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	EXPECT_EQ(*dataRetrive, "coucou");
	dataRetrive.reset();
	EXPECT_EQ(dataRetrive.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
	dataRetrive = dataWeak.lock();
	EXPECT_EQ(dataRetrive == nullptr, true);
	EXPECT_EQ(dataRetrive.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
}


