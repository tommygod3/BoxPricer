#include <fstream>
#include <iostream>

#include "order.h"

//Use default values or enter in program
#define DEBUG 1
//Enter in program with console or buttons
#define UI 0

Order::Order()
{
	//Makes an order with preset default values if debugging
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
	//If not debugging get values
	else if (!UI)
	{
		consoleInput();
	}
}

void Order::setFlute(std::string desiredFlute)
{
    flute = desiredFlute;
}

void Order::setPaperWeight(unsigned int desiredWeight)
{
    paperWeight = desiredWeight;
}

void Order::setPaperQuality(std::string desiredQuality)
{
    paperQuality = desiredQuality;
}
    
void Order::setStyle(std::string desiredStyle)
{
    style = desiredStyle;
}

void Order::setBoxLength(double desiredLength)
{
    boxLength = desiredLength;
}

void Order::setBoxWidth(double desiredWidth)
{
    boxWidth = desiredWidth;
}

void Order::setBoxHeight(double desiredHeight)
{
    boxHeight = desiredHeight;
}

void Order::setQuantity(unsigned int desiredQuantity)
{
    quantity = desiredQuantity;
}

void Order::doBlankSize(std::string filename)
{
	//Open file
	std::ifstream blankIn;
	blankIn.open(filename);
	//Set up parsing string 'checker'
	std::string checker;
	//Set up arithmetic variables to read into
	double paramCL, paramCW, paramCH, paramCAd, paramCA1, paramCA2;
	double paramDL, paramDW, paramDH, paramDAd, paramDA1, paramDA2;
	//Loop through file until we find the desired style in the
	//blanksize file, or we reach the end and it is not found
	while ((checker != style)&&(!blankIn.eof()))
	{
		//Discards previous line (or first line of just headings)
		getline(blankIn, checker);
		//Put first word (style) of the line into parser for comparison
		blankIn >> checker;
	}
	//Return with error if our desired style is not in the file
	if (blankIn.eof())
	{
		std::cout << "Style not found, check blanksize file." << std::endl;
		return;
	}
	//Read in the rest of the values to their appropriate variables
	blankIn >> paramCL >> paramCW >> paramCH >> paramCAd >> paramCA1 >> paramCA2 >> paramDL >> paramDW >> paramDH >> paramDAd >> paramDA1 >> paramDA2;
	//Set up variables for blanksizes
	allowance[0] = -1;
	allowance[1] = -1;
	//Go through flute values relevant to this style until we find the appropriate one
	while (blankIn.peek() != '\n')
	{
		blankIn >> checker;
		//This is what we want so assign allowances that follow it
		if (checker == flute)
		{
			blankIn >> allowance[0] >> allowance[1];
			break;
		}
		//Skip to next flute
		else
		{
			blankIn >> checker >> checker;
		}
	}
	//We did't find the flute we want so return with error
	if ((allowance[0]<0) || (allowance[1]<0))
	{
		std::cout << "Error calculating allowances, check "<< filename << std::endl;
		return;
	}
	//Do calculation here and set values
	boxChop = (paramCL*boxLength) + (paramCW*boxWidth) + (paramCH*boxHeight) + paramCAd + (paramCA1*allowance[0]) + (paramCA2*allowance[1]);
	boxDecal = (paramDL*boxLength) + (paramDW*boxWidth) + (paramDH*boxHeight) + paramDAd + (paramDA1*allowance[0]) + (paramDA2*allowance[1]);
}


void Order::doStockSheet(std::string filename)
{
	//Initialise variable to -1 so can detect if not found
	sheetChop = -1;
	//Set up file to read in from
	std::ifstream stockIn;
	stockIn.open(filename);
	//Set up parsing variables
	std::string strChecker1;
	std::string strChecker2;
	unsigned int intChecker1;
	unsigned int intChecker2;
	//Set up comparison variable
	unsigned int intCompare = 0;
	//Get rid of first line with headings
	getline(stockIn, strChecker1);
	//Loop through file until we find matching flute, pweight and pquality
	while (!stockIn.eof())
	{
		stockIn >> strChecker1 >> intChecker1 >> strChecker2;
		if ((strChecker1 == flute) && (intChecker1 == paperWeight) && (strChecker2 == paperQuality))
		{
			stockIn >> intChecker1 >> intChecker2;
			//Currently checks for biggest chop and decal and stores those
			if ((intChecker1 > intCompare)&&(intChecker1>boxChop)&&(intChecker2>boxDecal))
			{
				intCompare = intChecker1;
				sheetChop = intChecker1;
				sheetDecal = intChecker2;
				stockIn >> sheetPrice;
			}
			//If not successful throw away rest of line
			else
			{
				getline(stockIn, strChecker1);
			}
		}
		//If not successful throw away rest of line
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

void Order::doPricing()
{
	unsigned int boxPerSheet = (sheetChop / boxChop);
	orderCost = (quantity*sheetPrice) / boxPerSheet;
	customerPrice = (pricePerBox*quantity) + priceOnTop + orderCost;
}

std::string Order::generateInformation()
{
    double sqMeterOfBox = (boxChop*boxDecal)/1000000;
    std::string info = "Square meter of box: " + std::to_string(sqMeterOfBox) +
    "\nSquare meter of order: " + std::to_string((sqMeterOfBox*quantity)) + 
    "\nMinimum quantity of boxes to buy board in: " + std::to_string(((200/sqMeterOfBox)+15));
    return info;
}

void Order::doAllCalculations()
{
    doBlankSize("../data/Blanksize.txt");
	doStockSheet("../data/StockBoard.txt");
    doPricing();
}

void Order::consoleDisplay()
{
    std::cout << "Chop: " << boxChop << std::endl;
    std::cout << "Decal: " << boxDecal << std::endl;
    std::cout << "Cost of order: " << orderCost << std::endl;
    std::cout << "Price of order: " << customerPrice << std::endl;
    std::cout << generateInformation() << std::endl;
}

void Order::consoleInput()
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