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
		stockboard = new Stockboard("../data/");
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
			catch (std::invalid_argument &e)
			{
				//Tell user in console what error is
				std::cout << e.what() << std::endl;
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
		resetFlute();
		resetPaperWeight();
		resetPaperQuality();
		resetStyle();
		boxLength = 0;
		boxWidth = 0;
		boxHeight = 0;
		quantity = -1;
		pricePerBox = 0.30;
		priceOnTop = 1.5;
		fullHalfPolicy = 0;
	}

	void Order::resetFlute()
	{
		flute = "";
	}

	void Order::resetPaperWeight()
	{
		paperWeight = 0;
	}

	void Order::resetPaperQuality()
	{
		paperQuality = "";
	}

	void Order::resetStyle()
	{
		style = "";
	}

	void Order::setFlute(std::string desiredFlute)
	{
		if (desiredFlute.size()>3)
		{
			throw std::invalid_argument("Flute selection invalid: too many characters");
		}
		if (desiredFlute.size()==0)
		{
			throw std::invalid_argument("Flute selection invalid: input empty");
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
			throw std::invalid_argument("Paper Weight selection invalid: input empty");
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
			throw std::invalid_argument("Paper Quality selection invalid: input empty");
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
			throw std::invalid_argument("Style selection invalid: input empty");
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

	void Order::setFullHalf(int desiredFullHalf)
	{
		if (!checkStyleSet())
		{
			throw std::invalid_argument("Full/Half policy selection invalid: Style not set");
		}
		if (desiredFullHalf < 0 || desiredFullHalf > 2)
		{
			throw std::invalid_argument("Full/Half policy selection invalid: out of range");
		}
		if (desiredFullHalf == 1 || desiredFullHalf == 2)
		{
			if (!styleHasHalf())
			{
				throw std::invalid_argument("Full/Half policy selection invalid: no half chop available");
			}
		}
		fullHalfPolicy = desiredFullHalf;
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

	double Order::getSheetChop()
	{
		return sheetChop;
	}

	double Order::getSheetDecal()
	{
		return sheetDecal;
	}

	double Order::getSheetPrice()
	{
		return sheetPrice;
	}

	std::vector<std::string> Order::availableStyles()
	{
		std::vector<std::string> styles;
		for (unsigned int i = 0; i < blanksizes->blanksizeList.size(); i++)
		{
			std::string currentStyle = blanksizes->blanksizeList[i].bStyle;
			if (currentStyle[currentStyle.size() - 1] != 'H')
			{
				styles.push_back(currentStyle);
			}
		}
		return styles;
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
		//If this is a half, throw as this should only be on full chop and sets both
		if (styleNo > 0)
		{
			if (blanksizes->blanksizeList[styleNo - 1].bHasHalf == 1)
			{
				throw std::invalid_argument("Error with inputted values: cannot choose half chop, change chop policy");
			}
		}
		unsigned int allowanceNo = blanksizes->getMatch(style,flute).second;
		//Set allowance from data structure
		allowance = blanksizes->allowanceList[allowanceNo].second;
		//Set chop from data structure
		boxChop = (blanksizes->blanksizeList[styleNo].bParameters[0]*boxLength);
		boxChop += (blanksizes->blanksizeList[styleNo].bParameters[1]*boxWidth);
		boxChop += (blanksizes->blanksizeList[styleNo].bParameters[2]*boxHeight);
		boxChop += blanksizes->blanksizeList[styleNo].bParameters[3];
		boxChop += (blanksizes->blanksizeList[styleNo].bParameters[4]*allowance);
		//Set half chop if board has it
		if (blanksizes->blanksizeList[styleNo].bHasHalf == 1)
		{
			unsigned int halfChopStyleNo = styleNo + 1;
			boxHalfChop = (blanksizes->blanksizeList[halfChopStyleNo].bParameters[0] * boxLength);
			boxHalfChop += (blanksizes->blanksizeList[halfChopStyleNo].bParameters[1] * boxWidth);
			boxHalfChop += (blanksizes->blanksizeList[halfChopStyleNo].bParameters[2] * boxHeight);
			boxHalfChop += blanksizes->blanksizeList[halfChopStyleNo].bParameters[3];
			boxHalfChop += (blanksizes->blanksizeList[halfChopStyleNo].bParameters[4] * allowance);
		}
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
			//Cheapest if all full or all half chop:
			if (fullHalfPolicy == 0 || fullHalfPolicy == 1 || fullHalfPolicy == 2)
			{
				double forseenCost = quantity * stockboard->theStockboard[match].sheets[0].bSheetPrice;

				for (unsigned int i = 0; i < stockboard->theStockboard[match].sheets.size(); i++)
				{
					//Potential chop boxes per sheet is how many can fit horizontally max in this sheet
					//Worked out depoending on full/half chop policy
					double potentialCBoxesPerSheet;
					//Count how many full and half chops
					int potentialCountFull = 0;
					int potentialCountHalf = 0;
					//Potential decal boxes per sheet is how many can fit vertically max in this sheet
					double potentialDBoxesPerSheet = floor(stockboard->theStockboard[match].sheets[i].bSheetDecal / boxDecal);
					//If full only policy: potential chop boxes is just how many chops fit in sheet
					if (fullHalfPolicy == 0)
					{
						potentialCBoxesPerSheet = floor(stockboard->theStockboard[match].sheets[i].bSheetChop / boxChop);
						potentialCountFull = potentialCBoxesPerSheet * potentialDBoxesPerSheet;
					}
					//If half only policy: potential chop boxes is just how many half chops fit in sheet
					else if (fullHalfPolicy == 1)
					{
						potentialCBoxesPerSheet = floor(stockboard->theStockboard[match].sheets[i].bSheetChop / boxHalfChop);
						potentialCountHalf = potentialCBoxesPerSheet * potentialDBoxesPerSheet;
						potentialCBoxesPerSheet = potentialCBoxesPerSheet / 2;
					}
					//If both policy: potential chop boxes is worked out with loop, least wastage
					else if (fullHalfPolicy == 2)
					{
						double boardSize = stockboard->theStockboard[match].sheets[i].bSheetChop;
						double potentialWastage = boardSize;
						double calcChopInSheet = stockboard->theStockboard[match].sheets[i].bSheetChop / boxChop;
						double calcHalfInSheet = stockboard->theStockboard[match].sheets[i].bSheetChop / boxHalfChop;
						for (unsigned int j = 0; j < ceil(calcChopInSheet); j++)
						{
							for (unsigned int k = 0; k < ceil(calcHalfInSheet); k++)
							{
								double potentialUsed = (j * boxChop) + (k*boxHalfChop);
								if (potentialUsed > boardSize)
								{
									continue;
								}
								double comparedWaste = boardSize - potentialUsed;
								if (comparedWaste < potentialWastage)
								{
									potentialCountHalf = k * potentialDBoxesPerSheet;
									potentialCountFull = j * potentialDBoxesPerSheet;
									double K = k;
									potentialCBoxesPerSheet = j + (K / 2);
								}
							}
						}
					}
					if (potentialCBoxesPerSheet == 0 || potentialDBoxesPerSheet == 0)
					{
						continue;
					}
					double boxesPerSheet = potentialCBoxesPerSheet * potentialDBoxesPerSheet;
					int noOfSheets = ceil((double)quantity / boxesPerSheet);
					double potentialCost = noOfSheets * stockboard->theStockboard[match].sheets[i].bSheetPrice;
					if (potentialCost <= forseenCost)
					{
						forseenCost = potentialCost;
						cChop = stockboard->theStockboard[match].sheets[i].bSheetChop;
						cDecal = stockboard->theStockboard[match].sheets[i].bSheetDecal;
						cPrice = stockboard->theStockboard[match].sheets[i].bSheetPrice;
						cBoxesPerSheet = potentialCBoxesPerSheet;
						dBoxesPerSheet = potentialDBoxesPerSheet;
						countFull = potentialCountFull;
						countHalf = potentialCountHalf;
					}
				}
			}
			if (cChop < 0)
			{
				throw std::invalid_argument("Error with inputted values: no matched sheet in Stockboard file has chop big enough to fit " + std::to_string(boxChop));
			}
			if (cDecal < 0)
			{
				throw std::invalid_argument("Error with inputted values: no matched sheet in Stockboard file has decal big enough to fit " + std::to_string(boxDecal));
			}
		}
		//If pricing done by ordering in board (over 200):
		else
		{
			//Sets size of sheet to order accordingly, whichever is smallest: chop or half chop given option
			cDecal = boxDecal;
			countHalf = 0;
			countFull = 0;
			if (fullHalfPolicy == 0)
			{
				cChop = boxChop;
				countFull = 1;
			}
			if (fullHalfPolicy == 1)
			{
				cChop = 2*boxHalfChop;
				countHalf = 2;
			}
			if (fullHalfPolicy == 2)
			{
				if (2*boxHalfChop < boxChop)
				{
					cChop = 2 * boxHalfChop;
					countHalf = 2;
				}
				else
				{
					cChop = boxChop;
					countFull = 1;
				}
			}
			
			//Pricing is price per square meter * area of box
			if (getPricingTier() == 200)
			{
				cPrice = stockboard->theStockboard[match].prices[0] * ((cDecal*cChop)/1000000000);
			}
			if (getPricingTier() == 500)
			{
				cPrice = stockboard->theStockboard[match].prices[1] * ((cDecal*cChop) / 1000000000);
			}
			if (getPricingTier() == 1000)
			{
				cPrice = stockboard->theStockboard[match].prices[2] * ((cDecal*cChop) / 1000000000);
			}
			if (getPricingTier() == 3000)
			{
				cPrice = stockboard->theStockboard[match].prices[3] * ((cDecal*cChop) / 1000000000);
			}
			if (getPricingTier() == 5000)
			{
				cPrice = stockboard->theStockboard[match].prices[4] * ((cDecal*cChop) / 1000000000);
			}
			if (getPricingTier() == 10000)
			{
				cPrice = stockboard->theStockboard[match].prices[5] * ((cDecal*cChop) / 1000000000);
			}
			cBoxesPerSheet = 1;
			dBoxesPerSheet = 1;
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
		else
		{
			return -1;
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
		orderCost = noOfSheets() * sheetPrice;
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

	int Order::noOfSheets()
	{
		double boxesPerSheet = cBoxesPerSheet * dBoxesPerSheet;
		return ceil((double)quantity / boxesPerSheet);
	}

	bool Order::styleHasHalf()
	{
		if (!checkStyleSet())
		{
			return 0;
		}
		int match = blanksizes->getMatch(style, "").first;
		if (match == -1)
		{
			throw std::invalid_argument("Style selection invalid: not on blanksize.txt");
			return 0;
		}
		else if (blanksizes->blanksizeList[match].bHasHalf == 1)
		{
			return 1;
		}
		return 0;
	}

	std::pair<int, int> Order::getChopCounts()
	{
		return std::pair<int, int>(countFull/(int) dBoxesPerSheet, countHalf/(int) dBoxesPerSheet);
	}

	int Order::getDecalCount()
	{
		return dBoxesPerSheet;
	}

	int Order::getPolicy()
	{
		return fullHalfPolicy;
	}

	std::vector<std::string> Order::getValidInputs(int option)
	{
		//Makes copy of stockboard list and removes those not found on allowances
		std::vector<stockItem> copyList = stockboard->theStockboard;
		std::vector<unsigned int> toRemove;
		for (unsigned int i = 0 ; i < copyList.size(); i++)
		{
			bool match = 0;
			for (unsigned int j = 0; j < blanksizes->allowanceList.size(); j++)
			{
				if (copyList[i].sFlute == blanksizes->allowanceList[j].first)
				{
					match = 1;
				}
			}
			if (match == 0)
			{
				toRemove.push_back(i);
			}
		}
		std::reverse(toRemove.begin(), toRemove.end());
		for (unsigned int i = 0; i < toRemove.size(); i++)
		{
			copyList.erase(copyList.begin() + toRemove[i]);
		}
		if (checkFluteSet())
		{
			std::vector<unsigned int> toRemoveFlute;
			for (unsigned int i = 0; i < copyList.size(); i++)
			{
				if (copyList[i].sFlute != flute)
				{
					toRemoveFlute.push_back(i);
				}
			}
			std::reverse(toRemoveFlute.begin(), toRemoveFlute.end());
			for (unsigned int i = 0; i < toRemoveFlute.size(); i++)
			{
				copyList.erase(copyList.begin() + toRemoveFlute[i]);
			}
		}
		if (checkPaperWeightSet())
		{
			std::vector<unsigned int> toRemovePW;
			for (unsigned int i = 0; i < copyList.size(); i++)
			{
				if (copyList[i].sPaperWeight != paperWeight)
				{
					toRemovePW.push_back(i);
				}
			}
			std::reverse(toRemovePW.begin(), toRemovePW.end());
			for (unsigned int i = 0; i < toRemovePW.size(); i++)
			{
				copyList.erase(copyList.begin() + toRemovePW[i]);
			}
		}
		if (checkPaperQualitySet())
		{
			std::vector<unsigned int> toRemovePQ;
			for (unsigned int i = 0; i < copyList.size(); i++)
			{
				if (copyList[i].sPaperQuality != paperQuality)
				{
					toRemovePQ.push_back(i);
				}
			}
			std::reverse(toRemovePQ.begin(), toRemovePQ.end());
			for (unsigned int i = 0; i < toRemovePQ.size(); i++)
			{
				copyList.erase(copyList.begin() + toRemovePQ[i]);
			}
		}
		if (option == 0)
		{
			std::vector<std::string> fluteVec;
			for (unsigned int i = 0; i < copyList.size(); i++)
			{
				if (fluteVec.size() == 0)
				{
					fluteVec.push_back(copyList[i].sFlute);
				}
				else
				{
					bool dupe = 0;
					for (unsigned int j = 0; j < fluteVec.size(); j++)
					{
						if (fluteVec[j] == copyList[i].sFlute)
						{
							dupe = 1;
						}
					}
					if (dupe == 0)
					{
						fluteVec.push_back(copyList[i].sFlute);
					}
				}
			}
			return fluteVec;
		}
		if (option == 1)
		{
			std::vector<std::string> PWVec;
			for (unsigned int i = 0; i < copyList.size(); i++)
			{
				if (PWVec.size() == 0)
				{
					PWVec.push_back(std::to_string(copyList[i].sPaperWeight));
				}
				else
				{
					bool dupe = 0;
					for (unsigned int j = 0; j < PWVec.size(); j++)
					{
						if (PWVec[j] == std::to_string(copyList[i].sPaperWeight))
						{
							dupe = 1;
						}
					}
					if (dupe == 0)
					{
						PWVec.push_back(std::to_string(copyList[i].sPaperWeight));
					}
				}
			}
			return PWVec;
		}
		if (option == 2)
		{
			std::vector<std::string> PQVec;
			for (unsigned int i = 0; i < copyList.size(); i++)
			{
				if (PQVec.size() == 0)
				{
					PQVec.push_back(copyList[i].sPaperQuality);
				}
				else
				{
					bool dupe = 0;
					for (unsigned int j = 0; j < PQVec.size(); j++)
					{
						if (PQVec[j] == copyList[i].sPaperQuality)
						{
							dupe = 1;
						}
					}
					if (dupe == 0)
					{
						PQVec.push_back(copyList[i].sPaperQuality);
					}
				}
			}
			return PQVec;
		}
		return std::vector<std::string> {};
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
		if (getPricingTier() == 0)
		{
			info += "Sheet used's Chop: " + std::to_string(getSheetChop()) + ", Decal: " + std::to_string(getSheetDecal()) + ", Price: " + std::to_string(getSheetPrice()) + ", number of sheets used: " + std::to_string(noOfSheets()) + "\n";
		}
		else
		{
			info += "Board to order in's Chop: " + std::to_string(getSheetChop()) + ", Decal: " + std::to_string(getSheetDecal()) + ", Price: " + std::to_string(getSheetPrice()) + ", how many to order in: " + std::to_string(noOfSheets()) + "\n";
		}
		if (getPolicy() == 0 || getPolicy() == 2)
		{
			info += "Number of full chops to fit in sheet: " + std::to_string(getChopCounts().first) + "\n";
		}
		if (getPolicy() == 1 || getPolicy() == 2)
		{
			info += "Number of half chops to fit in sheet: " + std::to_string(getChopCounts().second) + "\n";
		}
		info += "Number of decals to fit in sheet: " + std::to_string(getDecalCount()) + "\n";
		if (getCheaperTier() != 0)
		{
			info += "Minimum quantity of boxes to next pricing tier: " + std::to_string(quantBoxNeeded()) + "\n";
		}
		return info;
	}
}
