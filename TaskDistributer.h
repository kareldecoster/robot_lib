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


using namespace std;

class TaskDistributer {
public:
	TaskDistributer();
	void addWarehouse(Warehouse wh);
	void removeWarehouse(Warehouse wh);
	void removeWarehouse(int warehouseId);
	void print();
	void addRobotController(RobotController robot);
	void removeRobotController(RobotController robot);
	void giveOrdersToRobotControllers(queue<Item> orders);
	virtual ~TaskDistributer();
private:
	list<Warehouse> warehouses;
	list<RobotController> robots;

	RobotController* getRobotByWarehouse(Warehouse wh);
};

#endif /* TASKDISTRIBUTER_H_ */

