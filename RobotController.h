#ifndef ROBOTCONTROLLER_H_
#define ROBOTCONTROLLER_H_

#include "Warehouse.h"
#include "Point.h"
#include "Item.h"
#include <iostream>
#include <queue>
#include <thread>

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
	void addItemToPick(Item item);
	bool isDone(void);
	const Point& getPosition();
	const Warehouse& getWarehouse() const;
	const int getContainerVolume();
private:
	Warehouse wh;
	queue<Item> itemsToBePicked;
	Point position;
	int containerVolume;
	thread* reader;
	volatile int readerDone;
	int cport_nr, bdrate;


	void readerLoop();
	void moveTo(Point dest);
	void move(direction dir, int distance);
	vector<int> findBestRoute(vector<int> subOptimalRouteOfStorageUnits);
	vector<Item> findBestRoute(vector<Item> subOptimalRouteOfStorageUnits);
	bool send_check(char toSend);

};

#endif /* ROBOTCONTROLLER_H_ */