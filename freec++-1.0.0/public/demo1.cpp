//
// 本程序演示采用CXmlFile类读取参数文件
// 参数文件名是demo1.xml，由main参数传入
// 

#include "_public.h"

// 定义用于存放参数的数据结构
struct st_args
{
  char connstr[51];    // 短信平台连接数据库的参数
  char logpath[301];   // 日志文件存放的目录
  char smsdata[301];   // 短信数据文件存放的根目录
  char cmpphmd[301];   // 移动号码段
  char sgiphmd[301];   // 联通号码段
  char smgphmd[301];   // 电信号码段
};

int main(int argc,char *argv[])
{
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./demo1 xmlfilename\n\n");

    printf("Example:./demo1 demo1.xml\n\n");

    printf("本程序演示采用CXmlFile类读取参数文件，demo1.xml是参数文件名。\n\n");

    return -1;
  }

  struct st_args stargs;
  memset(&stargs,0,sizeof(struct st_args));

  CXmlFile XmlFile;

  // 导入参数文件
  if (XmlFile.LoadFile(argv[1]) == FALSE)
  {
    printf("XmlFile.LoadFile(%s) failed.\n",argv[1]); return -1;
  }

  // 获取每个参数
  XmlFile.GetValue("connstr",stargs.connstr,50);
  XmlFile.GetValue("logpath",stargs.logpath,300);
  XmlFile.GetValue("smsdata",stargs.smsdata,300);
  XmlFile.GetValue("cmpphmd",stargs.cmpphmd,300);
  XmlFile.GetValue("sgiphmd",stargs.sgiphmd,300);
  XmlFile.GetValue("smgphmd",stargs.smgphmd,300);

  // 把全部的参数值打印出来
  printf("connstr=%s\n",stargs.connstr);
  printf("logpath=%s\n",stargs.logpath);
  printf("smsdata=%s\n",stargs.smsdata);
  printf("cmpphmd=%s\n",stargs.cmpphmd);
  printf("sgiphmd=%s\n",stargs.sgiphmd);
  printf("smgphmd=%s\n",stargs.smgphmd);

  return 0;
}

