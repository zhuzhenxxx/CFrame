
#include "_public.h"

CTcpClient TcpClient;
char strRecvBuffer[1024],strSendBuffer[1024];

BOOL login();
BOOL kaihu();
BOOL khlogin();
BOOL ck();
BOOL qk();
BOOL zz();
BOOL cye();
BOOL cls();

int main(int argc,char *argv[])
{
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./bankclient ip,port\n\n");

    printf("Example:/home/wucz/bank/bin/bankclient 193.112.167.234,5010\n\n");

    printf("本程序演示银行柜台业务系统的客户端。\n\n");

    return -1;
  }

  // 向服务器发起连接
  if (TcpClient.ConnectToServer(argv[1]) == FALSE)
  {
    printf("TcpClient.ConnectToServer(%s) failed.\n",argv[1]); return -1;
  }

  login();

  // kaihu();

  // ck();

  // khlogin();

  // qk();

  // zz();

  // cye();

  cls();

  return 0;
}

BOOL login()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>1</msgid><oper_id>wucz</oper_id><password>wuczpwd</password>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL kaihu()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>2</msgid><oper_id>wucz</oper_id><userid>100000002</userid><password>123456</password><cardid>440115675543216252</cardid><username>李四</username><tel>13922200001</tel>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL khlogin()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>3</msgid><userid>100000001</userid><password>123456</password>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL ck()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>4</msgid><oper_id>wucz</oper_id><userid>100000001</userid><je>10000.02</je>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL qk()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>5</msgid><oper_id>wucz</oper_id><userid>100000001</userid><je>500.01</je>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL zz()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>6</msgid><oper_id>wucz</oper_id><userid>100000001</userid><je>10.01</je><userid1>100000002</userid1>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL cye()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>7</msgid><userid>100000001</userid>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return TRUE;
}

BOOL cls()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<msgid>8</msgid><userid>100000001</userid>");

  // 把strSendBuffer内容发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return FALSE;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return FALSE;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  int itotalcount=0;

  GetXMLBuffer(strRecvBuffer,"totalcount",&itotalcount);

  for (int ii=0;ii<itotalcount;ii++)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));

    // 接收服务端返回的报文
    if (TcpClient.Read(strRecvBuffer)==FALSE)
    {
      printf("TcpClient.Read() failed.\n"); return FALSE;
    }

    printf("recv ok:%s\n",strRecvBuffer);
  }

  return TRUE;
}
