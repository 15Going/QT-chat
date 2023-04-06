#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextCharFormat>

class QUdpSocket;
class TcpServer;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum MessageType{Message,NewParticipant,ParticipantLeft,FileName,Refuse};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void newParticipant(QString userName,QString localHostName,QString ipAddress);
    void participantLeft(QString userName,QString localHostName,QString time);
    void sendMessage(MessageType type,QString serverAdress = "");

    void hasPendingFile(QString userName,QString serverAddress,
                        QString clientAddress,QString fileName);

    void closeEvent(QCloseEvent *);

    QString getIP();
    QString getUserName();
    QString getMessage();


private:
    Ui::Widget *ui;

    QUdpSocket *udpSocket;
    qint16 port;

    QString fileName;
    TcpServer *server;

    QColor color;

private slots:
    void processPendingDatagrams();
    void on_sendButton_clicked();

    void getFileName(QString);
    void on_sendToolBtn_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_sizeComBox_currentTextChanged(const QString &arg1);

    void on_boldToolBtn_clicked(bool checked);
    void on_italicToolBtn_clicked(bool checked);
    void on_colorToolBtn_clicked();

    void currentFormatChanged(const QTextCharFormat &format);
    bool saveFile(const QString &fileName);
    void on_saveToolBtn_clicked();
    void on_clearToolBtn_clicked();
    void on_pushButton_2_clicked();

    void on_underlineToolBtn_clicked(bool checked);
};
#endif // WIDGET_H
