#include "myclient.h"
#include "ui_myclient.h"
#include "mainwindow.h"
#include "Player.h"

#define NUMPOS 12
#define COMMANDLENGTH 12


MyClient::MyClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyClient),
    m_nNextBlockSize(0),
    str_with_send("")
{
    Init();
    m_pTcpSocket = new QTcpSocket(this);
    ui->setupUi(this);
    ui->line_IP->setText("127.0.0.1");
    ui->line_port->setText("4463");
    connect(ui->button_connect, SIGNAL(clicked()), this, SLOT(connect_to_server()));

    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(ui->button_send, SIGNAL(clicked()), SLOT(slotSendToServer()));

    connect(this, SIGNAL(game_start()),  SLOT(Start(  )));
    connect(wnd, SIGNAL(game_end()), this, SLOT(mainwindowclose()));

    ui->textEdit->setReadOnly(true);
    ui->close_game_button->setDisabled(true);
    ui->Start_Game_Button->setDisabled(true);
}

void MyClient::mainwindowclose() {
    int MyNum = wnd->GetMyNum();
    char str[40];
    int YourScore = wnd->GetScore(MyNum);
    int OpponentScore = wnd->GetScore(MyNum == 2? 1 : 2);
    sprintf(str, "Your score: %d.\n", YourScore);
    ui->textEdit->append(str);
    str[0] = '\0';
    sprintf(str, "Opponent's score: %d.\n", OpponentScore);
    ui->textEdit->append(str);
    if(YourScore > OpponentScore)
        ui->textEdit->append("You win.\n");
    else
        if (YourScore == OpponentScore)
            ui->textEdit->append("Draw.\n");
        else
            ui->textEdit->append("You lose.\n");
    send_key_to_server("game_over");
    ui->close_game_button->setEnabled(true);
}

//---------------------------------------------------------------------------
MyClient::~MyClient()
{
    delete ui;
}

//----------------------------------------------------------------------------
void MyClient::connect_to_server() {
    //QHostAddress::LocalHost

    m_pTcpSocket->connectToHost(ui->line_IP->text(), ui->line_port->text().toInt());

    ui->label_recive->setText(tr("Connecting to server..."));
    //ui->line_IP->setReadOnly(true);
    //ui->line_port->setReadOnly(true);
    //ui->line_send->setReadOnly(true);
}



//----------------------------------------------------------------------------
void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_3_0);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < (int)sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        str_with_send = "";
        in  >> time >> str_with_send;

        if(str_with_send.length() > COMMANDLENGTH)
            ui->textEdit->append(time.toString() + " " + str_with_send);

        if (str_with_send.contains("You are the ", Qt::CaseInsensitive)) {

             int num = str_with_send[NUMPOS].toLatin1() - '0';
             wnd->SetPlayerNum(num);
             ui->Start_Game_Button->setEnabled(true);
        }
        if (str_with_send.contains("Start", Qt::CaseInsensitive)) {
             QThread* my_thread = new QThread;
             connect(my_thread, SIGNAL(started()), this, SLOT(Start()));
             my_thread->start();
        }
       if(wnd) {
        if (str_with_send[0].toLatin1() == '2') {
            wnd->SendCommandTo(2,(str_with_send[1]).toLatin1());
        }
        if (str_with_send[0].toLatin1() == '1') {
            wnd->SendCommandTo(1,(str_with_send[1]).toLatin1());
        }
        if (str_with_send[0].toLatin1() == '0') {
            if (str_with_send[1].toLatin1() == '1')
            {
                wnd->snaketact();
            }
            else
            {
                wnd->figuretact();
            }
        }
       }
       m_nNextBlockSize = 0;
    }
}

// ----------------------------------------------------------------------
void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    ui->textEdit->append(strError);
}

// ----------------------------------------------------------------------
void MyClient::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_3_0);
    out << quint16(0) << QTime::currentTime() << ui->line_send->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    ui->line_send->setText("");
}


void MyClient::send_key_to_server(QString comand) {
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_3_0);
    out << quint16(0) << QTime::currentTime() << comand;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

void MyClient::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
    default:
    //Nothing To Do
    return;
    }

}

//--------------------------------------------------------------------------------------------------------------

void MyClient::Start() {
    wnd->show();
}

void MyClient::Init() {
    wnd = new MainWindow(this);
    wnd->setAttribute(Qt::WA_DeleteOnClose, true);
}


void MyClient::on_close_game_button_clicked()
{
    wnd->Delete();
    wnd->close();
    wnd = NULL;
}

void MyClient::on_Start_Game_Button_clicked()
{
    send_key_to_server("System: Start");
    ui->Start_Game_Button->setDisabled(true);
}
