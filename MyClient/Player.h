#ifndef PLAYER_H
#define PLAYER_H

#include "Game.h"
#include "Painter.h"

class Player
{
public:
    Player(int, int, int, FieldSettings*);
    void Command(char);
    void SnakeTact();
    void FigureTact();
    void DrawAll();
    int GetScore();
    bool HasFigureChanged();
    size_t NextType();
    int GetNum();
    bool IsFinish();
    void SetIAm();
    ~Player();
private:
    int Num;
    Heap *Hp;
    Snake *Sn;
    Figure *Fig;
    Painter *Drawer;
    int Score;				//Счет игрока
    char SnakeDest;
    FieldSettings *Set;
    bool StopGame;
    bool IAm;
};

#endif
