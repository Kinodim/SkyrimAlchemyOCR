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

void Recipes::InsertRecipe(const recipe& r){
	// calculate hash for map
	vector<string> ids = {r.id1, r.id2, r.id3};
	std::sort(ids.begin(), ids.end());
	string hash = "";
	for(string str :  ids){
		hash += str;
	}

	_recipes.insert(std::pair<string,recipe>(hash, r));
}

const vector<recipe> Recipes::GetRecipes(){
	vector<recipe> res;
	for(auto& it : _recipes){
		res.push_back(it.second);
	}

	return res;
}
