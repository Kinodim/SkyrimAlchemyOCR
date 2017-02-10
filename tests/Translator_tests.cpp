/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <gtest/gtest.h>
#include <algorithm>
#include "Translator.h"

using namespace std;

class TranslatorTest : public ::testing::Test {
	protected:
		TranslatorTest()
			:_t(Translator::Instance())
		{
			bool ret = _t.ReadDictionary("../data/dictionary.json");
		};
		Translator _t;
};

TEST(TranslatorTests, SingletonEmpty){
	vector<string> names_ger;
	Translator t = Translator::Instance();
	t.GetGermanItemNames(names_ger);
	ASSERT_LE(names_ger.size(), 0);
}

TEST_F(TranslatorTest, ReadDictionary){
	bool ret = _t.ReadDictionary("../data/dictionary.json");
	ASSERT_EQ(ret, true);
}

TEST_F(TranslatorTest, GetGermanItemNames){
	vector<string> names_ger;
	_t.GetGermanItemNames(names_ger);
	ASSERT_GT(names_ger.size(), 0);
	auto it = find(names_ger.begin(), names_ger.end(), "Weißkappe");
	ASSERT_NE(it, names_ger.end());
}

TEST_F(TranslatorTest, GetEnglishItemNames){
	vector<string> names_eng;
	_t.GetEnglishItemNames(names_eng);
	ASSERT_GT(names_eng.size(), 0);
	auto it = find(names_eng.begin(), names_eng.end(), "White Cap");
	ASSERT_NE(it, names_eng.end());
}


int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
