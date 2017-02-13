/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#ifndef RECIPE_H
#define RECIPE_H

#include <string>
#include <set>
#include <vector>

#include "json.hpp"

using std::string;
using std::set;
using nlohmann::json;

struct Recipe{
	set<string> ingredients;
	set<string> effects;

	Recipe(const set<string>& ingredients, const set<string>& effects){
		this->ingredients = ingredients;
		this->effects = effects;
	}
	Recipe()
		: Recipe(set<string>(), set<string>())
	{}
};

static std::ostream& operator<< (std::ostream& stream, const Recipe& r){
	stream << "Recipe for effects:" << std::endl;
	for(auto& eff : r.effects){
			stream << "\t" << eff << std::endl;
	}
	stream << "  with the following ingredients:" << std::endl;
	for(auto& ing : r.ingredients){
		stream << "\t" << ing << std::endl;
	}

	return stream;
}

static void to_json(json& j, const Recipe& r){
	j = json{{"ingredients",r.ingredients}, {"effects", r.effects}};
}

static void from_json(const json& j, Recipe& r){
	r.ingredients = j["ingredients"].get<set<string> >();
	r.effects = j["effects"].get<set<string> >();
}

#endif /* end of include guard: RECIPE_H */
