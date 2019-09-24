
#ifndef __FTP_H
#define __FTP_H

#include "_public.h"
#include "ftplib.h"

class Cftp
{
public:
  // ftp连接句柄
  netbuf *m_ftpconn;

  // 文件的大小
  unsigned int m_size;

  // 文件的时间modifytime
  char m_mtime[21];

  Cftp();
 ~Cftp();

  // 存放login()方法登录失败的原因
  BOOL m_connectfailed;
  BOOL m_loginfailed;
  BOOL m_optionfailed;

  void initdata();

  // 登录ftp服务器
  // in_host 服务器地址和端口，中间用":"分隔，如"192.168.1.1:21"
  // in_username ftp用户名
  // in_password ftp的密码
  // in_mode 传输模式，FTPLIB_PASSIVE是被动模式，FTPLIB_PORT是主动模式
  BOOL login(const char *in_host,const char *in_username,const char *in_password,const int in_mode=FTPLIB_PASSIVE);
  
  // 注销
  BOOL logout();

  // 获取ftp服务器上文件的时间
  BOOL mtime(const char *in_remotefilename);

  // 获取ftp服务器上文件的大小
  BOOL size(const char *in_remotefilename);

  // 向服务端发送site命令
  BOOL site(const char *in_command);

  // 改变ftp远程目录
  BOOL chdir(const char *in_remotedir);

  // 在ftp服务器上创建目录
  BOOL mkdir(const char *in_remotedir);

  // 删除ftp服务器上的目录
  BOOL rmdir(const char *in_remotedir);

  // 发送list命令列出ftp服务器目录中的文件，结果保存到本地文件中
  // 如果是列出当前目录，in_remotedir用"","*","."都行。
  BOOL nlist(const char *in_remotedir,const char *out_listfilename);

  // 发送dir命令列出ftp服务器目录中的文件，结果保存到本地文件中
  BOOL dir(const char *in_remotedir,const char *out_listfilename);

  // 从ftp服务器上获取文件
  // in_remotefilename 待获取的远程文件名
  // in_localfilename  本地文件名，可以与in_remotefilename不同
  // bCheckSize 文件传输完成后，是否核对本地和远程文件的大小，保证文件的完整性
  // 注意，文件在传输的过程中，采用临时文件命名的方法，即在in_localfilename后加".tmp"，在传输
  // 完成后才正式改为in_localfilename
  BOOL get(const char *in_remotefilename,const char *in_localfilename,const BOOL bCheckSize=TRUE);

  // 向ftp服务器发送文件
  // in_localfilename 本地待发送的文件名
  // in_remotefilename 远程文件名
  // bCheckSize 文件传输完成后，是否核对本地和远程文件的大小，保证文件的完整性
  // 注意，文件在传输的过程中，采用临时文件命名的方法，即在in_remotefilename后加".tmp"，在传输
  // 完成后才正式改为in_remotefilename
  BOOL put(const char *in_localfilename,const char *in_remotefilename,const BOOL bCheckSize=TRUE);

  // 删除ftp服务器上的文件
  BOOL ftpdelete(const char *in_remotefilename);

  // 把ftp服务器上的文件改名
  BOOL ftprename(const char *in_srcremotefilename,const char *in_dstremotefilename);

  // return a pointer to the last response received
  char *response();
};

#endif