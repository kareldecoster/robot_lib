
#include "TaskDistributer.h"

TaskDistributer::TaskDistributer() {
	// TODO Auto-generated constructor stub

}

void TaskDistributer::addWarehouse(Warehouse wh) {
	this->warehouses.push_back(wh);
}

void TaskDistributer::removeWarehouse(Warehouse wh) {
	this->warehouses.remove(wh);
}

void TaskDistributer::removeWarehouse(int warehouseId) {
	for (std::list<Warehouse>::iterator i = this->warehouses.begin(), e = this->warehouses.end(); i != e; )
	{
		if (i->getWarehouseId() == warehouseId)
			i = this->warehouses.erase(i);
		else
			++i;
	}
}

void TaskDistributer::print() {
	cout << warehouses.size() << endl;
}

TaskDistributer::~TaskDistributer() {
	// TODO Auto-generated destructor stub
}
