/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <gtest/gtest.h>
#include <ememory/memory.h>
#include "main.h"


class testClass : public ememory::EnableSharedFromThis<testClass> {
	public:
		testClass() {
			EMEMORY_INFO("create TestClass");
		}
		~testClass() {
			EMEMORY_INFO("delete TestClass");
		}
};


TEST(TestEnableSharedFromThis, base) {
	ememory::SharedPtr<testClass> data = ememory::makeShared<testClass>();
	EXPECT_EQ(data.useCount(), 1);
	ememory::WeakPtr<testClass> dataWeak = data;
	EXPECT_EQ(data.useCount(), 1);
	EXPECT_EQ(dataWeak.useCount(), 1);
	data.reset();
	EXPECT_EQ(data.useCount(), 0);
	EXPECT_EQ(dataWeak.useCount(), 0);
}
