/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <stdexcept>
#include <limits>
#include <cctype>
#include <sstream>
#include <vector>

#include "OCRProcessor.h"
#include "Translator.h"
#include "levenshtein.h"

using namespace std;

template<typename Out>
void split(const string& s, char delim, bool first, Out result){
	stringstream ss;
	ss.str(s);
	string item;
	while(getline(ss, item, delim)) {
		*(result++) = item;
		if(first && !ss.eof()){ 
			// we did first split and there is remaining string
			// now add remaining string and exit
			getline(ss, item);
			*(result++) = item;
			return;
		}
	}
}

vector<string> OCRProcessor::Split(const string& s, char delim){
	vector<string> elems;
	split(s, delim, false, back_inserter(elems));
	return elems;
}

vector<string> OCRProcessor::SplitFirst(const string& s, char delim){
	vector<string> elems;
	split(s, delim, true, back_inserter(elems));
	return elems;
}

string OCRProcessor::Trim(const string& str){
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first){
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::pair<std::string, int> OCRProcessor::PreprocessOCRText(std::string text){
	pair<string,int> res("",0);
	if(text == ""){
		return res;
	}
	vector<string> splitted = SplitFirst(text, '(');
	if(splitted.size() == 0){
		return res;
	}
	else if(splitted.size() > 1){
		res.second = ParseOCRNumber(Trim(splitted[1]));
	}
	else{
		res.second = 1;
	}
	res.first = Trim(splitted[0]);

	return res;
}

int OCRProcessor::ParseOCRNumber(std::string number){
	string num_str = "";
	for(char c : number){
		if(isdigit(c)){
			num_str += c;
			continue;
		}
		switch(c){
			case '(':
				num_str += "1";
				break;
			case 'B':
				num_str += "3";
				break;
			case 'S':
				num_str += "5";
				break;
			case 'G':
				num_str += "6";
				break;
			case 'O':
				num_str += "0";
				break;
			default:
				goto END_OF_LOOP;
		}
	}
END_OF_LOOP:

	int res = 1;
	try{
		res = stoi(num_str);
	}
	catch(invalid_argument){
	}
	return res;
}

int OCRProcessor::FindNearestMatch(const std::string& txt, std::string& res){
	int min = numeric_limits<int>::max();
	if(txt == ""){
		return min;
	}
	Translator& translator = Translator::Instance();
	if(!translator.hasDictionary()){
		bool success = translator.ReadDictionary(_dict_filename);
		if(!success){
			return min;
		}
	}

	vector<string> names;
	translator.GetGermanItemNames(names);
	for(string item : names){
		int dist = levenshteinDistance(txt, item);
		if(dist < min){
			res = item;
			min = dist;
		}
	}
	return min;
}

bool OCRProcessor::MatchOCRText(const std::string& txt, std::string& res){
	string candidate;
	int dist = FindNearestMatch(txt, candidate);
	if(dist >= txt.length() * _length_weight + _offset){
		return false;
	}
	res = candidate;
	return true;
}

std::pair<std::string, int> OCRProcessor::Process(const std::string& txt){
	pair<string,int> res("", 0);

	pair<string,int> preprocessed = PreprocessOCRText(txt);
	string matched = "";
	bool success = MatchOCRText(preprocessed.first, matched);
	if(!success){
		return res;
	}

	string translation = Translator::Instance().GetEnglishTranslation(matched);

	res.first = translation;
	res.second = preprocessed.second;

	return res;
}
