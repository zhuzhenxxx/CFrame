//
// 本程序演示采用CTcpServer类，采用多进程的方式，实现socket通讯的服务端
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:./demo16 port logfilename\n\n");

    printf("Example:./demo16 8000 demo16.log\n\n");

    printf("本程序演示采用CTcpServer类，采用多进程的方式，实现socket通讯的服务端。\n");

    return -1;
  }

  CLogFile logfile;

  if (logfile.Open(argv[2],"a+")==FALSE)
  {
    printf("logfile.Open(%s) failed.\n",argv[2]); return -1;
  }

  signal(SIGCHLD,SIG_IGN);

  CTcpServer TcpServer;

  // 服务端初始化
  if (TcpServer.InitServer(atoi(argv[1])) == FALSE)
  {
    logfile.Write("TcpServer.InitServer(%s) failed.\n",argv[1]); return -1;
  }

  while (TRUE)
  {
    // 等待客户端的连接
    if (TcpServer.Accept() == FALSE)
    {
      logfile.Write("TcpServer.Accept() failed.\n"); return -1;
    }

    if (fork()>0)
    {
      TcpServer.CloseClient(); continue;
    }

    logfile.Write("接受到一个新的连接。\n");

    TcpServer.CloseListen();

    char strRecvBuffer[1024],strSendBuffer[1024];

    while (TRUE)
    {
      memset(strRecvBuffer,0,sizeof(strRecvBuffer));
  
      // 读取客户端的报文，等时间是20秒
      if (TcpServer.Read(strRecvBuffer,20)==FALSE) 
      {
        logfile.Write("TcpServer.Read() failed.\n"); return -1;
      }

      logfile.Write("recv ok:%s\n",strRecvBuffer);

      memset(strSendBuffer,0,sizeof(strSendBuffer));
      strcpy(strSendBuffer,"ok");

      // 向客户端返回响应内容
      if (TcpServer.Write(strSendBuffer)==FALSE) 
      {
        logfile.Write("TcpServer.Write() failed.\n"); return -1;
      }
    }

    logfile.Write("send ok:%s\n",strSendBuffer);
  }

  return 0;
}

