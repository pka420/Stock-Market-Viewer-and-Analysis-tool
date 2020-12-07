#include "Stock.h"
#include "gnuplot-cpp/gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    std::cout << std::endl << "Press any key to continue..." << std::endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::cout << std::endl << "Press ENTER to continue..." << std::endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}

Stock::Stock() {
	//default constructor
}

Stock::Stock(std::string name)  {
	//constructor
	//POSTCONDITION: reserve size for our vectors
	this->name = name;
	open.reserve(60);
	close.reserve(60);
	change.reserve(60);
	days = 58;
	nextMove.reserve(days-14);
	correct.reserve(days-14);
}

Stock::~Stock() {
	open.clear();
	open.shrink_to_fit();
	close.clear();
	close.shrink_to_fit();
	change.clear();
	change.shrink_to_fit();
}

void Stock::read_csv(std::string str) {
	//PRECONDITION : string str is the name of input (csv) files.
	//POST CODITION: it reads the csv files for the data.
	// and it also calculates change.
	std::ifstream fin;
	fin.open(str);
	if(fin.fail()) {
		std::cerr<<"Cannot open file";
		abort();
	}
	
	std::string line;

	char ch;
	double temp;

	while(!fin.eof()) {
		fin>>temp;
		open.push_back(temp);
		fin>>ch; 		// reading ,
		fin>>temp;
		close.push_back(temp);
		fin>>ch;		// reading , 
	}
	//now the files have been read.
	days = open.size();
	calc_change();

	// for testing:
	/*for(int i=0; i<open.size();i++ ) {
		std::cout<<open[i]<<" , "<<close[i]<<" , "<<change[i]<<std::endl;
	}*/
	fin.close();
} 

void Stock::calc_change() {
	//POST CONDITION: fills up change by subtracting open value from close value
	// it helps the reac_csv function.
	for(int i=0;i<days;i++) {
		change.push_back( close[i] - open[i] );
	}
}

void Stock::rsi() {
	double AvgDown, AvgUp, RS, temp;
	int up_days, down_days;
	for(int i= 14; i<days; i++) {
		AvgUp = 0;
		AvgDown = 0;
		up_days = down_days = 0;
		for(int j = (i-14) ; j<i; j++) {
			if(change[j] > 0) {
				AvgUp = AvgUp + change[j];
				up_days++;
			}
			else {
				AvgDown = AvgDown + change[j];
				down_days++;
			}
		}
		AvgUp = AvgUp/up_days;
		AvgDown = AvgDown/down_days;
		AvgDown = fabs(AvgDown);

		RS = (AvgUp/AvgDown);
		temp = 100.00 - 100.00/(RS+1.00);
		//std::cout<<temp<<std::endl;
		RSI.push_back(temp);
	}

	/*for(int i=0;i<RSI.size();i++) {
		std::cout<<RSI[i]<<std::endl;
	}*/
	
}

void Stock::stochastic() {
	double L14, H14, temp;
	for(int i=14;i<days;i++) {
		L14 = H14 = close[i-14];
		for(int j = (i-13); j<i; j++) {
			if(H14 < close[j]) {
				H14 = close[j];
			}
			if(L14 > close[j]) {
				L14  = close[j];
			}
		}
		temp = (close[i] - L14)/(H14 - L14);
		temp = fabs(temp)*100;
		K.push_back(temp);

	}
	//for testing :	
	/*for(int i=0;i<K.size();i++) {
		std::cout<<K[i]<<std::endl;
	}*/
}

void Stock::setup_Investment(std::string str) {
	//std::cout<<"Reading file: "<<str<<std::endl;
	read_csv(str);
	//std::cout<<"Rsi values are: "<<std::endl;
	this->rsi();
	//std::cout<<"Stochastic values are: "<<std::endl;
	this->stochastic();
}

void Stock::gen_suggestions() {
//POSTCONDITION: 
	//it checks our rsi and stochastic calculations' results 
	//and tells us whether to buy, sell or hold a given stock
	std::cout<<"		>Generating suggestions......"<<std::endl;
	for(int i = 0;i< days-14;i++) {
		if(RSI[i] >=70 || K[i] >= 70)
			nextMove[i] = 2;			// state of sell it
		else if(RSI[i] <= 30 || K[i] <= 30) 
			nextMove[i] = 1;			// state of buying it
		else
			nextMove[i] = 3;
	}

}

void Stock::compare_suggestions() {
//POSTCODITION: 
	//compares our nextMove vector with the next day's change and
	//tells us the report of how much suggestions were correct

	int count_correct = 0;						// count the number of suggestions
	std::cout<<"		>Comparing suggestions......."<<std::endl;
	for(int i=0;i<days-14;i++) {
		if(change[i+1] < 0 && nextMove[i] == 2) {
			correct[i] = true;
			count_correct++;
		}
		else
			correct[i] = false;
	}

	std::cout<<"Analysing correct suggestions of "<< name <<" stock :"<<std::endl;
	std::cout<<count_correct<<" Suggestions were correctly predicted"<<std::endl;
}

void Stock::initiate_suggestion_system() {
	gen_suggestions();
	compare_suggestions();
}

int Stock::getDays() const{
	return days;
}

void Stock::setDays(const int d) {
	days = d;
}


void Stock::plot_rsi() {
	Gnuplot pk;
	std::string str = " Stock RSI Plot";
	str = name + str;
	pk.set_smooth().plot_x(RSI,str);
    pk.unset_smooth();
    wait_for_key();
}

void Stock::plot_stochastic() {
	Gnuplot pk;
	std::string str = " StockOsc Plot";
	str = name+str;
	pk.set_smooth().plot_x(K,str);
	pk.unset_smooth();
	wait_for_key();
}

void Stock::plot_stock() {
	std::string str = " stock Plot";
	str = name+str;

	double high, low;

	std::ofstream fout;
	fout.open("temp.dat");
	for(int i=1;i<=days;i++) {
		fout<<i<<" ";
		
		if(open[i-1] > close[i-1]) {
			high = open[i-1];
			low = close[i-1];
		}
		else {
			high = close[i-1];
			low = open[i-1];
		}

		fout<< open[i-1] <<" "<< ++high <<" ";
		fout<< --low <<" "<< close[i-1] <<" \n";
	}
	fout.close();
	Gnuplot pk;
	
	pk<<"set xrange [1:59]";
	pk<<"set datafile separator " " ";
	pk.set_title(str);
	//str = str+".png";
	//pk<<"set term png";
	pk<<"plot 'temp.dat' using 1:2:3:4:5 notitle with candlesticks ";
	wait_for_key();
} 

void Stock::plot_trend() {
	std::cout<<"hi"<<std::endl;
	plot_rsi();
	plot_stochastic();
	plot_stock();
	wait_for_key();
}