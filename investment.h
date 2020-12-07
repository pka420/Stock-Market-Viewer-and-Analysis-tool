#ifndef INVESTMENT_H
#define INVESTMENT_H

#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <fstream>



class investment {
private:

public:
	//investment()
	investment();						//default constructor
	~investment();						//destructor
	virtual void setup_Investment(std::string str) {}					//setup investment
	virtual void plot_trend() {}						// for plotting graphs.
	virtual void initiate_suggestion_system() {}		// to initiate suggestions

	//ofstream histFile;
};


#endif
