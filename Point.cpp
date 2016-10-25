
#include "Point.h"

Point::Point(int x, int y) {
	// TODO Auto-generated constructor stub
	this->x = x;
	this->y = y;
}

Point::Point(const Point& orig) : x(orig.getX()), y(orig.getY()) {
}

Point::~Point() {
	// TODO Auto-generated destructor stub
}

int Point::distanceTo(Point dest)
{
	return (max(abs(this->getX() - dest.getX()),abs(this->getY() - dest.getY()))) ;
}