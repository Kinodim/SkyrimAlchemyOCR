/**********************************************************************
*                     Dominik Schulz, 13.02.2017                     *
**********************************************************************/

#include <vector>
#include "Recipe.h"
#include "Recipes.h"
#include "Inventory.h"

class RecipeSearcher {
	public:
		RecipeSearcher() = default;
		void GetPossibleRecipes(const Inventory& inventory, std::vector<Recipe> res){
			res.clear();
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
