
#include "ManagerRobot.h"

ManagerRobot::ManagerRobot() {
	// TODO Auto-generated constructor stub

}

void ManagerRobot::addWarehouse(Warehouse wh) {
	this->warehouses.push_back(wh);
}

void ManagerRobot::removeWarehouse(Warehouse wh) {
	this->warehouses.remove(wh);
}

void ManagerRobot::removeWarehouse(int warehouseId) {
	for (std::list<Warehouse>::iterator i = this->warehouses.begin(), e = this->warehouses.end(); i != e; )
	{
		if (i->getWarehouseId() == warehouseId)
			i = this->warehouses.erase(i);
		else
			++i;
	}
}

void ManagerRobot::print() {
	cout << warehouses.size() << endl;
}

ManagerRobot::~ManagerRobot() {
	// TODO Auto-generated destructor stub
}
