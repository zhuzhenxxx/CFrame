////////////////////////////////////////////////////////
// 本软件由神农开源网提供，开源软件需要大家的努力。
// 技术交流请加QQ群701117364，我们一起把开源软件做得更好。
//
// 软件官方遵守协议约定
//
// 1) 禁止用于技术培训、出版等商业用途。
// 2) 禁止任何侵权行为，包括软件的源码程序、资料文档、文件信息。
// 3) 禁止软件产品插入与软件官方无关的任何广告信息，包括文字、图像、媒体信息。
// 4) 禁止软件产品内置与软件官方程序应用无关的任何第三方插件程序、组件。
// 5) 禁止软件程序存在任何不经过用户许可的远程响应执行的控制程序。
// 6) 禁止软件程序使用任何手段收集用户隐私信息与用户储存的数据。
//
//
//  用户遵守协议约定
//  1) 禁止用户修改软件官方任何的版权说明信息，包括程序的版权协议说明、软件官方链接、文字说明、图像标志、媒体信息。
//  2) 禁止用户通过任何方式破坏、侵犯软件官方的正常发展运营，包括软件官方程序的正常运行与商业授权机制。
//
////////////////////////////////////////////////////////

#include "_ftp.h"

Cftp::Cftp()
{
  m_ftpconn=0;

  initdata();

  FtpInit();

  m_connectfailed=FALSE;
  m_loginfailed=FALSE;
  m_optionfailed=FALSE;
}

Cftp::~Cftp()
{
  logout();
}

void Cftp::initdata()
{
  m_size=0;

  memset(m_mtime,0,sizeof(m_mtime));
}

BOOL Cftp::login(const char *in_host,const char *in_username,const char *in_password,const int in_mode)
{
  if (m_ftpconn != 0) { FtpQuit(m_ftpconn); m_ftpconn=0; }

  m_connectfailed=m_loginfailed=m_optionfailed=FALSE;

  if (FtpConnect(in_host,&m_ftpconn) == FALSE)  { m_connectfailed=TRUE; return FALSE; }

  if (FtpLogin(in_username,in_password,m_ftpconn) == FALSE)  { m_loginfailed=TRUE; return FALSE; }

  if (FtpOptions(FTPLIB_CONNMODE,(long)in_mode,m_ftpconn) == FALSE) { m_optionfailed=TRUE; return FALSE; }

  return TRUE;
}

BOOL Cftp::logout()
{
  if (m_ftpconn == 0) return FALSE;

  FtpQuit(m_ftpconn);

  m_ftpconn=0;

  return TRUE;
}

BOOL Cftp::get(const char *in_remotefilename,const char *in_localfilename,const BOOL bCheckSize)
{
  if (m_ftpconn == 0) return FALSE;

  MKDIR(in_localfilename);

  char strlocalfilenametmp[301];
  memset(strlocalfilenametmp,0,sizeof(strlocalfilenametmp));
  snprintf(strlocalfilenametmp,300,"%s.tmp",in_localfilename);

  if (FtpGet(strlocalfilenametmp,in_remotefilename,FTPLIB_IMAGE,m_ftpconn) == FALSE) return FALSE;
  
  // 获取远程服务器的文件的时间
  if (mtime(in_remotefilename) == FALSE) return FALSE;

  // 获取远程服务器的文件的大小
  if (size(in_remotefilename) == FALSE) return FALSE;

  if (bCheckSize==FALSE)
  {
    if (m_size != FileSize(strlocalfilenametmp)) return FALSE; 
  }

  UTime(strlocalfilenametmp,m_mtime);

  if (rename(strlocalfilenametmp,in_localfilename) != 0) return FALSE; 

  return TRUE;
}

BOOL Cftp::mtime(const char *in_remotefilename)
{
  if (m_ftpconn == 0) return FALSE;
  
  memset(m_mtime,0,sizeof(m_mtime));
  
  char strmtime[21];
  memset(strmtime,0,sizeof(strmtime));

  if (FtpModDate(in_remotefilename,strmtime,14,m_ftpconn) == FALSE) return FALSE;

  AddTime(strmtime,m_mtime,0+8*60*60,"yyyymmddhh24miss");

  return TRUE;
}

BOOL Cftp::size(const char *in_remotefilename)
{
  if (m_ftpconn == 0) return FALSE;

  m_size=0;
  
  if (FtpSize(in_remotefilename,&m_size,FTPLIB_IMAGE,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::site(const char *in_command)
{
  if (m_ftpconn == 0) return FALSE;
  
  if (FtpSite(in_command,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::chdir(const char *in_remotedir)
{
  if (m_ftpconn == 0) return FALSE;
  
  if (FtpChdir(in_remotedir,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::mkdir(const char *in_remotedir)
{
  if (m_ftpconn == 0) return FALSE;
  
  if (FtpMkdir(in_remotedir,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::rmdir(const char *in_remotedir)
{
  if (m_ftpconn == 0) return FALSE;
  
  if (FtpRmdir(in_remotedir,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::dir(const char *in_remotedir,const char *out_listfilename)
{
  if (m_ftpconn == 0) return FALSE;
  
  if (FtpDir(out_listfilename,in_remotedir,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::nlist(const char *in_remotedir,const char *out_listfilename)
{
  if (m_ftpconn == 0) return FALSE;
  
  if (FtpNlst(out_listfilename,in_remotedir,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::put(const char *in_localfilename,const char *in_remotefilename,const BOOL bCheckSize)
{
  if (m_ftpconn == 0) return FALSE;

  char strremotefilenametmp[301];
  memset(strremotefilenametmp,0,sizeof(strremotefilenametmp));
  snprintf(strremotefilenametmp,300,"%s.tmp",in_remotefilename);

  if (FtpPut(in_localfilename,strremotefilenametmp,FTPLIB_IMAGE,m_ftpconn) == FALSE) return FALSE;

  if (bCheckSize==TRUE)
  {
    if (size(in_remotefilename) == FALSE) return FALSE;

    if (m_size != FileSize(in_localfilename)) return FALSE; 
  }

  if (FtpRename(strremotefilenametmp,in_remotefilename,m_ftpconn) == FALSE) return FALSE;

  return TRUE;
}

BOOL Cftp::ftpdelete(const char *in_remotefilename)
{
  if (m_ftpconn == 0) return FALSE;

  if (FtpDelete(in_remotefilename,m_ftpconn) == FALSE) return FALSE;
  
  return TRUE;
}

BOOL Cftp::ftprename(const char *in_srcremotefilename,const char *in_dstremotefilename)
{
  if (m_ftpconn == 0) return FALSE;

  if (FtpRename(in_srcremotefilename,in_dstremotefilename,m_ftpconn) == FALSE) return FALSE;
  
  return TRUE;
}

char *Cftp::response()
{
  if (m_ftpconn == 0) return FALSE;

  return FtpLastResponse(m_ftpconn);
}

