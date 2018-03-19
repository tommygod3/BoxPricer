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
        //Pricing info
        double pricePerBox = 0.30;
        double priceOnTop = 1.5;
        //Calculated:
        unsigned int allowance;
        double sheetChop;
        double sheetDecal;
        double sheetPrice;
        //Outputs:
        double orderCost;
        double customerPrice;
		double customerPricePer;
        double boxChop;
        double boxDecal;
    public: 
		//Constructor and destructor
        Order();
        ~Order();
		//Function to reset to blank
		void resetAllValues();
        //Functions to set order values
        void setFlute(std::string desiredFlute);
        void setPaperWeight(std::string desiredWeight);
        void setPaperQuality(std::string desiredQuality);
        void setStyle(std::string desiredStyle);
        void setBoxLength(std::string desiredLength);
        void setBoxWidth(std::string desiredWidth);
        void setBoxHeight(std::string desiredHeight);
        void setQuantity(std::string desiredQuantity);
        void setPricePerBox(std::string desiredPricePer);
        void setPriceOnTop(std::string desiredPriceOnTop);
        //Functions to get output values
        double getOrderCost();
        double getCustomerPrice();
		double getCustomerPricePer();
        double getBoxChop();
        double getBoxDecal();
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