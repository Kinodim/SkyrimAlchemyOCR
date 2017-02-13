#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "extractItemsTextFromImage.h"
#include "Translator.h"
#include "OCRProcessor.h"
#include "RecipeSearcher.h"

using namespace std;

const string dictionary_file = "../data/dictionary.json";
const string recipes_file = "../data/recipes.json";
const string default_image_file = "../images/sample.jpg";
const int OCR_matching_offset = 1;
const double OCR_matching_length_weight = 0.5;

void ProcessImage(const string& filename, Inventory& inventory){
	vector<string> text;
	int res = extractItemsTextFromImage(filename, text);
	if(res == -1){
		return;
	}

	cout << endl << endl;
	cout << "Processing " << filename << flush << endl;
	OCRProcessor processor(dictionary_file, OCR_matching_offset, OCR_matching_length_weight);
	for(auto& t : text){
		cout << processor.Trim(t); //TODO: Refactor Trim to common header
		pair<string,int> entry = processor.Process(t);
		cout << "\tInterpreted as: " << entry.second << " times " << entry.first << endl;
		if(entry.first == ""){
			continue;
		}
		inventory.AddItem(entry.first, entry.second);
	}
}

int main(int argc, char *argv[])
{
	vector<string> image_files;
	if(argc > 1){
		for (int i = 1; i < argc; ++i) {
			image_files.push_back(argv[i]);
		}
	}else{
		image_files = {default_image_file};
	}

	Inventory inventory;
	for(const auto& file : image_files){
		ProcessImage(file, inventory);
	}

	cout << endl;
	cout << string(20, '-') << endl;
	cout << endl;

	cout << "Resulting Inventory:" << endl;
	cout << inventory;

	cout << endl;
	cout << string(20, '-') << endl;
	cout << endl;

	Recipes& recipes = Recipes::Instance();
	recipes.Load(recipes_file);
	vector<Recipe> possible_recipes;
	RecipeSearcher searcher;
	searcher.GetPossibleRecipes(inventory, possible_recipes);

	if(possible_recipes.empty()){
		cout << "No recipes found..." << endl;
		return 0;
	}
	for(const auto& r : possible_recipes){
		cout << r << endl;
	}

	return 0;
}
