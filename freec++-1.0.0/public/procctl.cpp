#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char *argv[])
{
  if ( (argc < 3) || (argc > 10) )
  {
    printf("\n");
    printf("Using:./procctl tvltime program argvs......\n");

    printf("Example:/htidc/htidc/bin/procctl 300 /bin/tar zcvf /home/sendtmp/zqczlog.tgz /tmp/htidc/log\n\n");
    printf("        /htidc/htidc/bin/procctl 300 /bin/mv /home/sendtmp/zqczlog.tgz /home/send/zqczlog.tgz\n\n");
 
    printf("这是后台程序的主调度程序，可以调度全部的后台程序。\n");
    printf("tvltime运行周期，被调度的程序运行结束后，在tvltime秒后会被procctl重新启动。\n");
    printf("program被调度的程序名，必须使用全路径。\n");
    printf("argvs被调度的程序的参数，最多7个参数，即procctl的参数总数不超过10个。\n");
    printf("注意，procctl不会被kill杀死，但可以用kill -9强行杀死。\n\n\n");
 
    return -1;
  }

  // 关闭全部的系统信号
  for(int i=0;i<50;i++)
  {
    signal(i,SIG_IGN); close(i);
  }

  // 脱离终端控制
  setpgrp();

  // 生成子进程，父进程退出
  if (fork() != 0) exit(0);

  // 启用SIGCHLD信号
  signal(SIGCHLD,SIG_DFL);

  int status;

  while (1)
  {
    if (fork() == 0)
    {
      if (argc == 3)
      {
        execl(argv[2],argv[2],(char *)0);
      }

      if (argc == 4)
      {
        execl(argv[2],argv[2],argv[3],(char *)0);
      }

      if (argc == 5)
      {
        execl(argv[2],argv[2],argv[3],argv[4],(char *)0);
      }

      if (argc == 6)
      {
        execl(argv[2],argv[2],argv[3],argv[4],argv[5],(char *)0);
      }
      if (argc == 7)
      {
        execl(argv[2],argv[2],argv[3],argv[4],argv[5],argv[6],(char *)0);
      }

      if (argc == 8)
      {
        execl(argv[2],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],(char *)0);
      }

      if (argc == 9)
      {
        execl(argv[2],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],(char *)0);
      }

      if (argc == 10)
      {
        execl(argv[2],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],argv[9],(char *)0);
      }
   
      exit(0);
    }
    else
    {
      wait(&status); sleep(atoi(argv[1]));
    }
  }

  exit(0);
}
