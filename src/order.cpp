#include <fstream>
#include <iostream>
#include <stdexcept>

#include "order.h"

namespace BP
{
	//Use default values or enter in program
    #define AUTO 0
	//Interact with program using console or buttons
    #define GUI 1

	Order::Order()
	{
		blanksizes = new Blanksizes();
		stockboard = new Stockboard();
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
			//Check file checking works and return if not
			try 
			{
				doAllCalculations();
			}
			catch(std::invalid_argument &e)
			{
				//Tell user in console what error is
				std::cout<<e.what()<<std::endl;
				return;
			}
			//Display output in console
			consoleDisplay();
		}
	}

	Order::~Order()
	{
		delete blanksizes;
		delete stockboard;
	}

	void Order::resetAllValues()
	{
		flute = "";
		paperWeight = 0;
		paperQuality = "";
		style = "";
		boxLength = 0;
		boxWidth = 0;
		boxHeight = 0;
		quantity = -1;
		pricePerBox = 0.05;
		priceOnTop = 100.00;
	}

	void Order::setFlute(std::string desiredFlute)
	{
		if (desiredFlute.size()>3)
		{
			throw std::invalid_argument("Flute selection invalid: too many characters");
		}
		if (desiredFlute.size()==0)
		{
			throw std::invalid_argument("Flute selection invalid: entry is empty");
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
		if (desiredWeight.size() == 0)
		{
			throw std::invalid_argument("Paper Weight selection invalid: entry is empty");
		}
		for (unsigned int i=0; i<desiredWeight.size();i++)
		{
			if (!isdigit(desiredWeight.at(i)))
			{
				throw std::invalid_argument("Paper Weight selection invalid: not an integer");
			}
		}
		paperWeight = std::stoul(desiredWeight);
	}

	void Order::setPaperQuality(std::string desiredQuality)
	{
		if (desiredQuality.size() == 0)
		{
			throw std::invalid_argument("Paper Quality selection invalid: entry is empty");
		}
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
		if (desiredStyle.size() == 0)
		{
			throw std::invalid_argument("Style selection invalid: entry is empty");
		}
		for (unsigned int i = 0; i<desiredStyle.size();i++)
		{
			if (!isdigit(desiredStyle.at(i)))
			{
				throw std::invalid_argument("Style selection invalid: not an integer");
			}
			desiredStyle.at(i) = toupper(desiredStyle.at(i));
		}
		style = desiredStyle;
	}

	void Order::setBoxLength(std::string desiredLength)
	{
		if (desiredLength.size() == 0)
		{
			throw std::invalid_argument("Box Length selection invalid: entry is empty");
		}
		for (unsigned int i=0; i<desiredLength.size();i++)
		{
			if (!isdigit(desiredLength.at(i)))
			{
				throw std::invalid_argument("Box Length selection invalid: not an integer");
			}
		}
		if (std::stod(desiredLength) <= 0)
		{
			throw std::invalid_argument("Box Length selection invalid: not a positive integer");
		}
		boxLength = std::stod(desiredLength);
	}

	void Order::setBoxWidth(std::string desiredWidth)
	{
		if (desiredWidth.size() == 0)
		{
			throw std::invalid_argument("Box Width selection invalid: entry is empty");
		}
		for (unsigned int i=0; i<desiredWidth.size();i++)
		{
			if (!isdigit(desiredWidth.at(i)))
			{
				throw std::invalid_argument("Box Width selection invalid: not an integer");
			}
		}
		if (std::stod(desiredWidth) <= 0)
		{
			throw std::invalid_argument("Box Width selection invalid: not a positive integer");
		}
		boxWidth = std::stod(desiredWidth);
	}

	void Order::setBoxHeight(std::string desiredHeight)
	{
		if (desiredHeight.size() == 0)
		{
			throw std::invalid_argument("Box Height selection invalid: entry is empty");
		}
		for (unsigned int i=0; i<desiredHeight.size();i++)
		{
			if (!isdigit(desiredHeight.at(i)))
			{
				throw std::invalid_argument("Box Height selection invalid: not an integer");
			}
		}
		if (std::stod(desiredHeight) <= 0)
		{
			throw std::invalid_argument("Box Height selection invalid: not a positive integer");
		}
		boxHeight = std::stod(desiredHeight);
	}

	void Order::setQuantity(std::string desiredQuantity)
	{
		if (desiredQuantity.size() == 0)
		{
			throw std::invalid_argument("Quantity selection invalid: entry is empty");
		}
		for (unsigned int i=0; i<desiredQuantity.size();i++)
		{
			if (!isdigit(desiredQuantity.at(i)))
			{
				throw std::invalid_argument("Quantity selection invalid: not an integer");
			}
		}
		quantity = std::stoi(desiredQuantity);
	}

	void Order::setPricePerBox(std::string desiredPricePer)
	{
		if (desiredPricePer.size() == 0)
		{
			throw std::invalid_argument("Price per box selection invalid: entry is empty");
		}
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
		pricePerBox = std::stod(desiredPricePer);
	}

	void Order::setPriceOnTop(std::string desiredPriceOnTop)
	{
		if (desiredPriceOnTop.size() == 0)
		{
			throw std::invalid_argument("Price on top selection invalid: entry is empty");
		}
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
		if (checkSet()!="Passed")
		{
			throw std::invalid_argument("Missing values: " + checkSet());
		}
		doBlankSize();
		doStockSheet();
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

	void Order::doBlankSize()
	{
		//Using blanksize structure:
		//Throws if inputs not found in file
		if (blanksizes->getMatch(style,flute).first<0)
		{
			throw std::invalid_argument("Error with inputted values: no match found for Style and Flute in Blanksize file");
		}
		//Get location in data structure using style and flute
		unsigned int indexNo = blanksizes->getMatch(style,flute).first;
		unsigned int allowanceNo = blanksizes->getMatch(style,flute).second;
		//Set allowances from data structure
		allowance[0] = blanksizes->blanksizeList[indexNo].bAllowances[allowanceNo].second[0];
		allowance[1] = blanksizes->blanksizeList[indexNo].bAllowances[allowanceNo].second[1];
		//Set chop from data structure
		boxChop = (blanksizes->blanksizeList[indexNo].bParameters[0]*boxLength);
		boxChop += (blanksizes->blanksizeList[indexNo].bParameters[1]*boxWidth);
		boxChop += (blanksizes->blanksizeList[indexNo].bParameters[2]*boxHeight);
		boxChop += blanksizes->blanksizeList[indexNo].bParameters[3];
		boxChop += (blanksizes->blanksizeList[indexNo].bParameters[4]*allowance[0]);
		boxChop += (blanksizes->blanksizeList[indexNo].bParameters[5]*allowance[1]);
		//Set decal from data structure
		boxDecal = (blanksizes->blanksizeList[indexNo].bParameters[6]*boxLength);
		boxDecal += (blanksizes->blanksizeList[indexNo].bParameters[7]*boxWidth);
		boxDecal += (blanksizes->blanksizeList[indexNo].bParameters[8]*boxHeight);
		boxDecal += blanksizes->blanksizeList[indexNo].bParameters[9];
		boxDecal += (blanksizes->blanksizeList[indexNo].bParameters[10]*allowance[0]);
		boxDecal += (blanksizes->blanksizeList[indexNo].bParameters[11]*allowance[1]);
	}


	void Order::doStockSheet()
	{
		//Using stockboard structure:
		//Read in positions of matches to vector
		std::vector<int> matches = stockboard->getMatches(flute,paperWeight,paperQuality);
		//Initialise comparing variables to do maths with and choose best sheet
		double cChop = -1;
		double cDecal = -1;
		double cPrice;
		if (matches.size()==0)
		{
			throw std::invalid_argument("Error with inputted values: no match found for Flute, Paper Weight and Paper Quality in Stockboard file");
		}
		for (unsigned int i = 0; i < matches.size(); i++)
		{
			int match = matches[i];
			//Checks for biggest chop
			if (stockboard->theStockboard[match].sSheetChop > cChop)
			{
				cChop = stockboard->theStockboard[match].sSheetChop;
				cDecal = stockboard->theStockboard[match].sSheetDecal;
				cPrice = stockboard->theStockboard[match].sSheetPrice;
			}
		}
		if (cChop < boxChop)
		{
			throw std::invalid_argument("Error with inputted values: no matched sheet in Stockboard file has chop big enough to fit " + std::to_string(boxChop));
		}
		if (cDecal < boxDecal)
		{
			throw std::invalid_argument("Error with inputted values: no matched sheet in Stockboard file has decal big enough to fit " + std::to_string(boxDecal));
		}
		sheetChop = cChop;
		sheetDecal = cDecal;
		sheetPrice = cPrice;
	}

	void Order::doPricing()
	{
		unsigned int boxPerSheet = (sheetChop / boxChop);
		unsigned int noOfSheets = ceil(quantity / boxPerSheet);
		orderCost = noOfSheets * sheetPrice;
		customerPrice = (pricePerBox*quantity) + priceOnTop + orderCost;
	}

	std::string Order::generateInformation()
	{
		double sqMeterOfBox = (boxChop*boxDecal)/1000000;
		std::string info = "Square metre of box: " + std::to_string(sqMeterOfBox) +
		"\n\nSquare metre of order: " + std::to_string((sqMeterOfBox*quantity)) + 
		"\n\nMinimum quantity of boxes to buy board in: " + std::to_string(int(ceil((200/sqMeterOfBox)+15)));
		return info;
	}

	void Order::consoleDisplay()
	{
		std::cout<< "--------------------Outputted Values:--------------------"<<std::endl;
		std::cout << "Chop: " << getBoxChop() << std::endl;
		std::cout << "Decal: " << getBoxDecal() << std::endl;
		std::cout << "Cost of order: " << getOrderCost() << std::endl;
		std::cout << "Price of order: " << getCustomerPrice() << std::endl;
		std::cout << generateInformation() << std::endl;
	}

	void Order::consoleInput()
	{
		std::cout<< "--------------------Inputted Values:--------------------"<<std::endl;

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
}
