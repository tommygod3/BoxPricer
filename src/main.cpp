#include <string>
#include <fstream>
#include <iostream>

//Use default values or enter in program
#define DEBUG 1

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
	void showOrderInfo();
};

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
	else
	{
		getBoxValues();
	}
}

//Function to get order input from console
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
	int intChecker1;
	int intChecker2;
	//Set up comparison variable
	int intCompare = 0;
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

void Order::getCustomerPrice()
{
	//Works out cost and price of order, displays through console
	//Temporary default pricing but also asks if want to customise these numbers
	std::string custom;
	//Temporary pricing values for example
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

void Order::showOrderInfo()
{
	//Displays order properties through console
	//Work out square meter of box
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
	a.doBlankSize("../data/Blanksize.txt");
	a.doStockSheet("../data/StockBoard.txt");
	a.showOrderInfo();
	a.getCustomerPrice();
	return 0;
}