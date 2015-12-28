CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei

all:  autotracker

autotracker.o: autotracker.cpp
	g++ -c $(CFLAGS) autotracker.cpp  

autotracker: autotracker.o 
	g++ $(LFLAGS) -o autotracker autotracker.o

clean:
	rm *.o autotracker
