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
const string output_file = "../data/selected_recipes.json";
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

void ShowRecipes(const vector<Recipe>& recipes){
	if(recipes.empty()){
		cout << "No recipes found..." << endl;
		return;
	}
	int recipe_id = 1;
	for(const auto& r : recipes){
		cout << "#" << recipe_id << ": " << r << endl;
		recipe_id++;
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
	ShowRecipes(possible_recipes);

	vector<Recipe> selected_recipes;
	while(true){
		string input;
		cout << "Selected Recipe ID (q to quit): ";
		getline(cin, input);
		if(input == "q"){
			break;
		}

		int selected_id = 0;
		try{
			selected_id = stoi(input);
		}
		catch(invalid_argument){
			cout << "Invalid input." << endl;
			continue;
		}
		if(selected_id < 1 || selected_id > possible_recipes.size()){
			cout << "Invalid index." << endl;
			continue;
		}

		selected_recipes.push_back(possible_recipes[selected_id - 1]);
		for(const auto& i : possible_recipes[selected_id - 1].ingredients){
			inventory.RemoveItem(i);
		}
		cout << string(20, '-') << endl;
		cout << "Selected Recipes:" << endl;
		ShowRecipes(selected_recipes);

		searcher.GetPossibleRecipes(inventory, possible_recipes);
		cout << string(20, '-') << endl;
		cout << "Possible Recipes:" << endl;
		ShowRecipes(possible_recipes);
	}

	if(selected_recipes.empty()){
		return 0;
	}
	ofstream outstream(output_file);
	if(!outstream){
		cout << "Error opening output file " << output_file << endl;
	}
	json j = json::array();
	for(const auto& r : selected_recipes){
		j.push_back(r);
	}
	outstream << setw(4) << j;

	return 0;
}
