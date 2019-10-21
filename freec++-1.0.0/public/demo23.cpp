//
// 本程序演示多线程的TCP客户端的线程同步操作。
// 

#include "_public.h"

pthread_mutex_t mutex;

// 发送报文的线程主函数
void *send_main(void *arg);
// 接收报文的线程主函数
void *recv_main(void *arg);

CTcpClient TcpClient;

int main(int argc,char *argv[])
{
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./demo23 ip,port\n\n");

    printf("Example:./demo23 193.112.167.234,5010\n\n");

    printf("本程序演示多线程的TCP客户端的线程同步操作。\n\n");

    return -1;
  }

  // 向服务器发起连接
  if (TcpClient.ConnectToServer(argv[1]) == FALSE)
  {
    printf("TcpClient.ConnectToServer(%s) failed.\n",argv[1]); return -1;
  }

  pthread_t psend1,psend2,precv;

  // 初始化互拆锁
  pthread_mutex_init(&mutex,NULL);

  // 创建发送线程一
  if ( pthread_create(&psend1,NULL,send_main,(void *)1) != 0)
  {
    printf("pthread_create psend1 failed.\n"); pthread_mutex_destroy(&mutex); return -1;
  }
  
  // 创建发送线程二
  if ( pthread_create(&psend2,NULL,send_main,(void *)2) != 0)
  {
    printf("pthread_create psend2 failed.\n"); pthread_mutex_destroy(&mutex); return -1;
  }

  // 创建接收线程
  if ( pthread_create(&precv,NULL,recv_main,(void*)4) != 0)
  {
    printf("pthread_create precv failed.\n"); pthread_mutex_destroy(&mutex); return -1;
  }

  // 等待线程退出
  pthread_join(psend1,NULL);
  pthread_join(psend2,NULL);
  pthread_join(precv ,NULL);

  // 销毁锁
  pthread_mutex_destroy(&mutex);

  return 0;
}

void *send_main(void *arg)
{
  long ll=(long)arg;
  
  printf("线程%ld开始运行。\n",ll);

  char strSendBuffer[1024];

  for (int ii=0;ii<100000;ii++)
  {
    memset(strSendBuffer,0,sizeof(strSendBuffer));
    sprintf(strSendBuffer,"%ld:%03d:%s",ll,ii,\
          "英超最后一轮，卡里克踢完了在曼联的最后一场正式比赛，这意味着红魔上次称霸欧冠的黄金一代全部退场。");

    pthread_mutex_lock(&mutex);

    // 把strSendBuffer内容发送给服务端
    if ( TcpWrite(TcpClient.m_sockfd,strSendBuffer) == FALSE)
    {
      pthread_mutex_unlock(&mutex); printf("TcpClient.Write() failed.\n"); exit(-1);
    }

    pthread_mutex_unlock(&mutex);
  }

  printf("线程%ld运行结束。\n",ll);

  pthread_exit(NULL);
}

void *recv_main(void *arg)
{
  char strRecvBuffer[1024];

  while (TRUE)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));

    // 接收服务端返回的报文
    if (TcpClient.Read(strRecvBuffer,20)==FALSE)
    {
      printf("TcpClient.Read() failed.\n"); exit(-1);
    }
  }

  pthread_exit(NULL);
}
