#ifndef ROBOTCONTROLLER_H_
#define ROBOTCONTROLLER_H_

#include "Warehouse.h"
#include "Point.h"
#include "Item.h"
#include <iostream>
#include <queue>

#include<boost/lockfree/queue.hpp>
#include<boost/atomic.hpp>
#include<boost/thread/thread.hpp>

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

using namespace std;

enum direction { UP, DOWN, LEFT, RIGHT };

class RobotController {
public:
	RobotController(Warehouse& wh, Point& location, int containerVolume, int cport_nr, int bdrate);
	virtual ~RobotController();
	void addItemsToPick(queue<Item> itemsToPick);
	void addItemToPick(Item* item);
	bool isDone(void);
	const Point& getPosition();
	const Warehouse& getWarehouse() const;
	const int getContainerVolume();
private:
	Warehouse wh;
	boost::lockfree::queue<Item*> itemsToBePicked{ 1024 };
	Point position;
	int containerVolume;
	boost::thread* reader;
	volatile boost::atomic_int readerDone;
	int cport_nr, bdrate;


	void readerLoop();
	void moveTo(Point dest);
	void move(direction dir, int distance);
	int calculateLengthOfRoute(vector<Item>route);
	vector<Item> findBestRoute(vector<Item> subOptimalRouteOfStorageUnits);
	void executeRun(vector<Item> route);
	bool sendAndCheck(char toSend);

};

#endif /* ROBOTCONTROLLER_H_ */