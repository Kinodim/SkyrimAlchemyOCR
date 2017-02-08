/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#ifndef RECIPE_H
#define RECIPE_H

#include <string>
#include <vector>

#include "json.hpp"

using std::string;
using std::vector;
using nlohmann::json;

struct recipe{
	string id1;
	string id2;
	string id3;
	vector<string> effects;

	recipe(string id1, string id2, string id3, vector<string> effects){
		this->id1 = id1;
		this->id2 = id2;
		this->id3 = id3;
		this->effects = effects;
	}
	recipe()
		: recipe("","","",vector<string>())
	{}
};

void to_json(json& j, const recipe& r){
	j = json{{"id1",r.id1}, {"id2", r.id2}, {"id3", r.id3}, {"effects", r.effects}};
}

void from_json(const json& j, recipe& r){
	r.id1 = j["id1"];
	r.id2 = j["id2"];
	r.id3 = j["id3"];
	r.effects = j["effects"].get<vector<string> >();

}

#endif /* end of include guard: RECIPE_H */
