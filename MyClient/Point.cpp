#include "Point.h"
#include <stdlib.h>
#include <iostream>

/*	Point 		*/
Point::Point(int xx = 0, int yy = 0) : 
X(xx), Y(yy) 
{
}

Point::Point() :
X(0), Y(0)
{
}

int Point::GetX()
{
	return X;
}

int Point::GetY()
{
	return Y;
}

Point Point::operator + (Point A)
{
	return Point(X + A.X, Y + A.Y);
}

Point Point::operator - (Point A)
{
	return Point(X - A.X, Y - A.Y);
}

bool Point::operator == (Point A)
{
	return X == A.X && Y == A.Y;
}

void Point::Swap(Point& A)
{
	std::swap(X, A.X );
	std::swap(Y, A.Y ); 
}

Point Point::Rotate()
{
	Point temp(Y,-X);
	return temp;
}
