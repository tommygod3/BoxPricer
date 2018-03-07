#include <stdexcept>
#include <fstream>

#include "blanksize.h"

namespace BP
{
    Blanksizes::Blanksizes()
    {
        //Resizes vector of blanksizes to be of size how many in file
        //Throws if file does not exist or is bad
        blanksizeList.resize(getLines("../data/Blanksize.txt"));
        //Reads file into data structure
        //Throws if format is wrong
        readIn("../data/Blanksize.txt");
    }

    unsigned int Blanksizes::getLines(std::string filename)
    {
        //Gets how many lines in file to return to resize data structure
        unsigned int counter = 0;
        std::ifstream blankIn (filename);
        std::string parser;
        //Throw if file not found
        if (!blankIn.is_open())
        {
            throw std::runtime_error("Error with blanksize file: cannot find " + filename);
        }
        while (getline(blankIn,parser))
        {
            counter++;
        }
        //Throw if loop terminated before end, file is bad
        if (!blankIn.eof())
        {
            throw std::runtime_error("Error with blanksize file: " + filename + " content is bad");
        }
        //Counter - 1 because first line is headings
        return counter - 1;
    }

    void Blanksizes::setStyle(std::string styleIn, unsigned int index)
    {
        //Set style, throw if input is wrong
        for (unsigned int i = 0; i<styleIn.size();i++)
		{
			if (!isdigit(styleIn.at(i)))
			{
				throw std::runtime_error("Error with blanksize file: Style not made of numbers - line " + std::to_string(index+1));
			}
            styleIn.at(i) = toupper(styleIn.at(i));
		}
        blanksizeList[index].bStyle = styleIn;
    }

    void Blanksizes::setParameters(std::vector<std::string> parametersIn, unsigned int index)
    {
        //Set a parameters, throw if input is wrong
        std::vector<double> dVec;
        for (unsigned int i = 0; i<parametersIn.size();i++)
        {
            unsigned int dotCount = 0;
            for (unsigned int j = 0; j<parametersIn[i].size();j++)
            {
                if (!isdigit(parametersIn[i].at(j)))
                {
                    if (parametersIn[i].at(j) != '.' || dotCount > 0)
                    {
                        throw std::runtime_error("Error with blanksize file: Parameter " + (std::to_string(i+1)) + " not a number - line "  + std::to_string(index+1));
                    }
                    else
                    {
                        dotCount++;
                    }
                }
            }
        }
        for (unsigned int i=0;i<parametersIn.size();i++)
        {
            dVec.push_back(std::stod(parametersIn[i]));
        }
        blanksizeList[index].bParameters = dVec;
    }
    
    void Blanksizes::setAllowance(std::vector<std::string> allowanceIn, unsigned int index)
    {
        //Set allowance, throw if input is wrong
        if (allowanceIn.size() % 3 != 0)
        {
            throw std::runtime_error("Error with blanksize file: Allowances invalid - line " + std::to_string(index+1));
        }
        std::vector<allowancePair> pairsIn;
        for (unsigned int i = 0; i<(allowanceIn.size()/3); i++)
        {
            //string[i] = allowanceIn[i*3+0]
            std::string fluteR = allowanceIn[i*3+0];
            for (unsigned int j = 0; j < fluteR.size(); j++)
            {
                if (!isalpha(fluteR.at(j)))
                {
                    throw std::runtime_error("Error with blankszie file: Flute " + std::to_string((i+1)) + " non-alphabetical - line " + std::to_string(index+1));
                }
                fluteR.at(j) = toupper(fluteR.at(j));
            }
            //pair[i].first = allowanceIn[i*3+1]
            std::string allowance1 = allowanceIn[i*3+1];
            for (unsigned int k = 0; k < allowance1.size(); k++)
            {
                if (!isdigit(allowance1.at(k)))
                {
                    throw std::runtime_error("Error with blankszie file: Allowance " + std::to_string((i+1)) + " part 1 is not a valid number - line " + std::to_string(index+1));
                }
            }
            //pair[i].second = allowanceIn[i*3+2]
            std::string allowance2 = allowanceIn[i*3+2];
            for (unsigned int z = 0; z < allowance2.size(); z++)
            {
                if (!isdigit(allowance2.at(z)))
                {
                    throw std::runtime_error("Error with blankszie file: Allowance " + std::to_string((i+1)) + " part 2 is not a valid number - line " + std::to_string(index+1));
                }
            }
            std::vector<unsigned int> allowanceInts;
            allowanceInts.push_back(std::stoul(allowance1));
            allowanceInts.push_back(std::stoul(allowance2));
            pairsIn.push_back(allowancePair(fluteR,allowanceInts));
        }
        blanksizeList[index].bAllowances = pairsIn;
    }
    
    void Blanksizes::readIn(std::string filename)
    {
        //Set up parser, file and line counter
        std::string parser;
        std::ifstream blankIn (filename);
        unsigned int lineNumber = 0;
        unsigned int maxLines = getLines(filename);
        //Get rid of title line
        getline(blankIn, parser);
        //Read through file 
        while (lineNumber<maxLines)
        {   
            std::vector<std::string> vParserP;
            std::vector<std::string> vParserA;
            
            //Set all attributes in blankfile line
            blankIn >> parser;
            setStyle(parser,lineNumber);
            for (unsigned int i=0;i<12;i++)
            {
                blankIn >> parser;
                vParserP.push_back(parser);
            }
            setParameters(vParserP,lineNumber);
            while (blankIn.peek() != '\n')
            {
                blankIn >> parser;
                vParserA.push_back(parser);
            }
            setAllowance(vParserA, lineNumber);
            
            lineNumber++;
        }
    }
    
    std::pair<int,int> Blanksizes::getMatch(std::string oStyle, std::string oFlute)
    {
        for (unsigned int i = 0; i < blanksizeList.size(); i++)
        {
            bool styleMatch = 0;
            if (blanksizeList[i].bStyle == oStyle)
            {
                styleMatch = 1;
            }
            for (unsigned int j = 0; j < blanksizeList[i].bAllowances.size(); j++)
            {
                if (blanksizeList[i].bAllowances[j].first == oFlute)
                {
                    if (styleMatch)
                    {
                        return std::pair<int,int>(i,j);
                    }
                }
            }
            
        }
        return std::pair<int,int>(-1,-1);
    }
}