//
// 本程序演示计时器类CTimer
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  CTimer Time;

  // 开始计时
  Time.Start();

  sleep(5);      // 睡5秒

  usleep(300);   // 睡300微秒

  double tt;

  // 计时结束，同时也是下一计时周期的开始
  tt=Time.Elapsed();

  printf("%f\n",tt);

  // 计时结束
  tt=Time.Elapsed();

  printf("%f\n",tt);

  return 0;
}

