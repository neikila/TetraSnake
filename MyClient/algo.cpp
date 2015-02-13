#include "algo.h"
#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"
#include <stdlib.h>
#include "NonClassFunction.h"

#define SIZEX 10
#define	SIZEY 25

#define UP 'u'
#define DOWN 'd'
#define RIGHT 'r'
#define LEFT 'l'

#define DEBUG false

size_t *Figure::NumberOfBox;
size_t *Figure::NumberOfForms;
size_t Figure::SizeOf_NumberOfForms;
Point**** Figure::mas;		

/*	Snake		*/
Snake::Snake() :
Direction(2), Size(3), MaxSize(7), Head(0)
{
	if (Size > 4)
		Size = 4;
	Mas = new Point[MaxSize];
    for (int i = 0; i < (int)Size; ++i)
	{
		Mas[i] = Point(5 + i,5);
	}
}

Snake::Snake(size_t SSize = 2, size_t MMaxSize = 7) :
Direction(2), Size(SSize), MaxSize(MMaxSize), Head(0)
{
	if (Size > 4)
		Size = 4;
	Mas = new Point[MMaxSize];
    for (size_t i = 0; i < Size; ++i)
	{
		Mas[i] = Point(5 + i,5);
	}
}

bool Snake::Add()
{
    static bool State = true;
    if (Size != MaxSize)
    {
        Point* Temp = new Point[MaxSize];
        if (Temp == NULL)
        {
        std::cout << "Prbolem with allocation memmory for Snake in Grow\n";
        State = false;
        }
        else
        {
        for (size_t i = 0; i < Size; ++i)
        {
            Temp[i] = Mas[ (Head + i) % Size ];
        }
        Head = 0;
        delete[] Mas;
        Mas = Temp;
        State = true;
        ++Size;
    }
    }
    else
    {
     State = false;
    }
        return State;
}
/*
bool Snake::Grow()
{
        static bool State = true;
        Point* Temp = new Point[MaxSize * 2];
        if (Temp == NULL)
        {
        std::cout << "Prbolem with allocation memmory for Snake in Grow\n";
        State = false;
        }
        else
        {
        for (int i = 0; i < Size; ++i)
        {
            Temp[i] = Mas[ (Head + i) % Size ];
        }
        Head = 0;
        MaxSize *= 2;
        delete[] Mas;
        Mas = Temp;
        State = true;
    }
        return State;
}*/

int Snake::GetDirection()
{
	return Direction;
}

size_t Snake::GetSize()
{
	return Size;
}

Point Snake::operator[] (int Index)
{
	return Mas[ (Index + Head) % Size ];
}

bool Snake::Move(Heap& Hp, Figure &Fig)
{
	size_t DeltaX;
	size_t DeltaY;
	switch (Direction)
	{
		case 0: DeltaX = 1; DeltaY = 0; break;
		case 1: DeltaX = 0; DeltaY = 1; break;
		case 2: DeltaX = -1; DeltaY = 0; break;
		case 3: DeltaX = 0; DeltaY = -1; break;
	}
	size_t Second = Head;
    Point temp = Mas[Second] + Point(DeltaX, DeltaY);
        if(Fig.IsPossibleToEatFigure(temp))
        {
            Add();
        }                   //Добавить поедание (рост)
    Head = (Head == 0? Size: Head) - 1;
	if (DEBUG)
	{
		std::cout << "Head = " << Head << std::endl;
		std::cout << "Direction = " << Direction << std::endl;
	}
	Mas[Head] = temp;     //перенсти после проверки
	if (DEBUG)
        for (size_t i = 0; i < Size; ++i)
		{
			std::cout << i << "| x = " << Mas[i].GetX() << "| y = " << Mas[i].GetY() << std::endl;
		}
    return true;
	if (Hp.IsFree(temp))
	{
		Mas[Head] = temp;
	}
	else 
	{
		return false;
	}
	return true;
}

void Snake::Turn(char c)
{
	switch (c)
	{
		case RIGHT: Direction = (Direction == 2 ? 2 : 0); break;
		case UP: Direction = (Direction == 3 ? 3 : 1); break;
		case LEFT: Direction = (Direction == 0 ? 0 : 2); break;
		case DOWN: Direction = (Direction == 1 ? 1 : 3); break;
	}
}

bool Snake::IsFree(Point &Destination)
{
	bool Result = true;
	for ( size_t i = 0; i < Size && Result; ++i )
	{
		if ( Destination == Mas[i])
		{
			Result = false;
		}
	}
	return Result;
}

/*	Figure 		*/
Figure::Figure(size_t TType, size_t X, size_t Y) :
Type(TType), Position(0), ZeroPoint(X, Y)
{
    IsActive = new bool [16];
    Reactivate();
}

Figure::Figure() :
Type(0), Position(0), ZeroPoint(5, 15)
{
        IsActive = new bool [16];
        Reactivate();
}

void Figure::Reactivate()
{
        for (int i = 0; i < 16; ++i )
        {
            IsActive[i] = true;
        }
}

bool Figure::Turn()
{
    Position = (Position + 1) % (NumberOfForms[Type]);
    return true;
}

Point Figure::operator[] (int Index)
{
	return ZeroPoint + *(mas[Type][Position][Index]);
}

size_t Figure::GetSize()
{
	return NumberOfBox[Type];
}

bool Figure::Fullmas(const char* Filename)
{
	TiXmlDocument *xml_file = new TiXmlDocument(Filename);
	if(!xml_file->LoadFile())
		return false;

	char string[5];
	string[4] = '\0';
	TiXmlElement *xml_level = 0;
	TiXmlElement *xml_body = 0, *xml_body_temp = 0;
	TiXmlElement *xml_entity = 0;

	xml_level = xml_file->FirstChildElement("level");
	xml_entity = xml_level->FirstChildElement("entity");

	size_t NumberOfRow = 0;
	SizeOf_NumberOfForms = 0;
	while( xml_entity != NULL )
	{
		++SizeOf_NumberOfForms;
		xml_entity = xml_entity->NextSiblingElement("entity");
	}

	NumberOfForms = new size_t [SizeOf_NumberOfForms];
	NumberOfBox = new size_t [SizeOf_NumberOfForms];
	mas = new Point*** [SizeOf_NumberOfForms];

	xml_entity = xml_level->FirstChildElement("entity");
	size_t IndexOfFigure = 0; 	//Индекс сущности
	while( xml_entity != NULL )
	{
		NumberOfBox[IndexOfFigure] = 0;	//Количество квадратов в фигуре
		xml_body = xml_entity->FirstChildElement("body");
		NumberOfForms[IndexOfFigure] = atoi(xml_body->Attribute("NumberOfRotations"));
		xml_body = xml_body_temp = xml_body->NextSiblingElement("body");
		while ( xml_body != NULL )	//Расчет общего количества точек в форме
		{
			sprintf(string,"%s",xml_body->Attribute("line"));
			string[4] = '\0';
			NumberOfBox[IndexOfFigure] += CountSym(string, '#');
			xml_body = xml_body->NextSiblingElement("body");
		}

		mas[IndexOfFigure] = new Point**[NumberOfForms[IndexOfFigure]];

		for (size_t k = 0; k < NumberOfForms[IndexOfFigure]; k++) //Выделение памяти под каждую точку для каждой формы конкретной фигуры
		{
			mas[IndexOfFigure][k]= new Point* [NumberOfBox[IndexOfFigure]];
			for (size_t j = 0; j < NumberOfBox[IndexOfFigure]; j++)
			{
				mas[IndexOfFigure][k][j] = new Point;
			}
		}
		xml_body = xml_body_temp;
		NumberOfRow = 0;
		size_t IndexOfBox = 0;
		while ( xml_body != NULL )	//Задание расположения элементов фигуры для начального положения
		{
			++NumberOfRow;
			sprintf(string,"%s",xml_body->Attribute("line"));
			string[4] = '\0';
			for (size_t i = 0; i < 4; ++i)
				if (string[i] == '#')
				{
					*mas[IndexOfFigure][0][IndexOfBox] = Point((int)(i - 1), (int)(4 - NumberOfRow - 1));
					++IndexOfBox;
				}
			xml_body = xml_body->NextSiblingElement("body");
		}
		xml_entity = xml_entity->NextSiblingElement("entity");
		++IndexOfFigure;
	}
    for (size_t i = 0; i < SizeOf_NumberOfForms; ++i)
        for (size_t k = 1; k < NumberOfForms[i]; ++k)
			for (int j = 0; j < 4; j++)
				(*mas[i][k][j])=mas[i][ k - 1 ][j]->Rotate();
    return true;
}
		
void Figure::CleanMas()
{
    for (size_t i = 0; i < SizeOf_NumberOfForms; ++i)
	{
        for (size_t k = 0; k < NumberOfForms[i]; ++k)
		{
			for (int j = 0; j < 4; j++)
				delete[] mas[i][k][j];
			delete[] mas[i][k];
		}
		delete[] mas[i];
	}
	delete[] mas;
}

bool Figure::IsActivePoint(int i)
{
    return IsActive[i];
}

bool Figure::IsPossibleToEatFigure(Point &HeadOfSnake)
{
	bool Result = false;
	for ( size_t i = 0; i < NumberOfBox[Type] && !Result; ++i )
	{
        if ( HeadOfSnake == ZeroPoint + *(mas[Type][Position][i]) && IsActive[i])
		{
                    IsActive[i] = false;
			Result = true;
		}
	}
	return Result;
}

Point* Figure::Count(Point &A)
{								
	Point *Temp = new Point [ NumberOfBox[Type] ];
    for (size_t i = 0; i < NumberOfBox[Type]; ++i)
		Temp[i] = A + *(mas[Type][Position][i]);
	return Temp;
}

bool Figure::Move(char c, Heap* Hp, Snake* Sn)
{
	size_t DeltaX = 0, DeltaY = 0;
	switch (c)
	{
		case RIGHT: DeltaX = 1; DeltaY = 0; break;
		case UP: DeltaX = 0; DeltaY = 1; break;
		case LEFT: DeltaX = -1; DeltaY = 0; break;
		case DOWN: DeltaX = 0; DeltaY = -1; break;
	}
	Point TempSingle (ZeroPoint.GetX() + DeltaX, ZeroPoint.GetY() + DeltaY);
    int Possible = 0;
    for (size_t i = 0; i < NumberOfBox[Type] && Possible == 0; ++i)
    {
        if (IsActive[i]) //Проверка на активность
        {
            Point Temp = TempSingle + *(mas[Type][Position][i]);
            if(Temp.GetY() == -1 || !Hp->IsFree(Temp) )
            {
                Possible = 1;
            }
      if (!Sn->IsFree(Temp))
            {
                Possible = 2;
            }
        }
    }
    if (Possible == 0)
	{
        ZeroPoint = TempSingle;
	}
	else
    {
                if(Possible == 1)
                {
                    Hp->AddFigure(*this);
                    ZeroPoint = Point(5, 15);
                    Reactivate();
                }
                else
                {
                }//delete[] Temp;
//delete[] Temp;
        }
    return Possible == 0;
}
/*	Heap		*/
Heap::Heap() :
Size(0)
{
    Mas = new int* [SIZEY];
    for (int i = 0; i < SIZEY; ++i)
        Mas[i] = new int [SIZEX];
    for (int i = 0; i < SIZEY; ++i)
    {
        for (int j = 0; j < SIZEX; ++j)
        {
            Mas[i][j] = 0;
        }
    }
}
void Heap::AddFigure(Figure &Fig)
{
    size_t Size = Fig.GetSize();
    for(size_t i = 0; i < Size; ++i)
        if (Fig.IsActivePoint(i))
            Mas[Fig[i].GetY()][Fig[i].GetX()] = 1;
}

bool Heap::IsFree(Point &Destination)
{
    return Mas[Destination.GetY()][Destination.GetX()] == 0;
}
/*	Player		*/
