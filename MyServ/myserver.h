#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class MyServer;
}

class MyServer : public QWidget
{
    Q_OBJECT

public:
    explicit MyServer(int nPort, QWidget *parent = 0);
    ~MyServer();
    QString return_number_of_client(QList<QTcpSocket *> clients, QTcpSocket* Socket);

public slots:
   virtual void slotNewConnection();
            void slotReadClient   ();
            void figuretact();
            void snaketact();
            void StartGame();

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    QString ReadClient(QTcpSocket* pClientSocket);

private:
    Ui::MyServer *ui;
    QTcpServer* m_ptcpServer;
    quint16     m_nNextBlockSize;
    QList<QTcpSocket *> _clients; //список пользователей
    bool GameOver;
    int num_of_conected;
signals:
    void BothReady();
};

#endif // MYSERVER_H
