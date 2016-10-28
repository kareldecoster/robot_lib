
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

Warehouse TaskDistributer::getWarehouse(int id) {
	for (std::list<Warehouse>::iterator i = this->warehouses.begin(), e = this->warehouses.end(); i != e; )
	{
		if (i->getWarehouseId() == id)
			return *i;
		else
			++i;
	}
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
		if (getRobotByWarehouse(&(getWarehouse(tmp->getWarehouseID()))) != nullptr) {
			getRobotByWarehouse(&getWarehouse(tmp->getWarehouseID()))->addItemToPick(tmp);
		}
		else {
			//TODO: write log
		}
		orders.pop();
	}
}

void TaskDistributer::setup(string filePath)
{
	ifstream myfile;
	myfile.open(filePath);
	while (!myfile.eof()) {
		int whId, rows, cols, p_x, p_y, containerVolume, com, bdrate;
		myfile >> whId;
		myfile >> rows;
		myfile >> cols;
		myfile >> p_x;
		myfile >> p_y;
		myfile >> containerVolume;
		myfile >> com;
		myfile >> bdrate;
		
		Warehouse warehouse(whId, rows, cols, Point(p_x, p_y));
		RobotController* robot = new RobotController(warehouse, Point(p_x, p_y), containerVolume, com, bdrate);
		addWarehouse(warehouse);
		addRobotController(robot);
	}
	myfile.close();

}

bool TaskDistributer::areAllRobotsDone()
{
	for (auto& robot : robots) {
		if (!robot->isDone()) {
			return false;
		}
	}
	return true;
}

RobotController * TaskDistributer::getRobotByWarehouse(const Warehouse* wh)
{
	for (std::list<RobotController*>::iterator i = this->robots.begin(), e = this->robots.end(); i != e; )
	{
		if ((*i)->getWarehouse().getWarehouseId() == wh->getWarehouseId())
			return &**i;
		else
			++i;
	}
	return nullptr;
}
