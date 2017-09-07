/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */

#include <etest/etest.hpp>

#include <ememory/UniquePtr.hpp>
#include <test-debug/debug.hpp>

TEST(TestEmemoryUniquePtr, Creation_1) {
	ememory::UniquePtr<uint32_t> testData;
	EXPECT_EQ(testData.get(), nullptr);
}

TEST(TestEmemoryUniquePtr, Creation_2) {
	ememory::UniquePtr<uint32_t> testData(new uint32_t(55));
	EXPECT_NE(testData.get(), nullptr);
	EXPECT_EQ(*testData.get(), 55);
}
TEST(TestEmemoryUniquePtr, Creation_3) {
	ememory::UniquePtr<uint32_t> testData = ememory::makeUniquePtr<uint32_t>(456789);
	EXPECT_NE(testData.get(), nullptr);
	EXPECT_EQ(*testData.get(), 456789);
}

uint32_t vals[] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

class testCreateAndDestroy {
	public:
		uint32_t m_val;
	public:
		testCreateAndDestroy(uint32_t _val):
		  m_val(_val) {
			vals[m_val] = 1;
		}
		~testCreateAndDestroy() {
			vals[m_val] = -1;
		}
};

TEST(TestEmemoryUniquePtr, reset) {
	memset(vals, 0, sizeof(vals));
	EXPECT_EQ(vals[1], 0);
	ememory::UniquePtr<testCreateAndDestroy> testData = ememory::makeUniquePtr<testCreateAndDestroy>(1);
	EXPECT_NE(testData.get(), nullptr);
	EXPECT_EQ(vals[1], 1);
	testData.reset();
	EXPECT_EQ(testData.get(), nullptr);
	EXPECT_EQ(vals[1], -1);
}
TEST(TestEmemoryUniquePtr, reset_2) {
	memset(vals, 0, sizeof(vals));
	EXPECT_EQ(vals[1], 0);
	ememory::UniquePtr<testCreateAndDestroy> testData = ememory::makeUniquePtr<testCreateAndDestroy>(1);
	EXPECT_NE(testData.get(), nullptr);
	EXPECT_EQ(vals[1], 1);
	testData = nullptr;
	EXPECT_EQ(testData.get(), nullptr);
	EXPECT_EQ(vals[1], -1);
}
TEST(TestEmemoryUniquePtr, overwrite) {
	memset(vals, 0, sizeof(vals));
	ememory::UniquePtr<testCreateAndDestroy> testData = ememory::makeUniquePtr<testCreateAndDestroy>(1);
	EXPECT_NE(testData.get(), nullptr);
	EXPECT_EQ(vals[1], 1);
	EXPECT_EQ(vals[2], 0);
	testData = ememory::makeUniquePtr<testCreateAndDestroy>(2);
	EXPECT_EQ(vals[1], -1);
	EXPECT_EQ(vals[2], 1);
	testData.reset();
	EXPECT_EQ(vals[1], -1);
	EXPECT_EQ(vals[2], -1);
}



