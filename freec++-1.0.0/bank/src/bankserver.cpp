//
// 本程序为银行柜台业务的后台服务程序
// 

#include "bank.h"

// 程序退出时调用的函数
void MAINEXIT(int sig);

CLogFile logfile;

// 用于处理客户端的连接的主程序
void *pthid_main(void *arg);

// 用于存放全部客户端连接的sock
vector<int> v_sock;

// TCP服务端
CTcpServer TcpServer;

// 数据库连接池
pthread_mutex_t mutex[5];
connection conn[5];

// 处理客户端报文的主函数
BOOL procmessage(char *strRecvBuffer,char *strSendBuffer,vector<struct st_bank> &vbank);

int main(int argc,char *argv[])
{
  if (argc != 4)
  {
    printf("\n");
    printf("Using:./bankserver port connstr logfilename\n\n");

    printf("Example:/home/wucz/bank/bin/bankserver 5010 scott/tiger@orcl11g_193.112.167.234 /home/wucz/bank/log/bankserver.log\n\n");

    printf("本程序为银行柜台业务的后台服务程序，展示了多线程、线程同步和数据库连接池技术。\n\n");

    return -1;
  }

  // 关闭全部的信号和输入输出
  // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
  // 但请不要用 "kill -9 +进程号" 强行终止
  CloseIOAndSignal(); signal(SIGINT,MAINEXIT); signal(SIGTERM,MAINEXIT);

  // 打开日志文件
  if (logfile.Open(argv[3],"a+")==FALSE)
  {
    printf("logfile.Open(%s) failed.\n",argv[3]); return -1;
  }

  // TCP服务端初始化
  if (TcpServer.InitServer(atoi(argv[1])) == FALSE)
  {
    logfile.Write("TcpServer.InitServer(%s) failed.\n",argv[1]); return -1;
  }

  // 初始化数据库连接池
  for (int ii=0;ii<5;ii++)
  {
     pthread_mutex_init(&mutex[ii],NULL);
    if (conn[ii].connecttodb(argv[2],"Simplified Chinese_China.ZHS16GBK") != 0)
    {
      logfile.Write("connect database failed.\n%s\n",conn[ii].m_cda.message); return -1;
    }
  }

  while (TRUE)
  {
    // 等待客户端的连接
    if (TcpServer.Accept() == FALSE)
    {
      logfile.Write("TcpServer.Accept() failed.\n"); MAINEXIT(-1);
    }

    // 把客户端的sock放入容器
    v_sock.push_back(TcpServer.m_connfd);

    logfile.Write("接受到一个新的连接。\n");

    pthread_t pthid;

    // 创建子线程，用于处理客户端的连接
    if ( pthread_create(&pthid,NULL,pthid_main,(void*)TcpServer.m_connfd) != 0)
    {
      logfile.Write("pthread_create failed.\n"); MAINEXIT(-1);
    }
  }

  return 0;
}

// 用于处理客户端的连接的主程序
void *pthid_main(void *arg)
{
  int sockfd = (long)arg;

  int  irecvlen=0;
  char strRecvBuffer[1024],strSendBuffer[1024];

  while (TRUE)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
    memset(strSendBuffer,0,sizeof(strSendBuffer));
  
    // 读取客户端的报文，等时间是80秒
    if (TcpRead(sockfd,strRecvBuffer,&irecvlen,80)==FALSE)
    {
      logfile.Write("TcpRead() failed.\n"); break;
    }

    logfile.Write("recv ok:%s\n",strRecvBuffer);

    vector<struct st_bank> vbank;

    // 处理客户端报文的主函数
    procmessage(strRecvBuffer,strSendBuffer,vbank);

    // 向客户端返回响应内容
    if (TcpWrite(sockfd,strSendBuffer)==FALSE)
    {
      logfile.Write("TcpWrite() failed.\n"); break;
    }

    logfile.Write("send ok:%s\n",strSendBuffer);

    // 如果交易有结果集，在这里返回
    if ( vbank.size()>0 )
    {
      for (UINT ii=0;ii<vbank.size();ii++)
      {
        memset(strSendBuffer,0,sizeof(strSendBuffer));
        sprintf(strSendBuffer,"<opertype>%d</opertype><je>%.2f</je><crttime>%s</crttime>",vbank[ii].opertype,vbank[ii].je,vbank[ii].crttime);
        if (TcpWrite(sockfd,strSendBuffer)==FALSE)
        {
          logfile.Write("TcpWrite() failed.\n"); break;
        }
      }
    }
  }

  pthread_exit(NULL);
}

void MAINEXIT(int sig)
{
  signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);

  if (sig>0) signal(sig,SIG_IGN);

  logfile.Write("main exit,sig=%d.",sig);

  logfile.Write("关闭数据库连接池。\n");

  // 关闭数据库连接池。
  for (int ii=0;ii<5;ii++)
  {
    pthread_mutex_destroy(&mutex[ii]);
    conn[ii].disconnect();
  }

  logfile.Write("关闭监听的sock。\n");

  // 关闭监听的sock
  TcpServer.CloseListen();

  logfile.Write("关闭客户端的sock。\n");

  // 关闭客户端的sock。
  for (UINT ii=0;ii<v_sock.size();ii++)
  {
    close(v_sock[ii]);
  }

  logfile.Write("程序退出。\n");

  // 关闭日志文件
  logfile.Close();

  exit(0);
}

// 处理客户端报文的主函数
BOOL procmessage(char *strRecvBuffer,char *strSendBuffer,vector<struct st_bank> &vbank)
{
  connection *pconn;

  int iconn=0;

  for (iconn=0;iconn<5;iconn++)
  {
    if (pthread_mutex_trylock(&mutex[iconn])==0)
    {
      logfile.Write("获取连接池%d\n",iconn);
      pconn=&conn[iconn]; break;
    }
  }

  if (iconn==5)
  {
    strcpy(strSendBuffer,"<retno>-1</retno><retmesg>系统繁忙。</retmesg>"); return FALSE;
  }

  struct st_bank stbank;
  memset(&stbank,0,sizeof(struct st_bank));

  GetXMLBuffer(strRecvBuffer,"msgid",&stbank.msgid);
  GetXMLBuffer(strRecvBuffer,"oper_id",stbank.oper_id,30);
  GetXMLBuffer(strRecvBuffer,"password",stbank.password,30);
  GetXMLBuffer(strRecvBuffer,"userid",stbank.userid,20);
  GetXMLBuffer(strRecvBuffer,"cardid",stbank.cardid,18);
  GetXMLBuffer(strRecvBuffer,"username",stbank.username,30);
  GetXMLBuffer(strRecvBuffer,"tel",stbank.tel,30);
  GetXMLBuffer(strRecvBuffer,"opertype",&stbank.opertype);
  GetXMLBuffer(strRecvBuffer,"je",&stbank.je);
  GetXMLBuffer(strRecvBuffer,"userid1",stbank.userid1,20);

  // 操作员登录业务
  if (stbank.msgid==1)
  {
    if (msgid_1(pconn,&stbank,&logfile)==0)
    {
      strcpy(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg>");
    }
    else
    {
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>操作员身份认证失败。</retmesg>");
    }
  }

  // 开户业务
  if (stbank.msgid==2)
  {
    if (msgid_2(pconn,&stbank,&logfile)==0)
    {
      pconn->commit();
      strcpy(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg>");
    }
    else
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>系统故障。</retmesg>");
    }
  }

  // 客户身份认证
  if (stbank.msgid==3)
  {
    if (msgid_3(pconn,&stbank,&logfile)==0)
    {
      strcpy(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg>");
    }
    else
    {
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>客户身份认证失败。</retmesg>");
    }
  }

  // 存款业务
  if (stbank.msgid==4)
  {
    if (msgid_4(pconn,&stbank,&logfile)==0)
    {
      pconn->commit();
      strcpy(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg>");
    }
    else
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>系统故障。</retmesg>");
    }
  }

  // 取款业务
  if (stbank.msgid==5)
  {
    int iret=msgid_5(pconn,&stbank,&logfile);

    // 成功
    if (iret==0)
    {
      conn->commit();
      strcpy(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg>");
    }

    // 余额不足
    if (iret==-1)
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>余额不足。</retmesg>");
    }

    // 系统故障
    if (iret>0)
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>系统故障。</retmesg>");
    }
  }

  // 转帐业务
  if (stbank.msgid==6)
  {
    // 判断转出帐户的余额
    // 判断输入帐户是否存在，并且帐户的状态不能是3-已销户
    int iret=msgid_6(pconn,&stbank,&logfile);

    // 成功
    if (iret==0)
    {
      conn->commit();
      strcpy(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg>");
    }

    // 余额不足
    if (iret==-1)
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>余额不足。</retmesg>");
    }

    // 转入帐户非法
    if (iret==-2)
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-2</retno><retmesg>转入帐户非法。</retmesg>");
    }

    // 系统故障
    if (iret>0)
    {
      pconn->rollback();
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>系统故障。</retmesg>");
    }

  }
  
  // 查余额
  if (stbank.msgid==7)
  {
    if (msgid_7(pconn,&stbank,&logfile)==0)
    {
      sprintf(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg><ye>%.2f</ye>",stbank.ye);
    }
    else
    {
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>客户信息不存在。</retmesg>");
    }
  }

  // 查流水
  if (stbank.msgid==8)
  {
    if (msgid_8(pconn,&stbank,&logfile,vbank)==0)
    {
      sprintf(strSendBuffer,"<retno>0</retno><retmesg>ok</retmesg><totalcount>%lu</totalcount>",vbank.size());
    }
    else
    {
      strcpy(strSendBuffer,"<retno>-1</retno><retmesg>系统故障。</retmesg>");
    }
  }

  pthread_mutex_unlock(&mutex[iconn]);

  return TRUE;
}




