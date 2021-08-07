ORAINCL = -I$(ORACLE_HOME)/rdbms/public
ORALIB =  -L$(ORACLE_HOME)/lib -L.
ORALIBS = -lclntsh
#CFLAGS = -O2 -Wall
CFLAGS = -g  -Wno-write-strings

all:	client remoteserver

client: client.cpp _freecplus.cpp
		g++ -g -o client client.cpp _freecplus.cpp
remoteserver: remoteserver.cpp _freecplus.cpp _ooci.cpp
		g++ $(CFLAGS) -o remoteserver remoteserver.cpp _freecplus.cpp _ooci.cpp \
			$(ORAINCL) $(ORALIB) $(ORALIBS)

clean:
		rm -f client remoteserver

