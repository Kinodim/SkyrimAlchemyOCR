/**********************************************************************
*                     Dominik Schulz, 13.02.2017                     *
**********************************************************************/

#include <map>

class Inventory {

	public:
		Inventory() = default;
		void AddItem(const std::string& name, int amount, bool /* sum */ = false ){
			_inventory[name] = amount; //TODO implement sum version
		}
		bool hasItem(const std::string& name) const {
			return _inventory.find(name) != _inventory.end();
		}
		int getItemCount(const std::string& name){
			if(!hasItem(name)){
				return 0;
			}
			return _inventory[name];
		}

	private:
		std::map<std::string, int> _inventory;
};
