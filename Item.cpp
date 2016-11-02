#include "Item.h"



Item::Item()
{
}

Item::Item(const int warehouseID, const int size, const int storageUnit, const int orderID, const int customerID, const int truck, const string& barcode) : barcode(barcode)
{
	this->warehouseID = warehouseID;
	this->size = size;
	this->storageUnit = storageUnit;
	this->orderID = orderID;
	this->customerID = customerID;
	this->truck = truck;
}

Item::Item(const Item& orig) : warehouseID(orig.getWarehouseID()), size(orig.getSize()), storageUnit(orig.getStorageUnit()), orderID(orig.getOrderID()), customerID(orig.getCustomerID()), truck(orig.getTruck()), barcode(orig.getBarcode())
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

int Item::getCustomerID() const
{
	return this->customerID;
}

void Item::setCustomerID(const int customerID)
{
	this->customerID = customerID;
}

int Item::getTruck() const
{
	return truck;
}

void Item::setTruck(const int truck)
{
	this->truck = truck;
}

string Item::getBarcode() const
{
	return this->barcode;
}

void Item::setBarcode(string barcode)
{
	this->barcode = barcode;
}

void Item::print() {
	const char t = '\t';
	std::cout <<this->warehouseID << t << this->getStorageUnit() << t << this->getOrderID() << t << this->getCustomerID() << t << this->getTruck() << endl;
}