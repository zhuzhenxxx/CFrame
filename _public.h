
#ifndef _public_H
#define _public_H

#include "_cmpublic.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下是XML格式字符串的相关操作函数和类

// 操作XMLBuffer的函数
// in_XMLBuffer，XML格式的字符串，如下：
// <filename>/tmp/readme.txt</filename><mtime>2018-01-01 12:20:35</mtime><size>10241</size>
// in_FieldName，字段的标签名
// out_Value，获取内容存放的变量的指针
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,int    *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned int *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,long   *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned long *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,double *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char   *out_Value,const int in_StrLen=0);

// 读取参数配置文件内容
// 参数文件并不标准的XML，而是一个简单的单标签XML。
class CXmlFile
{
public:
  // 存放参数文件全部的内容，由LoadFile载入到本变量中。
  // 注意，如果文件的内容大于20480，需要加大m_XMLBuffer的内存
  char m_XMLBuffer[20480];

  CXmlFile();

  // 把参数文件的内容载入到m_XMLBuffer变量中。
  bool LoadFile(const char *in_FileName);
  
  // 获取参数文件字段的内容。
  bool GetValue(const char *in_FieldName,int    *out_Value);
  bool GetValue(const char *in_FieldName,long   *out_Value);
  bool GetValue(const char *in_FieldName,UINT   *out_Value);
  bool GetValue(const char *in_FieldName,double *out_Value);
  bool GetValue(const char *in_FieldName,char   *out_Value,int in_len=0);
};

// 以上是XML格式字符串的相关操作函数和类
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下是目录操作的函数和类

// 用某文件或目录的全路径中的目录创建目录，以及该目录下的各级子目录
// pathorfilename 目录名或文件名
// bisfilename true-pathorfilename是文件名，否则是目录名
bool MKDIR(const char *pathorfilename,bool bisfilename=true);

// 读取某目录下的全部的文件
class CDir
{
public:
  char m_DirName[301];         // 目录名
  char m_FileName[301];        // 文件名，不包括目录名
  char m_FullFileName[301];    // 文件全名，包括目录名
  UINT m_FileSize;             // 文件的大小
  char m_ModifyTime[21];       // 文件最后一次被修改的时间
  char m_CreateTime[21];       // 文件生成的时间
  char m_AccessTime[21];       // 文件最后一次被访问的时间

  UINT m_uPOS;                 // 已读取m_vFileName容器的位置
  vector<string> m_vFileName;  //  存放OpenDir方法获取到的文件列表
  
  CDir();

  // 变量初始化
  void initdata();

  // 设置日期时间的格式，支持"yyyy-mm-dd hh:mi:ss"和"yyyymmddhhmiss"两种格式，缺省是前者
  char m_DateFMT[21];
  void SetDateFMT(const char *in_DateFMT);

  // 打开目录，获取文件名信息，存放于m_vFileName容器中
  // in_DirName，待打开的目录名
  // in_MatchStr，待获取文件名的匹配规则
  // in_MaxCount，获取文件的最大数量
  // bAndChild，是否打开各级子目录
  // bSort，是否对结果集进行排序
  bool OpenDir(const char *in_DirName,const char *in_MatchStr,const unsigned int in_MaxCount=10000,const bool bAndChild=false,BOOL bSort=false);

  // 这是一个递归函数，用于OpenDir()的调用。
  bool _OpenDir(const char *in_DirName,const char *in_MatchStr,const unsigned int in_MaxCount,const bool bAndChild);

  // 逐个读取目录中的文件信息
  bool ReadDir();

  ~CDir();
};

// 以上是目录操作的函数和类
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下是日志文件操作类

// 日志文件操作类
class CLogFile 
{
public:
  FILE   *m_tracefp;           // 日志文件指针
  char    m_filename[301];     // 日志文件全名
  char    m_openmode[11];      // 日志文件的打开方式
  char    m_stime[20];         // 日志文件写入时的当前时间变量
  char    m_message[10240];    // 被写入的日志内容
  bool    m_bBackup;           // 日志文件超出100M，是否自动备份
  bool    m_bEnBuffer;         // 写入日志时，是否启用操作系统的缓冲机制
  va_list m_ap;

  CLogFile();

  // filename日志文件名
  // openmode打开文件的方式，操作日志文件的权限,同打开文件函数(fopen)使用方法一致，一般采用"a+"
  // bBackup，true-备份，false-不备份，在多进程的服务程序中，如果多个进行共用一个日志文件，bBackup必须为false
  // bEnBuffer:true-启用缓冲区，false-不启用缓冲区，如果启用缓冲区，那么写进日志文件中的内容不会立即写入文件
  bool Open(const char *in_filename,const char *in_openmode,bool bBackup=true,BOOL bEnBuffer=false);

  // 如果日志文件大于100M，就备份它
  // 备份后的文件会在源文件名后加上日期时间
  bool BackupLogFile();

  // 写日志文件,它是个可变参数的方法,同printf函数。
  // Write()方法会写入时间，WriteEx()方法不写时间。
  bool Write(const char *fmt,...);
  bool WriteEx(const char *fmt,...);

  // 关闭日志文件
  void Close();

  ~CLogFile();
};

// 以上是日志文件操作类
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////// /////////////////////////////////////
// 以下是字符串操作相关的函数和类

// 删除字符串左边指定的字符
void DeleteLChar(char *in_string,const char in_char);

// 删除字符串右边指定的字符
void DeleteRChar(char *in_string,const char in_char);

// 删除字符串两边指定的字符
void DeleteLRChar(char *in_string,const char in_char);

// 删除字符串中间的字符串
void DeleteMStr(char *in_string,const char *in_str);

// 在字符串的左边补字符到指定长度
void LPad(char *in_string,const char in_char,unsigned int in_len);

// 在字符串的右边补字符到指定长度
void RPad(char *in_string,const char in_char,unsigned int in_len);

// 字符串替换函数
// 把in_string中的in_str1替换为in_str2
// bLoop是否循环执行替换
// 注意
// 1、如果in_str2比in_str1要长，替换后in_string会变长，所以必须保证in_string有足够的长度，否则内存会溢出
// 2、如果in_str2中包函了in_str1的内容，且bLoop为true，就会进入死循环，最终导致内存溢出
void UpdateStr(char *in_string,const char *in_str1,const char *in_str2,bool bLoop=true);

// 把小写转换成大写，忽略不是字母的字符
void ToUpper(char *str);
void ToUpper(string &str);

// 把大写转换成小写，忽略不是字母的字符
void ToLower(char *str);
void ToLower(string &str);

// 从一个字符串中提取数字、符号和小数点，并判断是否是一个合法的数
// 如果不合法，将返回空字符串
// bWithSign==true表示包括符号，bWithDOT==true表示包括圆点
void PickNumber(const char *strSrc,char *strDst,const bool bWithSign,const bool bWithDOT);

// 判断文件名是否匹配in_MatchStr指定的规则
// in_FileName文件名
// in_MatchStr规则表达式，如"*.txt,*.xml"，中间用逗号分隔
bool MatchFileName(const string in_FileName,const string in_MatchStr);

// 判断内容是否全部是数字
bool IsDigit(const char *strBuffer);

// 判断内容是否全部是大写字母
bool IsUpper(const char *strBuffer);

// 判断内容是否全部是ASCII字符
bool IsASCII(const char *strBuffer);

// 判断内容是否全部是数字和空格
bool IsDigitOrSpace(const char *strLine);

// 拆分字符串的类
// 字符串的格式为:内容1+分隔字符串+内容2+分隔字符串+内容3
// 如:num~!~name~!~address,分隔符为"~!~"
class CCmdStr 
{
public:
  vector<string> m_vCmdStr;  // 存放拆分后的字段内容。

  CCmdStr();

  // 拆分字符串到容器中
  void SplitToCmd(const string in_string,const char *in_sep,const bool bdeletespace=true);

  int CmdCount();

  // 获取字段的值，取每个字段的值inum从0开始
  bool GetValue(const int inum,char   *in_return);
  bool GetValue(const int inum,char   *in_return,const int in_len);
  bool GetValue(const int inum,int    *in_return);
  bool GetValue(const int inum,long   *in_return);
  bool GetValue(const int inum,double *in_return);

  ~CCmdStr();
};

// 以上是字符串操作相关的函数和类
///////////////////////////////////// /////////////////////////////////////


///////////////////////////////////// /////////////////////////////////////
// 以下是日期时间操作函数和类

/*
  取操作系统的时间
  out_stime是输出结果
  in_interval是偏移常量，单位是秒
  返回的格式由fmt决定，fmt目前的取值如下，如果需要，可以增加：
  yyyy-mm-dd hh:mi:ss，此格式是缺省格式
  yyyymmddhhmiss
  yyyy-mm-dd
  yyyymmdd
  hh:mi:ss
  hhmiss
  hh:mi
  hhmi
  hh
  mi
*/
void LocalTime(char *out_stime,const char *in_fmt=0,const int in_interval=0);

// 把字符串格式的时间转换为time_t
// stime为输入的时间，格式不限，但一定要包括yyyymmddhhmiss
time_t UTCTime(const char *stime);

/*
  把一个字符串表格的时间加上一个偏移量，得到偏移后的时间
  in_stime是传入的时间，任意格式，但是一定要包括yyyymmddhhmiss，是否有分隔符没有关系。
  把yyyy-mm-dd hh:mi:ss偏移in_interval秒
  传出的格式由fmt决定，fmt目前的取值如下，如果需要，可以增加：
  yyyy-mm-dd hh:mi:ss（此格式是缺省格式）
  yyyymmddhhmiss
  yyyymmddhhmiss
  yyyy-mm-dd
  yyyymmdd
  hh:mi:ss
  hhmiss
  hh:mi
  hhmi
  返回值：0-成功，-1-失败。
*/
int AddTime(const char *in_stime,char *out_stime,const int in_interval,const char *in_fmt=0);

// 这是一个精确到微秒的计时器
class CTimer
{
public:
  struct timeval m_start,m_end;

  CTimer();

  // 开始计时
  void Start();

  // 计算已逝去的时间，单位：秒，小数点后面是微秒
  double Elapsed();
};

// 以上是日期时间操作函数和类
///////////////////////////////////// /////////////////////////////////////


///////////////////////////////////// /////////////////////////////////////
// 以下是TCP/IP通讯的函数和类

#define TCPHEADLEN     5  // TCP报文头部的长度，不能超过10。
#define TCPBUFLEN  10240  // TCP报文的最大长度，不能超过TCPHEADLEN能表达的长度。

// 用于TCP通讯的客户类
class CTcpClient
{
public:
  int  m_sockfd;        // socket id
  char m_IP[21];        // 服务端的IP
  int  m_Port;          // 与服务端通讯的端口
  bool m_State;         // 连接状态
  char m_ConnStr[200];  // 与服务端的连接参数，格式：ip1,port1,ip2,port2,...
  bool m_bTimeOut;      // Read()方法是否超时，true-未超时，false-已超时
  int  m_BufLen;        // 接收到的报文的包大小

  CTcpClient();

  // 向服务端发起连接
  // in_ConnStr服务端参数，格式：ip1,port1,ip2,port2,...
  // 注意，in_ConnStr支持多组参数，这么做的目的是为了支持多个服务端
  bool ConnectToServer(const char *in_ConnStr);                // 连接服务器
  
  // 没有超时机制的接收函数
  bool Read(char *strRecvBuffer);
  // 有超时机制的接收函数
  bool Read(char *strRecvBuffer,const int iTimeOut);

  // 向服务端发送字符串，长度为字符串的长度
  bool Write(char *strSendBuffer);             

  // 向服务端发送字符串，需要指定长度
  bool Write(char *strSendBuffer,const int buflen);
  
  // 断开与服务端的连接
  void Close();

  ~CTcpClient();
};

// 用于TCP通讯的服务类
class CTcpServer
{
public:
  int m_socklen;                    // struct sockaddr_in的大小
  struct sockaddr_in m_clientaddr;  // 客户端的地址信息
  struct sockaddr_in m_servaddr;    // 服务端的地址信息
  int  m_listenfd;                  // 服务端用于监听的socket
  int  m_connfd;                    // 客户端的socket
  bool m_bTimeOut;                  // Read()方法是否超时，true-未超时，false-已超时
  int  m_BufLen;                    // 接收到的报文的包大小

  CTcpServer();
  bool InitServer(const unsigned int port); // 初始化TcpServer
  bool Accept();                            // 接受客户端的请求
  char *GetIP();                            // 获取客户端的ip地址
  
  // 没有超时机制的接收函数
  bool Read(char *strRecvBuffer);
  // 有超时机制的接收函数
  bool Read(char *strRecvBuffer,const int iTimeOut);

  // 向服务端发送字符串，长度为字符串的长度
  bool Write(char *strSendBuffer);             

  // 向服务端发送字符串，需要指定长度
  bool Write(char *strSendBuffer,const int buflen);
  
  // 关闭监听的sock，用于多进程服务程序的子进程代码中
  void CloseListen();

  // 关闭客户端的sock，用于多进程服务程序的父进程代码中
  void CloseClient();

  ~CTcpServer();
};

// 读取socket的数据
// fd socket
// strRecvBuffer 读取结果存放的缓冲区
// buflen 读取到的报文的长度
// iTimeOut 读取超时的时间
bool TcpRead(const int fd,char *strRecvBuffer,int *buflen,const int iTimeOut=0);

// 把数据写入socket
// fd socket
// strSendBuffer 待写入数据存放的缓冲区
// buflen 待写入数据的长度，如果为0，就认为strSendBuffer是一个字符串。
// iTimeOut 写入超时的时间
bool TcpWrite(const int fd,const char *strSendBuffer,const int buflen=0,const int iTimeOut=0);

// 从socket中读取数据，专用于TcpRead函数
bool Readn(const int fd,char *vptr,const size_t n);

// 向socket中写入数据，专用于TcpWrite函数
bool Writen(const int fd,const char *vptr,const size_t n);

// 以上是TCP/IP通讯的函数和类
///////////////////////////////////// /////////////////////////////////////


///////////////////////////////////// /////////////////////////////////////
// 以下是文件操作的类和函数

// 文件操作类
class CFile
{
public:
  FILE *m_fp;                 // 文件指针
  char  m_filename[301];      // 不包括路径的文件名
  char  m_filenametmp[301];   // 待生成文件的临时文件名
  bool  m_bEnBuffer;          // 是否启用缓冲区的标志，true-启用；false-不启用。
  int   m_filetype;           // 文件打开的方式:1-只读；2-只写；3-写完后重命名。

  CFile();
  CFile(bool bEnBuffer);
 ~CFile();  // 析构函数中调用CloseOnly函数

  void initdata();

  // 返回文件是否打开，true表示已经打开，false表示没有打开
  bool IsOpened();

  // 打开一个文件用于读
  // filename为全路径的文件名。
  // openmode为打开文件的方式，只能用"rt"或"rb"，
  // 用此方法打开的文件，可以用CloseAndRemove和CloseOnly关闭
  // 此方法将把m_filetype的值置为1
  bool OpenForRead(const char *filename, const char *openmode);

  // 关闭已打开的只读文件，并删除它。
  bool CloseAndRemove();

  // 打开一个文件用于写，不采用临时文件缓存，这种应用的场景比较少
  // 如果不采用临时文件缓存，其它程序很容易读取不完整的文件
  // filename为全路径的文件名。
  // openmode为打开文件的方式，同fopen使用方法一致，不包括"rt"和"rb"
  // bEnBuffer:true-启用缓冲区，false-不启用缓冲区
  // 用此方法打开的文件，只能用CloseOnly关闭
  // 此方法将把m_filetype的值置为2
  bool OpenForWrite(const char *filename, const char *openmode, bool bEnBuffer=false);

  bool Open(const char *filename, const char *openmode, bool bEnBuffer);

  // 打开一个文件用于写，采用临时文件缓存
  // 此方法先把内容写入"*.tmp"文件，关闭时调用CloseAndRename改为正式的文件名
  // filename为全路径的文件名。
  // openmode为打开文件的方式，同fopen使用方法一致，不包括"rt"和"rb"
  // bEnBuffer:true-启用缓冲区，false-不启用缓冲区
  // 用此方法打开的文件，可以用CloseAndRename关闭，如果用CloseOnly关闭，就不会生成正式文件
  // 此方法将把m_filetype的值置为3
  bool OpenForRename(const char *filename, const char *openmode, bool bEnBuffer=false);

  // 关闭文件指针，并把临时文件名改为正式文件名，如果改名失败，将返回false，成功返回true
  bool CloseAndRename();

  // 关闭文件指针
  // 如果采用了OpenForRename打开文件，就删除掉临时文件
  void CloseOnly();

  // 调用FGETS函数读取一行
  bool FFGETS(char *strBuffer,const int ReadSize,const char *strEndStr=0);

  // 调用fread从文件中读取数据
  size_t Fread(void *ptr,size_t size);

  // 调用fwrite向文件中写数据
  size_t Fwrite(const void *ptr,size_t size);
  
  // 调用fprintf往已打开的文件写入数据。
  void Fprintf(const char *fmt, ... );
};

// 调用fopen函数打开文件，如果文件名中包含的目录不存在，就创建目录
FILE *FOPEN(const char *filename,const char *mode);

// 更改文件的修改时间属性，mtime指定了时间，格式不限，只要包括了yyyy,mm,dd,hh,mi,ss即可。
int UTime(const char *filename,const char *mtime);

// 判断文本文件是否以strEnd结束
// 用于判断文件的完整性
bool CheckFileEnd(const char *strFileName,const char *strEnd);

// 从文件文件中读取一行
// strEndStr是一行数据的结束标志，如果为空，则以换行符"\n"为结束标志。
bool FGETS(const FILE *fp,char *strBuffer,const int ReadSize,const char *strEndStr=0);

// 获取文件的大小，返回字节数
int FileSize(const char *in_FullFileName);

// 获取文件的时间，即modtime
void FileMTime(const char *in_FullFileName,char *out_ModTime);

// 以上是文件操作的类和函数
///////////////////////////////////// /////////////////////////////////////

// 关闭全部的信号和输入输出
void CloseIOAndSignal();

//声明RAND函数，返回一个x-y的不重复随机数列
void RAND(const int x,const int y,int *arr);

//4个函数STRCPY,STRNCPY,STRCAT,STRNCAT弥补库函数缺陷 大小写便于区分
//C++里使用函数重载
//可以用来取代库函数中的strcpy,strncpy,strcat,strncat
//函数第二个参数是第一个参数dest的有效长度，一般为:占用字节(sizeof)-1
//重写库函数主要解决三个问题
//1.变量初始化
//2.内存溢出
//3.修复strncpy的n<len 不追加\0 缺陷
//赋值或赋值字符串

char *STRCPY(char *dest,size_t destlen,const char* src);

//赋值或赋值字符串前n个字符
char *STRNCPY(char* dest,size_t destlen,const char *src,size_t n);

//字符串拼接前n个字符
char *STRNCAT(char* dest,size_t destlen,const char* src,size_t n);

//SNPRINTF把结果输出到字符串,增加了str的初始化
int SNPRINTF(char *str,size_t size,const char *format,...);


#endif
