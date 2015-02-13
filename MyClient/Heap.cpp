#include "Game.h"

/*	Heap		*/
Heap::Heap(FieldSettings *Sset) :
  Size(0), FSet(Sset)
{
    Mas = new int* [FSet->GetH()];
    for (int i = 0; i < FSet->GetH(); ++i)
        Mas[i] = new int [FSet->GetW()];
    for (int i = 0; i < FSet->GetH(); ++i)
    {
        for (int j = 0; j < FSet->GetW(); ++j)
        {
            Mas[i][j] = -1;
        }
    }
}
void Heap::AddFigure(Figure &Fig)
{
    size_t Size = Fig.GetSize();int temp = 0;
    for(size_t i = 0; i < Size; ++i)
        if (Fig.IsActivePoint(i))
        {
            Mas[Fig[i].GetY()][Fig[i].GetX()] = (int)Fig.GetType();
           temp++ ;
        }
}

int Heap::IsFree(Point &Destination)
{
    return Mas[Destination.GetY()][Destination.GetX()];
}

void Heap::Clean(size_t i)
{
    for (; (int)i < FSet->GetH() - 1; ++i)
    {
        for (size_t j = 0; (int)j < FSet->GetW(); ++j)
        {
            Mas[i][j] = Mas[i + 1][j];
        }
    }
}

int Heap::Check()
{
    bool IsEmpty = true;
    int count = 0;
    for (size_t i = 0; (int)i < FSet->GetH();)
    {
        IsEmpty = false;
        for (size_t j = 0; (int)j < FSet->GetW() && !IsEmpty; ++j)
        {
            if(Mas[i][j] == -1)
            {
                IsEmpty = true;
            }
        }
        if (!IsEmpty)
        {
            Clean(i);
            ++count;
        }
        else
        {
            ++i;
        }
    }
    return count;
}

bool Heap::IsGameOver()
{
    bool Over = false;
    for (size_t i = (size_t)(FSet->GetH() - 4); (int)i < FSet->GetH() && !Over; ++i)
    {
        for (size_t j = 0; (int)j < FSet->GetW() && !Over; ++j)
        {
            if(Mas[i][j] != -1)
            {
                Over = true;
            }
        }
    }
    return Over;
}

Heap::~Heap()
{
    for (size_t i = 0; (int)i < FSet->GetH(); ++i)
    {
        delete[] Mas[i];
    }
    delete[] Mas;
}
