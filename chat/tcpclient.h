#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QTime>
#include <QFile>
#include <QHostAddress>

class QTcpSocket;

namespace Ui {
class TcpClient;
}

class TcpClient : public QDialog
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

    void setHostAddress(QHostAddress address);
    void setFileName(QString fileName);

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::TcpClient *ui;

    qint64 TotalBytes;
    qint64 bytesReceived;
    qint64 bytesToReceive;
    qint64 fileNameSize;
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;
    qint16 tcpPort;

    QTime timeStart;
    QTime timeEnd;

    qint16 blockSize;
    QHostAddress hostAddress;
    QTcpSocket *tcpClient;

private slots:
    void readMessage();
    void newConnect();
    void displayError(QAbstractSocket::SocketError);
    void on_tcpClientCancleBtn_clicked();
    void on_tcpClientCloseBtn_clicked();
};

#endif // TCPCLIENT_H
