/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

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

int main(int argc, char *argv[])
{

	json items = jsonFromFile("../data/ingredients.json");
	for(json::iterator it = items.begin(); it != items.end(); ++it){
		cout << (*it)["name"] << endl;
	}

	
	return 0;
}
