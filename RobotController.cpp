#include "RobotController.h"

RobotController::RobotController(Warehouse& wh, Point& location, int containerVolume, int cport_nr, int bdrate) : wh(wh), position(location) {
	this->containerVolume = containerVolume;
	readerDone = 0;
	this->cport_nr = cport_nr;
	this->bdrate = bdrate;

	//Initialize everything before starting the new thread.
	reader = new boost::thread(&RobotController::readerLoop, this);
}

RobotController::RobotController(RobotController & orig) : wh(orig.wh), position(orig.position), containerVolume(orig.containerVolume), cport_nr(orig.cport_nr), bdrate(orig.bdrate), readerDone(orig.readerDone)
{
	reader = new boost::thread(&RobotController::readerLoop, this);
}

void RobotController::readerLoop() {
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
		while (this->itemsToBePicked.pop(curr)) {

			if (curr->getSize() + plannedVolume <= containerVolume) {
				//Add it to the picklist for this run
				pickListForThisRun.push_back(*curr);
				plannedVolume += curr->getSize();
				delete curr;
			}
			else{
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

		}
	}
	RS232_CloseComport(cport_nr);
}

int RobotController::calculateLengthOfRoute(vector<Item>route) {
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

vector<Item> RobotController::findBestRoute(vector<Item> route) {
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

void RobotController::executeRun(vector<Item> route)
{
	//Pick the items of this route
	while (!route.empty()) {
		moveTo(wh.getPositionOfStorageUnit(route.back().getStorageUnit()));
		sendAndCheck('P');
		route.pop_back();
	}

	//Move to the drop-off zone and drop the items.
	moveTo(wh.getCollectionPoint());
	sendAndCheck('Q');
}



void RobotController::addItemsToPick(queue<Item> itemsToPick) {
	while (!itemsToPick.empty()) {
		addItemToPick(&(itemsToPick.front()));
		itemsToPick.pop();
	}
}

void RobotController::addItemToPick(Item* item)
{
	//Keeps running until the item is added successfully.
	while(!this->itemsToBePicked.push(item));
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
			sendAndCheck('L');
			break;
		case RIGHT:
			sendAndCheck('R');
			break;
		case DOWN:
			sendAndCheck('D');
			break;
		case UP:
			sendAndCheck('U');
			break;
		default:
			break;
		}
	}
}

bool RobotController::sendAndCheck(char toSend)
{
	unsigned char buf;
	RS232_cputs(cport_nr, &toSend);
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	volatile int n = RS232_PollComport(cport_nr, &buf, 1);
	while (n < 1) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		n = RS232_PollComport(cport_nr, &buf, 1);
	}
	if (buf == toSend) {
		return true;
	}
	//TODO: log error
	return false;
}