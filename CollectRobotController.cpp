#include "CollectRobotController.h"



CollectRobotController::CollectRobotController()
{
	readerDone = 0;
	reader = new boost::thread(&CollectRobotController::readerLoop, this);
}


vector<Item> CollectRobotController::getItemsToCollectByOrder(int order) {
	vector<Item> returnvec;
	for (auto& item : itemsToCollect) {
		if (item.getOrderID() == order) {
			returnvec.push_back(item);
		}
	}
	return returnvec;
}

CollectRobotController::~CollectRobotController()
{
	/*Item tmp;
	cout << "Need to collect the following items..\n";
	mutexForToCollect.lock();
	while (!itemsToCollect.empty()) {
		tmp = itemsToCollect.back();
		cout << tmp.getStorageUnit() << '\t' << tmp.getCustomerID() << '\t' << tmp.getOrderID() << endl;
		itemsToCollect.pop_back();
	}
	mutexForToCollect.unlock();
	cout << "Ready to collect the following items..\n";
	mutexForReady.lock();
	while (!itemsReadyToCollect.empty()) {
		tmp = itemsReadyToCollect.back();
		cout << tmp.getStorageUnit() << '\t' << tmp.getCustomerID() << '\t' << tmp.getOrderID() << endl;
		itemsReadyToCollect.pop_back();
	}
	mutexForReady.unlock();*/
	readerDone = 1;
	reader->join();
}

void CollectRobotController::addItemToCollect(Item item)
{
	mutexForToCollect.lock();
	this->itemsToCollect.push_back(item);
	mutexForToCollect.unlock();
}

void CollectRobotController::addItemReadyToCollect(Item item)
{
	mutexForReady.lock();
	this->itemsReadyToCollect.push_back(item);
	mutexForReady.unlock();
}

int CollectRobotController::isDone()
{
	return itemsReadyToCollect.empty() && itemsToCollect.empty();
}

bool CollectRobotController::areAllItemsReady(vector<Item> order) {
	bool retval = true;
	for (auto& item : order) {
		retval &= (find(itemsReadyToCollect.begin(), itemsReadyToCollect.end(), item)!=itemsReadyToCollect.end());
	}
	return retval;
}

bool CollectRobotController::areAllOrdersReady(int customerID) {
	vector<int> ids = getOrderIDsForCustomer(customerID);
	bool retval = true;
	for (auto& id : ids) {
		retval &= areAllItemsReady(getItemsToCollectByOrder(id));
	}
	return retval;
}


void CollectRobotController::removeOrderFromVectors(vector<Item> order) {
	//remove from lists
	for (auto& item : order) {
		itemsToCollect.erase(std::remove(itemsToCollect.begin(), itemsToCollect.end(), item), itemsToCollect.end());
		itemsReadyToCollect.erase(std::remove(itemsReadyToCollect.begin(), itemsReadyToCollect.end(), item), itemsReadyToCollect.end());
	}
}

vector<int> CollectRobotController::getOrderIDsForCustomer(int customerID) {
	vector<int> returnvec;
	for (auto& item : itemsToCollect) {
		if (item.getCustomerID() == customerID) {
			if (find(returnvec.begin(), returnvec.end(), item.getOrderID()) == returnvec.end()) {
				returnvec.push_back(item.getOrderID());
			}
		}
	}
	return returnvec;
}



void CollectRobotController::readerLoop() {
	while (!readerDone) {
		int index = 0;
		while (!itemsReadyToCollect.empty()) {
			mutexForToCollect.lock();
			mutexForReady.lock();
			if (areAllOrdersReady(itemsReadyToCollect.at(index).getCustomerID())) {
				int custID = itemsReadyToCollect.at(index).getCustomerID();
				cout << "Customer : " << custID << endl;
				for (auto& tmp : getOrderIDsForCustomer(custID)) {
					cout << "   Order : " << tmp << endl;
					//Collect order..
					for (auto& item : getItemsToCollectByOrder(tmp)) {
						item.print();
					}
					removeOrderFromVectors(getItemsToCollectByOrder(tmp));
				}
				index = 0;
			}
			else {
				index++;
				index %= itemsReadyToCollect.size();
			}
			mutexForReady.unlock();
			mutexForToCollect.unlock();
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
}