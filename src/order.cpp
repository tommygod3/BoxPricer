#include <fstream>
#include <iostream>
#include <stdexcept>

#include "order.h"

//Use default values or enter in program
#define AUTO 0
//interact with program using console or buttons
#define GUI 0

Order::Order()
{
	//Uses console input if not using GUI
	if (!GUI)
	{
		//Use default values that work
		if (AUTO)
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
		//Use console to input:
		else
		{
			consoleInput();
		}
		//Process and return console output:
		//Check values in are set
		if (checkSet() != "Passed")
		{
			std::cout<<checkSet()<<std::endl;
		}
		else
		{
			//Check file checking works and return if not
			try 
			{
				doAllCalculations();
			}
			catch(std::invalid_argument &e)
			{
				//Tel user in console what error is
				std::cout<<e.what()<<std::endl;
				return;
			}
			//Display output in console
			consoleDisplay();
		}
	}
}

void Order::setFlute(std::string desiredFlute)
{
	if (desiredFlute.size()>2)
	{
		throw std::invalid_argument("Flute selection invalid: too many characters");
	}
	for (unsigned int i=0; i<desiredFlute.size(); i++)
	{
		if (!isalpha(desiredFlute.at(i)))
		{
			throw std::invalid_argument("Flute selection invalid: non-alphabetical");
		}
		desiredFlute.at(i)=toupper(desiredFlute.at(i));
	}
    flute = desiredFlute;
}

void Order::setPaperWeight(std::string desiredWeight)
{
	for (unsigned int i=0; i<desiredWeight.size();i++)
	{
		if (!isdigit(desiredWeight.at(i)))
		{
			throw std::invalid_argument("Paper Weight selection invalid: not an integer");
		}
	}
	if (std::stoi(desiredWeight)<0)
	{
		throw std::invalid_argument("Paper Weight selection invalid: can't be negative");
	}
    paperWeight = std::stoul(desiredWeight);
}

void Order::setPaperQuality(std::string desiredQuality)
{
	for (unsigned int i= 0; i<desiredQuality.size();i++)
	{
		if (!isalpha(desiredQuality.at(i)))
		{
			throw std::invalid_argument("Paper Quality selection invalid: non-alphabetical");
		}
		desiredQuality.at(i) = toupper(desiredQuality.at(i));
	}
    paperQuality = desiredQuality;
}
    
void Order::setStyle(std::string desiredStyle)
{
	for (unsigned int i = 0; i<desiredStyle.size();i++)
	{
		if (!isdigit(desiredStyle.at(i)))
		{
			throw std::invalid_argument("Style selection invalid: not an integer");
		}
	}
	if (std::stoi(desiredStyle)<0)
	{
		throw std::invalid_argument("Style selection invalid: can't be negative");
	}
    style = desiredStyle;
}

void Order::setBoxLength(std::string desiredLength)
{
	for (unsigned int i=0; i<desiredLength.size();i++)
	{
		if (!isdigit(desiredLength.at(i)))
		{
			throw std::invalid_argument("Box Length selection invalid: not an integer");
		}
	}
	if (std::stoi(desiredLength)<0)
	{
		throw std::invalid_argument("Box Length selection invalid: can't be negative");
	}
    boxLength = std::stod(desiredLength);
}

void Order::setBoxWidth(std::string desiredWidth)
{
	for (unsigned int i=0; i<desiredWidth.size();i++)
	{
		if (!isdigit(desiredWidth.at(i)))
		{
			throw std::invalid_argument("Box Width selection invalid: not an integer");
		}
	}
	if (std::stoi(desiredWidth)<0)
	{
		throw std::invalid_argument("Box Width selection invalid: can't be negative");
	}
    boxWidth = std::stod(desiredWidth);
}

void Order::setBoxHeight(std::string desiredHeight)
{
	for (unsigned int i=0; i<desiredHeight.size();i++)
	{
		if (!isdigit(desiredHeight.at(i)))
		{
			throw std::invalid_argument("Box Height selection invalid: not an integer");
		}
	}
	if (std::stoi(desiredHeight)<0)
	{
		throw std::invalid_argument("Box Height selection invalid: can't be negative");
	}
    boxHeight = std::stod(desiredHeight);
}

void Order::setQuantity(std::string desiredQuantity)
{
	for (unsigned int i=0; i<desiredQuantity.size();i++)
	{
		if (!isdigit(desiredQuantity.at(i)))
		{
			throw std::invalid_argument("Quantity selection invalid: not an integer");
		}
	}
	if (std::stoi(desiredQuantity)<0)
	{
		throw std::invalid_argument("Quantity selection invalid: can't be negative");
	}
    quantity = std::stoi(desiredQuantity);
}

void Order::setPricePerBox(std::string desiredPricePer)
{
	unsigned int dotCount = 0;
	for (unsigned int i=0; i<desiredPricePer.size();i++)
	{
		if (!isdigit(desiredPricePer.at(i)))
		{
			if (desiredPricePer.at(i)!='.'|| dotCount>0)
			{
				throw std::invalid_argument("Price per box selection invalid: not a number");
			}
			else
			{
				dotCount++;
			}
		}
	}
	if (std::stoi(desiredPricePer)<0)
	{
		throw std::invalid_argument("Price per box selection invalid: can't be negative");
	}
	pricePerBox = std::stod(desiredPricePer);
}

void Order::setPriceOnTop(std::string desiredPriceOnTop)
{
	unsigned int dotCount = 0;
	for (unsigned int i=0; i<desiredPriceOnTop.size();i++)
	{
		if (!isdigit(desiredPriceOnTop.at(i)))
		{
			if (desiredPriceOnTop.at(i)!='.'|| dotCount>0)
			{
				throw std::invalid_argument("Price on top selection invalid: not a number");
			}
			else
			{
				dotCount++;
			}
		}
	}
	if (std::stoi(desiredPriceOnTop)<0)
	{
		throw std::invalid_argument("Price on top selection invalid: can't be negative");
	}
	priceOnTop = std::stod(desiredPriceOnTop);
}

double Order::getOrderCost()
{
	return orderCost;
}

double Order::getCustomerPrice()
{
	return customerPrice;
}

double Order::getBoxChop()
{
	return boxChop;
}

double Order::getBoxDecal()
{
	return boxDecal;
}

void Order::doAllCalculations()
{	
	doBlankSize("../data/Blanksize.txt");
	doStockSheet("../data/StockBoard.txt");
	doPricing();
}

std::string Order::checkSet()
{
	if (flute == "")
	{
		return "Flute not set";
	}
	if (paperWeight == 0)
	{
		return "Paper Weight not set";
	}
	if (paperQuality == "")
	{
		return "Paper Quality not set";
	}
	if (style == "")
	{
		return "Style not set";
	}
	if (boxLength == 0)
	{
		return "Length not set";
	}
	if (boxWidth == 0)
	{
		return "Width not set";
	}
	if (boxHeight == 0)
	{
		return "Height not set";
	}
	if (quantity == -1)
	{
		return "Quantity not set";
	}
	return "Passed";
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

void Order::consoleDisplay()
{
	std::cout<< "--------------------Calculated Values:--------------------"<<std::endl;
    std::cout << "Chop: " << getBoxChop() << std::endl;
    std::cout << "Decal: " << getBoxDecal() << std::endl;
    std::cout << "Cost of order: " << getOrderCost() << std::endl;
    std::cout << "Price of order: " << getCustomerPrice() << std::endl;
    std::cout << generateInformation() << std::endl;
}

void Order::consoleInput()
{
    std::cout << "Flute: ";
	std::string fluteIn;
	std::cin >> fluteIn;
	try
	{
		setFlute(fluteIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Weight: ";
	std::string weightIn;
	std::cin >> weightIn;
	try
	{
		setPaperWeight(weightIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Quality: ";
	std::string qualityIn;
	std::cin >> qualityIn;
	try
	{
		setPaperQuality(qualityIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Style: ";
	std::string styleIn;
	std::cin >> styleIn;
	try
	{
		setStyle(styleIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}
	
	std::cout << "Length: ";
	std::string lengthIn;
	std::cin >> lengthIn;
	try
	{
		setBoxLength(lengthIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Width: ";
	std::string widthIn;
	std::cin >> widthIn;
	try
	{
		setBoxWidth(widthIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Height: ";
	std::string heightIn;
	std::cin >> heightIn;
	try
	{
		setBoxHeight(heightIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Quantity: ";
	std::string quantityIn;
	std::cin >> quantityIn;
	try
	{
		setQuantity(quantityIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Price per box: ";
	std::string pricePerIn;
	std::cin >> pricePerIn;
	try
	{
		setPricePerBox(pricePerIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}

	std::cout << "Price on top: ";
	std::string priceOnTopIn;
	std::cin >> priceOnTopIn;
	try
	{
		setPriceOnTop(priceOnTopIn);
	}
	catch(std::invalid_argument &e)
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<"Restarting input"<<std::endl;
		consoleInput();
		return;
	}
}