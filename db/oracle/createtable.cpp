//
// 本程序演示创建一个表，用于存放商品信息。
//

#include "_ooci.h"

int main(int argc,char *argv[])
{
  // 数据库连接池
  connection conn;
  
  // 连接数据库，返回值0-成功，其它-失败
  // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
  if (conn.connecttodb("scott/tiger@orcl11g_127.0.0.1","Simplified Chinese_China.ZHS16GBK") != 0)
  {
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }
  
  // SQL语言操作类
  sqlstatement stmt(&conn);

  // 准备创建表的SQL，商品表：商品编号id，商品名称name，价格sal
  // 入库时间btime，商品说明memo，商品图片pic
  // prepare方法不需要判断返回值
  stmt.prepare("\
    create table goods(id    number(10),\
                       name  varchar2(30),\
                       sal   number(10,2),\
                       btime date,\
                       memo  clob,\
                       pic   blob,\
                       primary key (id))");

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  printf("create table goods ok.\n");

  return 0;
}

