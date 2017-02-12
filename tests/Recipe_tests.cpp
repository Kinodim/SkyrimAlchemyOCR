/**********************************************************************
*                     Dominik Schulz, 12.02.2017                     *
**********************************************************************/

#include <gtest/gtest.h>
#include "Recipe.h"
#include "json.hpp"

using namespace std;
using nlohmann::json;

class Recipe_Tests : public ::testing::Test {
	protected:
		void SetUp(){
			_r = Recipe({"in1", "in2", "in3"},{"someffect1", "othereffect2"});
		}

		Recipe _r;
};

TEST_F(Recipe_Tests, Constructor){
	ASSERT_EQ(3, _r.ingredients.size());
	ASSERT_EQ(2, _r.effects.size());
	ASSERT_EQ(*_r.ingredients.begin(), "in1");
}

TEST_F(Recipe_Tests, OutstreamOperator){
	// visual test, no assertions
	cout << _r;
}

TEST_F(Recipe_Tests, ToJson){
	json j;
	j = _r;
	ASSERT_EQ(2, j["effects"].size());
	ASSERT_EQ(3, j["ingredients"].size());
}

TEST_F(Recipe_Tests, FromJson){
	json j = {
		{"ingredients", {"ing1", "ing2", "ing3"}},
		{"effects", {"effone", "eff2"}}
	};
	_r = j;
	ASSERT_EQ(3, _r.ingredients.size());
	ASSERT_EQ(2, _r.effects.size());
	ASSERT_EQ(*_r.ingredients.begin(), "ing1");
	ASSERT_EQ(*_r.effects.begin(), "eff2");
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
