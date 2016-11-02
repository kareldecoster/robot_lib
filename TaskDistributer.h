#pragma once

#ifndef TASKDISTRIBUTER_H_
#define TASKDISTRIBUTER_H_

#include <list>
#include <queue>
#include "PickRobotController.h"
#include "CollectRobotController.h"
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
	void addItemReadyToCollect(Item item);

	// Make private..?
	void addPickRobotController(PickRobotController* robot);
	void removePickRobotController(PickRobotController& robot);
	void addWarehouse(Warehouse wh);
	Warehouse* getWarehouse(int id);
	void removeWarehouse(Warehouse wh);
	void removeWarehouse(int warehouseId);
	void setCollectRobotController(CollectRobotController* collect);
private:
	list<Warehouse> warehouses;
	list<PickRobotController*> pickers;
	CollectRobotController* collector;

	PickRobotController* getRobotByWarehouse(const Warehouse* wh);
};

#endif /* TASKDISTRIBUTER_H_ */

