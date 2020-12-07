#include "Stock.h"

int main(){
	std::string str = "tesla";
	Stock Tesla(str);
	str = "tesla.csv";
	Tesla.setup_Investment(str);
	str = "nvda";
	Stock Nvidia("nvda");
	str = "nvda.csv";
	Nvidia.setup_Investment(str);
	str = "amd";
	Stock AMD(str);
	str = "amd.csv";
	AMD.setup_Investment(str);
	//all the stocks are initialized.
	//std::list<investment> companies = {Tesla, AMD, Nvidia};
	// the list of investments is ready.
	Tesla.plot_trend();
	Nvidia.plot_trend();
	AMD.plot_trend();

	Tesla.initiate_suggestion_system();
	Nvidia.initiate_suggestion_system();
	AMD.initiate_suggestion_system();
	

	return 0;
}
