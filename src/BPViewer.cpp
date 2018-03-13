#include "BPViewer.h"

BPViewer::BPViewer(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(QSize(461, 682));
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
	

	connect(ui.pushButtonCalc, SIGNAL(clicked()), this, SLOT(calculateValues()));
	connect(ui.pushButtonReset, SIGNAL(clicked()), this, SLOT(resetValues()));

	connect(ui.lineEditFlute, SIGNAL(editingFinished()), this, SLOT(setFlute()));
	connect(ui.lineEditPW, SIGNAL(editingFinished()), this, SLOT(setPW()));
	connect(ui.lineEditPQ, SIGNAL(editingFinished()), this, SLOT(setPQ()));
	connect(ui.lineEditStyle, SIGNAL(editingFinished()), this, SLOT(setStyle()));
	connect(ui.spinLength, SIGNAL(editingFinished()), this, SLOT(setLength()));
	connect(ui.spinWidth, SIGNAL(editingFinished()), this, SLOT(setWidth()));
	connect(ui.spinHeight, SIGNAL(editingFinished()), this, SLOT(setHeight()));
	connect(ui.spinQuantity, SIGNAL(editingFinished()), this, SLOT(setQuantity()));
	connect(ui.doubleSpinPPB, SIGNAL(editingFinished()), this, SLOT(setPPB()));
	connect(ui.doubleSpinPOT, SIGNAL(editingFinished()), this, SLOT(setPOT()));
}

void BPViewer::showMessage(std::string text)
{
	QString message = QString::fromStdString(text);
	QMessageBox msg;
	//msg.setWindowTitle("Error");
	msg.setText("Error");
	msg.setInformativeText(message);
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
	QString inString = ui.spinLength->cleanText();
	if (ui.spinLength->cleanText() != "0")
	{
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
}

void BPViewer::setWidth()
{
	QString inString = ui.spinWidth->cleanText();
	if (ui.spinWidth->cleanText() != "0")
	{
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
}

void BPViewer::setHeight()
{
	QString inString = ui.spinHeight->cleanText();
	if (ui.spinHeight->cleanText() != "0")
	{
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
}

void BPViewer::setQuantity()
{
	QString inString = ui.spinQuantity->cleanText();
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
	QString inString = ui.doubleSpinPPB->cleanText();
	if (ui.doubleSpinPPB->cleanText() != "0.05")
	{
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
}

void BPViewer::setPOT()
{
	QString inString = ui.doubleSpinPOT->cleanText();
	if (ui.doubleSpinPOT->cleanText() != "100.00")
	{
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
	toString += " mm";
	ui.lineReadChop->setText(toString);

	toString = QString::number(order->getBoxDecal());
	toString += " mm";
	ui.lineReadDecal->setText(toString);

	toString = QString::number(order->getOrderCost(),'f',2);
	toString = QString(163) + toString;
	ui.lineReadCost->setText(toString);

	toString = QString::number(order->getCustomerPrice(),'f',2);
	toString = QString(163) + toString;
	ui.lineReadPrice->setText(toString);

	toString = "Square metre of box: " + QString::number(order->sqMetBox(),'g',5) + " m" + QChar(0x00B2) + "\n\n";
	toString += "Square metre of order: " + QString::number(order->sqMetOrder(),'g',5) + " m" + QChar(0x00B2) + "\n\n";
	toString += "Minimum quantity of boxes to buy board in: " + QString::number(order->quantBoxNeeded());
	ui.textInfo->setText(toString);
}

void BPViewer::resetValues()
{
	ui.lineEditFlute->setText("");
	ui.tickFlute->setValue(0);

	ui.lineEditPW->setText("");
	ui.tickPW->setValue(0);

	ui.lineEditPQ->setText("");
	ui.tickPQ->setValue(0);

	ui.lineEditStyle->setText("");
	ui.tickStyle->setValue(0);

	ui.spinLength->setValue(0);
	ui.tickLength->setValue(0);

	ui.spinWidth->setValue(0);
	ui.tickWidth->setValue(0);

	ui.spinHeight->setValue(0);
	ui.tickHeight->setValue(0);

	ui.spinQuantity->setValue(0);
	ui.tickQuantity->setValue(0);

	ui.doubleSpinPPB->setValue(0.05);
	ui.tickPPB->setValue(1);

	ui.doubleSpinPOT->setValue(100);
	ui.tickPOT->setValue(1);

	ui.lineReadChop->setText("");
	ui.lineReadDecal->setText("");
	ui.lineReadCost->setText("");
	ui.lineReadPrice->setText("");
	ui.textInfo->setText("");

	order->resetAllValues();
}