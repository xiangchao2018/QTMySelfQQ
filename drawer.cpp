#include "drawer.h"
#include<QGroupBox>
#include<QVBoxLayout>

Drawer::Drawer(QWidget *parent,Qt::WindowFlags f):QToolBox(parent,f)
{
    setWindowTitle(tr("MySelf QQ 2020"));
    setWindowIcon(QPixmap(":/image/qq.jpg"));
	QSize size(60,60);
    m_pToolBtn1=new QToolButton;
    m_pToolBtn1->setText(tr("water magic"));
    m_pToolBtn1->setIcon(QPixmap(":/image/1.jpg"));
    m_pToolBtn1->setIconSize(size);
    m_pToolBtn1->setAutoRaise(true);
    m_pToolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn1,SIGNAL(clicked()),this,SLOT(SlotShowChatWidget1()));

    m_pToolBtn2=new QToolButton;
    m_pToolBtn2->setText(tr("nihao"));
    m_pToolBtn2->setIcon(QPixmap(":/image/2.jpg"));
    m_pToolBtn2->setIconSize(size);
    m_pToolBtn2->setAutoRaise(true);
    m_pToolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn2, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget2()));

    m_pToolBtn3=new QToolButton;
    m_pToolBtn3->setText(tr("cherry"));
    m_pToolBtn3->setIcon(QPixmap(":/image/3.jpg"));
    m_pToolBtn3->setIconSize(size);
    m_pToolBtn3->setAutoRaise(true);
    m_pToolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn3, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget3()));

    m_pToolBtn4=new QToolButton;
    m_pToolBtn4->setText(tr("misswa"));
    m_pToolBtn4->setIcon(QPixmap(":/image/4.jpg"));
    m_pToolBtn4->setIconSize(size);
    m_pToolBtn4->setAutoRaise(true);
    m_pToolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn4, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget4()));

    m_pToolBtn5=new QToolButton;
    m_pToolBtn5->setText(tr("haihai"));
    m_pToolBtn5->setIcon(QPixmap(":/image/5.jpg"));
    m_pToolBtn5->setIconSize(size);
    m_pToolBtn5->setAutoRaise(true);
    m_pToolBtn5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn5, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget5()));

    m_pToolBtn6=new QToolButton;
    m_pToolBtn6->setText(tr("wawawa"));
    m_pToolBtn6->setIcon(QPixmap(":/image/6.jpg"));
    m_pToolBtn6->setIconSize(size);
    m_pToolBtn6->setAutoRaise(true);
    m_pToolBtn6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn6, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget6()));

    m_pToolBtn7=new QToolButton;
    m_pToolBtn7->setText(tr("yyyyy"));
    m_pToolBtn7->setIcon(QPixmap(":/image/7.jpg"));
    m_pToolBtn7->setIconSize(size);
    m_pToolBtn7->setAutoRaise(true);
    m_pToolBtn7->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn7, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget7()));

    m_pToolBtn8=new QToolButton;
    m_pToolBtn8->setText(tr("LLLLLL"));
    m_pToolBtn8->setIcon(QPixmap(":/image/8.jpg"));
    m_pToolBtn8->setIconSize(size);
    m_pToolBtn8->setAutoRaise(true);
    m_pToolBtn8->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn8, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget8()));

    m_pToolBtn9=new QToolButton;
    m_pToolBtn9->setText(tr("aaaaaa"));
    m_pToolBtn9->setIcon(QPixmap(":/image/9.jpg"));
    m_pToolBtn9->setIconSize(size);
    m_pToolBtn9->setAutoRaise(true);
    m_pToolBtn9->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_pToolBtn9, SIGNAL(clicked()), this, SLOT(SlotShowChatWidget9()));

    QGroupBox *groupBox=new QGroupBox;
    QVBoxLayout *layout=new QVBoxLayout(groupBox);

    layout->setMargin(20);
    layout->setAlignment(Qt::AlignLeft);
    layout->addWidget(m_pToolBtn1);
    layout->addWidget(m_pToolBtn2);
    layout->addWidget(m_pToolBtn3);
    layout->addWidget(m_pToolBtn4);
    layout->addWidget(m_pToolBtn5);
    layout->addWidget(m_pToolBtn6);
    layout->addWidget(m_pToolBtn7);
    layout->addWidget(m_pToolBtn8);
    layout->addWidget(m_pToolBtn9);
    layout->addStretch();
    this->addItem(groupBox,tr("gruop member"));
	setMinimumSize(450, 960);
}

void Drawer::SlotShowChatWidget1()
{
    m_pChatWidget1 = new Widget(nullptr,m_pToolBtn1->text());
	m_pChatWidget1->setWindowTitle(m_pToolBtn1->text());
	m_pChatWidget1->setWindowIcon(m_pToolBtn1->icon());
	m_pChatWidget1->show();
}

void Drawer::SlotShowChatWidget2()
{
    m_pChatWidget2 = new Widget(nullptr,m_pToolBtn2->text());
	m_pChatWidget2->setWindowTitle(m_pToolBtn2->text());
	m_pChatWidget2->setWindowIcon(m_pToolBtn2->icon());
	m_pChatWidget2->show();
}

void Drawer::SlotShowChatWidget3()
{
    m_pChatWidget3 = new Widget(nullptr,m_pToolBtn3->text());
	m_pChatWidget3->setWindowTitle(m_pToolBtn3->text());
	m_pChatWidget3->setWindowIcon(m_pToolBtn3->icon());
	m_pChatWidget3->show();
}

void Drawer::SlotShowChatWidget4()
{
    m_pChatWidget4 = new Widget(nullptr,m_pToolBtn4->text());
	m_pChatWidget4->setWindowTitle(m_pToolBtn4->text());
	m_pChatWidget4->setWindowIcon(m_pToolBtn4->icon());
	m_pChatWidget4->show();
}

void Drawer::SlotShowChatWidget5()
{
    m_pChatWidget5 = new Widget(nullptr,m_pToolBtn5->text());
	m_pChatWidget5->setWindowTitle(m_pToolBtn5->text());
	m_pChatWidget5->setWindowIcon(m_pToolBtn5->icon());
	m_pChatWidget5->show();
}

void Drawer::SlotShowChatWidget6()
{
    m_pChatWidget6 = new Widget(nullptr,m_pToolBtn6->text());
	m_pChatWidget6->setWindowTitle(m_pToolBtn6->text());
	m_pChatWidget6->setWindowIcon(m_pToolBtn6->icon());
	m_pChatWidget6->show();
}

void Drawer::SlotShowChatWidget7()
{
    m_pChatWidget7 = new Widget(nullptr,m_pToolBtn7->text());
	m_pChatWidget7->setWindowTitle(m_pToolBtn7->text());
	m_pChatWidget7->setWindowIcon(m_pToolBtn7->icon());
	m_pChatWidget7->show();
}

void Drawer::SlotShowChatWidget8()
{
    m_pChatWidget8 = new Widget(nullptr,m_pToolBtn8->text());
	m_pChatWidget8->setWindowTitle(m_pToolBtn8->text());
	m_pChatWidget8->setWindowIcon(m_pToolBtn8->icon());
	m_pChatWidget8->show();
}

void Drawer::SlotShowChatWidget9()
{
    m_pChatWidget9 = new Widget(nullptr,m_pToolBtn9->text());
	m_pChatWidget9->setWindowTitle(m_pToolBtn9->text());
	m_pChatWidget9->setWindowIcon(m_pToolBtn9->icon());
	m_pChatWidget9->show();
}
