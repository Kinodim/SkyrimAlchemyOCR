/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <string>
#include <vector>
#include "json.hpp"

class Translator{
	public:
		static Translator& Instance();
		bool ReadDictionary(std::string filename);
		void GetGermanItemNames(std::vector<std::string>& names);
		void GetEnglishItemNames(std::vector<std::string>& names);
	private:
		Translator() = default;
		nlohmann::json _dict;
};
