//
// 本程序演示采用CTcpServer类，采用多线程的方式，实现socket通讯的服务端
// 

#include "_public.h"

CLogFile logfile;

void *pthid_main(void *arg);

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:./demo22 port logfilename\n\n");

    printf("Example:./demo22 5010 demo22.log\n\n");

    printf("本程序演示采用CTcpServer类，采用多线程的方式，实现socket通讯的服务端。\n\n");

    return -1;
  }

  if (logfile.Open(argv[2],"a+")==FALSE)
  {
    printf("logfile.Open(%s) failed.\n",argv[2]); return -1;
  }

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

    logfile.Write("接受到一个新的连接。\n");

    pthread_t pthid;

    // 创建子线程
    if ( pthread_create(&pthid,NULL,pthid_main,(void*)TcpServer.m_connfd) != 0)
    {
      logfile.Write("pthread_create failed.\n"); return -1;
    }
  }

  return 0;
}

void *pthid_main(void *arg)
{
  int sockfd = (long)arg;

  int  irecvlen=0;
  char strRecvBuffer[1024],strSendBuffer[1024];

  while (TRUE)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
  
    // 读取客户端的报文，等时间是20秒
    if (TcpRead(sockfd,strRecvBuffer,&irecvlen,20)==FALSE)
    {
      logfile.Write("TcpRead() failed.\n"); pthread_exit(NULL);
    }

    logfile.Write("recv ok:%s\n",strRecvBuffer);

    memset(strSendBuffer,0,sizeof(strSendBuffer));
    strcpy(strSendBuffer,"ok");

    // 向客户端返回响应内容
    if (TcpWrite(sockfd,strSendBuffer)==FALSE)
    {
      logfile.Write("TcpWrite() failed.\n"); pthread_exit(NULL);
    }

    logfile.Write("send ok:%s\n",strSendBuffer);
  }

  pthread_exit(NULL);
}
