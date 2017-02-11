/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <vector>
#include <string>

class OCRProcessor{
	public:
		OCRProcessor(const std::string dict_filename)
			:OCRProcessor(dict_filename, 1, 0)
		{}
		OCRProcessor(const std::string dict_filename, int offset, double length_weight)
			:_dict_filename(dict_filename), _offset(offset), _length_weight(length_weight)
		{}
		std::string Trim(const std::string& str);
		std::vector<std::string> Split(const std::string& s, char delim);
		std::vector<std::string> SplitFirst(const std::string& s, char delim);
		int ParseOCRNumber(std::string number);
		std::pair<std::string, int> PreprocessOCRText(std::string text);
		int FindNearestMatch(const std::string& txt, std::string& res);
		bool MatchOCRText(const std::string& txt, std::string& res);
		std::pair<std::string, int> Process(const std::string& txt);
	private:
		std::string _dict_filename;
		int _offset = 0;
		double _length_weight = 0;
};
