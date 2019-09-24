# oracle头文件路径
ORAINCL = -I$(ORACLE_HOME)/rdbms/public

# oracle库文件路径
ORALIB =  -L$(ORACLE_HOME)/lib -L.

# oracle的oci库
ORALIBS = -lclntsh

#########################################

CFLAGS = -O2  -Wall
#CFLAGS = -Wall
#CFLAGS = -g -Wall  -Wno-write-strings


all: crtsufdata


crtsufdata: crtsufdata.cpp _public.h _public.cpp
	g++ $(CFLAGS) -o crtsufdata crtsufdata.cpp _public.cpp
        #cp crtsufdata ../bin/
clean:
	rm -f *.o *.a 
