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
	Item(Warehouse* wh, int size, int storageUnit, int orderID, string barcode);
	~Item();
	Warehouse* getWarehouse() const;
	void setWarehouse(Warehouse* wh);
	int getSize() const;
	void setSize(int size);
	int getStorageUnit() const;
	void setStorageUnit(int storageUnit);
	int getOrderID() const;
	void setOrderID(int orderID);
	string getBarcode() const;
	void setBarcode(string barcode);

private:
	Warehouse* warehouse;
	int size;
	int storageUnit;
	int orderID;
	string barcode;
};

inline bool operator< (const Item& lhs, const Item& rhs) { 
	// First order by warehouseID, them by storage unit.
	if (lhs.getWarehouse()->getWarehouseId() != rhs.getWarehouse()->getWarehouseId()) {
		return lhs.getWarehouse()->getWarehouseId() < rhs.getWarehouse()->getWarehouseId();
	}
	return lhs.getStorageUnit() < rhs.getStorageUnit(); 
}
inline bool operator> (const Item& lhs, const Item& rhs) { return rhs < lhs; }
inline bool operator<=(const Item& lhs, const Item& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Item& lhs, const Item& rhs) { return !(lhs < rhs); }

#endif /* ITEM_H_ */