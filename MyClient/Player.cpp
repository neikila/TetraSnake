#include "Player.h"

Player::Player(int num, int X, int Y, FieldSettings *Sset)
   : Num(num), Score(0), SnakeDest('0'), Set(Sset), StopGame(false), IAm(false)
{
    Drawer = new Painter (X, Y, Set);
    Sn = new Snake(Set);
    Hp = new Heap(Set);
    Fig = new Figure(Set);
}

void Player::SetIAm()
{
    IAm = true;
    Drawer->SetIAm();
}

bool Player::IsFinish()
{
    return StopGame;
}

int Player::GetNum()
{
    return Num;
}

void Player::FigureTact()
{
    if (!StopGame)
    {
        Fig->Move('d', Hp, Sn);
        switch(Hp->Check())
        {
        case 4: Score += 150;
        case 3: Score += 200;
        case 2: Score += 150;
        case 1: Score += 100;
        }
        if(Hp->IsGameOver())
        {
            StopGame = true;
        }
    }
}

void Player::SnakeTact()
{
    if (!StopGame)
    {
        Sn->Turn(SnakeDest);
        SnakeDest = '0';
        if(!Sn->Move(*Hp, *Fig))
        {
            Sn->Kill();
        }
        if(Hp->IsGameOver())
        {
            StopGame = true;
        }
    }
}

void Player::Command(char c)
{
    if (!StopGame)
    {
        switch (c)
        {
        case 'w':
            SnakeDest = 'u';
            break;
        case 'a':
            SnakeDest = 'l';
            break;
        case 'd':
            SnakeDest = 'r';
            break;
        case 's':
            SnakeDest = 'd';
            break;
        case 'j':
            Fig->Move('l', Hp, Sn);
            break;
        case 'l':
            Fig->Move('r', Hp, Sn);
            break;
        case 'i':
            Fig->Turn(Hp, Sn);
            break;
        case 'k':
            Fig->Move('d', Hp, Sn);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            Fig->NewFig(c - '0');
            break;
        }
    }
}

bool Player::HasFigureChanged()
{
    bool Result;
    if(!StopGame)
    {
        Result = Fig->HasChanged();
    }
    else
    {
        Result = false;
    }
    return Result;
}

int Player::GetScore()
{
return Score;
}

size_t Player::NextType()
{
    return Fig->GetNextType();
}

void Player::DrawAll()
{
    Drawer->Draw(Sn);
    Drawer->Draw(Fig);
    Drawer->Draw(Hp);
    Drawer->DrawField();
}

Player::~Player()
{
    delete Hp;
    delete Sn;
    delete Fig;
    delete Drawer;
}
