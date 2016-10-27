
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

void TaskDistributer::addRobotController(RobotController* robot)
{
	this->robots.push_back(robot);
}

void TaskDistributer::removeRobotController(RobotController& robot)
{	
	for (std::list<RobotController*>::iterator i = this->robots.begin(), e = this->robots.end(); i != e; )
	{
		if ((*i)->getWarehouse().getWarehouseId() == robot.getWarehouse().getWarehouseId())
			i = this->robots.erase(i);
		else
			++i;
	}
}

TaskDistributer::~TaskDistributer() {
	// TODO Auto-generated destructor stub
}

void TaskDistributer::giveOrdersToRobotControllers(queue<Item*> orders)
{
	while (!orders.empty()) {
		Item* tmp = orders.front();
		if (getRobotByWarehouse(*(tmp->getWarehouse())) != nullptr) {
			getRobotByWarehouse(*(tmp->getWarehouse()))->addItemToPick(tmp);
		}
		else {
			//TODO: write log
		}
		orders.pop();
	}
}

RobotController * TaskDistributer::getRobotByWarehouse(Warehouse wh)
{
	for (std::list<RobotController*>::iterator i = this->robots.begin(), e = this->robots.end(); i != e; )
	{
		if ((*i)->getWarehouse().getWarehouseId() == wh.getWarehouseId())
			return &**i;
		else
			++i;
	}
	return nullptr;
}
