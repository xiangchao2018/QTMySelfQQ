#include "client.h"

#include<QDebug>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QHBoxLayout>

Client::Client(QWidget *parent):QDialog(parent)
{
    QVBoxLayout *pMainLayout=new QVBoxLayout(this);
    m_pProgressLb=new QLabel(tr("Have send :"));
    m_pProgressBar=new QProgressBar;
    m_pWaitInfoLb=new QLabel;
    m_pCancleBtn=new QPushButton(tr("Cancel"));
    m_pCloseBtn=new QPushButton(tr("Close"));

    QHBoxLayout *pOneLayout=new QHBoxLayout;
    pOneLayout->addWidget(m_pProgressLb);
    pOneLayout->addWidget(m_pProgressBar);

    QHBoxLayout *pBtnLayout=new QHBoxLayout;
    pBtnLayout->addWidget(m_pCancleBtn);
    pBtnLayout->addWidget(m_pCloseBtn);

    pMainLayout->addLayout(pOneLayout);
    pMainLayout->addWidget(m_pWaitInfoLb);
    pMainLayout->addLayout(pBtnLayout);

    connect(m_pCancleBtn,SIGNAL(clicked()),this,SLOT(SlotCancelFileSend()));
    connect(m_pCloseBtn,SIGNAL(clicked()),this,SLOT(SlotCloseFileTransmitDialog()));

    setFixedSize(400,190);
    m_TotalBytes=0;
    m_BytesReceived=0;
    m_fileNameSize=0;
    m_pClient =new QTcpSocket(this);
    m_nPort=5555;
    connect(m_pClient,SIGNAL(readyRead()),this,SLOT(SlotReadMsg()));
    connect(m_pClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SlotDisplayErr(QAbstractSocket::SocketError)));
}

Client::~Client()
{

}

void Client::SlotDisplayErr(QAbstractSocket::SocketError sockErr)
{
    switch (sockErr)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
    default:
        qDebug()<<m_pClient->errorString();
    }
}

void Client::SlotNewConn()
{
    m_blockSize=0;
    m_pClient->abort();
    m_pClient->connectToHost(m_hostAddr,m_nPort);
    m_time.start();
}

void Client::SlotReadMsg()
{
    QDataStream in(m_pClient);
    in.setVersion(QDataStream::Qt_5_12);
    float useTime=m_time.elapsed();
    if(m_BytesReceived<=sizeof (qint64)*2)
    {
        if((m_pClient->bytesAvailable()>=sizeof(qint64)*2) &&(m_fileNameSize==0))
        {
            in>>m_TotalBytes>>m_fileNameSize;
            m_BytesReceived+=sizeof(qint64)*2;
        }
        if((m_pClient->bytesAvailable()>=sizeof(qint64)*2) &&(m_fileNameSize!=0))
        {
            in>>m_fileName;
            m_BytesReceived+=m_fileNameSize;
            if(!m_pLocFile->open(QFile::WriteOnly))
            {
                QMessageBox::warning(this,tr("warning"),tr("Can not read file %1:\n %2").arg(m_fileName).arg(m_pLocFile->errorString()));
                return;
            }
        }
        else{
            return;
        }
    }
    if(m_BytesReceived<m_TotalBytes)
    {
        m_BytesReceived+=m_pClient->bytesAvailable();
        m_inBlock=m_pClient->readAll();
        m_pLocFile->write(m_inBlock);
        m_inBlock.resize(0);
    }
    m_pProgressBar->setMaximum(m_TotalBytes);
    m_pProgressBar->setValue(m_BytesReceived);
    double speed =m_BytesReceived/useTime;
    m_pProgressLb->setText(tr("Have recevied %1MB(%2MB/s) \n").arg(m_BytesReceived/(1024*1024)).arg(speed*1000/(1024*1024)));
    if(m_BytesReceived==m_TotalBytes)
    {
        m_pLocFile->close();
        m_pClient->close();
        m_pProgressLb->setText(tr("Have received %1 success!").arg(m_fileName));
    }
}

void Client::SlotCancelFileSend()
{
    m_pClient->abort();
    if(m_pLocFile->isOpen())
    {
        m_pLocFile->close();
    }
}

void Client::SlotCloseFileTransmitDialog()
{
    m_pClient->abort();
    if(m_pLocFile->isOpen())
    {
        m_pLocFile->close();
    }
    close();
}

void Client::closeEvent(QCloseEvent *)
{
    SlotCloseFileTransmitDialog();
}

void Client::SetFileName(QString name)
{
    m_pLocFile=new QFile(name);
}

void Client::SetHostAddr(QHostAddress addr)
{
    m_hostAddr=addr;
    SlotNewConn();
}
