#ifndef FIELD_H
#define FIELD_H
class Field
{
private:
	Field(int, int);
	Field();
	int GetX();
	int GetY();
public:
	int SizeX;		//0,0 - координаты левого нижнего угла
	int SizeY;
};

#endif
