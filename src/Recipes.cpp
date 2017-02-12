/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#include "Recipes.h"

using std::string;
using std::vector;
using std::map;

Recipes& Recipes::Instance(){
	static Recipes instance;
	return instance;
}

void Recipes::InsertRecipe(const Recipe& r){
	// calculate hash for map
	string hash = "";
	for(string str : r.ingredients){
		hash += str;
	}

	_recipes.insert(std::pair<string,Recipe>(hash, r));
}

const vector<Recipe> Recipes::GetRecipes(){
	vector<Recipe> res;
	for(auto& it : _recipes){
		res.push_back(it.second);
	}

	return res;
}
