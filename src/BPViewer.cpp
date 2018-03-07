#include "BPViewer.h"

BPViewer::BPViewer(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	order = new BP::Order;

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(calculateValues()));

	connect(ui.lineEditFlute, SIGNAL(returnPressed()), this, SLOT(setFlute()));
	connect(ui.lineEditPW, SIGNAL(returnPressed()), this, SLOT(setPW()));
	connect(ui.lineEditPQ, SIGNAL(returnPressed()), this, SLOT(setPQ()));
	connect(ui.lineEditStyle, SIGNAL(returnPressed()), this, SLOT(setStyle()));
	connect(ui.lineEditLength, SIGNAL(returnPressed()), this, SLOT(setLength()));
	connect(ui.lineEditWidth, SIGNAL(returnPressed()), this, SLOT(setWidth()));
	connect(ui.lineEditHeight, SIGNAL(returnPressed()), this, SLOT(setHeight()));
	connect(ui.lineEditQuantity, SIGNAL(returnPressed()), this, SLOT(setQuantity()));
	connect(ui.lineEditPPB, SIGNAL(returnPressed()), this, SLOT(setPPB()));
	connect(ui.lineEditPOT, SIGNAL(returnPressed()), this, SLOT(setPOT()));
}

void BPViewer::setFlute()
{

}


void BPViewer::setPW()
{

}

void BPViewer::setPQ()
{

}

void BPViewer::setStyle()
{

}

void BPViewer::setLength()
{

}

void BPViewer::setWidth()
{

}

void BPViewer::setHeight()
{

}

void BPViewer::setQuantity()
{

}

void BPViewer::setPPB()
{

}

void BPViewer::setPOT()
{

}

void BPViewer::calculateValues()
{

}
	
/*
void BPViewer::multiplyNumber()
{
	QObject *origin;
	origin = sender();

	//Get text from label
	QString test = ui.label->text();
	//Set up to convert
	bool ok;
	//Convert
	int testInt = test.toInt(&ok, 10);
	//Calculate
	if (origin == ui.pushButton)
	{
		testInt = stuff->square(testInt);
	}
	if (origin == ui.pushButton1)
	{
		testInt = stuff->timesOne(testInt);
	}
	if (origin == ui.pushButton2)
	{
		testInt = stuff->timesTwo(testInt);
	}
	if (origin == ui.pushButton3)
	{
		testInt = stuff->timesThree(testInt);
	}

	//Turn back to QString
	test = QString::number(testInt);
	//Put back in label
	ui.label->setText(test);
}

void BPViewer::newNumber()
{
	QObject *origin;
	origin = sender();
	if (origin == ui.lineEdit)
	{
		QString entered = ui.lineEdit->text();
		ui.label->setText(entered);
		ui.lineEdit->clear();
	}
	if (origin == ui.lineEdit1)
	{
		QString entered = ui.lineEdit1->text();
		bool ok;
		stuff->intData1 = entered.toInt(&ok, 10);
		ui.label1->setText(entered);
		ui.lineEdit1->clear();
	}
	if (origin == ui.lineEdit2)
	{
		QString entered = ui.lineEdit2->text();
		bool ok;
		stuff->intData2 = entered.toInt(&ok, 10);
		ui.label2->setText(entered);
		ui.lineEdit2->clear();
	}
	if (origin == ui.lineEdit3)
	{
		QString entered = ui.lineEdit3->text();
		bool ok;
		stuff->intData3 = entered.toInt(&ok, 10);
		ui.label3->setText(entered);
		ui.lineEdit3->clear();
	}
}
*/