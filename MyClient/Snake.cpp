#include "Game.h"


/*	Snake		*/
Snake::Snake(FieldSettings	 *Sset) :
  IAmAlive(true), Direction(2), Size(3), MaxSize(7), Head(0), Set(Sset)
{
    if (Size > 4)
        Size = 4;
    Mas = new Point[MaxSize];
    for (int i = 0; i < (int)Size; ++i)
    {
        Mas[i] = Point(5 + i,5);
    }
}

Snake::Snake(FieldSettings *Sset, size_t SSize = 2, size_t MMaxSize = 7) :
  IAmAlive(true), Direction(2), Size(SSize), MaxSize(MMaxSize), Head(0), Set(Sset)
{
    if (Size > 4)
        Size = 4;
    Mas = new Point[MMaxSize];
    for (size_t i = 0; i < Size; ++i)
    {
        Mas[i] = Point(7 + i,5);
    }
}

bool Snake::Add()
{
    static bool Success = true;
    if (Size != MaxSize)
    {
        Point* Temp = new Point[MaxSize];
        if (Temp == NULL)
        {
            std::cout << "Prbolem with allocation memmory for Snake in Grow\n";
            Success = false;
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
            Success = true;
            ++Size;
        }
    }
    else
    {
        Success = false;
    }
    return Success;
}

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
    bool State = true;
    if (IAmAlive)
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
        if (temp.GetX() >= 0 && temp.GetX() < Set->GetW() && temp.GetY() >= 0 && temp.GetY() < Set->GetH() - 4)
        {
            if(Fig.IsPossibleToEatFigure(temp))
            {
                Add();
                if(!Fig.IsExsist())
                {
                    Fig.SearchType();
                }
            }
            else
            {
                if (Hp.IsFree(temp) != -1)
                {
                    State = false;
                }
                else
                {
                    if (!this->IsFree(temp))
                    {
                        State = false;
                    }
                }
            }
        }
        else
        {
            State = false;
        }
        if (State)
        {
            Head = (Head == 0? Size: Head) - 1;
            Mas[Head] = temp;
        }
    }
    else
    {
        State = false;
    }
    return State;
}

void Snake::Kill()
{
    IAmAlive = false;
}

bool Snake::IsAlive()
{
    return IAmAlive;
}

void Snake::Turn(char c)
{
        switch(c)
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
    if (IAmAlive)
    {
        for ( size_t i = 0; i < Size && Result; ++i )
        {
            if ( Destination == Mas[ (i + Head) % Size ])
            {
                Result = false;
            }
        }
    }
    return Result;
}

Snake::~Snake()
{
    delete[] Mas;
}
