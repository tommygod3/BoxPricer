#ifndef STOCKBOARD_H
#define STOCKBOARD_H

#include <string>
#include <vector>
#include <utility>

namespace BP
{
    struct stockItem
    {
        std::string sFlute;
        unsigned int sPaperWeight;
        std::string sPaperQuality;
        double sSheetChop;
        double sSheetDecal;
        double sSheetPrice;
    };

    class Stockboard
    {
    public:
        //Data structure, list of blinds
        std::vector<stockItem> theStockboard;
        //Functions:
        Stockboard();
        //Gets size of file to initialise data structure
        unsigned int getLines(std::string filename);
        //Functions to set members and throw if incorrect
        void setFlute(std::string fluteIn, unsigned int index);
        void setPaperWeight(std::string weightIn, unsigned int index);
        void setPaperQuality(std::string qualityIn, unsigned int index);
        void setSheet(std::vector<std::string> sheetIn, unsigned int index);
        //Read into data structure from file, throw if error
        void readIn(std::string filename);
        //Function to take in flute, weight and quality and check if it's in the structure
        std::vector<int> getMatches(std::string oFlute,unsigned int oWeight, std::string oQuality);
    };
}

#endif