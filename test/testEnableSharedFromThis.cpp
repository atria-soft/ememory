/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <gtest/gtest.h>
#include <ememory/memory.hpp>
#include "main.hpp"

namespace testESFT {
	class TestClassBase : public ememory::EnableSharedFromThis<TestClassBase> {
		public:
			TestClassBase() {
				EMEMORY_INFO("create TestClassBase");
			}
			~TestClassBase() {
				EMEMORY_INFO("delete TestClassBase");
			}
	};
	
	class TestClass : public testESFT::TestClassBase {
		public:
			TestClass() {
				EMEMORY_INFO("create TestClass");
			}
			~TestClass() {
				EMEMORY_INFO("delete TestClass");
			}
	};
}

TEST(TestEnableSharedFromThis, testClassBase) {
	ememory::SharedPtr<testESFT::TestClassBase> data = ememory::makeShared<testESFT::TestClassBase>();
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<testESFT::TestClassBase> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
}

TEST(TestEnableSharedFromThis, testClass) {
	ememory::SharedPtr<testESFT::TestClass> data = ememory::makeShared<testESFT::TestClass>();
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<testESFT::TestClass> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
}
