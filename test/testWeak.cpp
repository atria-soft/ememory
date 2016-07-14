/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <gtest/gtest.h>
#include <ememory/memory.h>
#include "main.h"

TEST(TestWeak, createAndDestroy) {
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<std::string> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
}

TEST(TestWeak, retrive) {
	ememory::SharedPtr<std::string> data = ememory::makeShared<std::string>("coucou");
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<std::string> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	ememory::SharedPtr<std::string> dataRetrive = dataWeak.lock();
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


