/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etest/etest.hpp>
#include <ememory/memory.hpp>
#include "main.hpp"

namespace testEnableCast {
	class testClass {
		public:
			testClass() {
				EMEMORY_INFO("create TestClass");
			}
			virtual ~testClass() {
				EMEMORY_INFO("delete TestClass");
			}
	};
	
	class testClassChild : public testEnableCast::testClass {
		public:
			testClassChild() {
				EMEMORY_INFO("create TestClassChild");
			}
			~testClassChild() {
				EMEMORY_INFO("delete TestClassChild");
			}
	};
}

TEST(TestEnableCast, base) {
	ememory::SharedPtr<testEnableCast::testClassChild> dataChild = ememory::makeShared<testEnableCast::testClassChild>();
	EXPECT_EQ(dataChild.useCount(), 1);
	ememory::SharedPtr<testEnableCast::testClass> data = dataChild;
	EXPECT_EQ(dataChild.useCount(), 2);
	EXPECT_EQ(data.useCount(), 2);
	ememory::SharedPtr<testEnableCast::testClassChild> dataChildStatic = ememory::staticPointerCast<testEnableCast::testClassChild>(data);
	EXPECT_EQ(dataChild.useCount(), 3);
	EXPECT_EQ(data.useCount(), 3);
	EXPECT_EQ(dataChildStatic.useCount(), 3);
	ememory::SharedPtr<testEnableCast::testClassChild> dataChildDynamic = ememory::dynamicPointerCast<testEnableCast::testClassChild>(data);
	EXPECT_EQ(dataChild.useCount(), 4);
	EXPECT_EQ(data.useCount(), 4);
	EXPECT_EQ(dataChildStatic.useCount(), 4);
	EXPECT_EQ(dataChildDynamic.useCount(), 4);
	ememory::SharedPtr<testEnableCast::testClassChild> dataChildReinterpret = ememory::reinterpretPointerCast<testEnableCast::testClassChild>(data);
	EXPECT_EQ(dataChild.useCount(), 5);
	EXPECT_EQ(data.useCount(), 5);
	EXPECT_EQ(dataChildStatic.useCount(), 5);
	EXPECT_EQ(dataChildDynamic.useCount(), 5);
	EXPECT_EQ(dataChildReinterpret.useCount(), 5);
	
}
