#pragma once
#ifndef ITEM_H_
#define ITEM_H_

#include "Warehouse.h"
#include <string>

using namespace std;

class Item
{
public:
	Item();
	Item(const int warehouseID, const int size, const int storageUnit, const int orderID, const string& barcode);
	Item(const Item& orig);
	~Item();
	int getWarehouseID() const;
	void setWarehouseID(int warehouseID);
	int getSize() const;
	void setSize(int size);
	int getStorageUnit() const;
	void setStorageUnit(int storageUnit);
	int getOrderID() const;
	void setOrderID(int orderID);
	string getBarcode() const;
	void setBarcode(string barcode);

private:
	int warehouseID;
	int size;
	int storageUnit;
	int orderID;
	string barcode;
};

inline bool operator< (const Item& lhs, const Item& rhs) { 
	// First order by warehouseID, them by storage unit.
	if (lhs.getWarehouseID() != rhs.getWarehouseID()) {
		return lhs.getWarehouseID() < rhs.getWarehouseID();
	}
	return lhs.getStorageUnit() < rhs.getStorageUnit(); 
}
inline bool operator> (const Item& lhs, const Item& rhs) { return rhs < lhs; }
inline bool operator<=(const Item& lhs, const Item& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Item& lhs, const Item& rhs) { return !(lhs < rhs); }

#endif /* ITEM_H_ */