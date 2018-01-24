#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>

//Use default values or enter in program
#define DEBUG 0

//Order or boxes
class Order
{
private:
	//Order inputted
	std::string flute;
	int paperWeight;
	std::string paperQuality;
	std::string style;
	double boxLength;
	double boxWidth;
	double boxHeight;
	int quantity;
	//To be worked out and returned
	double orderCost;
	double customerCost;

	double boxChop;
	double boxDecal;
	int allowance[2];

	double sheetChop;
	double sheetDecal;
	double sheetPrice;

public: 
	Order();
	void getBoxValues();
	void doBlankSize(std::string filename);
	void doStockSheet(std::string filename);
	void getCustomerPrice();
	void doOrderInfo();
};

Order::Order()
{
	if (DEBUG)
	{
		flute = "B";
		paperWeight = 125;
		paperQuality = "KT";
		style = "0200";
		boxLength = 305;
		boxWidth = 229;
		boxHeight = 229;
		quantity = 400;
	}
	else
	{
		getBoxValues();
	}
}

void Order::getBoxValues()
{
	std::cout << "Enter flute: ";
	std::cin >> flute;
	std::cout << std::endl;

	std::cout << "Enter Weight: ";
	std::cin >> paperWeight;
	std::cout << std::endl;

	std::cout << "Enter Quality: ";
	std::cin >> paperQuality;
	std::cout << std::endl;

	std::cout << "Enter Style: ";
	std::cin >> style;
	std::cout << std::endl;

	std::cout << "Enter Length: ";
	std::cin >> boxLength;
	std::cout << std::endl;

	std::cout << "Enter Width: ";
	std::cin >> boxWidth;
	std::cout << std::endl;

	std::cout << "Enter Height: ";
	std::cin >> boxHeight;
	std::cout << std::endl;

	std::cout << "Enter Quantity: ";
	std::cin >> quantity;
	std::cout << std::endl;
}

void Order::doBlankSize(std::string filename)
{
	std::ifstream blankIn;
	blankIn.open(filename);
	std::string checker;
	double paramCL, paramCW, paramCH, paramCAd, paramCA1, paramCA2;
	double paramDL, paramDW, paramDH, paramDAd, paramDA1, paramDA2;
	
	while ((checker != style)&&(!blankIn.eof()))
	{
		getline(blankIn, checker);
		blankIn >> checker;
	}
	if (blankIn.eof())
	{
		std::cout << "Style not found, check blanksize file." << std::endl;
		return;
	}
	blankIn >> paramCL >> paramCW >> paramCH >> paramCAd >> paramCA1 >> paramCA2 >> paramDL >> paramDW >> paramDH >> paramDAd >> paramDA1 >> paramDA2;
	allowance[0] = -1;
	allowance[1] = -1;
	while (!blankIn.eof())
	{
		blankIn >> checker;
		if (checker == flute)
		{
			blankIn >> allowance[0] >> allowance[1];
			break;
		}
		else
		{
			blankIn >> checker >> checker;
		}
	}
	if ((allowance[0]<0) || (allowance[1]<0))
	{
		std::cout << "Error calculating allowances, check "<< filename << std::endl;
		return;
	}
	boxChop = (paramCL*boxLength) + (paramCW*boxWidth) + (paramCH*boxHeight) + paramCAd + (paramCA1*allowance[0]) + (paramCA2*allowance[1]);
	boxDecal = (paramDL*boxLength) + (paramDW*boxWidth) + (paramDH*boxHeight) + paramDAd + (paramDA1*allowance[0]) + (paramDA2*allowance[1]);
}


void Order::doStockSheet(std::string filename)
{
	sheetChop = -1;
	std::ifstream stockIn;
	stockIn.open(filename);
	std::string strChecker1;
	std::string strChecker2;
	int intChecker1;
	int intChecker2;
	int intCompare = 0;
	getline(stockIn, strChecker1);
	while (!stockIn.eof())
	{
		stockIn >> strChecker1 >> intChecker1 >> strChecker2;
		if ((strChecker1 == flute) && (intChecker1 == paperWeight) && (strChecker2 == paperQuality))
		{
			stockIn >> intChecker1 >> intChecker2;
			if ((intChecker1 > intCompare)&&(intChecker1>boxChop)&&(intChecker2>boxDecal))
			{
				intCompare = intChecker1;
				sheetChop = intChecker1;
				sheetDecal = intChecker2;
				stockIn >> sheetPrice;
			}
			else
			{
				getline(stockIn, strChecker1);
			}
		}
		else
		{
			getline(stockIn, strChecker1);
		}
	}
	if (sheetChop<0)
	{
		std::cout << "Not found on stock board, check " << filename << std::endl;
		return;
	}
}

void Order::getCustomerPrice()
{
	std::string custom;
	//Temporary values for example
	double pricePerBox = 0.05;
	double priceOntop = 10;
	int boxPerSheet = (sheetChop / boxChop);
	orderCost = (quantity*sheetPrice) / boxPerSheet;
	std::cout << "Cost of order is " << orderCost << std::endl;
	std::cout << "Cost per box is " << orderCost / quantity << std::endl;
	customerCost = (pricePerBox*quantity) + priceOntop + orderCost;
	std::cout << "Price of order is currently " << customerCost << std::endl;
	std::cout << "Do you want to use custom pricing? y for yes, other for no: " << std::endl;
	std::cin >> custom;
	if ((custom == "y")||(custom == "Y"))
	{
		std::cout << "Enter price per box: ";
		std::cin >> pricePerBox;
		std::cout << std::endl;
		std::cout << "Enter price on top of order: ";
		std::cin >> priceOntop;
		std::cout << std::endl;
		customerCost = (pricePerBox*quantity) + priceOntop + orderCost;
		std::cout << "Price of order is now " << customerCost << std::endl;
	}
}

void Order::doOrderInfo()
{
	double sqMeterBox = ((boxChop*boxDecal) / 1000000);
	if ((style == "0200") || (style == "0201"))
	{
		std::cout << "1/2 Chop = " << ((boxChop / 2) + 24) << std::endl;
	}
	std::cout << "Chop = " << boxChop << std::endl;
	std::cout << "Decal = " << boxDecal << std::endl;
	std::cout << "Sq. meter of box = " << sqMeterBox << std::endl;
	std::cout << "Sq. meter of order = " << sqMeterBox * quantity << std::endl;
	std::cout << "Minimum quantity of boxes to buy board in = " << ((200 / sqMeterBox)+15) << std::endl;
}

int main()
{
	Order a;
	int b;
	a.doBlankSize("Blanksize.txt");
	a.doStockSheet("StockBoard.txt");
	a.doOrderInfo();
	a.getCustomerPrice();
	//To pause before close
	std::cin >> b;
	return 0;
}