#ifndef ALGO_H
#define ALGO_H

#include <stdlib.h>
#include <iostream>
#include "Point.h"
#include "NonClassFunction.h"
#include "Settings.h"

#define UP 'u'
#define DOWN 'd'
#define RIGHT 'r'
#define LEFT 'l'
//Паттерн изменение уровня сложности
//Изменение заключается в ликидации возможности поворота некоторой фигуры в одно из положений: Так если у зикзага два положения изначально - останется одно
class Figure;
class Heap;
class Snake;

class Heap
{
public:
    size_t GetSize();
    Point operator[] (int);	//Получение точку кучи
    int Check();
    int IsFree(Point&);
    Heap(FieldSettings*);
    void AddFigure(Figure&);
    void Clean(size_t);
    bool IsGameOver();
    ~Heap();
private:
    size_t Size;		//Количество блоков, из которых состоит куча
    int** Mas;		//Массив точек, характеризующих положение кучи (каждой её части) в поле
    FieldSettings *FSet;
};

class Snake
{
public:
    Snake(FieldSettings *Sset);
    Snake(FieldSettings *, size_t, size_t) ;
    int GetDirection();	//Получить направление
    size_t GetSize();
    Point operator[] (int);	//Получение точку змейки
    bool Move(Heap&, Figure&);		//Переместить змейку на одну клеточку в соответсвии с заданым направлением
    void Turn(char);	//Повернуть змейку. Передается сигнал.
    bool IsFree(Point&);
    bool Add();
    void Kill();
    bool IsAlive();
    ~Snake();
private:
    bool IAmAlive;
    int Direction;		//Направление движение змейки: 0 - право, 1 - вверх, 2 - влево, 3 - вниз. Направление задается относительно взгляда человека на экран.
    size_t Size; 		//Длина змейки
    size_t MaxSize; 		//Максимальная длинна змейк (ограничение)
    Point* Mas; 		//Массив точек, характеризующих положение змейки (каждой её части) в поле
    size_t Head;		//Указатель на голову змейки
    FieldSettings *Set;
};

class Figure
{
public:
    Figure(size_t, size_t, size_t, FieldSettings*);			//Конструктор копирования с параметром: форма (тип//вид) фигурки, координата по осям X и Y соответственно
    Figure(FieldSettings*);
    static bool Fullmas(const char*);	//Парсинг xml файла с фигурами
    static void CleanMas();
    size_t GetSize();
    Point operator[] (int);		//Получение точку фигуры
    bool Move(char, Heap*, Snake*);		//Переместить фигурку на одну клеточку в соответсвии с заданым направлением
    bool Turn(Heap *Hp, Snake *Sn);		//Повернуть фигурку. Передается сигнал.
    bool IsPossibleToEatFigure(Point&);
    Point* Count(Point &);
    bool IsActivePoint(int);
    bool IsExsist();
    void NewFig(size_t);
    void SearchType();
    size_t GetType();
    bool HasChanged();
    size_t GetNextType();
    ~Figure();
private:
    size_t AmountOfActive;
    bool *IsActive;			//Съеден ли блок фигуры змейкой
    size_t Type;
    size_t Position;
    FieldSettings *Set;
    Point ZeroPoint;		//Положение на поле точки с координатой 0;0 в XML-файле
    size_t NextType;
    bool IsNeed;
    static size_t *NumberOfForms;	//Количество вариантов поворота у фигуры
    static size_t *NumberOfBox;	//Количество квадратов в фигуре
    static size_t SizeOf_NumberOfForms;
    static Point ****mas;		//Трехмерный Массив указателей точек, характеризующих положение фигуры (каждой её части) в поле. Первый индекс - тип фигуры; Второй индекс - номер положения (поворот); Третий индекс - номер квадрата.
    void Reactivate();
    void Deactivate();
};


#endif
