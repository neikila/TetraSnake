#include "myclient.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Player.h"

MainWindow::MainWindow(MyClient* temp, QWidget* parent) : QGLWidget(parent), IAm(temp), MyNum(0)
{
    srand(0);
    IAm = temp;
    if(!Figure::Fullmas("settings.xml"))
    {
        exit(1);
    }
    if(!Painter::SetStaticData("ClientSettings.xml"))
    {
        exit(2);
    }
    Set = new FieldSettings;
    int Delta = 100 + Set->GetW() * Painter::GetGrade() + 2 * Painter::GetExtra();
    setGeometry(100, 100, 2 * Delta - 100, Set->GetH() * Painter::GetGrade() + 2 * Painter::GetExtra()); // Смещение и положение окна
    First = new Player(1, 0, 0, Set);
    Second = new Player(2, Delta, 0, Set);
    setFormat(QGLFormat(QGL::DoubleBuffer));    // Двойная буферизация
    glDepthFunc(GL_LEQUAL);                     // Буфер глубины
}

void MainWindow::SetPlayerNum(int Num)
{
    MyNum = Num;
    if (MyNum == 1)
        First->SetIAm();
    else
        Second->SetIAm();
}

void MainWindow::SendCommandTo(int Num, char c)
{
    switch (Num)
    {
    case 1: First->Command(c); break;
    case 2: Second->Command(c); break;
    }
    updateGL();
}

void MainWindow::IfFigureHasChanged(int FieldNum, size_t NextType)
{
    char str[2];
    str[0] = '0' + NextType;
    str[1] = '\0';
    if (FieldNum == MyNum)
        IAm->send_key_to_server(str);
}

void MainWindow::snaketact()
{
    First->SnakeTact();
    if (First->HasFigureChanged())
    {
        IfFigureHasChanged(First->GetNum(), First->NextType());
    }
    Second->SnakeTact();
    if (Second->HasFigureChanged())
    {
        IfFigureHasChanged(Second->GetNum(), Second->NextType());
    }

    if(IsOver())
        emit game_end();
    updateGL();
}

bool MainWindow::IsOver()
{
    return (First->IsFinish() && Second->IsFinish());
}

int MainWindow::GetMyNum()
{
    return MyNum;
}

int MainWindow::GetScore(int PlayerNum)
{
    int result = 0;
    switch (PlayerNum)
    {
    case 1: result = First->GetScore();
        break;
    case 2: result = Second->GetScore();
        break;
    }
    return result;
}

void MainWindow::figuretact()
{
    First->FigureTact();
    if (First->HasFigureChanged())
    {
        IfFigureHasChanged(First->GetNum(), First->NextType());
    }
    Second->FigureTact();
    if (Second->HasFigureChanged())
    {
        IfFigureHasChanged(Second->GetNum(), Second->NextType());
    }
    updateGL();
}

void MainWindow::initializeGL()
{
    qglClearColor(Qt::black);                   // Черный цвет фона
}

void MainWindow::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    wax=nWidth;
    way=nHeight;
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glMatrixMode(GL_PROJECTION);                        // устанавливаем матрицу
    glLoadIdentity();                                   // загружаем матрицу
    glOrtho(0, wax, 0, way, 1, 0);                      // подготавливаем плоскости для матрицы

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    qglColor(Qt::white);
    First->DrawAll();
    Second->DrawAll();
    swapBuffers();
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    char str[2];
    str[0] = str[1] = '\0';
    switch (ke->key())
    {
    case Qt::Key_W:
        str[0] = 'w';
        break;
    case Qt::Key_A:
        str[0]= 'a';
        break;
    case Qt::Key_D:
        str[0]= 'd';
        break;
    case Qt::Key_S:
        str[0]= 's';
        break;
    case Qt::Key_J:
        str[0]= 'j';
        break;
    case Qt::Key_L:
        str[0]= 'l';
        break;
    case Qt::Key_I:
        str[0]= 'i';
        break;
    case Qt::Key_K:
        str[0]= 'k';
        break;
    }
    IAm->send_key_to_server(str);
}

void MainWindow::Delete()
{
    Figure::CleanMas();
    Painter::ClearStaticData();
    delete First;
    delete Second;
    delete IAm;
    delete Set;
}
