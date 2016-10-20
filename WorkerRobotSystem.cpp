#include "WorkerRobotSystem.h"

WorkerRobotSystem::WorkerRobotSystem(Warehouse& wh, Point& location, int containerVolume) : wh(wh), position(location) {
	this->containerVolume = containerVolume;
	done = 0;
	robot_thread = new thread(&WorkerRobotSystem::loop, this);
}

void WorkerRobotSystem::loop() {
	while (!done) {
		while (!articlesToBePicked.empty()) {

			//TODO make sublist of items to pick this round
			vector<int> picking_list;
			int plannedVolume = 0;
			//TODO add size to articles..
			while (plannedVolume + 1 <= containerVolume && !articlesToBePicked.empty()) {
				picking_list.push_back(articlesToBePicked.front());
				//cout << "planning : " << articlesToBePicked.front() << endl;
				articlesToBePicked.pop();
				plannedVolume += 1;
			}
			//TODO optimize route
			vector<int> route = find_best_route(picking_list);
			
			//Pick the items of this route
			while (!route.empty()) {
				moveTo(wh.getPositionOfStorageUnit(route.back()));
				//pick
				//cout << "moving :"<< route.back() << endl;
				route.pop_back();
			}
			moveTo(wh.getCollectionPoint());
			cout << "Dropping..\n";
			//TODO drop items
		}
	}
}

vector<int> WorkerRobotSystem:: find_best_route(vector<int> route) {
	int size = route.size();
	int best_path = INT_MAX;
	vector<int>best_route;
	do {
		int curr_path = 0;
		for (int i = 0; i < size; i++) {

			if (i > 0) {
				curr_path += wh.getPositionOfStorageUnit(route.at(i))
					.distanceTo(wh.getPositionOfStorageUnit(route.at(i-1)));
			}
		}
		//Add distance from start to first item
		curr_path += wh.getPositionOfStorageUnit(route.at(0))
			.distanceTo(wh.getCollectionPoint());
		//Add distance from last item to drop off
		curr_path += wh.getPositionOfStorageUnit(route.at(size-1))
			.distanceTo(wh.getCollectionPoint());

		if (curr_path < best_path) {
			best_path = curr_path;
			best_route = route;
		}
	} while (std::next_permutation(route.begin(), route.end()));
		
	return best_route;
}


void WorkerRobotSystem::addArticlesToBePicked(queue<int> articles) {
	while (!articles.empty()) {
		this->articlesToBePicked.push(articles.front());
		articles.pop();
	}
}

bool WorkerRobotSystem::isDone() {
	return this->articlesToBePicked.empty();
}

const Warehouse& WorkerRobotSystem::getWarehouse() const {
	return wh;
}

WorkerRobotSystem::~WorkerRobotSystem() {
	done = 1;
	robot_thread->join();
	delete robot_thread;
	robot_thread = NULL;
}

void WorkerRobotSystem::moveTo(Point dest) {
	if (position.getX() > dest.getX()) {
		move(LEFT, position.getX() - dest.getX());
	}
	if (position.getX() < dest.getX()) {
		move(RIGHT, dest.getX() - position.getX());
	}
	if (position.getY() > dest.getY()) {
		move(UP, position.getY() - dest.getY());
	}
	if (position.getY() < dest.getY()) {
		move(DOWN, dest.getY() - position.getY());
	}
	position = dest;
}

void WorkerRobotSystem::move(direction dir, int distance) {
	switch (dir) {
	case LEFT:
		cout << distance << "L\n";
		break;
	case RIGHT:
		cout << distance << "R\n";
		break;
	case DOWN:
		cout << distance << "D\n";
		break;
	case UP:
		cout << distance << "U\n";
		break;
	default:
		break;
	}
}