#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "extractItemsTextFromImage.h"

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

	for(auto& t : text){
		cout << t << endl;
	}

	
	return 0;
}
