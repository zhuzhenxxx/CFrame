//
// 本程序演示更新商品表中数据
//

#include "_mysql.h"

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

  int iminid,imaxid;
  char strbtime[20];

  // 准备更新数据的SQL，不需要判断返回值
  stmt.prepare("\
    update goods set btime=str_to_date(:1,'%%Y-%%m-%%d %%h:%%i:%%s') where id>:2 and id<:3");
  // 为SQL语句绑定输入变量的地址
  stmt.bindin(1, strbtime,19);
  stmt.bindin(2,&iminid);
  stmt.bindin(3,&imaxid);

  // 手工指定id的范围为1到5，btime为2017-12-20 09:45:30，执行一次更新
  iminid=1;
  imaxid=5;
  memset(strbtime,0,sizeof(strbtime));
  strcpy(strbtime,"2019-08-10 09:45:30");

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 请注意，stmt.m_cda.rpc变量非常重要，它保存了SQL被执行后影响的记录数。
  printf("本次更新了goods表%ld条记录。\n",stmt.m_cda.rpc);

  // 提交事务
  conn.commit();

  return 0;
}

