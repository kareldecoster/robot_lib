#ifndef ROBOTCONTROLLER_H_
#define ROBOTCONTROLLER_H_

#include "Warehouse.h"
#include "Point.h"
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
	void addArticlesToBePicked(queue<int> articles);
	bool isDone(void);
	const Warehouse& getWarehouse() const;
private:
	Warehouse wh;
	queue<int> articlesToBePicked;
	Point position;
	int containerVolume;
	thread* robot_thread;
	volatile int done;

	void loop();
	void moveTo(Point dest);
	void move(direction dir, int distance);
	vector<int> RobotController::find_best_route(vector<int> pickingQ);
};

#endif /* ROBOTCONTROLLER_H_ */