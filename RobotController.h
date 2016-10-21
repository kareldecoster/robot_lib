#ifndef ROBOTCONTROLLER_H_
#define ROBOTCONTROLLER_H_

#include "Warehouse.h"
#include "Point.h"
#include "Item.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <thread>

using namespace std;

enum direction { UP, DOWN, LEFT, RIGHT };

class RobotController {
public:
	RobotController(Warehouse& wh, Point& location, int containerVolume);
	virtual ~RobotController();
	void addItemsToBePicked(queue<Item> itemsToPick);
	void addItemToPick(Item item);
	bool isDone(void);
	const Warehouse& getWarehouse() const;
private:
	Warehouse wh;
	queue<int> articlesToBePicked;
	Point position;
	int containerVolume;
	thread* reader;
	volatile int readerDone;

	void readerLoop();
	void moveTo(Point dest);
	void move(direction dir, int distance);
	vector<int> findBestRoute(vector<int> subOptimalRouteOfStorageUnits);
};

#endif /* ROBOTCONTROLLER_H_ */