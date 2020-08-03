#include "server.h"
#include<QHBoxLayout>
#include<QTcpServer>
#include<QTcpSocket>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
#include<QFileDevice>
#include<QtDebug>
#include<QCoreApplication>
#include<QHostAddress>


Server::Server(QWidget *parent) : QDialog(parent)
{
    //UI
    m_pFileSelectLb=new QLabel(tr("Select File/Folder"));
    m_pOpenSelectBtn=new QPushButton(tr("Open..."));
    m_pSendBtn=new QPushButton(tr("Send"));
    m_pProgressBar=new QProgressBar;
    m_pStatusLb=new QLabel(tr("Please Select need send file!"));
    m_pCloseBtn=new QPushButton(tr("Close"));

    QHBoxLayout *pOneLayout=new QHBoxLayout;
    pOneLayout->addWidget(m_pFileSelectLb);
    pOneLayout->addWidget(m_pOpenSelectBtn);
    QHBoxLayout*pTwoLayout=new QHBoxLayout;
    pTwoLayout->addWidget(m_pSendBtn);
    pTwoLayout->addWidget(m_pProgressBar);

    m_pMainLayout=new QVBoxLayout(this);
    m_pMainLayout->addLayout(pOneLayout);
    m_pMainLayout->addLayout(pTwoLayout);
    m_pMainLayout->addWidget(m_pStatusLb);
    m_pMainLayout->addWidget(m_pCloseBtn);
    connect(m_pOpenSelectBtn,SIGNAL(clicked()),this,SLOT(SlotOpenFilePath()));
    connect(m_pSendBtn,SIGNAL(clicked()),this,SLOT(SlotStartSendFile()));

    setFixedSize(400,207);
    m_nPort=5555;
    m_pServer=new QTcpServer(this);
    connect(m_pServer,SIGNAL(newConnection()),this,SLOT(SlotSendMsg()));
    InitServer();
}

Server::~Server()
{

}

void Server::InitServer()
{
    m_nPayloadSize=64*1024;
    m_nTotalBytes=0;
    m_nBytesWritten=0;
    m_nBytesTobeWrite=0;
    m_pProgressBar->reset();
    m_pOpenSelectBtn->setEnabled(true);
    m_pSendBtn->setEnabled(false);
    m_pServer->close();
}


void Server::SlotSendMsg()
{
    m_pSendBtn->setEnabled(false);
    m_pClientConn=m_pServer->nextPendingConnection();
    connect(m_pClientConn,SIGNAL(bytesWritten(qint64)),this,SLOT(SlotUpdateClientProgress(qint64)));
    m_pStatusLb->setText(tr("Start transmit file %1 !").arg(m_theFileName));
    m_pLocFile=new QFile(m_fileName);
    if(!m_pLocFile->open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,tr("warning"),tr("Can not read file %1:\n%2").arg(m_fileName).arg(m_pLocFile->errorString()));
        return;
    }
    m_nTotalBytes=m_pLocFile->size();
    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_12);
    time.start();
    QString curFile=m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);

    sendOut<<qint64(0)<<qint64(0)<<curFile;
    m_nTotalBytes+=m_outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<m_nTotalBytes<<qint64(m_outBlock.size()-sizeof (qint64)*2);
    m_nBytesTobeWrite=m_nTotalBytes-m_pClientConn->write(m_outBlock);
    m_outBlock.resize(0);
}

void Server::SlotUpdateClientProgress(qint64 numBytes)
{
    QCoreApplication::processEvents();
    m_nBytesWritten+=(int)numBytes;
    if(m_nBytesTobeWrite>0)
    {
        m_outBlock=m_pLocFile->read(qMin(m_nBytesTobeWrite,m_nPayloadSize));
        m_nBytesTobeWrite-=(int)m_pClientConn->write(m_outBlock);
        m_outBlock.resize(0);
    }
    else
    {
        m_pLocFile->close();
    }

    m_pProgressBar->setMaximum(m_nTotalBytes);
    m_pProgressBar->setValue(m_nBytesWritten);
    float useTime=time.elapsed();
    double speed=m_nBytesWritten/useTime;
    m_pStatusLb->setText(tr("Have send %1MB(%2MB/s) \n total %3MB have use time: %4 s").arg(m_nBytesWritten/(1024*1024)).arg(speed*1000/(1024*1024),0,'f',2).arg(m_nTotalBytes/(1024*1024)).arg(useTime/1000,0,'f',0));
    if(m_nBytesWritten==m_nTotalBytes)
    {
        m_pLocFile->close();
        m_pServer->close();
        m_pStatusLb->setText(tr("Send file %1 success!").arg(m_fileName));
    }
}

void Server::SlotOpenFilePath()
{
    m_fileName=QFileDialog::getOpenFileName(this);
    if(!m_fileName.isEmpty())
    {
        m_theFileName=m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);
        m_pStatusLb->setText(tr("need transmit file : %1 ").arg(m_theFileName));
        m_pSendBtn->setEnabled(true);
        m_pOpenSelectBtn->setEnabled(false);
    }
}

void Server::SlotStartSendFile()
{
    if(!m_pServer->listen(QHostAddress::Any,m_nPort))
    {
        qDebug()<<m_pServer->errorString();
        close();
        return;
    }

    m_pStatusLb->setText(tr("Waiting other to receive..."));
    emit SignalSendFileName(m_theFileName);
}

void Server::SlotCloseFileTransmitDialog()
{
    if(m_pServer->isListening())
    {
        m_pServer->close();
        if(m_pLocFile->isOpen())
        {
            m_pLocFile->close();
        }
        m_pClientConn->abort();
    }
    close();
}

void Server::closeEvent(QCloseEvent *)
{
    SlotCloseFileTransmitDialog();
}

void Server::Refused()
{
    m_pServer->close();
    m_pStatusLb->setText(tr("The other refuse receive file!"));
}

