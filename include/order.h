#ifndef ORDER_H
#define ORDER_H

#include <string>

#include "blanksize.h"
#include "stockboard.h"

namespace BP
{
    //Order or boxes
    class Order
    {
    private:
        //Blanksize data structure
        Blanksizes* blanksizes;
        Stockboard* stockboard;

        //Copy constructor and assignment operator to keep clean due to blanksizes pointer
        Order(const Order& copy_to_other);
        Order & operator= (const Order & other);
        //Inputs:
        //What kind of box
        std::string flute = "";
        unsigned int paperWeight = 0;
        std::string paperQuality = "";
        std::string style = "";
        //How large the box is
        double boxLength = 0;
        double boxWidth = 0;
        double boxHeight = 0;
        //How many of them to order
        int quantity = -1;
		//Full/Half/Both
		int fullHalfPolicy = 0;
		//Normal or forcing sheets only
		bool forceSheets = 0;
        //Pricing info
        double pricePerBox = 0.30;
        double priceOnTop = 1.5;
        //Calculated:
        unsigned int allowance;
        double sheetChop;
        double sheetDecal;
        double sheetPrice;
		double cBoxesPerSheet;
		double dBoxesPerSheet;
		int countFull = 0;
		int countHalf = 0;
        //Outputs:
        double orderCost;
		double orderCostPer;
        double customerPrice;
		double customerPricePer;
        double boxChop;
		double boxHalfChop;
        double boxDecal;
    public: 
		//Constructor and destructor
        Order();
        ~Order();
		//Function to reset to blank
		void resetAllValues();
		void resetFlute();
		void resetPaperWeight();
		void resetPaperQuality();
		void resetStyle();
        //Functions to set order values
        void setFlute(std::string desiredFlute);
        void setPaperWeight(std::string desiredWeight);
        void setPaperQuality(std::string desiredQuality);
        void setStyle(std::string desiredStyle);
        void setBoxLength(std::string desiredLength);
        void setBoxWidth(std::string desiredWidth);
        void setBoxHeight(std::string desiredHeight);
        void setQuantity(std::string desiredQuantity);
		void setFullHalf(int desiredFullHalf);
        void setPricePerBox(std::string desiredPricePer);
        void setPriceOnTop(std::string desiredPriceOnTop);
		void setForceSheets(bool value);
        //Functions to get output values
        double getOrderCost();
		double getOrderCostPer();
        double getCustomerPrice();
		double getCustomerPricePer();
        double getBoxChop();
		double getBoxHalfChop();
        double getBoxDecal();
		double getSheetChop();
		double getSheetDecal();
		double getSheetPrice();
		//Functions to return available inputs 
		std::vector<std::string> availableStyles();
		std::vector<std::string> getValidInputs(int option);
        //Functions that calculate and produce information
        void doAllCalculations();
        std::string checkSet();
        void doBlankSize();
        void doStockSheet();
		int getPricingTier();
		int getCheaperTier();
        void doPricing();
		double sqMetBox();
		double sqMetOrder();
		int quantBoxNeeded();
		int noOfSheets();
		bool styleHasHalf();
		std::pair<int, int> getChopCounts();
		int getDecalCount();
		int getPolicy();
		//Temp work around for sheet instead of box
		bool sheetNoBox();
		//Functions that check inputs are set
		bool checkFluteSet();
		bool checkPaperWeightSet();
		bool checkPaperQualitySet();
		bool checkStyleSet();
		bool checkBoxLengthSet();
		bool checkBoxWidthSet();
		bool checkBoxHeightSet();
		bool checkQuantitySet();
		bool checkPricePerBoxSet();
		bool checkPriceOnTopSet();
        //Functionality if using console
        void consoleDisplay();
        void consoleInput();
		std::string generateConsoleInformation();
    };
}

#endif