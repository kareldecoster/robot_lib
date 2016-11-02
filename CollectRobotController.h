#pragma once
#include "Item.h"
#include <vector>
#include <boost/thread/mutex.hpp>
#include<boost/atomic.hpp>
#include<boost/thread/thread.hpp>

class CollectRobotController
{
public:
	CollectRobotController();
	~CollectRobotController();
	void addItemToCollect(Item item);
	void addItemReadyToCollect(Item item);
	int isDone();

private:
	boost::thread* reader;
	vector <Item> itemsToCollect;
	boost::mutex mutexForToCollect;
	boost::mutex mutexForReady;
	vector <Item> itemsReadyToCollect;
	volatile boost::atomic_int readerDone;

	void readerLoop();
	vector<Item> getItemsToCollectByOrder(int order);
	bool areAllItemsReady(vector<Item> order);
	void removeOrderFromVectors(vector<Item> order);
	vector<int> getOrderIDsForCustomer(int customerID);
	bool areAllOrdersReady(int customerID);

};

