#include "BPViewer.h"

BPViewer::BPViewer(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowState(Qt::WindowMaximized);

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
	//Get text from label
	QString inString = ui.lineEditFlute->text();
	//Set with converted QString
	try
	{
		order->setFlute(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}


void BPViewer::setPW()
{
	QString inString = ui.lineEditPW->text();
	try
	{
		order->setPaperWeight(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setPQ()
{
	QString inString = ui.lineEditPQ->text();
	try
	{
		order->setPaperQuality(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setStyle()
{
	QString inString = ui.lineEditStyle->text();
	try
	{
		order->setStyle(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setLength()
{
	QString inString = ui.lineEditLength->text();
	try
	{
		order->setBoxLength(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setWidth()
{
	QString inString = ui.lineEditWidth->text();
	try
	{
		order->setBoxWidth(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setHeight()
{
	QString inString = ui.lineEditHeight->text();
	try
	{
		order->setBoxHeight(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setQuantity()
{
	QString inString = ui.lineEditQuantity->text();
	try
	{
		order->setQuantity(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setPPB()
{
	QString inString = ui.lineEditPPB->text();
	try
	{
		order->setPricePerBox(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::setPOT()
{
	QString inString = ui.lineEditPOT->text();
	try
	{
		order->setPriceOnTop(inString.toStdString());
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
	}
}

void BPViewer::calculateValues()
{
	try
	{
		order->doAllCalculations();
	}
	catch (std::invalid_argument &e)
	{
		QString message = QString::fromStdString(e.what());
		QMessageBox msg;
		msg.setText("Error");
		msg.setInformativeText(message);
		msg.exec();
		return;
	}
	//Set:
	QString toString = QString::number(order->getBoxChop());
	ui.lineReadChop->setText(toString);

	toString = QString::number(order->getBoxDecal());
	ui.lineReadDecal->setText(toString);

	toString = QString::number(order->getOrderCost());
	ui.lineReadCost->setText(toString);

	toString = QString::number(order->getCustomerPrice());
	ui.lineReadPrice->setText(toString);

	toString = QString::fromStdString(order->generateInformation());
	ui.textInfo->setText(toString);
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