#include "RobotController.h"

RobotController::RobotController(Warehouse& wh, Point& location, int containerVolume) : wh(wh), position(location) {
	this->containerVolume = containerVolume;
	readerDone = 0;
	reader = new thread(&RobotController::readerLoop, this);
}

void RobotController::readerLoop() {
	while (!readerDone) {
		while (!articlesToBePicked.empty()) {

			vector<int> pickListForThisRun;
			int plannedVolume = 0;
			//TODO add size to articles..
			while (plannedVolume + 1 <= containerVolume && !articlesToBePicked.empty()) {
				pickListForThisRun.push_back(articlesToBePicked.front());

				articlesToBePicked.pop();
				plannedVolume += 1;
			}
			vector<int> route = findBestRoute(pickListForThisRun);
			
			//Pick the items of this route
			while (!route.empty()) {
				moveTo(wh.getPositionOfStorageUnit(route.back()));
				//pick
				route.pop_back();
			}
			moveTo(wh.getCollectionPoint());
			//TODO drop items
		}
	}
}

vector<int> RobotController:: findBestRoute(vector<int> route) {
	int size = route.size();
	int best_path = INT_MAX;
	vector<int>best_route;
	do {
		int curr_path = 0;

		//Add distance from start to first item
		curr_path += wh.getPositionOfStorageUnit(route.at(0))
			.distanceTo(wh.getCollectionPoint());

		for (int i = 0; i < size; i++) {

			if (i > 0) {
				curr_path += wh.getPositionOfStorageUnit(route.at(i))
					.distanceTo(wh.getPositionOfStorageUnit(route.at(i-1)));
			}
		}

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


void RobotController::addArticlesToBePicked(queue<int> articles) {
	while (!articles.empty()) {
		this->articlesToBePicked.push(articles.front());
		articles.pop();
	}
}

bool RobotController::isDone() {
	return this->articlesToBePicked.empty();
}

const Warehouse& RobotController::getWarehouse() const {
	return wh;
}

RobotController::~RobotController() {
	readerDone = 1;
	reader->join();
	delete reader;
	reader = NULL;
}

void RobotController::moveTo(Point dest) {
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

void RobotController::move(direction dir, int distance) {
	ofstream myfile;
	myfile.open("output.txt", std::ofstream::out | std::ofstream::app);
	switch (dir) {
	case LEFT:
		myfile << distance << "L\n";
		break;
	case RIGHT:
		myfile << distance << "R\n";
		break;
	case DOWN:
		myfile << distance << "D\n";
		break;
	case UP:
		myfile << distance << "U\n";
		break;
	default:
		break;
		myfile.close();
	}
}