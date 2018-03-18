#include <stdexcept>
#include <fstream>

#include "blanksize.h"

namespace BP
{
	Blanksizes::Blanksizes(std::string filepath)
	{
		std::string blanksizeFilename = "Blanksize.txt";
		std::string allowanceFilename = "Allowance.txt";
		//Resizes vector of blanksizes and allowances to be of size how many in file
		//Throws if file does not exist or is bad
		blanksizeList.resize(getLines(filepath + blanksizeFilename, 0));
		allowanceList.resize(getLines(filepath + allowanceFilename, 1));
		//Reads files into data structure
		//Throws if format is wrong
		readInBlanksizes(filepath + blanksizeFilename);
		readInAllowances(filepath + allowanceFilename);
	}

	unsigned int Blanksizes::getLines(std::string filename, int filetype)
	{
		std::string type;
		if (filetype == 0)
		{
			type = "blanksize";
		}
		else
		{
			type = "allowance";
		}
		//Gets how many lines in file to return to resize data structure
		unsigned int counter = 0;
		std::ifstream fileIn(filename);
		std::string parser;
		//Throw if file not found
		if (!fileIn.is_open())
		{
			throw std::runtime_error("Error with " + type + " file: cannot find " + filename);
		}
		while (getline(fileIn, parser))
		{
			counter++;
		}
		//Throw if loop terminated before end, file is bad
		if (!fileIn.eof())
		{
			throw std::runtime_error("Error with " + type + " file: " + filename + " content is bad");
		}
		//Counter - 1 because first line is headings
		return counter - 1;
	}

	void Blanksizes::setStyle(std::string styleIn, unsigned int index)
	{
		//Set style, throw if input is wrong
		for (unsigned int i = 0; i < styleIn.size(); i++)
		{
			if (!isdigit(styleIn.at(i)))
			{
				throw std::runtime_error("Error with blanksize file: Style not made of numbers - line " + std::to_string(index + 1));
			}
			styleIn.at(i) = toupper(styleIn.at(i));
		}
		blanksizeList[index].bStyle = styleIn;
	}

	void Blanksizes::setParameters(std::vector<std::string> parametersIn, unsigned int index)
	{
		//Set all parameters, throw if input is wrong
		std::vector<double> dVec;
		if (parametersIn.size() != 10)
		{
			throw std::runtime_error("Error with blanksize file: " + std::to_string(parametersIn.size()) + " parameters instead of 10 - line " + std::to_string(index + 1));
		}
		for (unsigned int i = 0; i < parametersIn.size(); i++)
		{
			unsigned int dotCount = 0;
			for (unsigned int j = 0; j < parametersIn[i].size(); j++)
			{
				if (!isdigit(parametersIn[i].at(j)))
				{
					if (parametersIn[i].at(j) != '.' || dotCount > 0)
					{
						throw std::runtime_error("Error with blanksize file: Parameter " + (std::to_string(i + 1)) + " not a number - line " + std::to_string(index + 1));
					}
					else
					{
						dotCount++;
					}
				}
			}
		}
		for (unsigned int i = 0; i < parametersIn.size(); i++)
		{
			dVec.push_back(std::stod(parametersIn[i]));
		}
		blanksizeList[index].bParameters = dVec;
	}

	void Blanksizes::setAllowance(std::vector<std::string> allowanceIn, unsigned int index)
	{
		//Set allowance pair, throw if wrong
		allowancePair allowanceLine;
		if (allowanceIn.size() != 2)
		{
			throw std::runtime_error("Error with allowance  file: " + std::to_string(allowanceIn.size()) + " entries instead of 2 - line " + std::to_string(index + 1));
		}
		for (unsigned int i = 0; i < allowanceIn[0].size(); i++)
		{
			if (!isalpha(allowanceIn[0].at(i)))
			{
				throw std::runtime_error("Error with allowance file: Flute character " + (std::to_string(i + 1)) + " not a letter - line " + std::to_string(index + 1));
			}
			allowanceIn[0].at(0) = toupper(allowanceIn[0].at(0));
		}
		for (unsigned int i = 0; i < allowanceIn[1].size(); i++)
		{
			if (!isdigit(allowanceIn[1].at(i)))
			{
				throw std::runtime_error("Error with allowance file: Allowance character " + (std::to_string(i + 1)) + " not a number - line " + std::to_string(index + 1));
			}
		}
		allowanceList[index].first = allowanceIn[0];
		allowanceList[index].second = std::stoi(allowanceIn[1]);
	}

	void Blanksizes::readInBlanksizes(std::string filename)
	{
		//Set up parser, file and line counter
		std::string parser;
		std::ifstream blankIn(filename);
		unsigned int lineNumber = 0;
		unsigned int maxLines = blanksizeList.size();
		//Get rid of title line
		getline(blankIn, parser);
		//Read through file 
		while (lineNumber < maxLines)
		{
			std::vector<std::string> vParserP;
			//Set all attributes in blankfile line
			blankIn >> parser;
			setStyle(parser, lineNumber);
			while (blankIn.peek() != '\n')
			{
				blankIn >> parser;
				vParserP.push_back(parser);
			}
			setParameters(vParserP, lineNumber);
			lineNumber++;
		}
	}

	void Blanksizes::readInAllowances(std::string filename)
	{
		std::string parser;
		std::ifstream allowIn(filename);
		unsigned int lineNumber = 0;
		unsigned int maxLines = allowanceList.size();
		//Get rid of title
		getline(allowIn, parser);
		//Read through file
		while (lineNumber < maxLines)
		{
			std::vector<std::string> vParserA;
			allowIn >> parser;
			vParserA.push_back(parser);
			allowIn >> parser;
			vParserA.push_back(parser);
			setAllowance(vParserA, lineNumber);
			lineNumber++;
		}
	}
    
    std::pair<int,int> Blanksizes::getMatch(std::string oStyle, std::string oFlute)
    {
		int styleMatch = -1;
		int fluteMatch = -1;
        for (unsigned int i = 0; i < blanksizeList.size(); i++)
        {
            if (blanksizeList[i].bStyle == oStyle)
            {
                styleMatch = i;
            }
        }
		for (unsigned int j = 0; j < allowanceList.size(); j++)
		{
			if (allowanceList[j].first == oFlute)
			{
				fluteMatch = j;
			}
		}

        return std::pair<int,int>(styleMatch,fluteMatch);
    }
}