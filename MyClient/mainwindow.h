#ifndef SCENE3D_H
#define SCENE3D_H
/*
#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include "myclient.h"

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    MainWindow(MyClient* temp, int SizeX = 520, int SizeY = 520, QWidget *parent = 0);
    MyClient* IAm;
protected:
    int point; // набранные очки
    int gdx, gdy; // Координаты объектов (гусей)
    int cax, cay, cbx, cby; // Координаты курсора
    int wax ,way; // Размеры окна
    bool singling; // Для выделение области
    //void self_cursor(); // метод для рисования своего курсора
    void initializeGL(); // Метод для инициализирования opengl
    void resizeGL(int nWidth, int nHeight); // Метод вызываемый после каждого изменения размера окна
    void paintGL(); // Метод для вывода изображения на экран
    void keyPressEvent(QKeyEvent *ke); // Для перехвата нажатия клавиш на клавиатуре
    //void mouseMoveEvent(QMouseEvent *me); // Метод реагирует на перемещение указателя, но по умолчанию setMouseTracking(false)
    //void mousePressEvent(QMouseEvent *me); // Реагирует на нажатие кнопок мыши
    //void mouseReleaseEvent(QMouseEvent *me); // Метод реагирует на "отжатие" кнопки мыши
    void singling_lb(); // Рисуем рамку выделенной области
protected slots:
    void figuretact();
    void snaketact();
};

class Sleeper: public QThread
{
public:
    static void Pause(int);
};
*/
#endif // SCENE3D_H
