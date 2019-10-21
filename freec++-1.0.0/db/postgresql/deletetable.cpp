//
// 本程序演示删除商品表中数据
//

#include "_postgresql.h"

int main(int argc,char *argv[])
{
  // 数据库连接池
  connection conn;
  
  // 连接数据库，返回值0-成功，其它-失败
  // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
  if (conn.connecttodb("host=10.151.64.150 user=postgres password=oracle dbname=gxpt port=5432","gbk")!=0)
  {
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }

  // SQL语言操作类
  sqlstatement stmt(&conn);

  int iminid,imaxid;

  // 准备删除数据的SQL，不需要判断返回值
  stmt.prepare("delete from goods where id>:1 and id<:2");
  // 为SQL语句绑定输入变量的地址
  stmt.bindin(1,&iminid);
  stmt.bindin(2,&imaxid);

  // 手工指定id的范围为1到5
  iminid=1;
  imaxid=5;

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 请注意，stmt.m_cda.rpc变量非常重要，它保存了SQL被执行后影响的记录数。
  printf("本次从goods表中删除了%ld条记录。\n",stmt.m_cda.rpc); 

  // 提交事务
  conn.commitwork();

  return 0;
}

