#ifndef PAINTER_H
#define PAINTER_H 

#include "Game.h"

struct Color
{
    char name [5];
    float R;
    float G;
    float B;
    Color(double r = 0.0, double g = 0.0, double b = 0.0);
};

class Painter
{
private:
    int Width;
    int Height;
    static int Extra;
    static int Grade;
    int X0;
    int Y0;
    static Color SnakeC;
    static Color* FigureC;
    static size_t NumOfColor;
    bool IsMyWindow;
public:
    static bool SetStaticData(const char*);
    static void ClearStaticData();
    static int GetExtra();
    static int GetGrade();
    Painter(int X = 0, int Y = 0, FieldSettings *Set = NULL);
    void DrawField(void);
    void Draw(Snake*);
    void Draw(Figure*);
    void Draw(Heap*);
    void SetIAm();
};

#endif
