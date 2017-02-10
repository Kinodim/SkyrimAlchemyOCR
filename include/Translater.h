/**********************************************************************
*                     Dominik Schulz, 10.02.2017                     *
**********************************************************************/

#include <string>
#include <vector>
#include "json.hpp"

class Translater{
	public:
		static Translater& Instance();
		bool ReadDictionary(std::string filename);
		void GetGermanItemNames(std::vector<std::string>& names);
		void GetEnglishItemNames(std::vector<std::string>& names);
	private:
		Translater() = default;
		nlohmann::json _dict;
};
