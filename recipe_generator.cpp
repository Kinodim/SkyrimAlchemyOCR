/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include "json.hpp"
#include "recipe.h"

using namespace std;
//using json = nlohmann::json;
using nlohmann::json;


string get_file_contents(string filename){
	ifstream in(filename, ios::in);
	if(in){
		return(string(istreambuf_iterator<char>(in), istreambuf_iterator<char>()));
	}
	return "";
}

json jsonFromFile(string filename){
	string contents = get_file_contents(filename);

	return json::parse(contents);
}

set<string> getEffectsFromIngredient(json& ingredient){
	return {ingredient["effect1"], ingredient["effect2"], ingredient["effect3"], ingredient["effect4"]};
}

set<string> getCommonEffects(json& ingredient1, json& ingredient2){

}

void createRecipes(json& ingredients, vector<recipe>& recipes){
	for(auto& i : ingredients){
		for(auto& j : ingredients){
			if(i == j){
				continue;
			}
			set<string> effects1 = getEffectsFromIngredient(i);
			set<string> effects2= getEffectsFromIngredient(j);
			set<string> common_effects;
			set_intersection(effects1.begin(), effects1.end(), effects2.begin(), effects2.end(),
					inserter(common_effects, common_effects.begin()));
			for(auto& el : common_effects){
				cout << el << endl;
			}
			if(common_effects.size() > 0){
				cout << i["name"] << ", " << j["name"] << endl;
				goto end_of_loop;
			}


		}
	}
end_of_loop:
	;
}

int main(int argc, char *argv[])
{
	json ingredients = jsonFromFile("../data/ingredients.json");
	vector<recipe> recipes;

	createRecipes(ingredients, recipes);



	/*
	recipe rec1("01","fx","3d", {"one", "two"});
	recipe rec2("01","fx","3d", {"one", "three"});
	
	json j_rec1 = rec1;
	recipe rec3 = j_rec1;
	for(auto& s : rec3.effects){
		cout << s << endl;
	}
	
	ofstream out("recipe_out.json", ios::out);
	out << setw(4) << j_rec1;
	*/
	
	
	
	return 0;
}
