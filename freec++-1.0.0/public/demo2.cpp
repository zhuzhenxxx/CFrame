//
// 本程序演示采用CDir类获取目录及子目录中的文件名
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:./demo2 pathname match\n\n");

    printf("Example:./demo2 /tmp \"*.txt,*.cpp\"\n\n");

    printf("本程序演示采用CDir类读取目录中的文件名，参数1是目录名，参数2是文件名匹配的规则。\n\n");

    return -1;
  }

  CDir Dir;

  // 打开目录，读取该目录下全部的*.txt文件，包括子目录。
  if (Dir.OpenDir(argv[1],argv[2],10000,TRUE,TRUE)==FALSE)
  {
    printf("Dir.OpenDir(%s) failed.\n",argv[1]); return -1;
  }

  // 列出全部的文件名和其它信息
  while (TRUE)
  {
    if (Dir.ReadDir()==FALSE) break;

    printf("file=%s,size=%ld,mtime=%s\n",Dir.m_FullFileName,Dir.m_FileSize,Dir.m_ModifyTime);
  }

  return 0;
}

