#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTextBrowser>
#include<QTableWidget>
#include<QComboBox>
#include<QPushButton>
#include<QLabel>
#include<QToolButton>
#include"server.h"
#include"client.h"

enum MsgType 
{
	MESG,
	USRENTER,
	USERLEFT,
	FILENAME,
	REFUSE
};
class QUdpSocket;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString userName);
    ~Widget();

protected:
	void UserEnter(QString usrname, QString ipaddr); //处理新用户加入
	void UserLeft(QString usrname, QString time);//处理用户离开
	void SendMsg(MsgType type, QString srvaddr = "");  //广播UDP消息
    QString GetIP();//获取IP
    QString GetUsr();//获取用户名
    QString GetMsg();//获取聊天信息

    void HasPendingFile(QString usrname,QString srvAddr,QString clintAddr,QString filename);

private slots:
	void SlotProcessPendingDatagrams();   //接收UDP消息
    void SlotSendMsg();//发送消息
    void SlotGetFileName(QString);
    void SlotSendFile();

private:
    QTextBrowser *m_pTextBrower;
    QTextEdit *m_pTextEdit;
    QTableWidget *m_pTableWidget;
    QComboBox *m_pCombox;
    QPushButton *m_pSendBtn;
    QLabel *m_pUserNumLb;
    QPushButton *m_pExitBtn;
    QToolButton *m_pBoldTBtn;
    QToolButton *m_pItalicTBtn;
    QToolButton *m_pUnderLineTBtn;
    QToolButton *m_pColorTBtn;
    QToolButton *m_pSendTBtn;
    QToolButton *m_pSaveTBtn;
    QToolButton *m_pClearTBtn;

	QUdpSocket *m_pUdpSocket;
	qint16 m_nPort;
	QString m_sUsrName;

    QString m_fileName;
    Server *m_pServer;
};
#endif // WIDGET_H
