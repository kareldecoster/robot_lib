#include "PickRobotController.h"

PickRobotController::PickRobotController(Warehouse& wh, Point& location, int containerVolume, int cport_nr, int bdrate, CollectRobotController* collector) : wh(wh), position(location) {
	this->containerVolume = containerVolume;
	readerDone = 0;
	this->cport_nr = cport_nr;
	this->bdrate = bdrate;
	this->collector = collector;

	//Initialize everything before starting the new thread.
	reader = new boost::thread(&PickRobotController::readerLoop, this);
}

PickRobotController::PickRobotController(PickRobotController & orig) : wh(orig.wh), position(orig.position), containerVolume(orig.containerVolume), cport_nr(orig.cport_nr), bdrate(orig.bdrate), readerDone(orig.readerDone), collector(orig.collector)
{
	reader = new boost::thread(&PickRobotController::readerLoop, this);
}

void PickRobotController::readerLoop() {
	char mode[] = { '8','N','1',0 };
	if (RS232_OpenComport(this->cport_nr, this->bdrate, mode))
	{
		printf("Can not open comport %d\n", this->cport_nr);

		return;
	}

	while (!readerDone) {
		Item* curr;
		vector<Item> pickListForThisRun;
		int plannedVolume = 0;
		while (!this->itemsToBePicked.empty()) {
			while (this->itemsToBePicked.pop(curr)) {

				if (curr->getSize() + plannedVolume <= containerVolume) {
					//Add it to the picklist for this run
					pickListForThisRun.push_back(*curr);
					plannedVolume += curr->getSize();
				}
				else {
					//Find the optimal route to pick all the items of this run.
					vector<Item> route = findBestRoute(pickListForThisRun);

					//Collect items and drop them off.
					executeRun(route);

					//Add the item to the list for the next run
					pickListForThisRun.clear();
					plannedVolume = curr->getSize();
					pickListForThisRun.push_back(*curr);
				}
				if (this->itemsToBePicked.empty()) {
					//Find the optimal route to pick all the items of this run.
					vector<Item> route = findBestRoute(pickListForThisRun);

					//Collect items and drop them off.
					executeRun(route);
				}
				delete curr;

			}
		}
	}
	RS232_CloseComport(cport_nr);
}

int PickRobotController::calculateLengthOfRoute(vector<Item>route) {
	int currRouteLength = 0;
	int size = route.size();

	//Add distance from start to first item
	currRouteLength += wh.getPositionOfStorageUnit(route.at(0).getStorageUnit())
		.distanceTo(wh.getCollectionPoint());

	for (int i = 0; i < size; i++) {

		if (i > 0) {
			currRouteLength += wh.getPositionOfStorageUnit(route.at(i).getStorageUnit())
				.distanceTo(wh.getPositionOfStorageUnit(route.at(i - 1).getStorageUnit()));
		}
	}

	//Add distance from last item to drop off
	currRouteLength += wh.getPositionOfStorageUnit(route.at(size - 1).getStorageUnit())
		.distanceTo(wh.getCollectionPoint());
	return currRouteLength;
}

vector<Item> PickRobotController::findBestRoute(vector<Item> route) {
	int bestRouteLength = INT_MAX;
	vector<Item>bestRoute;
	//first sort to get all possible permutations.
	sort(route.begin(), route.end());
	do {
		int currRouteLength = calculateLengthOfRoute(route);

		if (currRouteLength < bestRouteLength) {
			bestRouteLength = currRouteLength;
			bestRoute = route;
		}
	} while (std::next_permutation(route.begin(), route.end()));

	return bestRoute;
}

void PickRobotController::executeRun(vector<Item> route)
{
	vector<Item> inBasket;
	//Pick the items of this route
	while (!route.empty()) {
		moveTo(wh.getPositionOfStorageUnit(route.back().getStorageUnit()));
		sendAndCheck('P');
		inBasket.push_back(route.back());
		route.pop_back();
	}
	//Move to the drop-off zone and drop the items.
	moveTo(wh.getCollectionPoint());
	sendAndCheck('Q');
	while (!inBasket.empty()) {
		collector->addItemReadyToCollect(inBasket.back());
		inBasket.pop_back();
	}
}



void PickRobotController::addItemsToPick(queue<Item> itemsToPick) {
	while (!itemsToPick.empty()) {
		addItemToPick(&(itemsToPick.front()));
		itemsToPick.pop();
	}
}

void PickRobotController::addItemToPick(Item* item)
{
	//Keeps running until the item is added successfully.
	while(!this->itemsToBePicked.push(item));
}

bool PickRobotController::isDone() {
	return this->itemsToBePicked.empty();
}

const Point & PickRobotController::getPosition()
{
	return position;
}

const Warehouse& PickRobotController::getWarehouse() const {
	return wh;
}

const int PickRobotController::getContainerVolume()
{
	return this->containerVolume;
}

PickRobotController::~PickRobotController() {
	readerDone = 1;
	reader->join();
	delete reader;
	reader = NULL;
}

void PickRobotController::moveTo(Point dest) {
	int dx = dest.getX()  - position.getX();
	int dy = dest.getY() - position.getY();
	if (dy >= 0 && dx >= 0) {
		// DOWN_RIGHT + DOWN + RIGHT
		int com = min(abs(dx),abs(dy));
		int diff_x = abs(dx) - com;
		int diff_y = abs(dy) - com;
		move(DOWN_RIGHT, com);
		move(DOWN, diff_y);
		move(RIGHT, diff_x);
	}
	if (dy < 0 && dx >= 0) {
		// UP_RIGHT + UP
		int com = min(abs(dx), abs(dy));
		int diff_x = abs(dx) - com;
		int diff_y = abs(dy) - com;
		move(UP_RIGHT, com);
		move(UP, diff_y);
		move(RIGHT, diff_x);
	}
	if (dy >= 0 && dx < 0) {
		// DOWN_LEFT + LEFT
		int com = min(abs(dx), abs(dy));
		int diff_x = abs(dx) - com;
		int diff_y = abs(dy) - com;
		move(DOWN_LEFT, com);
		move(DOWN, diff_y);
		move(LEFT, diff_x);
	}
	if (dy < 0 && dx < 0) {
		// UP_LEFT
		int com = min(abs(dx), abs(dy));
		int diff_x = abs(dx) - com;
		int diff_y = abs(dy) - com;
		move(UP_LEFT, com);
		move(UP, diff_y);
		move(LEFT, diff_x);
	}
	position = dest;
}

void PickRobotController::move(direction dir, int distance) {
	for (int i = 0; i < distance; i++) {
		switch (dir) {
		case LEFT:
			sendAndCheck(dir);
			break;
		case RIGHT:
			sendAndCheck(dir);
			break;
		case DOWN:
			sendAndCheck(dir);
			break;
		case UP:
			sendAndCheck(dir);
			break;
		case UP_LEFT:
			sendAndCheck(dir);
			break;
		case UP_RIGHT:
			sendAndCheck(dir);
			break;
		case DOWN_LEFT:
			sendAndCheck(dir);
			break;
		default:
			break;
		}
	}
}

bool PickRobotController::sendAndCheck(char toSend)
{
	unsigned char buf;
	RS232_cputs(cport_nr, &toSend);
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	volatile int n = RS232_PollComport(cport_nr, &buf, 1);
	while (n < 1) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		n = RS232_PollComport(cport_nr, &buf, 1);
	}
	if (buf == toSend) {
		return true;
	}
	return false;
}