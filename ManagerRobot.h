#pragma once

#ifndef MANAGERROBOT_H_
#define MANAGERROBOT_H_

#include <list>
#include "Warehouse.h"
#include "Point.h"
#include <algorithm>
#include <iostream>


using namespace std;

class ManagerRobot {
	list<Warehouse> warehouses;

public:
	ManagerRobot();
	void addWarehouse(Warehouse wh);
	void removeWarehouse(Warehouse wh);
	void removeWarehouse(int warehouseId);
	void print();
	virtual ~ManagerRobot();
};

#endif /* MANAGERROBOT_H_ */

