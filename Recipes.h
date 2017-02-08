/**********************************************************************
*                     Dominik Schulz, 08.02.2017                     *
**********************************************************************/

#ifndef RECIPES_H
#define RECIPES_H

#include <map>
#include <vector>
#include <string>

#include "recipe.h"

class Recipes {
	public:
		static Recipes& Instance();
		void InsertRecipe(const recipe& r);
		const std::vector<recipe> GetRecipes();
	private:
		Recipes() = default;
		std::map<std::string, recipe> _recipes;
};


#endif /* end of include guard: RECIPES_H */
