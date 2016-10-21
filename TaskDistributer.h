#pragma once

#ifndef TASKDISTRIBUTER_H_
#define TASKDISTRIBUTER_H_

#include <list>
#include "RobotController.h"
#include "Warehouse.h"
#include "Point.h"
#include <algorithm>
#include <iostream>


using namespace std;

class TaskDistributer {
	list<Warehouse> warehouses;
	list<RobotController> robots;

public:
	TaskDistributer();
	void addWarehouse(Warehouse wh);
	void removeWarehouse(Warehouse wh);
	void removeWarehouse(int warehouseId);
	void print();
	void addRobotController(RobotController robot);
	void removeRobotController(RobotController robot);
	virtual ~TaskDistributer();
};

#endif /* TASKDISTRIBUTER_H_ */

