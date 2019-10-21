//
// 本程序演示几个时间操作函数
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  char strLocalTime[20];
  memset(strLocalTime,0,sizeof(strLocalTime));

  // 获取当前时间
  LocalTime(strLocalTime,"yyyy-mm-dd hh24:mi:ss");
  printf("%s\n",strLocalTime);

  // 把当前时间的字符串形式转换为time_t
  printf("%ld\n",UTCTime(strLocalTime));

  // 以time_t的形式获取当前时间
  time_t  timer;
  time( &timer );
  printf("%ld\n",timer);

  // 把当前时间减一天
  AddTime(strLocalTime,strLocalTime,0-24*60*60,"yyyy-mm-dd hh24:mi:ss");
  printf("%s\n",strLocalTime);

  return 0;
}

