//
// 本程序演示采用CTcpClient类，实现socket通讯的客户端和文件传输。
// 

#include "_public.h"

// 把文件的内容发送给服务端
BOOL SendFile(int sockfd,char *filename,UINT filesize);

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:./demo13 ip,port filename\n\n");

    printf("Example:./demo13 193.112.167.234,5010 test1.jpg\n\n");

    printf("本程序演示采用CTcpClient类，实现socket通讯的客户端和文件传输。\n\n");

    return -1;
  }

  // 判断文件是否存
  if (access(argv[2],R_OK) != 0)
  {
    printf("file %s not exist.\n",argv[2]); return -1;
  }

  UINT uFileSize=0;
  char strMTime[20],strRecvBuffer[1024],strSendBuffer[1024];

  // 获取文件的时间和大小
  memset(strMTime,0,sizeof(strMTime));
  FileMTime(argv[2],strMTime);

  // 获取文件的大小
  uFileSize=FileSize(argv[2]);

  // 把文件的信息封装成一个xml，发送给服务端
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  snprintf(strSendBuffer,100,"<filename>%s</filename><mtime>%s</mtime><size>%lu</size>",argv[2],strMTime,uFileSize);

  CTcpClient TcpClient;

  // 向服务器发起连接
  if (TcpClient.ConnectToServer(argv[1]) == FALSE)
  {
    printf("TcpClient.ConnectToServer(%s) failed.\n",argv[1]); return -1;
  }

  // 把文件信息的xml发送给服务端
  if (TcpClient.Write(strSendBuffer)==FALSE)
  {
    printf("TcpClient.Write() failed.\n"); return -1;
  }

  printf("send xml:%s\n",strSendBuffer);

  printf("send file ...");

  // 把文件的内容发送给服务端
  if (SendFile(TcpClient.m_sockfd,argv[2],uFileSize)==FALSE)
  {
    printf("SendFile(%s) failed.\n",argv[2]); return -1;
  }

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // 接收服务端返回的确认报文
  if (TcpClient.Read(strRecvBuffer)==FALSE)
  {
    printf("TcpClient.Read() failed.\n"); return -1;
  }

  if (strcmp(strRecvBuffer,"ok")==0)
    printf("ok.\n");
  else
    printf("failed.\n");

  return 0;
}

// 把文件的内容发送给服务端
BOOL SendFile(int sockfd,char *filename,UINT filesize)
{
  int  bytes=0;
  int  total_bytes=0;
  int  onread=0;
  char buffer[1000];

  FILE *fp=NULL;

  if ( (fp=fopen(filename,"rb")) == NULL ) 
  {
    printf("fopen(%s) failed.\n",filename); return FALSE;
  }

  while (TRUE)
  {
    memset(buffer,0,sizeof(buffer));

    if ((filesize-total_bytes) > 1000) onread=1000;
    else onread=filesize-total_bytes;

    bytes=fread(buffer,1,onread,fp);

    if (bytes > 0)
    {
      if (Writen(sockfd,buffer,bytes) == FALSE)
      {
        printf("Writen() failed.\n"); fclose(fp); fp=NULL; return FALSE;
      }
    }

    total_bytes = total_bytes + bytes;

    if ((UINT)total_bytes == filesize) break;
  }

  fclose(fp);

  return TRUE;
}

