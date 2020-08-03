#ifndef DRAWER_H
#define DRAWER_H

#include<QToolBox>
#include<QToolButton>
#include"widget.h"


class Drawer : public QToolBox
{
	Q_OBJECT
public:
    Drawer(QWidget *parent = nullptr, Qt::WindowFlags f = nullptr);

private slots:
	void SlotShowChatWidget1();
	void SlotShowChatWidget2();
	void SlotShowChatWidget3();
	void SlotShowChatWidget4();
	void SlotShowChatWidget5();
	void SlotShowChatWidget6();
	void SlotShowChatWidget7();
	void SlotShowChatWidget8();
	void SlotShowChatWidget9();
private:
	QToolButton *m_pToolBtn1;
	QToolButton *m_pToolBtn2;
	QToolButton *m_pToolBtn3;
	QToolButton *m_pToolBtn4;
	QToolButton *m_pToolBtn5;
	QToolButton *m_pToolBtn6;
	QToolButton *m_pToolBtn7;
	QToolButton *m_pToolBtn8;
	QToolButton *m_pToolBtn9;

	Widget *m_pChatWidget1;
	Widget *m_pChatWidget2;
	Widget *m_pChatWidget3;
	Widget *m_pChatWidget4;
	Widget *m_pChatWidget5;
	Widget *m_pChatWidget6;
	Widget *m_pChatWidget7;
	Widget *m_pChatWidget8;
	Widget *m_pChatWidget9;
};

#endif // DRAWER_H
