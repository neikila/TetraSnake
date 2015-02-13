#ifndef POINT_H
#define POINT_H

class Point
{
public:
	Point(int, int);
	Point();
	int GetX();
	int GetY();
	Point operator + (Point);
	Point operator - (Point);
	bool operator == (Point);
	void Swap(Point&);
	Point Rotate();
private:
	int X;			//Координаты задаются относительно поля
	int Y;
};

#endif
