#include "Item.h"



Item::Item()
{
}

Item::Item(const int warehouseID, const int size, const int storageUnit, const int orderID, const string& barcode) : barcode(barcode)
{
	this->warehouseID = warehouseID;
	this->size = size;
	this->storageUnit = storageUnit;
	this->orderID = orderID;
}

Item::Item(const Item & orig) : warehouseID(orig.getWarehouseID()), size(orig.getSize()), storageUnit(orig.getStorageUnit()), orderID(orig.getOrderID()), barcode(orig.getBarcode())
{
}


Item::~Item()
{
}

int Item::getWarehouseID() const
{
	return this->warehouseID;
}

void Item::setWarehouseID(int whid)
{
	this->warehouseID = whid;
}

int Item::getSize() const
{
	return this->size;
}

void Item::setSize(int size)
{
	this->size = size;
}

int Item::getStorageUnit() const
{
	return this->storageUnit;
}

void Item::setStorageUnit(int storageUnit)
{
	this->storageUnit = storageUnit;
}

int Item::getOrderID() const
{
	return this->orderID;
}

void Item::setOrderID(int orderID)
{
	this->orderID = orderID;
}

string Item::getBarcode() const
{
	return this->barcode;
}

void Item::setBarcode(string barcode)
{
	this->barcode = barcode;
}
