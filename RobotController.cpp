#include "RobotController.h"

RobotController::RobotController(Warehouse& wh, Point& location, int containerVolume, int cport_nr, int bdrate) : wh(wh), position(location) {
	this->containerVolume = containerVolume;
	readerDone = 0;
	this->cport_nr = cport_nr;
	this->bdrate = bdrate;
	//Initialize everything before starting the new thread.
	reader = new thread(&RobotController::readerLoop, this);
}

void RobotController::readerLoop() {
	char mode[] = { '8','N','1',0 };

	if (RS232_OpenComport(this->cport_nr, this->bdrate, mode))
	{
		printf("Can not open comport\n");

		return;
	}

	while (!readerDone) {
		while (!itemsToBePicked.empty()) {

			vector<Item> pickListForThisRun;
			int plannedVolume = 0;
			
			//plan which items you can pick this run, taking the size into consideration.
			while (!itemsToBePicked.empty() && plannedVolume + itemsToBePicked.front().getSize() <= containerVolume) {
				pickListForThisRun.push_back(itemsToBePicked.front());
				plannedVolume += itemsToBePicked.front().getSize();
				itemsToBePicked.pop();
			}

			//Find the optimal route to pick all the items of this run.
			vector<Item> route = findBestRoute(pickListForThisRun);
			
			//Pick the items of this route
			while (!route.empty()) {
				moveTo(wh.getPositionOfStorageUnit(route.back().getStorageUnit()));
				//pick
				send_check('P');
				route.pop_back();
			}

			//Move to the drop-off zone and drop the items.
			moveTo(wh.getCollectionPoint());
			//drop
			send_check('Q');
		}
	}
	RS232_CloseComport(cport_nr);
}

vector<int> RobotController:: findBestRoute(vector<int> route) {
	int size = route.size();
	int best_path = INT_MAX;
	vector<int>best_route;
	sort(route.begin(), route.end());	//first sort to get all possible permutations.
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

vector<Item> RobotController::findBestRoute(vector<Item> route) {
	int size = route.size();
	int best_path = INT_MAX;
	vector<Item>best_route;
	sort(route.begin(), route.end());	//first sort to get all possible permutations.
	do {
		int curr_path = 0;

		//Add distance from start to first item
		curr_path += wh.getPositionOfStorageUnit(route.at(0).getStorageUnit())
			.distanceTo(wh.getCollectionPoint());

		for (int i = 0; i < size; i++) {

			if (i > 0) {
				curr_path += wh.getPositionOfStorageUnit(route.at(i).getStorageUnit())
					.distanceTo(wh.getPositionOfStorageUnit(route.at(i - 1).getStorageUnit()));
			}
		}

		//Add distance from last item to drop off
		curr_path += wh.getPositionOfStorageUnit(route.at(size - 1).getStorageUnit())
			.distanceTo(wh.getCollectionPoint());

		if (curr_path < best_path) {
			best_path = curr_path;
			best_route = route;
		}
	} while (std::next_permutation(route.begin(), route.end()));

	return best_route;
}



void RobotController::addItemsToPick(queue<Item> itemsToPick) {
	while (!itemsToPick.empty()) {
		addItemToPick(itemsToPick.front());
		itemsToPick.pop();
	}
}

void RobotController::addItemToPick(Item item)
{
	this->itemsToBePicked.push(item);
}

bool RobotController::isDone() {
	return this->itemsToBePicked.empty();
}

const Point & RobotController::getPosition()
{
	return position;
}

const Warehouse& RobotController::getWarehouse() const {
	return wh;
}

const int RobotController::getContainerVolume()
{
	return this->containerVolume;
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
	for (int i = 0; i < distance; i++) {
		switch (dir) {
		case LEFT:
			send_check('L');
			break;
		case RIGHT:
			send_check('R');
			break;
		case DOWN:
			send_check('D');
			break;
		case UP:
			send_check('U');
			break;
		default:
			break;
		}
	}
}

bool RobotController::send_check(char toSend)
{
	unsigned char buf;
	RS232_cputs(cport_nr, &toSend);
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	volatile int n = RS232_PollComport(cport_nr, &buf, 1);
	while (n < 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		n = RS232_PollComport(cport_nr, &buf, 1);
	}
	if (buf == toSend) {
		return true;
	}
	//TODO: log error
	return false;
}