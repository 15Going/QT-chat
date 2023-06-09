#include "tcpclient.h"
#include "ui_tcpclient.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>

TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);

    setFixedSize(350,180);

    TotalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    tcpClient = new QTcpSocket(this);
    tcpPort = 6666;

    connect(tcpClient,&QTcpSocket::readyRead,this,&TcpClient::readMessage);
    connect(tcpClient,&QTcpSocket::errorOccurred,
            this,&TcpClient::displayError);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::setHostAddress(QHostAddress address)
{
    hostAddress = address;
    newConnect();
}

void TcpClient::setFileName(QString fileName)
{
    localFile = new QFile(fileName);
}

void TcpClient::closeEvent(QCloseEvent *)
{
    on_tcpClientCloseBtn_clicked();
}

void TcpClient::readMessage()
{
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_4_7);

    timeEnd = QTime::currentTime();
    float useTime = timeEnd.msec() - timeStart.msec();

    if(bytesReceived <= sizeof(qint64)*2){
        if((tcpClient->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0)){
            in>>TotalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tcpClient->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
            in>>fileName;
            bytesReceived +=fileNameSize;
            if(!localFile->open(QFile::WriteOnly)){
                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2.")
                                     .arg(fileName).arg(localFile->errorString()));
                return;
            }
        }else{
            return;
        }
    }
    if(bytesReceived < TotalBytes){
        bytesReceived += tcpClient->bytesAvailable();
        inBlock = tcpClient->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesReceived);

    double speed = bytesReceived / useTime;
    ui->tcpClientStatusLabel->setText(tr("已接收 %1MB(%2MB/s)"
                                         "\n共%3MB已用时：%4秒\n估计剩余时间：%5秒")
                                      .arg(bytesReceived / (1024 * 1024))
                                      .arg(speed * 1000 / (1024 * 1024))
                                      .arg(TotalBytes / (1024 * 1024))
                                      .arg(useTime / 1000)
                                      .arg(TotalBytes / speed / 1000 - useTime / 1000));
    if(bytesReceived == TotalBytes){
        localFile->close();
        tcpClient->close();
        ui->tcpClientStatusLabel->setText(tr("接收文件%1完成").arg(fileName));
    }
}

void TcpClient::newConnect()
{
    blockSize = 0;
    tcpClient->abort();
    tcpClient->connectToHost(hostAddress,tcpPort);
    timeStart = QTime::currentTime();
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    default:
        qDebug()<<tcpClient->errorString();
    }
}

void TcpClient::on_tcpClientCancleBtn_clicked()
{
    tcpClient->abort();
    if(localFile->isOpen())
        localFile->close();
}


void TcpClient::on_tcpClientCloseBtn_clicked()
{
    tcpClient->abort();
    if(localFile->isOpen())
        localFile->close();
    close();
}

