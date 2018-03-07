#include <stdexcept>
#include <fstream>

//Testing
#include <iostream>

#include "stockboard.h"

namespace BP
{
    Stockboard::Stockboard()
    {
        //Resizes vector of stockitems to be of size how many in file
        //Throws if file does not exist or is bad
        theStockboard.resize(getLines("../data/Stockboard.txt"));
        //Reads file into data structure
        //Throws if format is wrong
        readIn("../data/Stockboard.txt");
    }

    unsigned int Stockboard::getLines(std::string filename)
    {
        //Gets how many lines in file to return to resize data structure
        unsigned int counter = 0;
        std::ifstream stockIn (filename);
        std::string parser;
        //Throw if file not found
        if (!stockIn.is_open())
        {
            throw std::runtime_error("Error with stockboard file: cannot find " + filename);
        }
        while (getline(stockIn,parser))
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
        //Set flute, throw if input is wrong
        for (unsigned int i = 0; i<fluteIn.size();i++)
		{
			if (!isalpha(fluteIn.at(i)))
			{
				throw std::runtime_error("Error with stockboard file: Flute non-alphabetical - line " + std::to_string(index+1));
			}
            fluteIn.at(i) = toupper(fluteIn.at(i));
		}
        theStockboard[index].sFlute = fluteIn;
    }

    void Stockboard::setPaperWeight(std::string weightIn, unsigned int index)
    {
        //Set paper weight, throw if input is wrong
        for (unsigned int i = 0; i<weightIn.size();i++)
		{
			if (!isdigit(weightIn.at(i)))
			{
				throw std::runtime_error("Error with stockboard file: Paper Weight non-alphabetical - line " + std::to_string(index+1));
			}
		}
        theStockboard[index].sPaperWeight = std::stod(weightIn);
    }
    
    void Stockboard::setPaperQuality(std::string qualityIn, unsigned int index)
    {
        //Set paper quality, throw if input is wrong
        for (unsigned int i = 0; i<qualityIn.size();i++)
		{
			if (!isalpha(qualityIn.at(i)))
			{
				throw std::runtime_error("Error with stockboard file: Paper Quality non-alphabetical - line " + std::to_string(index+1));
			}
            qualityIn.at(i) = toupper(qualityIn.at(i));
		}
        theStockboard[index].sPaperQuality = qualityIn;
    }
    
    void Stockboard::setSheet(std::vector<std::string> sheetIn, unsigned int index)
    {
        //Set sheet info, throw if input is wrong
        std::vector<double> dVec;
        for (unsigned int i = 0; i<sheetIn.size();i++)
        {
            unsigned int dotCount = 0;
            for (unsigned int j = 0; j<sheetIn[i].size();j++)
            {
                if (!isdigit(sheetIn[i].at(j)))
                {
                    if (sheetIn[i].at(j) != '.' || dotCount > 0)
                    {
                        std::string fault;
                        if (i==0)
                        {
                            fault = "chop";
                        }
                        if (i==1)
                        {
                            fault = "decal";
                        }
                        if (i==2)
                        {
                            fault = "price";
                        }
                        throw std::runtime_error("Error with stockboard file: Sheet " + fault + " not a number - line "  + std::to_string(index+1));
                    }
                    else
                    {
                        dotCount++;
                    }
                }
            }
        }
        for (unsigned int i=0;i<sheetIn.size();i++)
        {
            dVec.push_back(std::stod(sheetIn[i]));
        }
        theStockboard[index].sSheetChop = dVec[0];
        theStockboard[index].sSheetDecal = dVec[1];
        theStockboard[index].sSheetPrice = dVec[2];
    }
    
    void Stockboard::readIn(std::string filename)
    {
        //Set up parser, file and line counter
        std::string parser;
        std::ifstream stockIn (filename);
        unsigned int lineNumber = 0;
        unsigned int maxLines = getLines(filename);
        //Get rid of title line
        getline(stockIn, parser);
        //Read through file 
        while (lineNumber<maxLines)
        {   
            std::vector<std::string> vParserS;
            //Set all attributes in blankfile line
            stockIn >> parser;
            setFlute(parser,lineNumber);
            stockIn >> parser;
            setPaperWeight(parser,lineNumber);
            stockIn >> parser;
            setPaperQuality(parser,lineNumber);
            while (stockIn.peek() != '\n')
            {
                stockIn >> parser;
                vParserS.push_back(parser);
            }
            setSheet(vParserS, lineNumber);
            
            lineNumber++;
        }
    }
    
    std::vector<int> Stockboard::getMatches(std::string oFlute,unsigned int oWeight, std::string oQuality)
    {
        std::vector<int> lineMatches;
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
                    lineMatches.push_back(i);
                }
            }
        }
        return lineMatches;
    }
}