#include "widget.h"
#include<QGridLayout>
#include<QHBoxLayout>
#include<QUdpSocket>
#include<QHostInfo>
#include<QMessageBox>
#include<QScrollBar>
#include<QDateTime>
#include<QNetworkInterface>
#include<QProcess>
#include<QFileDialog>

Widget::Widget(QWidget *parent, QString userName)
{
    setFixedSize(730,650);

    m_pTextBrower=new QTextBrowser;
    m_pTextEdit=new QTextEdit;
    m_pTableWidget=new QTableWidget;
	m_pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	QStringList sListHeader;
	sListHeader << "usrname" << "ipaddr";
	m_pTableWidget->setColumnCount(2);
	m_pTableWidget->setHorizontalHeaderLabels(sListHeader);

    m_pBoldTBtn=new QToolButton;
    m_pSaveTBtn=new QToolButton;
    m_pSendTBtn=new QToolButton;
    QHBoxLayout *pTBtnLayout=new QHBoxLayout;
    pTBtnLayout->addWidget(m_pBoldTBtn);
    pTBtnLayout->addWidget(m_pSaveTBtn);
    pTBtnLayout->addWidget(m_pSendTBtn);

    m_pSendBtn=new QPushButton(tr("Send"));
    m_pUserNumLb=new QLabel(tr("Online number 0"));
    m_pExitBtn =new QPushButton(tr("Exit"));

    QGridLayout *mainLayout=new QGridLayout(this);
    QHBoxLayout *pBtnLayout=new QHBoxLayout;
    pBtnLayout->addWidget(m_pSendBtn);
    pBtnLayout->addWidget(m_pUserNumLb);
    pBtnLayout->addWidget(m_pExitBtn);
    mainLayout->addWidget(m_pTextBrower,0,0,4,1);
    mainLayout->addLayout(pTBtnLayout,4,0,1,1);
    mainLayout->addWidget(m_pTextEdit,5,0,2,1);
    mainLayout->addWidget(m_pTableWidget,0,1,6,1);
    mainLayout->addLayout(pBtnLayout,6,0,1,3);
    connect(m_pSendBtn,SIGNAL(clicked()),this,SLOT(SlotSendMsg()));
    connect(m_pSendTBtn,SIGNAL(clicked()),this,SLOT(SlotSendFile()));
	//connect(m_pExitBtn,SIGNAL(clicked()),this,SLOT(Slot));

    //发送UDP广播
    m_sUsrName=userName;
    m_pUdpSocket=new QUdpSocket(this);
    m_nPort=23232;
    m_pUdpSocket->bind(m_nPort,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(SlotProcessPendingDatagrams()));
    SendMsg(USRENTER);

    //传输文件
    m_pServer=new Server(this);
    connect(m_pServer,SIGNAL(SignalSendFileName(QString)),this,SLOT(SlotGetFileName(QString)));
}

Widget::~Widget()
{
}

void Widget::SendMsg(MsgType type, QString srvaddr)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString address=GetIP();
    out<<type<<GetUsr();
    switch (type)
    {
    case MESG:
        if(m_pTextEdit->toPlainText()=="")
        {
            QMessageBox::warning(nullptr,tr("warning"),tr("send content is empty!"),QMessageBox::Ok);
            return;
        }
        out<<address<<GetMsg();
        m_pTextBrower->verticalScrollBar()->setValue(m_pTextBrower->verticalScrollBar()->maximum());
        break;
    case USRENTER:
        out<<address;
        break;
    case USERLEFT:
        break;
    case FILENAME:
    {
        int row =m_pTableWidget->currentRow();
        QString clientAddr=m_pTableWidget->item(row,1)->text();
        out<<address<<clientAddr<<m_fileName;
        break;
    }
    case REFUSE:
        out<<srvaddr;
        break;
    default:
        break;
    }

    m_pUdpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,m_nPort);
}

void Widget::SlotProcessPendingDatagrams()
{
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_pUdpSocket->pendingDatagramSize());
		m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;
        QString usrName,ipAddr,msg;
        QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch (msgType)
        {
        case MESG:
            in>>usrName>>ipAddr>>msg;
            m_pTextBrower->setTextColor(Qt::blue);
            m_pTextBrower->setCurrentFont(QFont("Times New Roman",12));
            m_pTextBrower->append("["+usrName+"]"+time);
            m_pTextBrower->append(msg);
            break;
        case USRENTER:
            in>>usrName>>ipAddr;
            UserEnter(usrName,ipAddr);
            break;
        case USERLEFT:
            in>>usrName>>ipAddr;
            UserLeft(usrName,ipAddr);
            break;
        case FILENAME:
        {
            in>>usrName>>ipAddr;
            QString clientAddr,fileName;
            in>>clientAddr>>fileName;
            HasPendingFile(usrName,ipAddr,clientAddr,fileName);
            break;
        }
        case REFUSE:
        {
            in>>usrName;
            QString srvAddr;
            in>>srvAddr;
            QString ipAddr=GetIP();
            if(ipAddr==srvAddr)
            {
                m_pServer->Refused();
            }
            break;
        }
        }
    }
}

void Widget::SlotSendMsg()
{
    SendMsg(MESG);
}

void Widget::UserEnter(QString usrname, QString ipaddr)
{
    bool bIsEmpty=m_pTableWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
    if(bIsEmpty)
    {
        QTableWidgetItem *pUser=new QTableWidgetItem(usrname);
        QTableWidgetItem *pIP=new QTableWidgetItem(ipaddr);
        m_pTableWidget->insertRow(0);
        m_pTableWidget->setItem(0,0,pUser);
        m_pTableWidget->setItem(0,1,pIP);
        m_pTextBrower->setTextColor(Qt::gray);
        m_pTextBrower->setCurrentFont(QFont("Times New Roman",10));
        m_pTextBrower->append(tr("%1 online!").arg(usrname));
        m_pUserNumLb->setText(tr("Online num: %1").arg(m_pTableWidget->rowCount()));
        SendMsg(USRENTER);
    }
}

void Widget::UserLeft(QString usrname, QString time)
{
    int rowNum=m_pTableWidget->findItems(usrname,Qt::MatchExactly).first()->row();
    m_pTableWidget->removeRow(rowNum);
    m_pTextBrower->setTextColor(Qt::gray);
    m_pTextBrower->setCurrentFont(QFont("Times New Roman",10));
    m_pTextBrower->append(tr("%1 leave at %2 !").arg(usrname).arg(time));
    m_pTextBrower->setText(tr("Online num: %1").arg(m_pTableWidget->rowCount()));
}

QString Widget::GetIP()
{
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
       if(addr.protocol()==QAbstractSocket::IPv4Protocol)
       {
           return addr.toString();
       }
    }
    return 0;
}

QString Widget::GetUsr()
{
    return m_sUsrName;
}

QString Widget::GetMsg()
{
    QString msg=m_pTextEdit->toHtml();
    m_pTextEdit->clear();
    m_pTextEdit->setFocus();
    return msg;
}

void Widget::SlotGetFileName(QString name)
{
    m_fileName=name;
    SendMsg(FILENAME);
}

void Widget::SlotSendFile()
{
    if(m_pTableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(NULL,tr("warning"),tr("Please choose usr first..."),QMessageBox::Yes);
        return;
    }
    m_pServer->show();
    m_pServer->InitServer();
}

void Widget::HasPendingFile(QString usrname, QString srvAddr, QString clintAddr, QString filename)
{
    QString ipAddr=GetIP();
    if(ipAddr==clintAddr)
    {
        int btn=QMessageBox::information(this,tr("Receive file"),tr("from %1(%2) file :%3 , receive or not ?").arg(usrname).arg(clintAddr).arg(filename),QMessageBox::Yes|QMessageBox::No);
        if(btn==QMessageBox::Yes)
        {
            QString name=QFileDialog::getSaveFileName(0,tr("save file"),filename);
            if(!name.isEmpty())
            {
                Client *pClient=new Client(this);
                pClient->SetFileName(name);
                pClient->SetHostAddr(QHostAddress(srvAddr));
                pClient->show();
            }
            else
            {
                SendMsg(REFUSE,srvAddr);
            }
        }
    }
}
