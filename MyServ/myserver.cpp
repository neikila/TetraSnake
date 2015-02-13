#include "myserver.h"
#include "ui_myserver.h"

QTimer *timer = NULL;
QTimer *timerSn = NULL;

MyServer::MyServer(int nPort, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyServer),
    m_nNextBlockSize(0),
    GameOver(false),
    num_of_conected(0)
{
    ui->setupUi(this);
    m_ptcpServer = new QTcpServer(this);
    if (m_ptcpServer->listen(QHostAddress::Any, nPort)) {
         ui->textEdit->setText("Server Start");
    }
    else {
        ui->textEdit->setText("Server Error");
        m_ptcpServer->close();
        return;
        }

    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    ui->textEdit->setReadOnly(true);
    ui->label->setText("<H1>TetraSnake Server</H1>");

}

MyServer::~MyServer()
{
    delete ui;
}

//-----------------------------------------------------------------------
void MyServer::slotNewConnection()
{
    if (num_of_conected == 2) {
        QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
        sendToClient(pClientSocket, "You are late! By!");
        return;
    }
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    _clients.append(pClientSocket);
    ++num_of_conected;

    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(this, SIGNAL(BothReady()), this, SLOT(StartGame()));

    sendToClient(pClientSocket, "Server Response: Connected!");
    if (_clients.length() == 2) {
        for (int j = 0; j < _clients.length(); ++j) {
            sendToClient(_clients.at(j), "Your Opponent was found!");
            char buf[32];
            sprintf(buf, "You are the %d player", j + 1);
            sendToClient(_clients.at(j), buf);
        }
    }
    ui->textEdit->append("CONNECT!");
}

void MyServer::StartGame()
{
    for (int j = 0; j < _clients.length(); ++j) {
       sendToClient(_clients.at(j), "Start!");
   }
   if (timer == NULL)
   {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(figuretact()));
        timer->start(700);
   }
   if (timerSn == NULL)
   {
        timerSn = new QTimer(this);
        connect(timerSn, SIGNAL(timeout()), this, SLOT(snaketact()));
        timerSn->start(400);
   }
}
//------------------------------------------------------------------------------------------
QString MyServer::return_number_of_client(QList<QTcpSocket *> clients, QTcpSocket* Socket){
    QString client_number;
    for (int j = 0; j < clients.length(); ++j) {
        if (Socket == clients.at(j)) {
            client_number = QString::number(j+1);
        }
    }
    return client_number;
}

void MyServer::figuretact()
{
    QString str("02");
    for (int j = 0; j < _clients.length(); ++j) {
        if (!GameOver)
            sendToClient(_clients.at(j), str);
    }
}

void MyServer::snaketact()
{
    QString str("01");
    for (int j = 0; j < _clients.length(); ++j) {
        if(!GameOver)
            sendToClient(_clients.at(j), str);
    }
}

//-------------------------------------------------------------------------------
void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QString str;
    str = ReadClient(pClientSocket);

    for (int j = 0; j < _clients.length(); ++j) {
        QString client_number;
        client_number = return_number_of_client(_clients, pClientSocket);
        if (!GameOver)
        {
             sendToClient(_clients.at(j),client_number + str);
        }
    }

}

//--------------------------------------------------------------------------------
QString MyServer::ReadClient(QTcpSocket* pClientSocket) {
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_3_0);
    QString str;
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < (int)sizeof(quint16)) {
                break;
            }
        in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        in  >> time >> str;

        QString number_of_client;
        number_of_client = return_number_of_client(_clients, pClientSocket);
        QString strMessage = time.toString() + " " + "Client " + number_of_client + " has sent - " + str;
        ui->textEdit->append(strMessage);
        if (str.contains("game_over", Qt::CaseInsensitive)) {
            timer->stop();
            timerSn->stop();
            GameOver = true;
        }
        static int ReadNum = 0;
        if (ReadNum != 2 && str.contains("System: Start", Qt::CaseInsensitive)) {
            ++ReadNum;
            if (ReadNum == 2)
                emit BothReady();
        }

        m_nNextBlockSize = 0;
    }
    return str;
}

// ----------------------------------------------------------------------
void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_3_0);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}
