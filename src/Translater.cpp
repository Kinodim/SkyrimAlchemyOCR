/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <json.hpp>
#include <fstream>

#include "Translater.h"

using namespace std;
using nlohmann::json;

Translater& Translater::Instance(){
	static Translater instance;
	return instance;
}

bool Translater::ReadDictionary(std::string filename){
	ifstream input(filename, ios::in);
	if(!input){
		cout << "Error opening dictionary file " << filename << endl;
		return false;
	}
	input >> _dict;

	return true;
}

void Translater::GetGermanItemNames(std::vector<std::string>& names){
	names.clear();
	for(auto& elem : _dict){
		names.push_back(elem["ger"].get<string>());
	}
}

void Translater::GetEnglishItemNames(std::vector<std::string>& names){
	names.clear();
	for(auto& elem : _dict){
		names.push_back(elem["eng"].get<string>());
	}
}
