#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "extractItemsTextFromImage.h"
#include "Translater.h"

using namespace std;

int main(int argc, char *argv[])
{
	/*
	string filename;
	if(argc > 1){
		filename = argv[1];
	}else{
		filename = "../images/sample.jpg";
	}

	vector<string> text;
	extractItemsTextFromImage(filename, text);

	for(auto& t : text){
		cout << t << endl;
	}
	*/

	Translater translater = Translater::Instance();
	translater.ReadDictionary("../data/dictionary.json");
	vector<string> ger_names;
	translater.GetEnglishItemNames(ger_names);
	for(string name : ger_names){
		cout << name << endl;
	}

	
	return 0;
}
