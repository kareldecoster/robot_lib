
#include "TaskDistributer.h"

TaskDistributer::TaskDistributer() {
	// TODO Auto-generated constructor stub
	collector = nullptr;
}

void TaskDistributer::addWarehouse(Warehouse wh) {
	this->warehouses.push_back(wh);
}

void TaskDistributer::removeWarehouse(Warehouse wh) {
	this->warehouses.remove(wh);
}

Warehouse* TaskDistributer::getWarehouse(int id) {
	for (std::list<Warehouse>::iterator i = this->warehouses.begin(), e = this->warehouses.end(); i != e; )
	{
		if (i->getWarehouseId() == id)
			return &(*i);
		else
			++i;
	}
	return nullptr;
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

void TaskDistributer::setCollectRobotController(CollectRobotController * collect)
{
	this->collector = collect;
}

void TaskDistributer::print() {
	cout << warehouses.size() << endl;
}

void TaskDistributer::addPickRobotController(PickRobotController* robot)
{
	this->pickers.push_back(robot);
}

void TaskDistributer::removePickRobotController(PickRobotController& robot)
{	
	for (std::list<PickRobotController*>::iterator i = this->pickers.begin(), e = this->pickers.end(); i != e; )
	{
		if ((*i)->getWarehouse().getWarehouseId() == robot.getWarehouse().getWarehouseId())
			i = this->pickers.erase(i);
		else
			++i;
	}
}

TaskDistributer::~TaskDistributer() {
	while (!pickers.empty()) {
		PickRobotController* rc = pickers.front();
		delete rc;
		pickers.pop_front();
	}
	warehouses.clear();
	delete collector;
}

void TaskDistributer::giveOrdersToRobotControllers(queue<Item*> orders)
{
	while (!orders.empty()) {
		Item* tmp = orders.front();
		collector->addItemToCollect(*tmp);
		if (getRobotByWarehouse((getWarehouse(tmp->getWarehouseID()))) != nullptr) {
			getRobotByWarehouse(getWarehouse(tmp->getWarehouseID()))->addItemToPick(tmp);
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
	collector = new CollectRobotController();

	while (!myfile.eof()) {
		int whId, rows, cols, p_x, p_y, containerVolume, com, bdrate;
		if (!(myfile >> whId >> rows >> cols >> p_x >> p_y >> containerVolume >> com >> bdrate)) {
			return EXIT_FAILURE;
		}
		
		Warehouse warehouse(whId, rows, cols, Point(p_x, p_y));
		if (find(begin(warehouses), end(warehouses), warehouse) == end(warehouses)) {
			PickRobotController* robot = new PickRobotController(warehouse, Point(p_x, p_y), containerVolume, com, bdrate, this);
			addWarehouse(warehouse);
			addPickRobotController(robot);
		}
	}
	myfile.close();
	return EXIT_SUCCESS;

}

bool TaskDistributer::areAllRobotsDone()
{
	bool done = true;
	for (auto& robot : pickers) {
		done &= robot->isDone();
	}
	done &= collector->isDone();
	return done;
}

PickRobotController * TaskDistributer::getRobotByWarehouse(const Warehouse* wh)
{
	for (std::list<PickRobotController*>::iterator i = this->pickers.begin(), e = this->pickers.end(); i != e; )
	{
		if ((*i)->getWarehouse().getWarehouseId() == wh->getWarehouseId())
			return &**i;
		else
			++i;
	}
	return nullptr;
}

void TaskDistributer::addItemReadyToCollect(Item item) {
	this->collector->addItemReadyToCollect(item);
}