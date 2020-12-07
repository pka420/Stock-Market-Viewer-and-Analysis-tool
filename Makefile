OBJS	= main.o investment.o Stock.o
SOURCE	= main.cpp investment.cpp Stock.cpp 
HEADER	= investment.h Stock.h gnuplot_i.hpp
OUT	= tool
CC	 = g++
FLAGS	 = -g -c 
LFLAGS	 =  -lstdc++

all: $(OBJS)
		#$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
		$(CC) -o $(OUT) $(OBJS) $(LFLAGS)


main.o: main.cpp
		$(CC) $(FLAGS) main.cpp

investment.o: investment.cpp
		$(CC) $(FLAGS) investment.cpp

Stock.o: Stock.cpp
		$(CC) $(FLAGS) Stock.cpp


clean:
		rm -f $(OBJS) $(OUT)
