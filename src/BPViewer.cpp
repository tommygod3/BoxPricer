#include "BPViewer.h"

BPViewer::BPViewer(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(QSize(461, 637));
	//In distribution folder make sure structured so can get this
	this->setWindowIcon(QIcon("../resources/box.png"));
	//Will give error message and close upon start up if order's
	//construction throws because missing data file
	try
	{
		order = new BP::Order;
	}
	catch (std::runtime_error &r)
	{
		showMessage(r.what());
		QTimer::singleShot(0, this, SLOT(close()));
	}

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

void BPViewer::showMessage(std::string text)
{
	QString message = QString::fromStdString(text);
	QMessageBox msg;
	msg.setWindowTitle("Error");
	msg.setText(message);
	//In distribution folder make sure structured so can get this
	msg.setIconPixmap(QPixmap("../resources/error.png"));
	msg.setWindowIcon(QIcon("../resources/box.png"));
	msg.exec();
}

void BPViewer::setFlute()
{
	//Get text from label
	QString inString = ui.lineEditFlute->text();
	//Set with converted QString
	try
	{
		order->setFlute(inString.toStdString());
		ui.tickFlute->setValue(1);
		
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}


void BPViewer::setPW()
{
	QString inString = ui.lineEditPW->text();
	try
	{
		order->setPaperWeight(inString.toStdString());
		ui.tickPW->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setPQ()
{
	QString inString = ui.lineEditPQ->text();
	try
	{
		order->setPaperQuality(inString.toStdString());
		ui.tickPQ->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setStyle()
{
	QString inString = ui.lineEditStyle->text();
	try
	{
		order->setStyle(inString.toStdString());
		ui.tickStyle->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setLength()
{
	QString inString = ui.lineEditLength->text();
	try
	{
		order->setBoxLength(inString.toStdString());
		ui.tickLength->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setWidth()
{
	QString inString = ui.lineEditWidth->text();
	try
	{
		order->setBoxWidth(inString.toStdString());
		ui.tickWidth->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setHeight()
{
	QString inString = ui.lineEditHeight->text();
	try
	{
		order->setBoxHeight(inString.toStdString());
		ui.tickHeight->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setQuantity()
{
	QString inString = ui.lineEditQuantity->text();
	try
	{
		order->setQuantity(inString.toStdString());
		ui.tickQuantity->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setPPB()
{
	QString inString = ui.lineEditPPB->text();
	try
	{
		order->setPricePerBox(inString.toStdString());
		ui.tickPPB->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

void BPViewer::setPOT()
{
	QString inString = ui.lineEditPOT->text();
	try
	{
		order->setPriceOnTop(inString.toStdString());
		ui.tickPOT->setValue(1);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
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
		showMessage(e.what());
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