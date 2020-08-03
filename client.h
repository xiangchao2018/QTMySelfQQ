#ifndef CLIENT_H
#define CLIENT_H

#include<QDialog>
#include<QHostAddress>
#include<QFile>
#include<QTime>
#include<QLabel>
#include<QProgressBar>
#include<QPushButton>
#include<QTcpSocket>


class Client : public QDialog
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent=nullptr);
    ~Client();
    void SetHostAddr(QHostAddress addr);
    void SetFileName(QString name);

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void SlotNewConn();
    void SlotReadMsg();
    void SlotDisplayErr(QAbstractSocket::SocketError);
    void SlotCancelFileSend();
    void SlotCloseFileTransmitDialog();

private:
    QLabel *m_pProgressLb;
    QProgressBar *m_pProgressBar;
    QLabel *m_pWaitInfoLb;
    QPushButton *m_pCancleBtn;
    QPushButton *m_pCloseBtn;

    QTcpSocket *m_pClient;
    quint64 m_blockSize;
    QHostAddress m_hostAddr;
    qint16 m_nPort;
    qint64 m_TotalBytes;
    qint64 m_BytesReceived;
    qint64 m_fileNameSize;
    QString m_fileName;
    QFile *m_pLocFile;
    QByteArray m_inBlock;
    QTime m_time;
};

#endif // CLIENT_H
