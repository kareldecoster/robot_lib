
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
	while (!robots.empty()) {
		RobotController* rc = robots.front();
		delete rc;
		robots.pop_front();
	}
	warehouses.clear();
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

int TaskDistributer::setup(string filePath)
{
	ifstream myfile;
	myfile.open(filePath);
	if (!myfile) {
		return EXIT_FAILURE;
	}

	while (!myfile.eof()) {
		int whId, rows, cols, p_x, p_y, containerVolume, com, bdrate;
		if (!(myfile >> whId >> rows >> cols >> p_x >> p_y >> containerVolume >> com >> bdrate)) {
			return EXIT_FAILURE;
		}
		
		Warehouse warehouse(whId, rows, cols, Point(p_x, p_y));
		if (find(begin(warehouses), end(warehouses), warehouse) == end(warehouses)) {
			RobotController* robot = new RobotController(warehouse, Point(p_x, p_y), containerVolume, com, bdrate);
			addWarehouse(warehouse);
			addRobotController(robot);
		}
	}
	myfile.close();
	return EXIT_SUCCESS;

}

bool TaskDistributer::areAllRobotsDone()
{
	bool done = true;
	for (auto& robot : robots) {
		done &= robot->isDone();
	}
	return done;
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
