/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/


#include <gtest/gtest.h>
#include <algorithm>
#include "OCRProcessor.h"

using namespace std;

class OCRProcessor_Tests : public ::testing::Test{
	protected:
		OCRProcessor_Tests()
			:_processor("../data/dictionary.json", 1, 0.5)
		{}
		OCRProcessor _processor;
};

TEST_F(OCRProcessor_Tests, Trim){
	ASSERT_EQ("", _processor.Trim(""));
	ASSERT_EQ("Item", _processor.Trim("Item"));
	ASSERT_EQ("Item", _processor.Trim("Item "));
	ASSERT_EQ("Item", _processor.Trim(" Item"));
	ASSERT_EQ("Item", _processor.Trim(" Item "));
}

TEST_F(OCRProcessor_Tests, Split){
	vector<string> res;
	res = _processor.Split("", '(' );
	ASSERT_EQ(0, res.size());
	res = _processor.Split("Item", '(' );
	ASSERT_EQ(1, res.size());
	ASSERT_EQ("Item", res[0]);
	res = _processor.Split("Item ((2)", '(' );
	ASSERT_EQ(3, res.size());
	ASSERT_EQ("Item ", res[0]);
	ASSERT_EQ("2)", res[2]);
}

TEST_F(OCRProcessor_Tests, SplitFirst){
	vector<string> res;
	res = _processor.SplitFirst("Item", '(' );
	ASSERT_EQ(1, res.size());
	res = _processor.SplitFirst("Item ((2)", '(' );
	ASSERT_EQ(2, res.size());
	ASSERT_EQ("Item ", res[0]);
	ASSERT_EQ("(2)", res[1]);
}

TEST_F(OCRProcessor_Tests, ParseOCRNumber){
	int res = _processor.ParseOCRNumber("5");
	ASSERT_EQ(5, res);
	res = _processor.ParseOCRNumber("");
	ASSERT_EQ(1, res);
	res = _processor.ParseOCRNumber("S");
	ASSERT_EQ(5, res);
	res = _processor.ParseOCRNumber("(BSG0O(");
	ASSERT_EQ(1356001, res);
	res = _processor.ParseOCRNumber("S(OG");
	ASSERT_EQ(5106, res);
	res = _processor.ParseOCRNumber("1Sx1");
	ASSERT_EQ(15, res);
}
TEST_F(OCRProcessor_Tests, PreprocessOCRText){
	pair<string, int> res;
	res = _processor.PreprocessOCRText("Someitem (1)");
	ASSERT_EQ("Someitem", res.first);
	ASSERT_EQ(1, res.second);
	res = _processor.PreprocessOCRText("Someitem ");
	ASSERT_EQ("Someitem", res.first);
	ASSERT_EQ(1, res.second);
	res = _processor.PreprocessOCRText("Someitem ( 2)");
	ASSERT_EQ("Someitem", res.first);
	ASSERT_EQ(2, res.second);
	res = _processor.PreprocessOCRText(" Weißkappe (2S)");
	ASSERT_EQ("Weißkappe", res.first);
	ASSERT_EQ(25, res.second);
	res = _processor.PreprocessOCRText("Caniswurzel ((2)");
	ASSERT_EQ("Caniswurzel", res.first);
	ASSERT_EQ(12, res.second);
	res = _processor.PreprocessOCRText("Cäniswurzel(1Gäl)");
	ASSERT_EQ("Cäniswurzel", res.first);
	ASSERT_EQ(16, res.second);
}

TEST_F(OCRProcessor_Tests, FindNearestMatch){
	int dist;
	string nearest;
	dist = _processor.FindNearestMatch("", nearest);
	ASSERT_LT(1000, dist);
	dist = _processor.FindNearestMatch("Salz", nearest);
	ASSERT_EQ(0, dist);
	ASSERT_EQ("Salz", nearest);
	dist = _processor.FindNearestMatch("Nomirus Fiiulnis", nearest);
	ASSERT_EQ(4, dist);
	ASSERT_EQ("Namiras Fäulnis", nearest);
}

TEST_F(OCRProcessor_Tests, MatchOCRText){
	string res;
	ASSERT_FALSE(_processor.MatchOCRText("", res));

	ASSERT_TRUE(_processor.MatchOCRText("Salz", res));
	ASSERT_EQ("Salz", res);

	ASSERT_TRUE(_processor.MatchOCRText("Menschliches Herz", res));
	ASSERT_EQ("Menschliches Herz", res);

	ASSERT_TRUE(_processor.MatchOCRText("Nomirus Fiiulnis", res));
	ASSERT_EQ("Namiras Fäulnis", res);

	ASSERT_FALSE(_processor.MatchOCRText("Random Text", res));
}

TEST_F(OCRProcessor_Tests, Process){
	pair<string, int> res;

	res = _processor.Process("");
	ASSERT_EQ("", res.first);
	ASSERT_EQ(0, res.second);

	res = _processor.Process("Menschliches Herz");
	ASSERT_EQ("Human Heart", res.first);
	ASSERT_EQ(1, res.second);

	res = _processor.Process("Nomirus Fiiulnis (S)");
	ASSERT_EQ("Namira's Rot", res.first);
	ASSERT_EQ(5, res.second);

	res = _processor.Process("Mom Tupinellu (S)");
	ASSERT_EQ("Mora Tapinella", res.first);
	ASSERT_EQ(5, res.second);
}



int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
