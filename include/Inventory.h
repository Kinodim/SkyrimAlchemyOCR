/**********************************************************************
*                     Dominik Schulz, 13.02.2017                     *
**********************************************************************/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <iostream>

class Inventory {

	public:
		Inventory() = default;
		void AddItem(const std::string& name, int amount, bool /* sum */ = false ){
			_inventory[name] = amount; //TODO implement sum version
		}
		bool RemoveItem(const std::string& name){
			if(!hasItem(name)){
				return false;
			}
			_inventory[name] -= 1;
			return true;
		}
		bool hasItem(const std::string& name) const {
			const auto& it = _inventory.find(name); 
			return (it != _inventory.end() && it->second > 0);
		}
		int getItemCount(const std::string& name){
			if(!hasItem(name)){
				return 0;
			}
			return _inventory[name];
		}

	private:
		std::map<std::string, int> _inventory;

	friend std::ostream& operator<< (std::ostream& stream, const Inventory& i);
};

std::ostream& operator<< (std::ostream& stream, const Inventory& i){

	for(const auto& p : i._inventory){
		stream << p.first << " x " << p.second << std::endl;
	}

	return stream;
}

#endif /* end of include guard: INVENTORY_HINVENTORY_H */
