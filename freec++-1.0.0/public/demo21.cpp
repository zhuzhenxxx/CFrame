#include "_public.h"

pthread_t pthid1,pthid2;

// 第一个线程的主函数
void *pth1_main(void *arg);

// 第二个线程的主函数
void *pth2_main(void *arg);

int ii=10;

int main()
{
  // 创建线程一
  if ( pthread_create(&pthid1,NULL,pth1_main,(void*)3) != 0)
  {
    printf("pthread_create pthid1 failed.\n"); return -1;
  }
  
  // 创建线程二
  if ( pthread_create(&pthid2,NULL,pth2_main,(void*)4) != 0)
  {
    printf("pthread_create pthid2 failed.\n"); return -1;
  }

  pthread_join(pthid1,NULL);
  pthread_join(pthid2,NULL);
  
  return 0;
}

// 第一个线程的主函数
void *pth1_main(void *arg)
{
  long jj = (long)arg;
  printf("这是第一个线程，jj=%ld\n",jj);

  printf("1这是第一个线程，ii=%d\n",ii);
  ii=20;
  sleep(10);
  printf("2这是第一个线程，ii=%d\n",ii);

  pthread_exit(NULL);
}

// 第二个线程的主函数
void *pth2_main(void *arg)
{
  long jj = (long)arg;
  printf("这是第二个线程，jj=%ld\n",jj);

  printf("1这是第二个线程，ii=%d\n",ii);
  sleep(20);
  printf("2这是第二个线程，ii=%d\n",ii);

  pthread_exit(NULL);
}

