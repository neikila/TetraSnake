#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QDialog>
#include <QtNetwork>
#include <QWidget>
#include <QtGui>

#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>

#include "Player.h"

namespace Ui {
class MyClient;
}

class MainWindow;

class MyClient : public QDialog
{
    Q_OBJECT

public:
    explicit MyClient(QWidget *parent = 0);
    ~MyClient();
    void send_key_to_server(QString command);
    //void game_changes(QString str);
    void Init();


private slots:
    void connect_to_server();
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
    void keyPressEvent(QKeyEvent* event);
    void Start();
    void mainwindowclose();


    //void on_button_connect_clicked();
    void on_close_game_button_clicked();

    void on_Start_Game_Button_clicked();

signals:
     void game_start();


private:
    Ui::MyClient *ui;
    QTcpSocket* m_pTcpSocket;
    quint16 m_nNextBlockSize;
    QString str_with_send;
    MainWindow *wnd;
};

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    MainWindow(MyClient* temp, QWidget *parent = 0);
    void figuretact();
    void snaketact();
    void SetPlayerNum(int);
    void SendCommandTo(int Num, char c);
    int GetMyNum();
    int GetScore(int);
    void Delete();
protected:
    Player *First;
    Player *Second;
    MyClient *IAm;
    FieldSettings *Set;
    int wax ,way; // Размеры окна
    bool singling; // Для выделение области
    int MyNum;
    void initializeGL(); // Метод для инициализирования opengl
    void resizeGL(int nWidth, int nHeight); // Метод вызываемый после каждого изменения размера окна
    void paintGL(); // Метод для вывода изображения на экран
    void keyPressEvent(QKeyEvent *ke); // Для перехвата нажатия клавиш на клавиатуре
    void singling_lb(); // Рисуем рамку выделенной области
    void IfFigureHasChanged(int, size_t);
    bool IsOver();
private:
    char str[2];
    char strsn[2];
signals:
    void game_end();
};
#endif // MYCLIENT_H
