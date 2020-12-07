#ifndef STOCK_H
#define STOCK_H

#include <vector>
#include "investment.h"

//code goes here
class Stock: public investment {
private:
	std::string name;
	std::vector<double> open, close, change;
	//open and close value
	// change is the difference between the two
	std::vector<double> RSI;		// Relative strength Index
	std::vector<double> K;			// Stochastic Calculator
	int days;						// total number of days.
	//std::std::vector<bool> overpriced;	
	std::vector<int> nextMove;					//1 for buying, 2 for selling
												//3 for holding, 4 or everything means calculating
	std::vector<bool> correct;				// to indicate whether our prediction was correct or not
public:
	Stock();
	Stock(std::string); 				// constructor
	~Stock();				// destructor

	
	void setup_Investment(std::string str);			// to setup investment
	void read_csv(std::string str);		// to read from csv files
	void calc_change();					// in order to calculate change vetor from open and close

	void rsi();								// to calculate rsi 
	void stochastic();						// to calculate schoastic
	void plot_stochastic();					// to plot schoastic
	void plot_rsi();						// to plot rsi
	void plot_stock();						// to plot just the stock
	void plot_trend();						// just a shortcut containing other plots data

	void gen_suggestions();
	void compare_suggestions();
	void initiate_suggestion_system();

	void setDays(const int);				// to set the number of days you want to calculate/etc
	int getDays() const;					// to get the total number of days 
};

#endif
