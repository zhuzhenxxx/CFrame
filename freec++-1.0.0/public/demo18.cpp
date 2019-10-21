//
// 本程序演示Cftp客户端类，采用ftp协议从服务器上获取文件
//

#include "_ftp.h"

int main(int argc,char *argv[])
{
  Cftp ftp;

  // 登录远程FTP服务器
  if (ftp.login("193.112.167.234:21","freec","qWE12#$(*0776",FTPLIB_PASSIVE) == FALSE)
  {
    printf("ftp.login() failed.\n"); return -1;
  }

  // 改变本地目录
  chdir("/tmp");

  // 进入ftp服务器上文件存放的目录
  if (ftp.chdir("/home/freec/freec++-1.0.0/public") == FALSE)
  {
    printf("ftp.chdir() failed.\n"); return -1;
  }

  // 获取对方目录文件的列表，存放在"/tmp/ftp.list"文件中
  if (ftp.nlist("*.cpp","/tmp/ftp.list")== FALSE)
  {
    printf("ftp.nlist() failed.\n"); return -1;
  }

  CFile File;

  File.OpenForRead("/tmp/ftp.list","rt");

  char strFileName[101];

  // 逐行读取文件的内容，并把文件get到本地
  while (TRUE)
  {
    // 从文件中读取一行
    memset(strFileName,0,sizeof(strFileName));
    if (File.FFGETS(strFileName,100) == FALSE) break;

    printf("get %s ...",strFileName);

    // 从远程取文件
    if (ftp.get(strFileName,strFileName)==FALSE)
    {
      printf("ftp.get(%s) failed.\n",strFileName); break;
    }
    
    printf("ok.\n");
  }

  File.CloseAndRemove();

  ftp.logout();

  return 0;
}

