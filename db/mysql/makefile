#########################################

# mysql头文件存放路径
MYSQLINCL = -I/usr/local/mysql/include

# mysql库文件保存路径
MYSQLLIB = -L//usr/local/mysql/lib

# mysql链接库
MYSQLLIBS = -lmysqlclient

#CFLAGS = -O2
CFLAGS = -O2 -Wall

CFLAGS = -g -Wno-write-strings 

all:	createtable inserttable selecttable updatetable deletetable 

createtable: createtable.cpp _mysql.h _mysql.cpp
	g++ $(CFLAGS) -o createtable createtable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) _mysql.cpp -lm -lc

inserttable: inserttable.cpp _mysql.h _mysql.cpp
	g++ $(CFLAGS) -o inserttable inserttable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) _mysql.cpp -lm -lc

selecttable: selecttable.cpp _mysql.h _mysql.cpp
	g++ $(CFLAGS) -o selecttable selecttable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) _mysql.cpp -lm -lc

updatetable: updatetable.cpp _mysql.h _mysql.cpp
	g++ $(CFLAGS) -o updatetable updatetable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) _mysql.cpp -lm -lc

deletetable: deletetable.cpp _mysql.h _mysql.cpp
	g++ $(CFLAGS) -o deletetable deletetable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) _mysql.cpp -lm -lc


clean:	
	rm -rf createtable inserttable selecttable updatetable deletetable
