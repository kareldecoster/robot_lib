#pragma once

#ifndef TASKDISTRIBUTER_H_
#define TASKDISTRIBUTER_H_

#include <list>
#include <queue>
#include "RobotController.h"
#include "Warehouse.h"
#include "Point.h"
#include "Item.h"
#include <algorithm>
#include <iostream>
#include <fstream>


using namespace std;

class TaskDistributer {
public:
	TaskDistributer();
	void print();
	int setup(string filePath);
	bool areAllRobotsDone();
	void giveOrdersToRobotControllers(queue<Item*> orders);
	virtual ~TaskDistributer();

	// Make private..?
	void addRobotController(RobotController* robot);
	void removeRobotController(RobotController& robot);
	void addWarehouse(Warehouse wh);
	Warehouse getWarehouse(int id);
	void removeWarehouse(Warehouse wh);
	void removeWarehouse(int warehouseId);
private:
	list<Warehouse> warehouses;
	list<RobotController*> robots;

	RobotController* getRobotByWarehouse(const Warehouse* wh);
};

#endif /* TASKDISTRIBUTER_H_ */

