#include "Game.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "NonClassFunction.h"


size_t *Figure::NumberOfBox;
size_t *Figure::NumberOfForms;
size_t Figure::SizeOf_NumberOfForms;
Point**** Figure::mas;		

/*	 Figure 		*/
Figure::Figure(size_t TType, size_t X, size_t Y, FieldSettings *Sett) :
  Type(TType), Position(0), Set(Sett), ZeroPoint(X, Y)
{
    IsActive = new bool [16];
    Reactivate();
}

Figure::Figure(FieldSettings *Sett) :
  Type(1), Position(0), Set(Sett), ZeroPoint(Set->GetW() / 2, Set->GetH() - 3)
{
    IsActive = new bool [16];
    Reactivate();
}

size_t Figure::GetType()
{
    return Type;
}

void Figure::Deactivate()
{
    for (int i = 0; i < 16; ++i )
    {
        IsActive[i] = false;
    }
}

void Figure::Reactivate()
{
    AmountOfActive = NumberOfBox[Type];
    for (int i = 0; i < 16; ++i )
    {
        IsActive[i] = true;
    }
}

bool Figure::HasChanged()
{
    return IsNeed;
}

size_t Figure::GetNextType()
{
    return NextType;
}

void Figure::SearchType()
{
    NextType = rand() % SizeOf_NumberOfForms;
    IsNeed = true;
    Deactivate();
}

void Figure::NewFig(size_t type)
{
    Type = type;
    AmountOfActive = NumberOfBox[type];
    Position = 0;
    Reactivate();
    ZeroPoint = Point(Set->GetW() / 2, Set->GetH() - 3);
    IsNeed = false;
}

bool Figure::Turn(Heap* Hp, Snake* Sn)
{
    int PositionTemp = (Position + 1) % (NumberOfForms[Type]);
    int Possible = 0;
    for (size_t i = 0; i < NumberOfBox[Type] && Possible == 0; ++i)
    {
        if (IsActive[i]) //Проверка на активность
        {
            Point Temp = ZeroPoint + *(mas[Type][PositionTemp][i]);
            if (Temp.GetX() >= 0 && Temp.GetX() < Set->GetW() && Temp.GetY() < Set->GetH() && Temp.GetY() >= 0)
            {
                if(Temp.GetY() == -1 || Hp->IsFree(Temp) != -1 )
                {
                    Possible = 1;
                }
                else
                {
                    if (!Sn->IsFree(Temp))
                    {
                        Possible = 2;
                    }
                }
            }
            else
            {
                Possible = 3;
            }
        }
    }
    if (Possible == 0)
        Position = PositionTemp;
    return Possible == 0;
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

    bool Possible = true;
    for (size_t i = 0; i < NumberOfBox[Type] && Possible; ++i)
    {
        if (IsActive[i]) //Проверка на активность
        {
            Point Temp = TempSingle + *(mas[Type][Position][i]);
            if (Temp.GetX() >= 0 && Temp.GetX() < Set->GetW() && Temp.GetY() < Set->GetH())
            {
                if(Temp.GetY() == -1 || Hp->IsFree(Temp) != -1 )
                {
                    if((int)DeltaY == -1)
                    {
                        Hp->AddFigure(*this);
                        SearchType();
                    }
                    Possible = false;
                }
                else
                {
                    if (!Sn->IsFree(Temp))
                    {
                        Possible = false;
                    }
                }
            }
            else
            {
                Possible = false;
            }
        }
    }
    if (Possible)
    {
        ZeroPoint = TempSingle;
    }
    return Possible;
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
    delete xml_file;
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

bool Figure::IsExsist()
{
    return !(AmountOfActive == 0);
}

bool Figure::IsPossibleToEatFigure(Point &HeadOfSnake)
{
    bool Result = false;
    for ( size_t i = 0; i < NumberOfBox[Type] && !Result; ++i )
    {
        if ( HeadOfSnake == ZeroPoint + *(mas[Type][Position][i]) && IsActive[i])
        {
            --AmountOfActive;
            IsActive[i] = false;
            Result = true;
        }
    }
    return Result;
}

Point* Figure::Count(Point &A)
{								
    Point *Temp = new Point [NumberOfBox[Type]];
    for (size_t i = 0; i < NumberOfBox[Type]; ++i)
        Temp[i] = A + *(mas[Type][Position][i]);
    return Temp;
}

Figure::~Figure()
{
    delete[] IsActive;
}
