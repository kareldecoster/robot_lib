#pragma once
#ifndef ITEM_H_
#define ITEM_H_

#include "Warehouse.h"
#include <iostream>
#include <string>

using namespace std;

class Item
{
public:
	Item();
	Item(const int warehouseID, const int size, const int storageUnit, const int orderID, const int customerID, const int truck, const string& barcode);
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
	int getCustomerID() const;
	void setCustomerID(const int customerID);
	int getTruck() const;
	void setTruck(const int truck);
	string getBarcode() const;
	void setBarcode(string barcode);
	void print();

private:
	int warehouseID;
	int size;
	int storageUnit;
	int orderID;
	int customerID;
	int truck;
	string barcode;
};

inline bool operator==(const Item& lhs, const Item& rhs) {
	if (lhs.getCustomerID() == rhs.getCustomerID() && lhs.getOrderID() == rhs.getOrderID() && lhs.getStorageUnit() == rhs.getStorageUnit() && lhs.getWarehouseID() == rhs.getWarehouseID()) {
		return true;
	}
	return false;
}

inline bool operator< (const Item& lhs, const Item& rhs) { 
	// First order by warehouseID, them by storage unit.
	if (lhs.getCustomerID() != rhs.getCustomerID()) {
		return lhs.getCustomerID() < rhs.getCustomerID();
	}
	return lhs.getOrderID() < rhs.getOrderID();
}
inline bool operator> (const Item& lhs, const Item& rhs) { return rhs < lhs; }
inline bool operator<=(const Item& lhs, const Item& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Item& lhs, const Item& rhs) { return !(lhs < rhs); }

#endif /* ITEM_H_ */