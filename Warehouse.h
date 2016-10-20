#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

#include "Point.h"

class Warehouse {
	int warehouseId;
	int rows;
	int cols;
	Point collectionPoint;


public:
	Warehouse(int warehouseId, int rows, int cols, Point& collectionPoint);
	Warehouse(const Warehouse& wh);
	virtual ~Warehouse();
	const Point& getCollectionPoint() const;
	int getCols() const;
	int getRows() const;
	int getWarehouseId() const;
	bool isInBounds(Point pnt);
	bool operator==(const Warehouse& wh);
	Point getPositionOfStorageUnit(int storageUnit);
};

#endif /* WAREHOUSE_H_ */

