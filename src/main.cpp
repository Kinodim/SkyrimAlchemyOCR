#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "extractItemsTextFromImage.h"
#include "Translator.h"

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

	Translator translator = Translator::Instance();
	translator.ReadDictionary("../data/dictionary.json");
	vector<string> ger_names;
	translator.GetEnglishItemNames(ger_names);
	for(string name : ger_names){
		cout << name << endl;
	}

	
	return 0;
}
