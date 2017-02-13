/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#include <fstream>
#include "Recipes.h"
#include "json.hpp"

using std::string;
using std::vector;
using std::map;
using std::ifstream;
using nlohmann::json;

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

bool Recipes::Load(std::string filename){
	_recipes.clear();

	ifstream in(filename, std::ios::in);
	if(!in){
		std::cout << "Error opening file " << filename << std::endl;
		return false;
	}

	json j;
	in >> j;

	for(const auto& el : j){
		Recipe r = el;
		InsertRecipe(r);
	}

	return true;
}
