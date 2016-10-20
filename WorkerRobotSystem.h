#ifndef WORKERROBOTSYSTEM_H_
#define WORKERROBOTSYSTEM_H_

#include "Warehouse.h"
#include "Point.h"
#include <iostream>
#include <queue>
#include <thread>

using namespace std;

enum direction { UP, DOWN, LEFT, RIGHT };

class WorkerRobotSystem {
public:
	WorkerRobotSystem(Warehouse& wh, Point& location, int containerVolume);
	virtual ~WorkerRobotSystem();
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
	vector<int> WorkerRobotSystem::find_best_route(vector<int> pickingQ);
};

#endif /* WORKERROBOTSYSTEM_H_ */