/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include "json.hpp"
#include "Recipe.h"
#include "Recipes.h"

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

void insertRecipe(const set<string>& ingredients, const set<string>& effects){
	Recipe r = {ingredients, effects};
	Recipes::Instance().InsertRecipe(r);
}

inline set<string> getEffectsFromIngredient(json& ingredient){
	return {ingredient["effect1"], ingredient["effect2"], ingredient["effect3"], ingredient["effect4"]};
}

void createRecipes(json& ingredients){
	for(auto& i : ingredients){
		for(auto& j : ingredients){
			if(i == j){
				continue;
			}

			set<string> recipe_ingredients;
			// check for common effects from the two selected ingredients
			set<string> effects1 = getEffectsFromIngredient(i);
			set<string> effects2 = getEffectsFromIngredient(j);
			set<string> effects_junction;
			set_intersection(effects1.begin(), effects1.end(), effects2.begin(), effects2.end(),
					inserter(effects_junction, effects_junction.begin()));
			if(effects_junction.size() == 0){
				continue;
			}

			recipe_ingredients.insert(i["name"].get<string>());
			recipe_ingredients.insert(j["name"].get<string>());
			insertRecipe(recipe_ingredients, effects_junction);

			set<string> effects_union;
			set_union(effects1.begin(), effects1.end(), effects2.begin(), effects2.end(),
					inserter(effects_union, effects_union.begin()));
			set<string> unused_effects;
			set_difference(effects_union.begin(), effects_union.end(), effects_junction.begin(),
					effects_junction.end(),	inserter(unused_effects, unused_effects.begin()));
			for(auto& k : ingredients){
				if(k == i || k == j){
					continue;
				}
				set<string> effects3 = getEffectsFromIngredient(k);
				set<string> common_effects;
				set_intersection(unused_effects.begin(), unused_effects.end(), effects3.begin(),
						effects3.end(),	inserter(common_effects, common_effects.begin()));
				if(common_effects.size() == 0){
					continue;
				}

				set<string> all_ingredients = recipe_ingredients;
				all_ingredients.insert(k["name"].get<string>());
				common_effects.insert(effects_junction.begin(),effects_junction.end());
			} // end of k for
			//goto end_of_loop;

		} // end of j for
	} // end of i for
end_of_loop:
	;
}

int main(int argc, char *argv[])
{
	json ingredients = jsonFromFile("../data/ingredients.json");

	createRecipes(ingredients);
	vector<Recipe> recipes = Recipes::Instance().GetRecipes();
	for(auto& r : recipes){
		cout << r << endl;
	}

	json json_recipes = recipes;
	//cout << setw(4) << json_recipes << endl;

	ofstream out_file("../data/recipes.json", ios::out);
	out_file << setw(4) << json_recipes;

	return 0;
}
