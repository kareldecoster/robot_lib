
#ifndef POINT_H_
#define POINT_H_

#include <cstdlib>

class Point {
	int x, y;
public:
	Point(int x, int y);
	Point(const Point& orig);
	virtual ~Point();
	virtual int distanceTo(Point dest);
	
	int getX() const {
		return x;
	}

	void setX(int x) {
		this->x = x;
	}

	int getY() const {
		return y;
	}

	void setY(int y) {
		this->y = y;
	}
};

#endif /* POINT_H_ */

