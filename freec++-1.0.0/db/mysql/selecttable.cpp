//
// 本程序演示从商品表中查询数据
//

#include "_mysql.h"

// 定义用于查询数据的结构，与表中的字段对应
struct st_GOODS
{
  long id;          // 商品编号，用long数据类型对应mysql无小数的bigint
  char name[31];    // 商品名称，用char对应mysql的varchar，注意，表中字段的长度是30，char定义的长度是31，要留C语言的结束符
  double sal;       // 商品价格，用double数据类型对应mysql有小数的decimal
  char btime[20];   // 入库时间，用char对应mysql的datetime，格式可以在SQL语句中指定，本程序将指定为'%%Y-%%m-%%d %%h:%%i:%%s'
} stgoods;

int main(int argc,char *argv[])
{
  // 数据库连接池
  connection conn;
  
  // 连接数据库，返回值0-成功，其它-失败
  // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
  if (conn.connecttodb("193.112.167.234,test,testpwd,testdb,3306","gbk") != 0)
  {
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }
  
  // SQL语言操作类
  sqlstatement stmt(&conn);

  int iminid,imaxid;

  // 准备查询数据的SQL，不需要判断返回值
  stmt.prepare("\
    select id,name,sal,date_format(btime,'%%Y-%%m-%%d %%h:%%i:%%s')\
      from goods where id>:1 and id<:2");
  // 为SQL语句绑定输入变量的地址
  stmt.bindin(1,&iminid);
  stmt.bindin(2,&imaxid);

  // 为SQL语句绑定输出变量的地址
  stmt.bindout(1,&stgoods.id);
  stmt.bindout(2, stgoods.name,30);
  stmt.bindout(3,&stgoods.sal);
  stmt.bindout(4, stgoods.btime,19);

  // 手工指定id的范围为1到8，执行一次查询
  iminid=1;
  imaxid=8;

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  while (1)
  {
    // 先把结构体变量初始化，然后才获取记录
    memset(&stgoods,0,sizeof(stgoods));

    // 获取一条记录，一定要判断返回值，0-成功，其它-无记录
    if (stmt.next() !=0) break;
    
    // 把获取到的记录的值打印出来
    printf("id=%ld,name=%s,sal=%.02f,btime=%s\n",stgoods.id,stgoods.name,stgoods.sal,stgoods.btime);
  }

  // 请注意，stmt.m_cda.rpc变量非常重要，它保存了SQL被执行后影响的记录数。
  printf("本次查询了goods表%ld条记录。\n",stmt.m_cda.rpc);
  
  return 0;
}

