#include <stdexcept>
#include <fstream>

//Testing
#include <iostream>

#include "stockboard.h"

namespace BP
{
	Stockboard::Stockboard(std::string filepath)
	{
		std::string stockboardFilename = "Stockboard.txt";
		//Resizes vector of stockitems to be of size how many in file
		//Throws if file does not exist or is bad
		theStockboard.resize(getLines(filepath + stockboardFilename));
		//Reads file into data structure
		//Throws if format is wrong
		readIn(filepath + stockboardFilename);
	}

	unsigned int Stockboard::getLines(std::string filename)
	{
		//Gets how many lines in file to return to resize data structure
		unsigned int counter = 0;
		std::ifstream stockIn(filename);
		std::string parser;
		//Throw if file not found
		if (!stockIn.is_open())
		{
			throw std::runtime_error("Error with stockboard file: cannot find " + filename);
		}
		while (getline(stockIn, parser))
		{
			counter++;
		}
		//Throw if loop terminated before end, file is bad
		if (!stockIn.eof())
		{
			throw std::runtime_error("Error with stockboard file: " + filename + " content is bad");
		}
		//Counter - 1 because first line is headings
		return counter - 1;
	}

	void Stockboard::setFlute(std::string fluteIn, unsigned int index)
	{
		//Set flute, throw if file is wrong
		for (unsigned int i = 0; i < fluteIn.size(); i++)
		{
			if (!isalpha(fluteIn.at(i)))
			{
				throw std::runtime_error("Error with stockboard file: Flute non-alphabetical - line " + std::to_string(index + 1));
			}
			fluteIn.at(i) = toupper(fluteIn.at(i));
		}
		theStockboard[index].sFlute = fluteIn;
	}

	void Stockboard::setPaperWeight(std::string weightIn, unsigned int index)
	{
		//Set paper weight, throw if file is wrong
		for (unsigned int i = 0; i < weightIn.size(); i++)
		{
			if (!isdigit(weightIn.at(i)))
			{
				throw std::runtime_error("Error with stockboard file: Paper Weight non-alphabetical - line " + std::to_string(index + 1));
			}
		}
		theStockboard[index].sPaperWeight = std::stod(weightIn);
	}

	void Stockboard::setPaperQuality(std::string qualityIn, unsigned int index)
	{
		//Set paper quality, throw if file is wrong
		for (unsigned int i = 0; i < qualityIn.size(); i++)
		{
			if (!isalpha(qualityIn.at(i)))
			{
				throw std::runtime_error("Error with stockboard file: Paper Quality non-alphabetical - line " + std::to_string(index + 1));
			}
			qualityIn.at(i) = toupper(qualityIn.at(i));
		}
		theStockboard[index].sPaperQuality = qualityIn;
	}

	void Stockboard::setSheet(std::vector<std::string> sheetIn, unsigned int index)
	{
		//Set sheet info, throw if file is wrong
		sheet assigner;
		for (unsigned int i = 0; i < sheetIn.size(); i++)
		{
			unsigned int dotCount = 0;
			for (unsigned int j = 0; j < sheetIn[i].size(); j++)
			{
				if (!isdigit(sheetIn[i].at(j)))
				{
					if (sheetIn[i].at(j) != '.' || dotCount > 0)
					{
						std::string fault;
						if (i == 0)
						{
							fault = "chop";
						}
						if (i == 1)
						{
							fault = "decal";
						}
						if (i == 2)
						{
							fault = "price";
						}
						throw std::runtime_error("Error with stockboard file: Type of board " + std::to_string(index + 1) + "'s " + fault + " not a number");
					}
					else
					{
						dotCount++;
					}
				}
			}
		}
		assigner.bSheetChop = std::stod(sheetIn[0]);
		assigner.bSheetDecal = std::stod(sheetIn[1]);
		assigner.bSheetPrice = std::stod(sheetIn[2]);
		
		theStockboard[index].sheets.push_back(assigner);
	}

	void Stockboard::setPrices(std::vector<std::string> pricesIn, unsigned int index)
	{
		//Set pricing information, throw if file is wrong
		std::vector<unsigned int> iVec;
		for (unsigned int i = 0; i < pricesIn.size(); i++)
		{
			for (unsigned int j = 0; j < pricesIn[i].size(); j++)
			{
				if (!isdigit(pricesIn[i].at(j)))
				{
					std::string fault;
					if (i == 0)
					{
						fault = "200";
					}
					if (i == 1)
					{
						fault = "500";
					}
					if (i == 2)
					{
						fault = "1K";
					}
					if (i == 3)
					{
						fault = "3K";
					}
					if (i == 4)
					{
						fault = "5K";
					}
					if (i == 5)
					{
						fault = "10K";
					}
					throw std::runtime_error("Error with stockboard file: Type of board " + std::to_string(index + 1) + "'s price per " + fault + " not a number");
				}
			}
			iVec.push_back(std::stoi(pricesIn[i]));
		}
		theStockboard[index].prices = iVec;
	}

    void Stockboard::readIn(std::string filename)
    {
        //Set up parser, file and line counter
        std::string parser;
        std::ifstream stockIn (filename);
		//Counter for how many unique boards and for how many lines in file
        unsigned int lineNumber = 0;
		unsigned int boardsNumber = 0;
        unsigned int maxLines = getLines(filename);
        //Get rid of title line
        getline(stockIn, parser);
        //Read through file 
        while (lineNumber<maxLines)
        {   
			//Get first 3 words in string
            std::vector<std::string> vParserS;
			for (unsigned int i = 0; i < 3; ++i)
			{
				stockIn >> parser;
				vParserS.push_back(parser);
			}
			//If they are unique: add new type of board
			if (getBoardMatch(vParserS[0], std::stoi(vParserS[1]), vParserS[2]) == -1)
			{
				setFlute(vParserS[0], boardsNumber);
				setPaperWeight(vParserS[1], boardsNumber);
				setPaperQuality(vParserS[2], boardsNumber);
				std::vector<std::string> vParserBoards;
				for (unsigned int i = 0; i < 3; ++i)
				{
					stockIn >> parser;
					vParserBoards.push_back(parser);
				}
				setSheet(vParserBoards, boardsNumber);

				std::vector<std::string> vParserPricing;
				for (unsigned int i = 0; i < 6; ++i)
				{
					stockIn >> parser;
					vParserPricing.push_back(parser);
				}
				setPrices(vParserPricing, boardsNumber);
				boardsNumber++;
			}
			//If they are no unique, add line's sheet info only to the relevant board type
			else
			{
				std::vector<std::string> vParserS1;
				for (unsigned int i = 0; i < 3; ++i)
				{
					stockIn >> parser;
					vParserS1.push_back(parser);
				}
				setSheet(vParserS1, getBoardMatch(vParserS[0], std::stoi(vParserS[1]), vParserS[2]));
				getline(stockIn, parser);
			}
			//Progress through file
			lineNumber++;
        }
    }
    
	int Stockboard::getBoardMatch(std::string oFlute, unsigned int oWeight, std::string oQuality)
    {
		//Returns index of match in stockboard, -1 if no match
        for (unsigned int i = 0; i < theStockboard.size(); i++)
        {
            bool fluteMatch = 0;
            bool weightMatch = 0;
            if (theStockboard[i].sFlute == oFlute)
            {
                fluteMatch = 1;
            }
            if (theStockboard[i].sPaperWeight == oWeight)
            {
                weightMatch = 1;
            }
            if(theStockboard[i].sPaperQuality == oQuality)
            {
                if (fluteMatch && weightMatch)
                {
					return i;
                }
            }
        }
		return -1;
    }
}