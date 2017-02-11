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
	_dict.clear();
	input >> _dict;
	if(_dict.empty() ||
			_dict[0]["ger"].is_null() ||
			_dict[0]["eng"].is_null()){
		cout << "Loaded dictionary " << filename << " has wrong structure" << endl;
		_dict.clear();
		return false;
	}

	return true;
}

void Translator::GetGermanItemNames(std::vector<std::string>& names){
	names.clear();
	if(_dict.empty()){
		return;
	}
	for(auto& elem : _dict){
		string german = elem["ger"].get<string>();
		names.push_back(german);
	}
}

void Translator::GetEnglishItemNames(std::vector<std::string>& names){
	names.clear();
	if(_dict.empty()){
		return;
	}
	for(auto& elem : _dict){
		names.push_back(elem["eng"].get<string>());
	}
}

string Translator::GetGermanTranslation(std::string txt){
	string res = "";
	for(auto& el : _dict){
		if(el["eng"] == txt){
			return el["ger"];
		}
	}
	return "";
}

string Translator::GetEnglishTranslation(std::string txt){
	string res = "";
	for(auto& el : _dict){
		if(el["ger"] == txt){
			return el["eng"];
		}
	}
	return "";
}

bool Translator::hasDictionary(){
	return !_dict.empty();
}
