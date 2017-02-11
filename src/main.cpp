#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "extractItemsTextFromImage.h"
#include "Translator.h"
#include "OCRProcessor.h"

using namespace std;

int main(int argc, char *argv[])
{
	string filename;
	if(argc > 1){
		filename = argv[1];
	}else{
		filename = "../images/sample.jpg";
	}

	vector<string> text;
	extractItemsTextFromImage(filename, text);

	OCRProcessor processor("../data/dictionary.json", 1, 0.5);
	for(auto& t : text){
		cout << t;
		pair<string,int> entry = processor.Process(t);
		cout << "\tInterpreted as: " << entry.second << " times " << entry .first << endl;
	}
	
	return 0;
}
