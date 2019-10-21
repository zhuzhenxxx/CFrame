//
// 本程序演示创建一个表，用于存放商品信息。
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

  // 准备创建表的SQL，商品表：商品编号id，商品名称name，价格sal
  // 入库时间btime，商品说明memo，商品图片pic
  // prepare方法不需要判断返回值
  stmt.prepare("\
    create table goods(id    int,\
                       name  varchar(30),\
                       sal   numeric(8,2),\
                       btime timestamp,\
                       memo  text,\
                       pic   bytea,\
                       primary key (id))");
  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 提交事务，注意，在postgresql数据库中，创建表也要提交事务，但删除表却不用，奇怪
  conn.commitwork();

  printf("create table goods ok.\n");

  return 0;
}

