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
		blanksizes = new Blanksizes("../data/");
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
		pricePerBox = 0.30;
		priceOnTop = 1.5;
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

	double Order::getCustomerPricePer()
	{
		return customerPricePer;
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
		if (!checkFluteSet())
		{
			return "Flute not set";
		}
		if (!checkPaperWeightSet())
		{
			return "Paper Weight not set";
		}
		if (!checkPaperQualitySet())
		{
			return "Paper Quality not set";
		}
		if (!checkStyleSet())
		{
			return "Style not set";
		}
		if (!checkBoxLengthSet())
		{
			return "Length not set";
		}
		if (!checkBoxWidthSet())
		{
			return "Width not set";
		}
		if (!checkBoxHeightSet())
		{
			return "Height not set";
		}
		if (!checkQuantitySet())
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
			throw std::invalid_argument("Error with inputted values: no match found for Style in Blanksize file");
		}
		if (blanksizes->getMatch(style, flute).second<0)
		{
			throw std::invalid_argument("Error with inputted values: no match found for Flute in Allowance file");
		}
		//Get location in data structure using style and flute
		unsigned int styleNo = blanksizes->getMatch(style,flute).first;
		unsigned int allowanceNo = blanksizes->getMatch(style,flute).second;
		//Set allowance from data structure
		allowance = blanksizes->allowanceList[allowanceNo].second;
		//Set chop from data structure
		boxChop = (blanksizes->blanksizeList[styleNo].bParameters[0]*boxLength);
		boxChop += (blanksizes->blanksizeList[styleNo].bParameters[1]*boxWidth);
		boxChop += (blanksizes->blanksizeList[styleNo].bParameters[2]*boxHeight);
		boxChop += blanksizes->blanksizeList[styleNo].bParameters[3];
		boxChop += (blanksizes->blanksizeList[styleNo].bParameters[4]*allowance);
		//Set decal from data structure
		boxDecal = (blanksizes->blanksizeList[styleNo].bParameters[5]*boxLength);
		boxDecal += (blanksizes->blanksizeList[styleNo].bParameters[6]*boxWidth);
		boxDecal += (blanksizes->blanksizeList[styleNo].bParameters[7]*boxHeight);
		boxDecal += blanksizes->blanksizeList[styleNo].bParameters[8];
		boxDecal += (blanksizes->blanksizeList[styleNo].bParameters[9]*allowance);
	}

	void Order::doStockSheet()
	{
		//Using stockboard structure:
		//Read in positions of matches to vector
		int match = stockboard->getBoardMatch(flute,paperWeight,paperQuality);
		//Initialise comparing variables to do maths with and choose best sheet
		double cChop = -1;
		double cDecal = -1;
		double cPrice;
		if (match == -1)
		{
			throw std::invalid_argument("Error with inputted values: no match found for Flute, Paper Weight and Paper Quality in Stockboard file");
		}
		//If under 200:
		if (getPricingTier() == 0)
		{
			for (unsigned int i = 0; i < stockboard->theStockboard[match].sheets.size(); i++)
			{
				//Checks for biggest chop - TODO LEAST WASTAGE
				if (stockboard->theStockboard[match].sheets[i].bSheetChop > cChop)
				{
					cChop = stockboard->theStockboard[match].sheets[i].bSheetChop;
					cDecal = stockboard->theStockboard[match].sheets[i].bSheetDecal;
					cPrice = stockboard->theStockboard[match].sheets[i].bSheetPrice;
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
		}
		else
		{
			//Placeholder: TODO ADD PRICING LOGIC
			for (unsigned int i = 0; i < stockboard->theStockboard[match].sheets.size(); i++)
			{
				//Checks for biggest chop
				if (stockboard->theStockboard[match].sheets[i].bSheetChop > cChop)
				{
					cChop = stockboard->theStockboard[match].sheets[i].bSheetChop;
					cDecal = stockboard->theStockboard[match].sheets[i].bSheetDecal;
					cPrice = stockboard->theStockboard[match].sheets[i].bSheetPrice;
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
		}
		sheetChop = cChop;
		sheetDecal = cDecal;
		sheetPrice = cPrice;
	}
	
	int Order::getPricingTier()
	{
		if (sqMetOrder() >= 10000)
		{
			return 10000;
		}
		else if (sqMetOrder() >= 5000)
		{
			return 5000;
		}
		else if (sqMetOrder() >= 3000)
		{
			return 3000;
		}
		else if (sqMetOrder() >= 1000)
		{
			return 1000;
		}
		else if (sqMetOrder() >= 500)
		{
			return 500;
		}
		else if (sqMetOrder() >= 200)
		{
			return 200;
		}
		else if (sqMetOrder() > 0)
		{
			return 0;
		}
	}

	int Order::getCheaperTier()
	{
		if (getPricingTier() == 5000 && (stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[4] > stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[5]))
		{
			return 10000;
		}
		else if (getPricingTier() == 3000 && (stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[3] > stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[4]))
		{
			return 5000;
		}
		else if (getPricingTier() == 1000 && (stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[2] > stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[3]))
		{
			return 3000;
		}
		else if (getPricingTier() == 500 && (stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[1] > stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[2]))
		{
			return 1000;
		}
		else if (getPricingTier() == 200 && (stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[0] > stockboard->theStockboard[stockboard->getBoardMatch(flute, paperWeight, paperQuality)].prices[1]))
		{
			return 500;
		}
		else if (getPricingTier() == 0)
		{
			return 200;
		}
		return 0;
	}

	void Order::doPricing()
	{
		unsigned int boxCPerSheet = (sheetChop / boxChop);
		unsigned int boxDPerSheet = (sheetDecal / boxDecal);
		unsigned int boxPerSheet = boxCPerSheet *boxDPerSheet;
		int noOfSheets = ceil((double)quantity / boxPerSheet);
		orderCost = noOfSheets * sheetPrice;
		customerPrice = ((pricePerBox*quantity) + orderCost) * priceOnTop;
		if (quantity == 0)
		{
			customerPricePer = 0;
		}
		else
		{
			customerPricePer = customerPrice / quantity;
		}
	}

	double Order::sqMetBox()
	{
		return (boxChop*boxDecal)/1000000;
	}

	double Order::sqMetOrder()
	{
		return sqMetBox()*quantity;
	}

	int Order::quantBoxNeeded()
	{
		return ceil(getCheaperTier()/ sqMetBox());
	}

	bool Order::checkFluteSet()
	{
		return (flute != "") ? 1 : 0;
	}

	bool Order::checkPaperWeightSet()
	{
		return (paperWeight != 0) ? 1 : 0;
	}

	bool Order::checkPaperQualitySet()
	{
		return (paperQuality != "") ? 1 : 0;
	}

	bool Order::checkStyleSet()
	{
		return (style != "") ? 1 : 0;
	}

	bool Order::checkBoxLengthSet()
	{
		return (boxLength != 0) ? 1 : 0;
	}

	bool Order::checkBoxWidthSet()
	{
		return (boxWidth != 0) ? 1 : 0;
	}

	bool Order::checkBoxHeightSet()
	{
		return (boxHeight != 0) ? 1 : 0;
	}

	bool Order::checkQuantitySet()
	{
		return (quantity != -1) ? 1 : 0;
	}

	bool Order::checkPricePerBoxSet()
	{
		return (boxLength != 0.05) ? 1 : 0;
	}

	bool Order::checkPriceOnTopSet()
	{
		return (boxLength != 100.00) ? 1 : 0;
	}

	void Order::consoleDisplay()
	{
		std::cout<< "--------------------Outputted Values:--------------------"<<std::endl;
		std::cout << "Chop: " << getBoxChop() << std::endl;
		std::cout << "Decal: " << getBoxDecal() << std::endl;
		std::cout << "Cost of order: " << getOrderCost() << std::endl;
		std::cout << "Price of order: " << getCustomerPrice() << std::endl;
		std::cout << generateConsoleInformation() << std::endl;
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

	std::string Order::generateConsoleInformation()
	{
		std::string info = "Square metre of box: " + std::to_string(sqMetBox()) + "\n";
		info += "Square metre of order: " + std::to_string(sqMetOrder()) + "\n";
		if (getCheaperTier() != 0)
		{
			info += "Minimum quantity of boxes to next pricing tier: " + std::to_string(quantBoxNeeded());
		}
		return info;
	}
}
