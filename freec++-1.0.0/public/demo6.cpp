//
// 本程序演示文件操作类CFile，打开并逐行读取文件。
// 

#include "_public.h"

struct st_emp
{
  int id;
  char name[31];
  char sex[3];
  int  age;
  char memo[301];
}stemp;


int main(int argc,char *argv[])
{
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./demo6 xmlfilename\n\n");

    printf("Example:./demo6 demo6.xml\n\n");

    printf("本程序演示文件操作类CFile，打开并逐行读取文件，xmlfilename是数据文件名。\n");

    return -1;
  }

  CFile File;

  // 打开文件
  if (File.OpenForRead(argv[1],"rt")==FALSE)
  {
    printf("File.OpenForRead(%s) failed.\n",argv[1]); return -1;
  }
  
  char strBuffer[1024];

  while (TRUE)
  {
    memset(&strBuffer,0,sizeof(strBuffer));

    if (File.FFGETS(strBuffer,1000,"<endl/>")==FALSE) break;

    memset(&stemp,0,sizeof(stemp));

    GetXMLBuffer(strBuffer,"id",  &stemp.id);
    GetXMLBuffer(strBuffer,"name",stemp.name,30);
    GetXMLBuffer(strBuffer,"sex", stemp.sex,2);
    GetXMLBuffer(strBuffer,"age",&stemp.age);
    GetXMLBuffer(strBuffer,"memo",stemp.memo,300);

    printf("id=%d,name=%s,sex=%s,age=%d,memo=%s\n",stemp.id,stemp.name,stemp.sex,stemp.age,stemp.memo);
  }

  // 关闭文件
  File.CloseOnly();

  return 0;
}

