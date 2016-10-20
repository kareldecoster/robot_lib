#include "Warehouse.h"

Warehouse::Warehouse(int warehouseId, int rows, int cols, Point& collectionPoint) :
	collectionPoint(collectionPoint)
{
	this->warehouseId = warehouseId;
	this->rows = rows;
	this->cols = cols;
}

Warehouse::Warehouse(const Warehouse& wh) :
	warehouseId(wh.getWarehouseId()),
	rows(wh.getRows()),
	cols(wh.getCols()),
	collectionPoint(wh.getCollectionPoint())
{

}


Warehouse::~Warehouse() {
	// TODO Auto-generated destructor stub
}

const Point& Warehouse::getCollectionPoint() const {
	return collectionPoint;
}

int Warehouse::getCols() const {
	return cols;
}

int Warehouse::getRows() const {
	return rows;
}

int Warehouse::getWarehouseId() const {
	return warehouseId;
}

bool Warehouse::isInBounds(Point pnt) {
	if (pnt.getX()<0 || pnt.getX()>(getCols() - 1) || pnt.getY()<0 || pnt.getY()>(getRows() - 1)) {
		return false;
	}
	return true;
}

bool Warehouse::operator ==(const Warehouse& wh) {
	return wh.getWarehouseId() == this->getWarehouseId();
}

Point Warehouse::getPositionOfStorageUnit(int storageUnit) {
	return Point((storageUnit-1) / rows, (storageUnit-1)%rows);
}