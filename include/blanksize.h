#ifndef BLANKSIZE_H
#define BLANKSIZE_H

#include <string>
#include <vector>
#include <utility>

namespace BP
{
    //Allowance is flute and allowance
    using allowancePair = std::pair<std::string,unsigned int>;

    struct blanksize
    {
        std::string bStyle;
        std::vector<double> bParameters;
    };

    class Blanksizes
    {
    public:
        //Data structure, list of blinds
        std::vector<blanksize> blanksizeList;
		std::vector<allowancePair> allowanceList;
        //Functions:
        Blanksizes(std::string filepath);
        //Gets size of file to initialise data structure
        unsigned int getLines(std::string filename, int filetype);
        //Functions to set members and throw if incorrect
        void setStyle(std::string styleIn, unsigned int index);
        void setParameters(std::vector<std::string> parametersIn, unsigned int index);
        void setAllowance(std::vector<std::string> allowanceIn, unsigned int index);
        //Read into data structure from files, throw if error
        void readInBlanksizes(std::string filename);
		void readInAllowances(std::string filename);
        //Function to take in a style and flute and check if it's in the structure
        std::pair<int,int> getMatch(std::string oStyle, std::string oFlute);
    };
}

#endif