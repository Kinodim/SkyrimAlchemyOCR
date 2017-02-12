/**********************************************************************
*                     Dominik Schulz, 13.02.2017                     *
**********************************************************************/

#include <gtest/gtest.h>
#include "Inventory.h"

using namespace std;

class Inventory_Tests : public ::testing::Test {
	protected:
		void SetUp(){
			_i.AddItem("ItemOne", 2);
			_i.AddItem("Another", 4);
		}

		Inventory _i;
};

TEST_F(Inventory_Tests, hasItem){
	ASSERT_TRUE(_i.hasItem("ItemOne"));
	ASSERT_TRUE(_i.hasItem("Another"));
	ASSERT_FALSE(_i.hasItem(""));
	ASSERT_FALSE(_i.hasItem("töst"));
}

TEST_F(Inventory_Tests, getItemCount){
	ASSERT_EQ(2, _i.getItemCount("ItemOne"));
	ASSERT_EQ(0, _i.getItemCount("Not"));
	ASSERT_EQ(4, _i.getItemCount("Another"));
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
