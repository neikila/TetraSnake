#include "Painter.h"
#include <QtOpenGL>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"

int Painter::Extra;
int Painter::Grade;
Color Painter::SnakeC;
Color* Painter::FigureC;
size_t Painter::NumOfColor;

Painter::Painter(int X, int Y, FieldSettings *Set)
    : X0(X), Y0(Y), IsMyWindow(false)
{
    if (Set != NULL)
    {
        Width = Set->GetW();
        Height = Set->GetH();
    }
    else
    {
        Width = 10;
        Height = 25;
    }
}

bool Painter::SetStaticData(const char* Filename)
{
    TiXmlDocument *xml_file = new TiXmlDocument(Filename);
    if(!xml_file->LoadFile())
        return false;

    TiXmlElement *xml_level = 0;
    TiXmlElement *xml_body = 0;
    TiXmlElement *xml_entity = 0;

    xml_level = xml_file->FirstChildElement("level");
    xml_entity = xml_level->FirstChildElement("entity");


    xml_body = xml_entity->FirstChildElement("body");
    Extra = atoi(xml_body->Attribute("border"));
    xml_body = xml_body->NextSiblingElement("body");
    Grade = atoi(xml_body->Attribute("boxwidth"));

    xml_entity = xml_entity->NextSiblingElement("entity");
    xml_body = xml_entity->FirstChildElement("body");
    NumOfColor = 0;
    while( xml_body != NULL )
    {
        ++NumOfColor;
        xml_body = xml_body->NextSiblingElement("body");
    }

    FigureC = new Color [NumOfColor];

    xml_body = xml_entity->FirstChildElement("body");
    size_t i = 0;
    while( xml_body != NULL )
    {
        FigureC[i] = Color(atof(xml_body->Attribute("R")), atof(xml_body->Attribute("G")), atof(xml_body->Attribute("B")));
        xml_body = xml_body->NextSiblingElement("body");
        ++i;
    }
    xml_entity = xml_entity->NextSiblingElement("entity");
    xml_body = xml_entity->FirstChildElement("body");
    SnakeC = Color(atof(xml_body->Attribute("R")), atof(xml_body->Attribute("G")), atof(xml_body->Attribute("B")));
    delete xml_file;
    return true;
}

int Painter::GetExtra()
{
    return Extra;
}

int Painter::GetGrade()
{
    return Grade;
}

void Painter::DrawField(void)
{
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(1);

        //Отрисовка сетки поля
        glBegin(GL_LINES);
        for (size_t i = 0; (int)i <= Width; i++)
        {
            glVertex2f(X0 + i * Grade + Extra, Y0 + Extra);
            glVertex2f(X0 + i * Grade + Extra, Y0 + Height * Grade + Extra);
        }

        for (size_t i = 0; (int)i <= Height; i++)
        {
            glVertex2f(X0 + Extra, Y0 + Grade * i + Extra);
            glVertex2f(X0 + Width * Grade + Extra, Y0 + Grade * i + Extra);
        }
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(X0 + Extra, Y0 + (Height - 4) * Grade + Extra);
        glVertex2f(X0 + Width * Grade + Extra, Y0 + (Height - 4) * Grade + Extra);
        glColor3f(1.0, 1.0, 1.0);
        glEnd();

        //Рамка по бокам
        if (IsMyWindow)
            glColor3f(0.7, 0.3, 0.6);
        glRecti(X0, Y0, X0 + Extra, Y0 + Height * Grade + Extra);
        glRecti(X0, Y0 + Height * Grade + Extra, X0 + Extra + Width * Grade, Y0 + Height * Grade + 2 * Extra);
        glRecti(X0 + Extra + Width * Grade, Y0 + Height * Grade + 2 * Extra, X0 + Width * Grade + 2 * Extra, Y0 + Extra);
        glRecti(X0 + Width * Grade + 2 * Extra, Y0 + Extra, X0 + Extra, Y0);
        glColor3f(1.0, 1.0, 1.0);
}

void Painter::SetIAm()
{
    IsMyWindow = true;
}

void Painter::Draw(Snake* S)
{
    if (S == NULL)
        return;
    if (S->IsAlive())
    {
        glColor3f(SnakeC.R, SnakeC.G, SnakeC.B);
        //Перевод в локальный координаты поля
    int deltax = X0 + Extra, deltay = Y0 + Extra;

        for (size_t i = 0; i < S->GetSize(); ++i)
        {
            int tempx = Grade * (*S)[i].GetX();
            int tempy = Grade * (*S)[i].GetY();
            glRecti(deltax + tempx, deltay + tempy, deltax + tempx + Grade, deltay + tempy + Grade);
        }
        glColor3f(1.0, 1.0, 1.0);
    }
}

void Painter::Draw(Figure* F)
{
    if (F == NULL)
        return;
    //Перевод в локальный координаты поля
    int deltax = X0 + Extra, deltay = Y0 + Extra;
    glColor3f(FigureC[F->GetType()].R, FigureC[F->GetType()].G, FigureC[F->GetType()].B);
    for (size_t i = 0; i < F->GetSize(); ++i)
    {
        if(F->IsActivePoint(i))
        {
            int tempx = Grade * (*F)[i].GetX();
            int tempy = Grade * (*F)[i].GetY();
            glRecti(deltax + tempx, deltay + tempy, deltax + tempx + Grade, deltay + tempy + Grade);
        }
    }
    glColor3f(1.0, 1.0, 1.0);
}

void Painter::ClearStaticData()
{
    delete[] FigureC;
}

void Painter::Draw(Heap* Hp)
{
    if (Hp == NULL)
        return;
    //Перевод в локальный координаты поля
    int deltax = X0 + Extra, deltay = Y0 + Extra;
    static Point Temp;
    int FigColor;
    for (size_t i = 0; (int)i < Width; ++i)
    {
        for (size_t j = 0; (int)j < Height; ++j)
        {
            Temp = Point(i, j);
            FigColor = Hp->IsFree(Temp);
            if( FigColor > -1)
            {
                glColor3f(FigureC[FigColor].R, FigureC[FigColor].G, FigureC[FigColor].B);
                int tempx = Grade * i;
                int tempy = Grade * j;
                glRecti(deltax + tempx, deltay + tempy, deltax + tempx + Grade, deltay + tempy + Grade);
            }
        }
    }
    glColor3f(1.0, 1.0, 1.0);
}

Color::Color(double r, double g, double b): R(r), G(g), B(b) {}
