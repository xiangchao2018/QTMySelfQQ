#ifndef SERVER_H
#define SERVER_H

#include<QObject>
#include<QLabel>
#include<QPushButton>
#include<QProgressBar>
#include<QDialog>
#include<QTime>
#include <QVBoxLayout>

class QFile;
class QTcpServer;
class QTcpSocket;

class Server : public QDialog
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    void InitServer();
    void Refused();
protected:
    void closeEvent(QCloseEvent *);

signals:
    void SignalSendFileName(QString fileName);

private slots:
    void SlotSendMsg();
    void SlotUpdateClientProgress(qint64 numBytes);
    void SlotOpenFilePath();
    void SlotStartSendFile();
    void SlotCloseFileTransmitDialog();


private:

    //UI
    QLabel *m_pFileSelectLb;
    QPushButton *m_pOpenSelectBtn;
    QPushButton *m_pSendBtn;
    QProgressBar *m_pProgressBar;
    QLabel *m_pStatusLb;
    QPushButton *m_pCloseBtn;
    QVBoxLayout *m_pMainLayout;

    //server
    qint16 m_nPort;
    QTcpServer *m_pServer;
    QString m_fileName;
    QString m_theFileName;
    QFile *m_pLocFile;
    qint64 m_nTotalBytes;
    qint64 m_nBytesWritten;
    qint64 m_nBytesTobeWrite;
    qint64 m_nPayloadSize;
    QByteArray m_outBlock;   //缓存一次发送的数据
    QTcpSocket *m_pClientConn; //客户端连接套接字
    QTime time;
};

#endif // SERVER_H
