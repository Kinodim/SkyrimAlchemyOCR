/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <json.hpp>
#include <fstream>

#include "Translator.h"

using namespace std;
using nlohmann::json;

Translator& Translator::Instance(){
	static Translator instance;
	return instance;
}

bool Translator::ReadDictionary(std::string filename){
	ifstream input(filename, ios::in);
	if(!input){
		cout << "Error opening dictionary file " << filename << endl;
		return false;
	}
	input >> _dict;

	return true;
}

void Translator::GetGermanItemNames(std::vector<std::string>& names){
	names.clear();
	for(auto& elem : _dict){
		names.push_back(elem["ger"].get<string>());
	}
}

void Translator::GetEnglishItemNames(std::vector<std::string>& names){
	names.clear();
	for(auto& elem : _dict){
		names.push_back(elem["eng"].get<string>());
	}
}
