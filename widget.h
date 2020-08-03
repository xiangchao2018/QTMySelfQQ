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
	void UserEnter(QString usrname, QString ipaddr); //�������û�����
	void UserLeft(QString usrname, QString time);//�����û��뿪
	void SendMsg(MsgType type, QString srvaddr = "");  //�㲥UDP��Ϣ
    QString GetIP();//��ȡIP
    QString GetUsr();//��ȡ�û���
    QString GetMsg();//��ȡ������Ϣ

    void HasPendingFile(QString usrname,QString srvAddr,QString clintAddr,QString filename);

private slots:
	void SlotProcessPendingDatagrams();   //����UDP��Ϣ
    void SlotSendMsg();//������Ϣ
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
