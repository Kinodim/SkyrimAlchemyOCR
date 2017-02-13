/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#ifndef RECIPES_H
#define RECIPES_H

#include <map>
#include <vector>
#include <string>

#include "Recipe.h"

class Recipes {
	public:
		static Recipes& Instance();
		void InsertRecipe(const Recipe& r);
		const std::vector<Recipe> GetRecipes();
		bool Load(std::string filename);
	private:
		Recipes() = default;
		std::map<std::string, Recipe> _recipes;
};


#endif /* end of include guard: RECIPES_H */
