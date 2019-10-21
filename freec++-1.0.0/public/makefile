# oracle头文件路径
ORAINCL = -I$(ORACLE_HOME)/rdbms/public

# oracle库文件路径
ORALIB =  -L$(ORACLE_HOME)/lib -L.

# oracle的oci库
ORALIBS = -lclntsh

#########################################

CFLAGS = -O2 -Wall
CFLAGS = -Wall
CFLAGS = -g -Wall  -Wno-write-strings


all:	procctl demo1 demo2 demo3 demo4 demo5 demo6 demo7 demo8 demo11 demo12 demo13 demo14\
	demo15 demo16 libftp.a demo18 demo21 demo22 demo23 

procctl: procctl.cpp 
	g++ $(CFLAGS) -o procctl procctl.cpp 

demo1: demo1.cpp 
	g++ $(CFLAGS) -o demo1 demo1.cpp _public.cpp

demo2: demo2.cpp
	g++ $(CFLAGS) -o demo2 demo2.cpp _public.cpp

demo3: demo3.cpp
	g++ $(CFLAGS) -o demo3 demo3.cpp _public.cpp

demo4: demo4.cpp
	g++ $(CFLAGS) -o demo4 demo4.cpp _public.cpp

demo5: demo5.cpp
	g++ $(CFLAGS) -o demo5 demo5.cpp _public.cpp

demo6: demo6.cpp
	g++ $(CFLAGS) -o demo6 demo6.cpp _public.cpp

demo7: demo7.cpp
	g++ $(CFLAGS) -o demo7 demo7.cpp _public.cpp

demo8: demo8.cpp
	g++ $(CFLAGS) -o demo8 demo8.cpp _public.cpp

demo11: demo11.cpp
	g++ $(CFLAGS) -o demo11 demo11.cpp _public.cpp

demo12: demo12.cpp
	g++ $(CFLAGS) -o demo12 demo12.cpp _public.cpp

demo13: demo13.cpp
	g++ $(CFLAGS) -o demo13 demo13.cpp _public.cpp

demo14: demo14.cpp
	g++ $(CFLAGS) -o demo14 demo14.cpp _public.cpp

demo15: demo15.cpp
	g++ $(CFLAGS) -o demo15 demo15.cpp _public.cpp

demo16: demo16.cpp
	g++ $(CFLAGS) -o demo16 demo16.cpp _public.cpp

libftp.a: ftplib.c
	gcc -c -o libftp.a ftplib.c

demo18: demo18.cpp _ftp.h _ftp.cpp  libftp.a
	g++ $(CFLAGS) -o demo18 demo18.cpp _public.cpp _ftp.cpp libftp.a -lm -lc

demo21: demo21.cpp
	g++ $(CFLAGS) -o demo21 demo21.cpp _public.cpp -lpthread

demo22: demo22.cpp
	g++ $(CFLAGS) -o demo22 demo22.cpp _public.cpp -lpthread

demo23: demo23.cpp
	g++ $(CFLAGS) -o demo23 demo23.cpp _public.cpp -lpthread

pspccfiles: pspccfiles.cpp
	g++ $(CFLAGS) -o pspccfiles pspccfiles.cpp _public.cpp $(ORAINCL) $(ORALIB) $(ORALIBS) _ooci.cpp

clean:
	rm -f *.o *.a 
	rm -rf procctl demo1 demo2 demo3 demo4 demo5 demo6 demo7 demo8 demo11 demo12 demo13 demo14
	rm -rf demo15 demo16 demo18 demo21 demo22 demo23 pspccfiles
