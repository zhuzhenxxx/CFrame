//
// 本程序演示向商品表中插入10条记录。
//

#include "_mysql.h"

// 定义用于操作数据的结构，与表中的字段对应
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
  if (conn.connecttodb("127.0.0.1,test,testpwd,testdb,3306","gbk") != 0)
  {
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }
  
  // SQL语言操作类
  sqlstatement stmt(&conn);
  
  // 准备插入数据的SQL，不需要判断返回值
  stmt.prepare("\
    insert into goods(id,name,sal,btime) \
               values(:1,:2,:3,str_to_date(:4,'%%Y-%%m-%%d %%h:%%i:%%s'))");
  // 为SQL语句绑定输入变量的地址
  stmt.bindin(1,&stgoods.id);
  stmt.bindin(2, stgoods.name,30);
  stmt.bindin(3,&stgoods.sal);
  stmt.bindin(4, stgoods.btime,19);

  // 模拟商品数据，向表中插入10条测试信息
  for (int ii=1;ii<=10;ii++)
  {
    // 结构体变量初始化
    memset(&stgoods,0,sizeof(stgoods));

    // 为结构体的变量指定值
    stgoods.id=ii;
    sprintf(stgoods.name,"商品名称%02d",ii);
    stgoods.sal=ii*2.11;
    strcpy(stgoods.btime,"2018-03-01 12:25:31");

    // 每次指定变量的值后，执行SQL语句，一定要判断返回值，0-成功，其它-失败。
    if (stmt.execute() != 0)
    {
      printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
    }

    printf("insert ok(id=%d).\n",ii);
  }

  printf("insert table goods ok.\n");

  // 提交数据库事务
  conn.commit();

  return 0;
}

