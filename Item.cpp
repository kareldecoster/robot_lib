#include "Item.h"



Item::Item()
{
}

Item::Item(Warehouse * wh, int size, int storageUnit, int orderID, string barcode) : this->barcode(barcode)
{
	this->warehouse = wh;
	this->size = size;
	this->storageUnit = storageUnit;
	this->orderID = orderID;
}

Item::Item(const Item & orig) : warehouse(orig.getWarehouse()), size(orig.getSize()), storageUnit(orig.getStorageUnit()), orderID(orig.getOrderID()), barcode(orig.getBarcode())
{
}


Item::~Item()
{
}

Warehouse * Item::getWarehouse() const
{
	return this->warehouse;
}

void Item::setWarehouse(Warehouse * wh)
{
	this->warehouse = wh;
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
