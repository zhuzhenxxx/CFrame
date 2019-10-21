#########################################
# 请注意，编译时，确定以下几点：
# 1、已安装oracle的客户端或oracle服务端程序，并且必须安装好OCI软件包
# 2、设置好ORACLE_HOME环境变量
#
#########################################

# oracle头文件路径
ORAINCL = -I$(ORACLE_HOME)/rdbms/public

# oracle库文件路径
ORALIB =  -L$(ORACLE_HOME)/lib -L.

# oracle的oci库
ORALIBS = -lclntsh

# CFLAGS = -O2
# CFLAGS = -O2 -Wall

CFLAGS = -g -Wall -Wno-write-strings -Wno-unused-variable

all:    bankserver bankclient

bankserver: bankserver.cpp _ooci.h _ooci.cpp bank.h bank.cpp
	g++ $(CFLAGS) -o bankserver bankserver.cpp $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp bank.cpp -lm -lc
	cp bankserver ../bin/.

bankclient: bankclient.cpp 
	g++ $(CFLAGS) -o bankclient bankclient.cpp _public.cpp -lm -lc
	cp bankclient ../bin/.

clean:
	rm -rf bankserver bankclient
