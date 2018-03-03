#include <string>

//Order or boxes
class Order
{
private:
	//Order inputted
    //What kind of box
	std::string flute;
	unsigned int paperWeight;
	std::string paperQuality;
	std::string style;
    //How large the box is
	double boxLength;
	double boxWidth;
	double boxHeight;
    //How many of them to order
	unsigned int quantity;
    //Used for calculations
	unsigned int allowance[2];
	double sheetChop;
	double sheetDecal;
	double sheetPrice;
    //Pricing information
    double pricePerBox = 0.05;
    double priceOnTop = 100.00;
public:
	//To be worked out and displayed
	double orderCost;
	double customerPrice;
	double boxChop;
	double boxDecal;
public: 
	Order();
	//Functions to set order values used by buttons
    void setFlute(std::string desiredFlute);
    void setPaperWeight(unsigned int desiredWeight);
    void setPaperQuality(std::string desiredQuality);
    void setStyle(std::string desiredStyle);
    void setBoxLength(double desiredLength);
    void setBoxWidth(double desiredWidth);
    void setBoxHeight(double desiredHeight);
    void setQuantity(unsigned int desiredQuantity);
    //Functions to calculate
    void doBlankSize(std::string filename);
	void doStockSheet(std::string filename);
    void doPricing();
    std::string generateInformation();
    void doAllCalculations();
    //Display info to console
    void consoleDisplay();
    void consoleInput();
};