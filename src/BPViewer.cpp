#include "BPViewer.h"

BPViewer::BPViewer(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	//Make sure size right
	QRect rec = QApplication::desktop()->screenGeometry();
	int height = rec.height();
	if (height > 800)
	{
		this->setFixedSize(QSize(533, 763));
	}
	else
	{
		this->setFixedSize(QSize(533, ((height*9)/ 10)));
	}
	//In distribution folder make sure structured so can get this
	this->setWindowIcon(QIcon("../resources/boxpic.png"));
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

	connect(ui.comboBoxFlute, SIGNAL(activated(int)), this, SLOT(setFlute()));
	connect(ui.comboBoxPW, SIGNAL(activated(int)), this, SLOT(setPW()));
	connect(ui.comboBoxPQ, SIGNAL(activated(int)), this, SLOT(setPQ()));
	connect(ui.comboBoxStyle, SIGNAL(activated(int)), this, SLOT(setStyle()));
	connect(ui.spinLength, SIGNAL(editingFinished()), this, SLOT(setLength()));
	connect(ui.spinWidth, SIGNAL(editingFinished()), this, SLOT(setWidth()));
	connect(ui.spinHeight, SIGNAL(editingFinished()), this, SLOT(setHeight()));
	connect(ui.spinQuantity, SIGNAL(editingFinished()), this, SLOT(setQuantity()));
	connect(ui.doubleSpinPPB, SIGNAL(editingFinished()), this, SLOT(setPPB()));
	connect(ui.doubleSpinPOT, SIGNAL(editingFinished()), this, SLOT(setPOT()));
	connect(ui.comboBoxFullHalf, SIGNAL(currentIndexChanged(int)), this, SLOT(setFullPolicy()));
	connect(ui.radioButtonRush, SIGNAL(clicked()), this, SLOT(setForceSheets()));

	//Add styles
	ui.comboBoxStyle->addItem("");
	std::vector<std::string> styles = order->availableStyles();
	for (unsigned int i = 0; i < styles.size(); i++)
	{
		ui.comboBoxStyle->addItem(QString::fromStdString(styles[i]));
	}
	//Temp override change backend 0 to "custom"
	int overrideIndex = ui.comboBoxStyle->findText("0");
	ui.comboBoxStyle->setItemText(overrideIndex, "Custom");
	//Add default full chop only option
	ui.comboBoxFullHalf->addItem("Full Chop Only");
	//TODO: Set up first 3 correctly
	resetFluteList();
	resetPWList();
	resetPQList();
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
	msg.setWindowIcon(QIcon("../resources/boxpic.png"));
	msg.exec();
}

void BPViewer::resetFluteList()
{
	QString before = ui.comboBoxFlute->currentText();
	
	ui.comboBoxFlute->clear();
	ui.comboBoxFlute->addItem("");
	std::vector<std::string> fluteVector = order->getValidInputs(0);
	for (unsigned int i = 0; i < fluteVector.size(); i++)
	{
		ui.comboBoxFlute->addItem(QString::fromStdString(fluteVector[i]));
	}
	if (ui.comboBoxFlute->findText(before) != -1)
	{
		if (ui.comboBoxFlute->findText(before) != 0)
		{
			ui.comboBoxFlute->setCurrentIndex(ui.comboBoxFlute->findText(before));
		}
		else
		{
			order->resetFlute();
			ui.tickFlute->setValue(0);
		}
	}
}

void BPViewer::resetPWList()
{
	QString before = ui.comboBoxPW->currentText();

	ui.comboBoxPW->clear();
	ui.comboBoxPW->addItem("");
	std::vector<std::string> PWVector = order->getValidInputs(1);
	for (unsigned int i = 0; i < PWVector.size(); i++)
	{
		ui.comboBoxPW->addItem(QString::fromStdString(PWVector[i]));
	}
	if (ui.comboBoxPW->findText(before) != -1)
	{
		if (ui.comboBoxPW->findText(before) != 0)
		{
			ui.comboBoxPW->setCurrentIndex(ui.comboBoxPW->findText(before));
		}
		else
		{
			order->resetPaperWeight();
			ui.tickPW->setValue(0);
		}
	}
}

void BPViewer::resetPQList()
{
	QString before = ui.comboBoxPQ->currentText();

	ui.comboBoxPQ->clear();
	ui.comboBoxPQ->addItem("");
	std::vector<std::string> PQVector = order->getValidInputs(2);
	for (unsigned int i = 0; i < PQVector.size(); i++)
	{
		ui.comboBoxPQ->addItem(QString::fromStdString(PQVector[i]));
	}
	if (ui.comboBoxPQ->findText(before) != -1)
	{
		if (ui.comboBoxPQ->findText(before) != 0)
		{
			ui.comboBoxPQ->setCurrentIndex(ui.comboBoxPQ->findText(before));
		}
		else
		{
			order->resetPaperQuality();
			ui.tickPQ->setValue(0);
		}
	}
}

void BPViewer::setFullPolicy()
{
	int index = ui.comboBoxFullHalf->currentIndex();
	if (index == -1 || !order->checkStyleSet())
	{
		return;
	}
	try
	{
		order->setFullHalf(index);
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
}

//TODO: 3 set functions get vector from getValidInputs, when 1 is set, reset 2 and 3, etc...
void BPViewer::setFlute()
{
	QString fluteIn = ui.comboBoxFlute->currentText();
	if (fluteIn == "" && order->checkFluteSet())
	{
		ui.tickFlute->setValue(0);
		order->resetFlute();
	}
	else if (fluteIn == "" && !order->checkFluteSet())
	{
		return;
	}
	else
	{
		try
		{
			order->setFlute(fluteIn.toStdString());
			ui.tickFlute->setValue(1);
		}
		catch (std::invalid_argument &e)
		{
			showMessage(e.what());
		}
	}
	resetPWList();
	resetPQList();
}


void BPViewer::setPW()
{
	QString PWIn = ui.comboBoxPW->currentText();
	if (PWIn == "" && order->checkPaperWeightSet())
	{
		ui.tickPW->setValue(0);
		order->resetPaperWeight();
	}
	else if (PWIn == "" && !order->checkPaperWeightSet())
	{
		return;
	}
	else
	{
		try
		{
			order->setPaperWeight(PWIn.toStdString());
			ui.tickPW->setValue(1);
		}
		catch (std::invalid_argument &e)
		{
			showMessage(e.what());
		}
	}
	resetFluteList();
	resetPQList();
}

void BPViewer::setPQ()
{
	QString PQIn = ui.comboBoxPQ->currentText();
	if (PQIn == "" && order->checkPaperQualitySet())
	{
		ui.tickPQ->setValue(0);
		order->resetPaperQuality();
	}
	else if (PQIn == "" && !order->checkPaperQualitySet())
	{
		return;
	}
	else
	{
		try
		{
			order->setPaperQuality(PQIn.toStdString());
			ui.tickPQ->setValue(1);
		}
		catch (std::invalid_argument &e)
		{
			showMessage(e.what());
		}
	}
	resetFluteList();
	resetPWList();
}

void BPViewer::setStyle()
{
	QString styleIn = ui.comboBoxStyle->currentText();
	//Temp override "custom" means 0
	if (styleIn == "Custom")
	{
		styleIn = "0";
	}
	if (styleIn == "")
	{
		ui.tickStyle->setValue(0);
		order->resetStyle();
		ui.comboBoxFullHalf->clear();
		ui.comboBoxFullHalf->addItem("Full Chop Only");
		ui.comboBoxFullHalf->setCurrentIndex(0);
		return;
	}
	try
	{
		order->setStyle(styleIn.toStdString());
		ui.tickStyle->setValue(1);
		if (order->styleHasHalf())
		{
			ui.comboBoxFullHalf->clear();
			ui.comboBoxFullHalf->addItem("Full Chop Only");
			ui.comboBoxFullHalf->addItem("Half Chop Only");
			ui.comboBoxFullHalf->addItem("Full and Half Chop");
			ui.comboBoxFullHalf->setCurrentIndex(0);
		}
		else if (!order->styleHasHalf())
		{
			ui.comboBoxFullHalf->clear();
			ui.comboBoxFullHalf->addItem("Full Chop Only");
			ui.comboBoxFullHalf->setCurrentIndex(0);
		}
	}
	catch (std::invalid_argument &e)
	{
		showMessage(e.what());
	}
	//Override check:
	if (styleIn == "0")
	{
		order->setBoxHeight("1");
		ui.tickHeight->setValue(1);
		QMessageBox msg;
		msg.setText("Warning");
		msg.setInformativeText("Using custom sheet: Length will be interpreted as desired Chop and Width as desired Decal, Height will be ignored.");
		msg.setWindowIcon(QIcon("../resources/boxpic.png"));
		msg.exec();
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

void BPViewer::setForceSheets()
{
	bool valueIn = ui.radioButtonRush->isChecked();
	order->setForceSheets(valueIn);
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
	//Reset full and half as sometimes empty
	QString toString = "";
	ui.lineReadChop->setText(toString);
	ui.lineReadHalfChop->setText(toString);
	//Set:
	if (order->getChopCounts().first > 0)
	{
		toString = QString::number(order->getBoxChop());
		toString += " mm";
		ui.lineReadChop->setText(toString);
	}
	
	if (order->getChopCounts().second > 0)
	{
		toString = QString::number(order->getBoxHalfChop());
		toString += " mm";
		ui.lineReadHalfChop->setText(toString);
	}

	toString = QString::number(order->getBoxDecal());
	toString += " mm";
	ui.lineReadDecal->setText(toString);

	toString = QString::number(order->getOrderCost(),'f',3);
	toString = QString(163) + toString;
	ui.lineReadCost->setText(toString);

	toString = QString::number(order->getCustomerPrice(),'f',3);
	toString = QString(163) + toString;
	ui.lineReadPrice->setText(toString);

	toString = QString::number(order->getCustomerPricePer(), 'f', 3);
	toString = QString(163) + toString;
	ui.lineReadPricePer->setText(toString);

	toString = "Square metre of box: " + QString::number(order->sqMetBox(), 'f', 3) + " m" + QChar(0x00B2) + "\n";
	toString += "Square metre of order: " + QString::number(order->sqMetOrder(), 'f', 3) + " m" + QChar(0x00B2) + "\n\n";
	toString += "Cost per box: " + QString(163) + QString::number(order->getOrderCostPer(), 'f', 3) + "\n\n";
	if (order->getPricingTier() == 0)
	{
		toString += "Sheet used's info:\nChop: " + QString::number(order->getSheetChop(), 'f', 0) + " mm\n";
		toString += "Decal: " + QString::number(order->getSheetDecal(), 'f', 0) + " mm\n";
		toString += "Cost per sheet: " + QString(163) + QString::number(order->getSheetPrice(), 'f', 3) + "\n";
		toString += "Number of sheets used: " + QString::number(order->noOfSheets(), 'f', 0) + "\n\n";
	}
	else
	{
		toString += "Board to order in's info:\nChop: " + QString::number(order->getSheetChop(), 'f', 0) + " mm\n";
		toString += "Decal: " + QString::number(order->getSheetDecal(), 'f', 0) + " mm\n";
		toString += "Cost per sheet: " + QString(163) + QString::number(order->getSheetPrice(), 'f', 3) + "\n";
		toString += "Number to order in: " + QString::number(order->noOfSheets(), 'f', 0) + "\n\n";
	}
	if ((order->getPolicy() == 0 || order->getPolicy() == 2)&&(order->getChopCounts().first != 0))
	{
		toString += "Number of full chops per sheet: " + QString::number(order->getChopCounts().first) + "\n";
	}
	if ((order->getPolicy() == 1 || order->getPolicy() == 2) && (order->getChopCounts().second != 0))
	{
		toString += "Number of half chops per sheet: " + QString::number(order->getChopCounts().second) + "\n";
	}
	toString += "Number of decals per sheet: " + QString::number(order->getDecalCount()) + "\n\n";
	if (order->getCheaperTier() != 0)
	{
		toString += "Minimum quantity of boxes to next pricing tier: " + QString::number(order->quantBoxNeeded());
	}
	//Style only override check:
	if (order->sheetNoBox())
	{
		toString = "Square metre of sheet: " + QString::number(order->sqMetBox(), 'f', 3) + " m" + QChar(0x00B2) + "\n";
		toString += "Square metre of order: " + QString::number(order->sqMetOrder(), 'f', 3) + " m" + QChar(0x00B2) + "\n\n";
		if (order->getPricingTier() == 0)
		{
			toString += "Sheet used's info:\nChop: " + QString::number(order->getSheetChop(), 'f', 0) + " mm\n";
			toString += "Decal: " + QString::number(order->getSheetDecal(), 'f', 0) + " mm\n";
			toString += "Cost per sheet: " + QString(163) + QString::number(order->getSheetPrice(), 'f', 3) + "\n";
			toString += "Number of sheets used: " + QString::number(order->noOfSheets(), 'f', 0) + "\n\n";
		}
		else
		{
			toString += "Board to order in's info:\nChop: " + QString::number(order->getSheetChop(), 'f', 0) + " mm\n";
			toString += "Decal: " + QString::number(order->getSheetDecal(), 'f', 0) + " mm\n";
			toString += "Cost per sheet: " + QString(163) + QString::number(order->getSheetPrice(), 'f', 3) + "\n";
			toString += "Number to order in: " + QString::number(order->noOfSheets(), 'f', 0) + "\n\n";
		}
		if (order->getCheaperTier() != 0)
		{
			toString += "Minimum quantity of sheets to next pricing tier: " + QString::number(order->quantBoxNeeded());
		}
	}
	ui.textInfo->setText(toString);
}

void BPViewer::resetValues()
{
	QMessageBox resetBox;
	resetBox.setWindowIcon(QIcon("../resources/boxpic.png"));
	resetBox.setText("Are you sure you want to reset all values?");
	resetBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	resetBox.setDefaultButton(QMessageBox::No);
	int ret = resetBox.exec(); 
	if (ret == QMessageBox::Yes)
	{
		ui.comboBoxFlute->setCurrentIndex(0);
		ui.tickFlute->setValue(0);

		ui.comboBoxPW->setCurrentIndex(0);
		ui.tickPW->setValue(0);

		ui.comboBoxPQ->setCurrentIndex(0);
		ui.tickPQ->setValue(0);

		ui.comboBoxStyle->setCurrentIndex(0);
		ui.tickStyle->setValue(0);

		ui.spinLength->setValue(0);
		ui.tickLength->setValue(0);

		ui.spinWidth->setValue(0);
		ui.tickWidth->setValue(0);

		ui.spinHeight->setValue(0);
		ui.tickHeight->setValue(0);

		ui.spinQuantity->setValue(0);
		ui.tickQuantity->setValue(0);

		ui.comboBoxFullHalf->clear();
		ui.comboBoxFullHalf->addItem("Full Chop Only");
		ui.comboBoxFullHalf->setCurrentIndex(0);
		ui.tickFullHalf->setValue(1);

		ui.doubleSpinPPB->setValue(0.30);
		ui.tickPPB->setValue(1);

		ui.doubleSpinPOT->setValue(1.50);
		ui.tickPOT->setValue(1);

		ui.radioButtonRush->setChecked(0);

		ui.lineReadChop->setText("");
		ui.lineReadHalfChop->setText("");
		ui.lineReadDecal->setText("");
		ui.lineReadCost->setText("");
		ui.lineReadPrice->setText("");
		ui.lineReadPricePer->setText("");
		ui.textInfo->setText("");

		order->resetAllValues();
		//Resets lists
		resetFluteList();
		resetPWList();
		resetPQList();
	}
}