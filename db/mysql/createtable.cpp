//
// 本程序演示创建一个表，用于存放商品信息。
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

  // 准备创建表的SQL，商品表：商品编号id，商品名称name，价格sal
  // 入库时间btime，商品说明memo，商品图片pic
  // prepare方法不需要判断返回值
  stmt.prepare("\
    create table goods(id    bigint(10),\
                       name  varchar(30),\
                       sal   decimal(8,2),\
                       btime datetime,\
                       memo  longtext,\
                       pic   longblob,\
                       primary key (id))");

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  printf("create table goods ok.\n");

  return 0;
}

