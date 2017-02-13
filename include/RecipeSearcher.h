/**********************************************************************
*                     Dominik Schulz, 13.02.2017                     *
**********************************************************************/

#ifndef RECIPESEARCHER_H
#define RECIPESEARCHER_H

#include <vector>
#include <iostream>
#include "Recipe.h"
#include "Recipes.h"
#include "Inventory.h"

class RecipeSearcher {
	public:
		RecipeSearcher() = default;
		void GetPossibleRecipes(const Inventory& inventory, std::vector<Recipe>& res){
			res.clear();

			std::vector<Recipe> recipes = Recipes::Instance().GetRecipes();
			if(recipes.empty()){
				std::cout << "Warning: No recipes loaded!" << std::endl;
				return;
			}

			for(const auto& r : Recipes::Instance().GetRecipes()){
				bool possible = true;
				for(const auto& i : r.ingredients){
					if(! inventory.hasItem(i)){
						possible = false;
						break;
					}
				}
				if(!possible){
					continue; // with recipe loop
				}

				res.push_back(r);
			} // end of recipe loop
		}
};

#endif /* end of include guard: RECIPESEARCHER_H */
