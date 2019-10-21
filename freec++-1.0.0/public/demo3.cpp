//
// 本程序演示采用CLogFile类写日志
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./demo3 logfilename\n\n");

    printf("Example:./demo3 demo3.log\n\n");

    printf("本程序演示采用CLogFile类写程序运行的日志，参数是日志文件名。\n\n");

    return -1;
  }

  CLogFile logfile;

  // 创建日志文件
  if (logfile.Open(argv[1],"a+")==FALSE)
  {
    printf("logfile.Open(%s) failed.\n",argv[1]); return -1;
  }

  // 往日志文件中写入2000000条内容。
  for (int ii=0;ii<2000000;ii++)
  {
    logfile.Write("%010d，这是第%010d行日志，程序已循环了%10d次。\n",ii+1,ii+1,ii+1);
  }

  return 0;
}

