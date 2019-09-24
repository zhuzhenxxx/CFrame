
#include "_public.h"
/**
 * 作者：朱振兴
 * 框架结构：
      XML：
      简单XML格式文件的加载和XML字符串操作，用于参数配置文件和XML字符串的解析
      日期和时间：
      获取操作系统的时间，时间运算和精确到微秒的计时器。
      字符串和数字：
      字符串的格式化、删除空格、替换、大小写转换、正则表达式、拆分、数字读取。
      日志文件：
      日志文件的创建和备份，日志内容的格式化写入。
      目录操作：
      目录的创建，目录及各级子目录文件名的读取，排序。
      文件操作：
      文件的创建，内容的读取和格式化写入，文件的删除和缓冲式命名方法。
      TCP/IP通讯：
      TCP/IP通讯的客户端和服务端
      FTP客户端：
      采用ftp协议，实现文件的收发以及其他ftp客户端的全部功能
      数据库驱动：
      实现了数据库连接池和SQL语句操作两大功能，支持oracle、MySQL,简单易用、充分利用c语言的指针优势。

      部分函数重载了库函数
 */
// 操作XMLBuffer的函数
// in_XMLBuffer，XML格式的字符串
// in_FieldName，字段的标签名
// out_Value，获取内容存放的变量的指针
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char *out_Value,const int in_Len)
{
  strcpy(out_Value,"");

  char *start=NULL,*end=NULL;
  char m_SFieldName[51],m_EFieldName[51];

  int m_NameLen = strlen(in_FieldName);
  memset(m_SFieldName,0,sizeof(m_SFieldName));
  memset(m_EFieldName,0,sizeof(m_EFieldName));

  snprintf(m_SFieldName,50,"<%s>",in_FieldName);
  snprintf(m_EFieldName,50,"</%s>",in_FieldName);

  start=0; end=0;

  start = (char *)strstr(in_XMLBuffer,m_SFieldName);

  if (start != 0)
  {
    end   = (char *)strstr(start,m_EFieldName);
  }

  if ((start==0) || (end == 0))
  {
    return false;
  }

  int   m_ValueLen = end - start - m_NameLen - 2 + 1 ;

  if ( ((m_ValueLen-1) <= in_Len) || (in_Len == 0) )
  {
    strncpy(out_Value,start+m_NameLen+2,m_ValueLen-1);
  }
  else
  {
    strncpy(out_Value,start+m_NameLen+2,in_Len);
  }

  DeleteLRChar(out_Value,' ');

  return true;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,int *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51]; 

  memset(strTemp,0,sizeof(strTemp));
  
  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atoi(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned int *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51]; 

  memset(strTemp,0,sizeof(strTemp));
  
  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = (unsigned int)atoi(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,long *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51]; 

  memset(strTemp,0,sizeof(strTemp));
  
  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atol(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned long *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51]; 

  memset(strTemp,0,sizeof(strTemp));
  
  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = (unsigned long)atol(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,double *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51]; 

  memset(strTemp,0,sizeof(strTemp));
  
  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atof(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,float *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51]; 

  memset(strTemp,0,sizeof(strTemp));
  
  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atof(strTemp); return true;
  }

  return false;
}

// 读取XML文件中的内容
CXmlFile::CXmlFile()
{
  memset(m_XMLBuffer,0,sizeof(m_XMLBuffer));
}

bool CXmlFile::LoadFile(const char *in_FileName)
{
  memset(m_XMLBuffer,0,sizeof(m_XMLBuffer));

  char strLine[1024]; 

  FILE *fp=0;

  if ( (fp=fopen(in_FileName,"rt")) == NULL) return false;

  memset(strLine,0,sizeof(strLine));

  while (true)
  {
    memset(strLine,0,sizeof(strLine));

    if (FGETS(fp,strLine,1000) == false) break;

    // 防止m_XMLBuffer溢出
    if ( (strlen(m_XMLBuffer)+strlen(strLine)) >= (UINT)20000 ) break;
  
    strcat(m_XMLBuffer,strLine);
  }

  fclose(fp);

  fp=0;

  if (strlen(m_XMLBuffer) < 10) return false;

  return true;
}

bool CXmlFile::GetValue(const char *in_FieldName,char *out_Value,int in_len)
{
  return GetXMLBuffer(m_XMLBuffer,in_FieldName,out_Value,in_len);
}

bool CXmlFile::GetValue(const char *in_FieldName,int *out_Value)
{
  return GetXMLBuffer(m_XMLBuffer,in_FieldName,out_Value);
}

bool CXmlFile::GetValue(const char *in_FieldName,long *out_Value)
{
  return GetXMLBuffer(m_XMLBuffer,in_FieldName,out_Value);
}

bool CXmlFile::GetValue(const char *in_FieldName,UINT *out_Value)
{
  return GetXMLBuffer(m_XMLBuffer,in_FieldName,out_Value);
}

bool CXmlFile::GetValue(const char *in_FieldName,double *out_Value)
{
  return GetXMLBuffer(m_XMLBuffer,in_FieldName,out_Value);
}

CDir::CDir()
{
  m_uPOS=0;

  memset(m_DateFMT,0,sizeof(m_DateFMT));
  strcpy(m_DateFMT,"yyyy-mm-dd hh:mi:ss");

  m_vFileName.clear();

  initdata();
}

void CDir::initdata()
{
  memset(m_DirName,0,sizeof(m_DirName));
  memset(m_FileName,0,sizeof(m_FileName));
  memset(m_FullFileName,0,sizeof(m_FullFileName));
  m_FileSize=0;
  memset(m_CreateTime,0,sizeof(m_CreateTime));
  memset(m_ModifyTime,0,sizeof(m_ModifyTime));
  memset(m_AccessTime,0,sizeof(m_AccessTime));
}

// 设置日期时间的格式，支持"yyyy-mm-dd hh:mi:ss"和"yyyymmddhhmiss"两种格式，缺省是前者
void CDir::SetDateFMT(const char *in_DateFMT)
{
  memset(m_DateFMT,0,sizeof(m_DateFMT));
  strcpy(m_DateFMT,in_DateFMT);
}

// 打开目录，获取文件名信息，存放于m_vFileName容器中
// in_dirname，待打开的目录名
// in_MatchStr，待获取文件名的匹配规则
// in_MaxCount，获取文件的最大数量
// bAndChild，是否打开各级子目录
// bSort，是否对结果时行排序
bool CDir::OpenDir(const char *in_DirName,const char *in_MatchStr,const unsigned int in_MaxCount,const BOOL bAndChild,BOOL bSort)
{
  m_uPOS=0;
  m_vFileName.clear();

  // 如果目录不存在，就创建该目录
  if (MKDIR(in_DirName,false) == FALSE) return FALSE;

  bool bRet=_OpenDir(in_DirName,in_MatchStr,in_MaxCount,bAndChild);

  if (bSort==true)
  {
    sort(m_vFileName.begin(), m_vFileName.end());
  }

  return bRet;
}

// 打开目录，这是个递归函数
bool CDir::_OpenDir(const char *in_DirName,const char *in_MatchStr,const unsigned int in_MaxCount,const BOOL bAndChild)
{
  DIR *dir;

  if ( (dir=opendir(in_DirName)) == NULL ) return false;

  char strTempFileName[1024];

  struct dirent *st_fileinfo;
  struct stat st_filestat;

  while ((st_fileinfo=readdir(dir)) != NULL)
  {
    // 以"."打头的文件不处理
    if (st_fileinfo->d_name[0]=='.') continue;
        
    memset(strTempFileName,0,sizeof(strTempFileName));

    snprintf(strTempFileName,300,"%s//%s",in_DirName,st_fileinfo->d_name);

    UpdateStr(strTempFileName,"//","/");

    stat(strTempFileName,&st_filestat);

    // 判断是否是目录
    if (S_ISDIR(st_filestat.st_mode))
    {
      if (bAndChild == true)
      {
        if (_OpenDir(strTempFileName,in_MatchStr,in_MaxCount,bAndChild) == false) 
        {
          closedir(dir); return false;
        }
      }
    }
    else
    {
      if (MatchFileName(st_fileinfo->d_name,in_MatchStr) == false) continue;

      m_vFileName.push_back(strTempFileName);

      if ( m_vFileName.size()>in_MaxCount ) break;
    }
  }

  closedir(dir);

  return true;
}

/*
st_gid 

Numeric identifier of group that owns file (UNIX-specific) This field will always be zero on NT systems. A redirected file is classified as an NT file.

st_atime

Time of last access of file.

st_ctime

Time of creation of file.

st_dev

Drive number of the disk containing the file (same as st_rdev).

st_ino

Number of the information node (the inode) for the file (UNIX-specific). On UNIX file systems, the inode describes the file date and time stamps, permissions, and content. When files are hard-linked to one another, they share the same inode. The inode, and therefore st_ino, has no meaning in the FAT, HPFS, or NTFS file systems.

st_mode

Bit mask for file-mode information. The _S_IFDIR bit is set if path specifies a directory; the _S_IFREG bit is set if path specifies an ordinary file or a device. User read/write bits are set according to the file’s permission mode; user execute bits are set according to the filename extension.

st_mtime

Time of last modification of file.

st_nlink

Always 1 on non-NTFS file systems.

st_rdev

Drive number of the disk containing the file (same as st_dev).

st_size

Size of the file in bytes; a 64-bit integer for _stati64 and _wstati64

st_uid

Numeric identifier of user who owns file (UNIX-specific). This field will always be zero on NT systems. A redirected file is classified as an NT file.
*/

bool CDir::ReadDir()
{
  initdata();

  // 如果已读完，清空容器
  if (m_uPOS >= m_vFileName.size()) 
  {
    m_uPOS=0; m_vFileName.clear(); return false;
  }

  int pos=0;

  pos=m_vFileName[m_uPOS].find_last_of("/");

  // 目录名
  memset(m_DirName,0,sizeof(m_DirName));
  strcpy(m_DirName,m_vFileName[m_uPOS].substr(0,pos).c_str());

  // 文件名
  memset(m_FileName,0,sizeof(m_FileName));
  strcpy(m_FileName,m_vFileName[m_uPOS].substr(pos+1,m_vFileName[m_uPOS].size()-pos-1).c_str());

  // 文件全名，包括路径
  snprintf(m_FullFileName,300,"%s",m_vFileName[m_uPOS].c_str());

  struct stat st_filestat;

  stat(m_FullFileName,&st_filestat);

  m_FileSize=st_filestat.st_size;

  struct tm nowtimer;

  if (strcmp(m_DateFMT,"yyyy-mm-dd hh:mi:ss") == 0)
  {
    nowtimer = *localtime(&st_filestat.st_mtime); nowtimer.tm_mon++;
    snprintf(m_ModifyTime,20,"%04u-%02u-%02u %02u:%02u:%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_ctime); nowtimer.tm_mon++;
    snprintf(m_CreateTime,20,"%04u-%02u-%02u %02u:%02u:%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_atime); nowtimer.tm_mon++;
    snprintf(m_AccessTime,20,"%04u-%02u-%02u %02u:%02u:%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);
  }

  if (strcmp(m_DateFMT,"yyyymmddhhmiss") == 0)
  {
    nowtimer = *localtime(&st_filestat.st_mtime); nowtimer.tm_mon++;
    snprintf(m_ModifyTime,20,"%04u%02u%02u%02u%02u%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_ctime); nowtimer.tm_mon++;
    snprintf(m_CreateTime,20,"%04u%02u%02u%02u%02u%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);

    nowtimer = *localtime(&st_filestat.st_atime); nowtimer.tm_mon++;
    snprintf(m_AccessTime,20,"%04u%02u%02u%02u%02u%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);
  }

  m_uPOS++;

  return true;
}

CDir::~CDir()
{
  m_vFileName.clear();

  // m_vDirName.clear();
}

/* 删除字符串左边指定的字符 */
void DeleteLChar(char *in_string,const char in_char)
{
  if (in_string == 0) return;

  if (strlen(in_string) == 0) return;

  char strTemp[strlen(in_string)+1];

  int iTemp=0;

  memset(strTemp,0,sizeof(strTemp));
  strcpy(strTemp,in_string);

  while ( strTemp[iTemp] == in_char )  iTemp++; 
  
  memset(in_string,0,strlen(in_string)+1);

  strcpy(in_string,strTemp+iTemp);

  return;
}

/* 删除字符串右边指定的字符 */
void DeleteRChar(char *in_string,const char in_char)
{
  if (in_string == 0) return;

  int istrlen = strlen(in_string);

  while (istrlen>0)
  {
    if (in_string[istrlen-1] != in_char) break;

    in_string[istrlen-1]=0;

    istrlen--;
  }
}

/* 删除字符串两边指定的字符 */
void DeleteLRChar(char *in_string,const char in_char)
{
  DeleteLChar(in_string,in_char);
  DeleteRChar(in_string,in_char);
}

// 在字符串的左边补字符到指定长度
void LPad(char *in_string,const char in_char,unsigned int in_len)
{
  if (strlen(in_string)>=in_len) return;

  char str[in_len+1];

  memset(str,0,sizeof(str));

  for (unsigned int ii=0;ii<in_len-strlen(in_string);ii++)
  {
    str[ii]=in_char;
  }

  strcpy(str+in_len-strlen(in_string),in_string);

  strcpy(in_string,str);
}

// 在字符串的右边补字符到指定长度
void RPad(char *in_string,const char in_char,unsigned int in_len)
{
  if (strlen(in_string)>=in_len) return;

  for (unsigned int ii=strlen(in_string);ii<in_len;ii++)
  {
    in_string[ii]=in_char;
  }

  in_string[in_len]=0;
}


CLogFile::CLogFile() 
{ 
  m_tracefp = 0;
  memset(m_filename,0,sizeof(m_filename));
  memset(m_openmode,0,sizeof(m_openmode));
  m_bBackup=true;
  m_bEnBuffer=false;
}

CLogFile::~CLogFile() 
{ 
  Close();
}

void CLogFile::Close() 
{ 
  if (m_tracefp != 0) 
  {
    fclose(m_tracefp); m_tracefp=0;
  }
}

/*
  有关文件定位的函数的用法:
  rewind(FILE *fp),使位置指针重返文件的开头,函数无返回值.
  fseek(FILE *fp,位移量,起始点),起始点的取值是:SEEK_SET(0)-文件开始;
  SEEK_CUR(1)-当前位置;SEEK_END(2)-文件未尾,位移量指以起始点为基点,
  移动的字节数(正值往后移,负值往前移),为long数据类型,ANSI C标准规定
  在数字的未尾加上字母L,就表示是long型.
  ftell(FILE *fp),取得文件中的当前位置,返回-1L表示出错.
  ferror(FILE *fp),在调用各种输入输出函数(如putc,getc,fread,fwrite)时,
  如果出现了错误,除了函数返回值有所反应外,还可用ferror函数检查,如果返
  回0表示未出错,非0表示出错,但在调用clearerr(FILE *fp)后,ferror返回0.

*/
// filename日志文件名
// openmode打开文件的方式，操作日志文件的权限,同打开文件函数(FOPEN)使用方法一致
// bBackup，true-备份，false-不备份，在多进程的服务程序中，如果多个进行共用一个日志文件，bBackup必须为FALSE
// bEnBuffer:true-启用缓冲区，false-不启用缓冲区，如果启用缓冲区，那么写进日志文件中的内容不会立即写入文件是
bool CLogFile::Open(const char *in_filename,const char *in_openmode,BOOL bBackup,BOOL bEnBuffer)
{
  if (m_tracefp != 0) { fclose(m_tracefp); m_tracefp=0; }

  m_bEnBuffer=bEnBuffer;

  memset(m_filename,0,sizeof(m_filename));
  strcpy(m_filename,in_filename);

  memset(m_openmode,0,sizeof(m_openmode));
  strcpy(m_openmode,in_openmode);

  if ((m_tracefp=FOPEN(m_filename,m_openmode)) == NULL) return false;

  m_bBackup=bBackup;

  return true;
}

// 如果日志文件大于100M，就备份它
bool CLogFile::BackupLogFile()
{
  // 不备份
  if (m_bBackup == false) return true;

  if (m_tracefp == 0) return true;

  fseek(m_tracefp,0L,2); 

  if (ftell(m_tracefp) > 100*1024*1024) 
  {
    fclose(m_tracefp); m_tracefp=0;

    char strLocalTime[21];
    memset(strLocalTime,0,sizeof(strLocalTime));
    LocalTime(strLocalTime,"yyyymmddhhmiss");

    char bak_filename[301]; 
    memset(bak_filename,0,sizeof(bak_filename));
    snprintf(bak_filename,300,"%s.%s",m_filename,strLocalTime);
    rename(m_filename,bak_filename);

    if ((m_tracefp=FOPEN(m_filename,m_openmode)) == NULL) return false;
  }

  return true;
}

bool CLogFile::Write(const char *fmt,...)
{
  if (BackupLogFile() == false) return FALSE;

  memset(m_stime,0,sizeof(m_stime));
  memset(m_message,0,sizeof(m_message));

  LocalTime(m_stime);

  va_start(m_ap,fmt);
  vsnprintf(m_message,10000,fmt,m_ap);
  va_end(m_ap);

  if (m_tracefp == 0) 
  {
    fprintf(stdout,"%s %s",m_stime,m_message);
    if (m_bEnBuffer==false) fflush(stdout);
  }
  else
  {
    fprintf(m_tracefp,"%s %s",m_stime,m_message);
    if (m_bEnBuffer==false) fflush(m_tracefp);
  }

  return true;  
}

bool CLogFile::WriteEx(const char *fmt,...)
{
  memset(m_stime,0,sizeof(m_stime));
  memset(m_message,0,sizeof(m_message));

  LocalTime(m_stime);

  va_start(m_ap,fmt);
  vsnprintf(m_message,10000,fmt,m_ap);
  va_end(m_ap);

  if (m_tracefp == 0) 
  {
    fprintf(stdout,"%s",m_message);
    if (m_bEnBuffer==false) fflush(stdout);
  }
  else
  {
    fprintf(m_tracefp,"%s",m_message);
    if (m_bEnBuffer==false) fflush(m_tracefp);
  }

  return true;  
}

CCmdStr::CCmdStr()
{
  m_vCmdStr.clear();
}


//拆分字符串到容器
void CCmdStr::SplitToCmd(const string in_string,const char *in_sep,const bool bdeletespace)
{
  // 清除所有的旧数据
  m_vCmdStr.clear();

  int iPOS=0;
  string srcstr,substr;

  srcstr=in_string;

  char str[2048];

  while ( (iPOS=srcstr.find(in_sep)) >= 0)
  {
    substr=srcstr.substr(0,iPOS);

    if (bdeletespace == true)
    {
      memset(str,0,sizeof(str));

      strncpy(str,substr.c_str(),2000);

      DeleteLRChar(str,' ');

      substr=str;
    }

    m_vCmdStr.push_back(substr);

    iPOS=iPOS+strlen(in_sep);

    srcstr=srcstr.substr(iPOS,srcstr.size()-iPOS);
  }

  substr=srcstr;

  if (bdeletespace == true)
  {
    memset(str,0,sizeof(str));

    strncpy(str,substr.c_str(),2000);

    DeleteLRChar(str,' ');

    substr=str;
  }

  m_vCmdStr.push_back(substr);

  return;    
}


//返回m_vCmdStr容器的大小
int CCmdStr::CmdCount()
{
  return m_vCmdStr.size();
}

bool CCmdStr::GetValue(const int inum,char *in_return)
{
  strcpy(in_return,"");

  if (inum >= m_vCmdStr.size()) return false;

  strcpy(in_return,m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum,char *in_return,const int in_len)
{
  strcpy(in_return,"");

  if (inum >= m_vCmdStr.size()) return false;

  if (m_vCmdStr[inum].length() > (unsigned int)in_len)
  {
    strncpy(in_return,m_vCmdStr[inum].c_str(),in_len);
  }
  else
  {
    strcpy(in_return,m_vCmdStr[inum].c_str());
  }

  return true;
}

bool CCmdStr::GetValue(const int inum,int *in_return)
{
  (*in_return) = 0;

  if (inum >= m_vCmdStr.size()) return false;

  (*in_return) = atoi(m_vCmdStr[inum].c_str()); 

  return true;
}

bool CCmdStr::GetValue(const int inum,long *in_return)
{
  (*in_return) = 0;

  if (inum >= m_vCmdStr.size()) return false;

  (*in_return) = atol(m_vCmdStr[inum].c_str()); 

  return true;
}


bool CCmdStr::GetValue(const int inum,double *in_return)
{
  (*in_return) = 0;

  if (inum >= m_vCmdStr.size()) return false;

  (*in_return) = (double)atof(m_vCmdStr[inum].c_str()); 

  return true;
}

CCmdStr::~CCmdStr()
{
  m_vCmdStr.clear();
}



void ToUpper(char *str)
{
  if (str == 0) return;

  if (strlen(str) == 0) return;

  int istrlen=strlen(str);

  for (int ii=0;ii<istrlen;ii++) 
  {
    if ( (str[ii] >= 97) && (str[ii] <= 122) ) str[ii]=str[ii] - 32;
  }
}

void ToUpper(string &str)
{
  if (str.empty()) return;

  char strtemp[str.size()+1];

  memset(strtemp,0,sizeof(strtemp));
  strcpy(strtemp,str.c_str());

  ToUpper(strtemp);

  str=strtemp;

  return;
}

void ToLower(char *str)
{
  if (str == 0) return;

  if (strlen(str) == 0) return;

  int istrlen=strlen(str);

  for (int ii=0;ii<istrlen;ii++) 
  {
    if ( (str[ii] >= 65) && (str[ii] <= 90) ) str[ii]=str[ii] + 32;
  }
}

void ToLower(string &str)
{
  if (str.empty()) return;

  char strtemp[str.size()+1];

  memset(strtemp,0,sizeof(strtemp));
  strcpy(strtemp,str.c_str());

  ToLower(strtemp);

  str=strtemp;

  return;
}

// 删除字符串中间的字符串
void DeleteMStr(char *in_string,const char *in_str)
{
  if (in_string == 0) return;

  if (strlen(in_string) == 0) return;
  
  char strTemp[strlen(in_string)+1];  
  char *strPos=0;

  while (true)
  {
    strPos=strstr(in_string,in_str);
    if (strPos != 0)
    {
      memset(strTemp,0,sizeof(strTemp));
      strncpy(strTemp,in_string,strPos-in_string);
      strcat(strTemp,strPos+strlen(in_str));
      strcpy(in_string,strTemp);
    }
    else
    {
      break;
    }
  }
}

// 把字符串中的某字符串用另一个字符串代替
void UpdateStr(char *in_string,const char *in_str1,const char *in_str2,bool bLoop)
{
  if (in_string == 0) return;

  if (strlen(in_string) == 0) return;

  char strTemp[2048];  

  char *strStart=in_string;

  char *strPos=0;

  while (true)
  {
    if (strlen(in_string) >2000) break;

    if (bLoop == true)
    {
      strPos=strstr(in_string,in_str1);
    }
    else
    {
      strPos=strstr(strStart,in_str1);
    }

    if (strPos == 0) break;

    memset(strTemp,0,sizeof(strTemp));
    strncpy(strTemp,in_string,strPos-in_string);
    strcat(strTemp,in_str2);
    strcat(strTemp,strPos+strlen(in_str1));
    strcpy(in_string,strTemp);

    strStart=strPos+strlen(in_str2);
  }
}

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
void LocalTime(char *out_stime,const char *in_fmt,const int in_interval)
{

  if(in_fmt==0)  memset(out_stime,0,20);
  else
  memset(out_stime,0,strlen(in_fmt)+1);

  time_t  timer;
  struct tm nowtimer;

  time( &timer ); timer=timer+in_interval;
  nowtimer = *localtime ( &timer ); nowtimer.tm_mon++;

  if (in_fmt==0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh:mi:ss") == 0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh:mi") == 0)
  {
    snprintf(out_stime,17,"%04u-%02u-%02u %02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh") == 0)
  {
    snprintf(out_stime,14,"%04u-%02u-%02u %02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd") == 0)
  {
    snprintf(out_stime,11,"%04u-%02u-%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return;
  }

  if (strcmp(in_fmt,"yyyy-mm") == 0)
  {
    snprintf(out_stime,8,"%04u-%02u",nowtimer.tm_year+1900,nowtimer.tm_mon); return;
  }

  if (strcmp(in_fmt,"hh:mi:ss") == 0)
  {
    snprintf(out_stime,9,"%02u:%02u:%02u",nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec); return;
  }

  if (strcmp(in_fmt,"hh:mi") == 0)
  {
    snprintf(out_stime,5,"%02u:%02u",nowtimer.tm_hour,nowtimer.tm_min); return;
  }

  if (strcmp(in_fmt,"yyyymmddhhmiss") == 0)
  {
    snprintf(out_stime,15,"%04u%02u%02u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec);
    return;
  }

  if (strcmp(in_fmt,"yyyymmddhhmi") == 0)
  {
    snprintf(out_stime,13,"%04u%02u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min);
    return;
  }

  if (strcmp(in_fmt,"yyyymmddhh") == 0)
  {
    snprintf(out_stime,11,"%04u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour);
    return;
  }

  if (strcmp(in_fmt,"yyyymmdd") == 0)
  {
    snprintf(out_stime,9,"%04u%02u%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return;
  }

  if (strcmp(in_fmt,"hhmiss") == 0)
  {
    snprintf(out_stime,7,"%02u%02u%02u",nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec); return;
  }

  if (strcmp(in_fmt,"hhmi") == 0)
  {
    snprintf(out_stime,5,"%02u%02u",nowtimer.tm_hour,nowtimer.tm_min); return;
  }

  if (strcmp(in_fmt,"hh") == 0)
  {
    snprintf(out_stime,3,"%02u",nowtimer.tm_hour); return;
  }

  if (strcmp(in_fmt,"mi") == 0)
  {
    snprintf(out_stime,3,"%02u",nowtimer.tm_min); return;
  }
}


// 把字符串格式的时间转换为time_t
// stime为输入的时间，格式不限，但一定要包括yyyymmddhhmiss
time_t UTCTime(const char *stime)
{
  char strtime[21],yyyy[5],mm[3],dd[3],hh[3],mi[3],ss[3];
  memset(strtime,0,sizeof(strtime));
  memset(yyyy,0,sizeof(yyyy));
  memset(mm,0,sizeof(mm));
  memset(dd,0,sizeof(dd));
  memset(hh,0,sizeof(hh));
  memset(mi,0,sizeof(mi));
  memset(ss,0,sizeof(ss));

  PickNumber(stime,strtime,false,FALSE);

  if (strlen(strtime) != 14) return -1;

  strncpy(yyyy,strtime,4);
  strncpy(mm,strtime+4,2);
  strncpy(dd,strtime+6,2);
  strncpy(hh,strtime+8,2);
  strncpy(mi,strtime+10,2);
  strncpy(ss,strtime+12,2);

  struct tm time_str;

  time_str.tm_year = atoi(yyyy) - 1900;
  time_str.tm_mon = atoi(mm) - 1;
  time_str.tm_mday = atoi(dd);
  time_str.tm_hour = atoi(hh);
  time_str.tm_min = atoi(mi);
  time_str.tm_sec = atoi(ss);
  time_str.tm_isdst = 0;

  return mktime(&time_str);
}

CTimer::CTimer()
{
  memset(&m_start,0,sizeof(struct timeval));
  memset(&m_end,0,sizeof(struct timeval));

  // 开始计时
  Start();
}

// 开始计时
void CTimer::Start()
{
  gettimeofday( &m_start, NULL );
}

// 计算已逝去的时间，单位：秒，小数点后面是微秒
double CTimer::Elapsed()
{

  gettimeofday( &m_end, NULL );

  double dstart,dend;

  dstart=dend=0;

  char strtemp[51];
  memset(strtemp,0,sizeof(strtemp));
  snprintf(strtemp,30,"%ld.%ld",m_start.tv_sec,m_start.tv_usec);
  dstart=atof(strtemp);

  memset(strtemp,0,sizeof(strtemp));
  snprintf(strtemp,30,"%ld.%ld",m_end.tv_sec,m_end.tv_usec);
  dend=atof(strtemp);

  // 重新开始计时
  Start();

  return dend-dstart;
}

// 判断文本文件是否以strEnd结束
bool CheckFileEnd(const char *strFileName,const char *strEnd)
{
  FILE *fp=0;

  if ( (fp=fopen(strFileName,"rt")) == NULL ) return false;

  char strBuf[301];
  memset(strBuf,0,sizeof(strBuf));

  fseek(fp,-30L,2);
  fread(strBuf,1,30,fp);
  fclose(fp);

  fp=0;

  if ( strstr(strBuf,strEnd) == NULL) return false;

  return true;
}

// 从文件文件中读取一行
// strEndStr是一行数据的结束标志，如果为空，则以换行符"\n"为结束标志。
bool FGETS(const FILE *fp,char *strBuffer,const int ReadSize,const char *strEndStr)
{
  char strLine[ReadSize+1];

  memset(strLine,0,sizeof(strLine));

  while (true)
  {
    memset(strLine,0,ReadSize+1);

    if (fgets(strLine,ReadSize,(FILE *)fp) == 0) break;

    for (int ii=0;ii<3;ii++)
    {
      DeleteRChar(strLine,'\n'); 
      DeleteRChar(strLine,'\r'); 
    }

    // 防止strBuffer溢出
    if ( (strlen(strBuffer)+strlen(strLine)) >= (UINT)ReadSize ) break;

    if (strEndStr != 0) 
    {
      if (strlen(strBuffer) > 0) strcat(strBuffer,"\n");
    }

    strcat(strBuffer,strLine);

    if (strEndStr == 0) return true; 

    if (strncmp(strLine+strlen(strLine)-strlen(strEndStr),strEndStr,strlen(strEndStr)) == 0) 
    {
      strBuffer[strlen(strBuffer)-strlen(strEndStr)]=0; return true;
    }
  }

  return false;
}

// 判断字符串中的负号和圆点是否合法
bool JudgeSignDOT(const char *strSrc,const char *strBZ)
{
  char *pos=0;
  pos=(char *)strstr(strSrc,strBZ);

  // 如果没有包括待搜索的字符串，就返回true
  if (pos == 0) return true;

  // 如果strlen(pos)==1，表示结果中只有符号，没有其它字符，返回false
  if (strlen(pos)==1) return false;

  // 如果待搜索的字符串是+号，就一定要是第一个字符
  if ( (strcmp(strBZ,"+") == 0) && (strncmp(strSrc,"+",1) != 0) ) return false;

  // 如果待搜索的字符串是-号，就一定要是第一个字符
  if ( (strcmp(strBZ,"-") == 0) && (strncmp(strSrc,"-",1) != 0) ) return false;

  // 如果包括多个待搜索的字符串，就返回false
  if (strstr(pos+1,strBZ) > 0) return false;

  return true;
}

// 从一个字符串中提取数字，bWithSign==true表示包括负号，bWithDOT==TRUE表示包括圆点
void PickNumber(const char *strSrc,char *strDst,const bool bWithSign,const BOOL bWithDOT)
{
  char strtemp[1024];
  memset(strtemp,0,sizeof(strtemp));
  strncpy(strtemp,strSrc,1000);
  DeleteLRChar(strtemp,' ');

  // 为了防止strSrc和strDst为同一变量的情况，所以strDst不能初始化

  // 判断字符串中的负号是否合法
  if ( (bWithSign==true) && (JudgeSignDOT(strtemp,"-") == false) )
  {
    strcpy(strDst,""); return;
  }

  // 判断字符串中的正号是否合法
  if ( (bWithSign==true) && (JudgeSignDOT(strtemp,"+") == false) )
  {
    strcpy(strDst,""); return;
  }

  // 判断字符串中的圆点是否合法
  if ( (bWithDOT==true) && (JudgeSignDOT(strtemp,".") == false) )
  {
    strcpy(strDst,""); return;
  }

  UINT iPosSrc,iPosDst,iLen;
  iPosSrc=iPosDst=iLen=0;

  iLen=strlen(strtemp);

  for (iPosSrc=0;iPosSrc<iLen;iPosSrc++)
  {
    if ( (bWithSign==true) && (strtemp[iPosSrc] == '+') )
    {
      strDst[iPosDst++]=strtemp[iPosSrc]; continue;
    }

    if ( (bWithSign==true) && (strtemp[iPosSrc] == '-') )
    {
      strDst[iPosDst++]=strtemp[iPosSrc]; continue;
    }

    if ( (bWithDOT==true) && (strtemp[iPosSrc] == '.') )
    {
      strDst[iPosDst++]=strtemp[iPosSrc]; continue;
    }
      
    if (isdigit(strtemp[iPosSrc])) strDst[iPosDst++]=strtemp[iPosSrc];
  }

  strDst[iPosDst]=0;

  return;
}


// 获取文件的大小，返回字节数
int FileSize(const char *in_FullFileName)
{
  struct stat st_filestat;

  if (stat(in_FullFileName,&st_filestat) < 0) return -1;

  return st_filestat.st_size;
}

// 获取文件的时间，即modtime
void FileMTime(const char *in_FullFileName,char *out_ModTime)
{
  struct tm nowtimer;
  struct stat st_filestat;

  stat(in_FullFileName,&st_filestat);

  nowtimer = *localtime(&st_filestat.st_mtime); 
  nowtimer.tm_mon++;

  snprintf(out_ModTime,15,"%04u%02u%02u%02u%02u%02u",\
             nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday,\
             nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec);
}

// 判断文件名是否和MatchFileName匹配，如果不匹配，返回失败
bool MatchFileName(const string in_FileName,const string in_MatchStr) 
{ 
  // 如果用于比较的字符是空的，返回false
  if (in_MatchStr.size() == 0) return false;

  // 如果被比较的字符串是“*”，返回true
  if (in_MatchStr == "*") return true;

  // 处理文件名匹配规则中的时间匹配dd-nn.mm
  char strTemp[2049];
  memset(strTemp,0,sizeof(strTemp));
  strncpy(strTemp,in_MatchStr.c_str(),2000);

  UINT ii,jj;
  INT  iPOS1,iPOS2;
  CCmdStr CmdStr,CmdSubStr;

  string strFileName,strMatchStr;

  strFileName=in_FileName;
  strMatchStr=strTemp;

  // 把字符串都转换成大写后再来比较
  ToUpper(strFileName);
  ToUpper(strMatchStr);

  CmdStr.SplitToCmd(strMatchStr,",");

  for (ii=0;ii<CmdStr.CmdCount();ii++)
  {
    // 如果为空，就一定要跳过，否则就会被配上
    if (CmdStr.m_vCmdStr[ii].empty() == true) continue;

    iPOS1=iPOS2=0;
    CmdSubStr.SplitToCmd(CmdStr.m_vCmdStr[ii],"*");

    for (jj=0;jj<CmdSubStr.CmdCount();jj++)
    {
      // 如果是文件名的首部
      if (jj == 0)
      {
        if (strncmp(strFileName.c_str(),CmdSubStr.m_vCmdStr[jj].c_str(),CmdSubStr.m_vCmdStr[jj].size()) != 0) break;
      }

      // 如果是文件名的尾部
      if (jj == CmdSubStr.CmdCount()-1)
      {
        if (strcmp(strFileName.c_str()+strFileName.size()-CmdSubStr.m_vCmdStr[jj].size(),CmdSubStr.m_vCmdStr[jj].c_str()) != 0) break;
      }

      iPOS2=strFileName.find(CmdSubStr.m_vCmdStr[jj],iPOS1);

      if (iPOS2 < 0) break;

      iPOS1=iPOS2+CmdSubStr.m_vCmdStr[jj].size();
    }

    if (jj==CmdSubStr.CmdCount()) return true;
  }

  return false;
}

// 判断该行的内容是否全部是数字
bool IsDigit(const char *strBuffer)
{
  int len=strlen(strBuffer);

  for (int i=0; i<len;i++)
  {
    if (isdigit(strBuffer[i]) == 0) return false;
  }

  return true;
}

// 判断内容是否全部是大写字母
bool IsUpper(const char *strBuffer)
{
  int len=strlen(strBuffer);

  for (int i=0; i<len;i++)
  {
    if (isupper(strBuffer[i]) == 0) return false;
  }

  return true;
}

// 判断内容是否全部是ASCII字符
bool IsASCII(const char *strBuffer)
{
  int len=strlen(strBuffer);

  for (int i=0; i<len;i++)
  {
    if (isascii(strBuffer[i]) == 0) return false;
  }

  return true;
}


// 判断该行的内容是否全部是数字和空格
bool IsDigitOrSpace(const char *strLine)
{
  int len=strlen(strLine);

  for (int i=0; i<len;i++)
  {
    if ( (strLine[i] != ' ') && (isdigit(strLine[i]) == 0) ) return false;
  }

  return true;
}


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
int AddTime(const char *in_stime,char *out_stime,const int in_interval,const char *in_fmt)
{
  time_t  timer;
  struct tm nowtimer;

  timer=UTCTime(in_stime)+in_interval;

  nowtimer = *localtime ( &timer ); nowtimer.tm_mon++;

  // 为了防止in_stime和out_stime为同一变量的情况，所以out_stime在此处初始化，代码不可提前
  out_stime[0]=0;

  if (in_fmt==0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh:mi:ss") == 0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"yyyymmddhhmiss") == 0)
  {
    snprintf(out_stime,15,"%04u%02u%02u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd") == 0)
  {
    snprintf(out_stime,11,"%04u-%02u-%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return 0;
  }
  if (strcmp(in_fmt,"yyyymmdd") == 0)
  {
    snprintf(out_stime,9,"%04u%02u%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return 0;
  }

  if (strcmp(in_fmt,"hh:mi:ss") == 0)
  {
    snprintf(out_stime,9,"%02u:%02u:%02u",nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"hh:mi") == 0)
  {
    snprintf(out_stime,9,"%02u:%02u",nowtimer.tm_hour,nowtimer.tm_min); return 0;
  }

  if (strcmp(in_fmt,"hhmi") == 0)
  {
    snprintf(out_stime,7,"%02u%02u",nowtimer.tm_hour,nowtimer.tm_min); return 0;
  }

  return -1;
}

CTcpClient::CTcpClient()
{
  m_sockfd=-1;
  memset(m_IP,0,sizeof(m_IP));
  m_Port=0;
  m_State=false;
  memset(m_ConnStr,0,sizeof(m_ConnStr));
  m_bTimeOut=false;
}

bool CTcpClient::ConnectToServer(const char *in_ConnStr)
{
  m_State=false;

  if (m_sockfd != -1)
  {
    close(m_sockfd); m_sockfd = -1;
  }

  memset(m_ConnStr,0,sizeof(m_ConnStr));
  strcpy(m_ConnStr,in_ConnStr);

  struct hostent* h;
  struct sockaddr_in servaddr;

  CCmdStr CmdStr;
  CmdStr.SplitToCmd(m_ConnStr,",");

  UINT ii;

  for (ii=0; ii<CmdStr.CmdCount(); ii=ii+2)
  {
    CmdStr.GetValue(ii,m_IP);
    CmdStr.GetValue(ii+1,&m_Port);

    if ( (m_sockfd = socket(AF_INET,SOCK_STREAM,0) ) < 0)
      return false;

    if ( !(h = gethostbyname(m_IP)) )
    {
      close(m_sockfd);  m_sockfd = -1; return false;
    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_Port);  // 指定服务端的通讯端口
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);

    if (connect(m_sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) == 0)
    {
      m_State=true; return TRUE;
    }
    else
    {
      close(m_sockfd);  m_sockfd = -1;
    }
  }

  return false;
}

bool CTcpClient::Read(char *strRecvBuffer)
{
  if (m_sockfd == -1) return false;

  m_BufLen = 0;
  return(TcpRead(m_sockfd,strRecvBuffer,&m_BufLen));
}

bool CTcpClient::Read(char *strRecvBuffer,const int iTimeOut)
{
  if (m_sockfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_sockfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = iTimeOut; timeout.tv_usec = 0;

  m_bTimeOut = false;

  int i;
  if ( (i = select(m_sockfd+1,&tmpfd,NULL,NULL,&timeout)) <= 0 )
  {
    if (i==0) m_bTimeOut = true;
    return false;
  }

  m_BufLen = 0;
  return(TcpRead(m_sockfd,strRecvBuffer,&m_BufLen));
}

bool CTcpClient::Write(char *strSendBuffer)
{
  if (m_sockfd == -1) return false;

  int buflen = strlen(strSendBuffer);

  return(Write(strSendBuffer,buflen));
}

bool CTcpClient::Write(char *strSendBuffer,const int buflen)
{
  if (m_sockfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_sockfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5; timeout.tv_usec = 0;
  
  m_bTimeOut = false;

  int i;
  if ( (i=select(m_sockfd+1,NULL,&tmpfd,NULL,&timeout)) <= 0 )
  {
    if (i==0) m_bTimeOut = true;
    return false;
  }

  return(TcpWrite(m_sockfd,strSendBuffer,buflen));
}

void CTcpClient::Close()
{
  if (m_sockfd > 0)
  {
    close(m_sockfd); m_sockfd=-1;
  }

  m_State=false;
}

CTcpClient::~CTcpClient()
{
  Close();
}

CTcpServer::CTcpServer()
{
  m_listenfd=-1;
  m_connfd=-1;
  m_socklen=0;
  m_bTimeOut=false;
}

bool CTcpServer::InitServer(const unsigned int port)
{
  m_listenfd = socket(AF_INET,SOCK_STREAM,0);

  // WINDOWS平台如下
  //char b_opt='1';
  //setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&b_opt,sizeof(b_opt));
  //setsockopt(m_listenfd,SOL_SOCKET,SO_KEEPALIVE,&b_opt,sizeof(b_opt));

  // Linux如下
  int opt = 1; unsigned int len = sizeof(opt);
  setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,len);
  setsockopt(m_listenfd,SOL_SOCKET,SO_KEEPALIVE,&opt,len);

  memset(&m_servaddr,0,sizeof(m_servaddr));
  m_servaddr.sin_family = AF_INET;
  m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  m_servaddr.sin_port = htons(port);
  if (bind(m_listenfd,(struct sockaddr *)&m_servaddr,sizeof(m_servaddr)) != 0 )
  {
    CloseListen(); return false;
  }

  if (listen(m_listenfd,5) != 0 )
  {
    CloseListen(); return false;
  }

  m_socklen = sizeof(struct sockaddr_in);

  return true;
}

bool CTcpServer::Accept()
{
  if (m_listenfd == -1) return false;

  if ((m_connfd=accept(m_listenfd,(struct sockaddr *)&m_clientaddr,(socklen_t*)&m_socklen)) < 0)
      return false;

  return true;
}

char *CTcpServer::GetIP()
{
  return(inet_ntoa(m_clientaddr.sin_addr));
}

bool CTcpServer::Read(char *strRecvBuffer)
{
  if (m_connfd == -1) return false;

  m_BufLen = 0;
  return(TcpRead(m_connfd,strRecvBuffer,&m_BufLen));
}

bool CTcpServer::Read(char *strRecvBuffer,const int iTimeOut)
{
  if (m_connfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_connfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = iTimeOut; timeout.tv_usec = 0;

  m_bTimeOut = false;

  int i;
  if ( (i = select(m_connfd+1,&tmpfd,NULL,NULL,&timeout)) <= 0 )
  {
    if (i==0) m_bTimeOut = true;
    return false;
  }

  m_BufLen = 0;
  return(TcpRead(m_connfd,strRecvBuffer,&m_BufLen));
}

bool CTcpServer::Write(char *strSendBuffer)
{
  if (m_connfd == -1) return false;

  int buflen = strlen(strSendBuffer);

  return(Write(strSendBuffer,buflen));
}

bool CTcpServer::Write(char *strSendBuffer,const int buflen)
{
  if (m_connfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_connfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5; timeout.tv_usec = 0;
  
  m_bTimeOut = false;

  int i;
  if ( (i=select(m_connfd+1,NULL,&tmpfd,NULL,&timeout)) <= 0 )
  {
    if (i==0) m_bTimeOut = true;
    return false;
  }

  return(TcpWrite(m_connfd,strSendBuffer,buflen));
}

void CTcpServer::CloseListen()
{
  if (m_listenfd > 0)
  {
    close(m_listenfd); m_listenfd=-1;
  }
}

void CTcpServer::CloseClient()
{
  if (m_connfd > 0)
  {
    close(m_connfd); m_connfd=-1; 
  }
}

CTcpServer::~CTcpServer()
{
  CloseListen(); CloseClient();
}

bool TcpRead(const int fd,char *strRecvBuffer,int *buflen,const int iTimeOut)
{
  if (fd == -1) return false;

  if (iTimeOut > 0)
  {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(fd,&tmpfd);

    struct timeval timeout;
    timeout.tv_sec = iTimeOut; timeout.tv_usec = 0;

    int i;
    if ( (i = select(fd+1,&tmpfd,NULL,NULL,&timeout)) <= 0 ) return false;
  }

  (*buflen) = 0;

  char strBufLen[11]; memset(strBufLen,0,sizeof(strBufLen));

  if (Readn(fd,(char*)strBufLen,TCPHEADLEN) == false) return FALSE;

  (*buflen) = atoi(strBufLen);

  if ( (*buflen) > TCPBUFLEN ) return false;

  if (Readn(fd,strRecvBuffer,(*buflen)) == false) return FALSE;

  return true;
}

bool TcpWrite(const int fd,const char *strSendBuffer,const int buflen,const int iTimeOut)
{
  if (fd == -1) return false;

  if (iTimeOut > 0)
  {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(fd,&tmpfd);

    struct timeval timeout;
    timeout.tv_sec = 5; timeout.tv_usec = 0;

    if ( select(fd+1,NULL,&tmpfd,NULL,&timeout) <= 0 ) return false;
  }
  
  int ibuflen=0;

  // 如果长度为0，就采用字符串的长度
  if (buflen==0) ibuflen=strlen(strSendBuffer);
  else ibuflen=buflen;

  if (ibuflen>TCPBUFLEN) return false;

  char strBufLen[11]; 
  memset(strBufLen,0,sizeof(strBufLen));
  sprintf(strBufLen,"%d",ibuflen);

  char strTBuffer[TCPHEADLEN+ibuflen];
  memset(strTBuffer,0,sizeof(strTBuffer));
  memcpy(strTBuffer,strBufLen,TCPHEADLEN);
  memcpy(strTBuffer+TCPHEADLEN,strSendBuffer,ibuflen);

  if (Writen(fd,strTBuffer,TCPHEADLEN+ibuflen) == false) return FALSE;

  return true;
}

bool Readn(const int fd,char *vptr,const size_t n)
{
  int nLeft,nread,idx;

  nLeft = n;
  idx = 0;

  while(nLeft > 0)
  {
    if ( (nread = recv(fd,vptr + idx,nLeft,0)) <= 0) return false;

    idx += nread;
    nLeft -= nread;
  }

  return true;
}

bool Writen(const int fd,const char *vptr,const size_t n)
{
  int nLeft,idx,nwritten;
  nLeft = n;  
  idx = 0;
  while(nLeft > 0 )
  {    
    if ( (nwritten = send(fd, vptr + idx,nLeft,0)) <= 0) return false;      
    
    nLeft -= nwritten;
    idx += nwritten;
  }

  return true;
}

CFile::CFile()
{
  initdata();
}

CFile::~CFile()
{
  CloseOnly();
}

void CFile::initdata()
{
  memset(m_filename, 0, sizeof(m_filename) );
  memset(m_filenametmp, 0, sizeof(m_filenametmp) );

  m_fp = 0;

  m_bEnBuffer = false;

  m_filetype=0; 
}

// 打开一个文件用于读，filename为全路径的文件名。
// openmode为打开文件的方式，只能用"rt"或"rb"，返回true说明打开成功，false说明打开文件失败
bool CFile::OpenForRead(const char *filename,const char *openmode)
{
  // 先关闭已打开文件,避免重复打开文件
  CloseOnly();

  if ( (strcmp(openmode,"r")!=0) && (strcmp(openmode,"rt")!=0) && (strcmp(openmode,"rb")!=0) ) return false;

  if ( (m_fp=FOPEN(filename,openmode)) == NULL ) return false;

  strncpy(m_filename,filename,300);

  m_filetype=1;

  return true;
}

// 读取文件的每行，并把每一行的回车换行符去掉，还可以判断每行的内容是否以strEndStr结束，
// 是否在每行的最后补充strPadStr字符
bool CFile::FFGETS(char *strBuffer,const int ReadSize,const char *strEndStr)
{
  if ( m_filetype != 1 ) return false;

  return FGETS(m_fp,strBuffer,ReadSize+1,strEndStr);
}

// 调用fread从文件中读取数据。
size_t CFile::Fread(void *ptr, size_t size)
{
  if ( m_filetype != 1 ) return 0;

  return fread(ptr,1,size,m_fp);
}


// 打开一个文件用于写，filename为不包括路径的文件名
// openmode为打开文件的方式，同FOPEN使用方法一致，不包括"rt"和"rb"
// bEnBuffer:true-启用缓冲区，false-不启用缓冲区
bool CFile::OpenForWrite(const char *filename, const char *openmode, BOOL bEnBuffer)
{
  // 先关闭已打开文件,避免重复打开文件
  CloseOnly();

  if ( (strcmp(openmode,"r")==0) || (strcmp(openmode,"rt")==0) || (strcmp(openmode,"rb")==0) ) return false;

  if ( (m_fp = FOPEN(filename, openmode )) == NULL ) return false;

  strncpy( m_filename, filename, 300);

  m_bEnBuffer = bEnBuffer;

  m_filetype=2;

  return true;
}

bool CFile::Open(const char *filename, const char *openmode, BOOL bEnBuffer)
{
  // 先关闭已打开文件,避免重复打开文件
  CloseOnly();

   if ( (strcmp(openmode,"r")==0) || (strcmp(openmode,"rt")==0) || (strcmp(openmode,"rb")==0) ) return false;
  if ( (m_fp = FOPEN(filename, openmode )) == NULL ) return false;

  strncpy( m_filename, filename, 300);
  memset(m_filename,0,sizeof(m_filename));

  strncpy(m_filename,filename,300);
  m_bEnBuffer = bEnBuffer;

  return true;
}


bool CFile::OpenForRename(const char *filename,const char *openmode,BOOL bEnBuffer)
{
  // 先关闭已打开文件,避免重复打开文件
  CloseOnly();

  if ( (strcmp(openmode,"r")==0) || (strcmp(openmode,"rt")==0) || (strcmp(openmode,"rb")==0) ) return false;

  strncpy(m_filename,filename,300);
  snprintf(m_filenametmp,300,"%s.tmp",filename);

  if ( (m_fp = FOPEN(m_filenametmp, openmode )) == NULL ) return false;

  m_bEnBuffer = bEnBuffer;

  m_filetype=3;

  return true;
}

void CFile::Fprintf(const char *fmt, ... )
{
  if ( m_filetype==1 ) return;

  va_list arg;

  if ( m_fp == 0 ) return;

  va_start( arg, fmt );
  vfprintf( m_fp, fmt, arg );
  va_end( arg );

  if ( m_bEnBuffer == false ) fflush(m_fp);
}

// 如果m_filetype==1或m_filetype==2，就关闭它。
// 如果m_filetype==3，关闭文件指针，并删除掉临时文件。
void CFile::CloseOnly()
{
  if (m_fp == 0) return;

  fclose( m_fp );

  m_fp=0;

  if (m_filetype==3)
  {
    // 删除掉临时文件
    remove(m_filenametmp); 
  }

  initdata();
}

bool CFile::IsOpened()
{
  if (m_fp == NULL) return false;

  return true;
}

// 关闭文件指针，并把临时文件名改为正式文件名，如果改名失败，将返回false，成功返回true
bool CFile::CloseAndRename()
{
  if (m_filetype!=3) return false;

  if (m_fp==0) return false;

  fclose( m_fp );

  m_fp=0;

  if (rename(m_filenametmp,m_filename) != 0) return false;

  return true;
}


// 调用fwrite向文件中写数据
size_t CFile::Fwrite(const void *ptr, size_t size )
{
  if ( m_filetype == 1 ) return 0;

  size_t tt=fwrite(ptr,1,size,m_fp);

  if ( m_bEnBuffer == false ) fflush(m_fp); 

  return tt;
}

// 关于已打开的只读文件，并删除它。
bool CFile::CloseAndRemove()
{
  if (m_filetype!=1) return false;
  
  if (m_fp==0) return false;

  // 先关闭文件
  if ( m_fp != 0 ) fclose( m_fp );

  m_fp=0;

  if (remove(m_filename) != 0) return false; 

  return true;
}

// 调用fopen函数打开文件，如果文件名中包含的目录不存在，就创建目录
FILE *FOPEN(const char *filename,const char *mode)
{
  if (MKDIR(filename) == false) return NULL;

  return fopen(filename,mode);
}

// 用某文件或目录的全路径中的目录创建目录，以级该目录下的各级子目录
bool MKDIR(const char *filename,BOOL bisfilename)
{
  // 检查目录是否存在，如果不存在，逐级创建子目录
  char strPathName[301];

  for (UINT ii=1; ii<strlen(filename);ii++)
  {
    if (filename[ii] != '/') continue;

    memset(strPathName,0,sizeof(strPathName));
    strncpy(strPathName,filename,ii);

    if (access(strPathName,F_OK) == 0) continue;

    if (mkdir(strPathName,00755) != 0) return false;
  }

  if (bisfilename==false)
  {
    if (access(filename,F_OK) != 0) 
    {
      if (mkdir(filename,00755) != 0) return false;
    }
  }

  return true;
}

// 更改文件的修改时间属性
int UTime(const char *filename,const char *mtime)
{
  struct utimbuf stutimbuf;
  
  stutimbuf.actime=stutimbuf.modtime=UTCTime(mtime);

  return utime(filename,&stutimbuf);
}

// 关闭全部的信号和输入输出
void CloseIOAndSignal()
{
  int ii=0;

  for (ii=0;ii<50;ii++)
  {
    signal(ii,SIG_IGN); close(ii);
  }
}


//destlen表示dest的有效长度，一般为：占用字节(sizeof)-1 不包含'\0'
char *STRCPY(char* dest,size_t destlen,const char* src)
{
  memset(dest,0,destlen+1);    //dest需要一个位置放'\0'
    
    if(strlen(src)>destlen)    //>=也可以
      strncpy(dest,src,destlen);
    else
      strcpy(dest,src);
                  
   return dest;
} 

//赋值或赋值字符串前n个字符

//destlen表示dest的有效长度 一般为：占用字节(sizeof-1)  不包含'\0'

char *STRNCPY(char* dest,size_t destlen,const char *src,size_t n)
{
  memset(dest,0,destlen+1);
                             
  if(n>destlen)
    strncpy(dest,src,destlen);
  else
    strncpy(dest,src,n);
                                            
   return dest;
} 
//
//字符串拼接
//destlen表示dest的有效长度 一般为：占用字节(sizeof)-1,不包含'\0'
char *STRCAT(char* dest,size_t destlen,const char *src)
{
  memset(dest+strlen(dest),0,destlen-strlen(dest)+1);

  int left = destlen - strlen(dest);  //还可容纳的字符数

  if(strlen(src)>left)
    strncpy(dest+strlen(dest),src,left);
  else
    strcpy(dest+strlen(dest),src);

  return dest;
}


//字符串拼接前n个字符
//
char *STRNCAT(char* dest,size_t destlen,const char *src,size_t n)
{
  memset(dest+strlen(dest),0,destlen-strlen(dest)+1);
  
  int left=destlen-strlen(dest);  //还可容纳的字符数
  
  if(n>left)
    strncpy(dest+strlen(dest),src,left);
  else
    strncpy(dest+strlen(dest),src,n);

  return dest;
}
