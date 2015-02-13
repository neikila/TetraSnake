#include <stdlib.h> 
#include "Field.h"

/* 	Field		*/
Field::Field(int xx = 0, int yy = 0) : 
SizeX(xx), SizeY(yy) 
{
};

Field::Field() :
SizeX(0), SizeY(0)
{
}

int Field::GetX()
{
	return SizeX;
}

int Field::GetY()
{
	return SizeY;
}
