//
// 本程序演示如何把Oracle的BLOB字段的内容提取到磁盘文件中。
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

  // 不需要判断返回值
  stmt.prepare("select pic from goods where id=1");
  stmt.bindblob();

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 获取一条记录，一定要判断返回值，0-成功，1403-无记录，其它-失败。
  if (stmt.next() != 0) return 0;

  // 把BLOB字段中的内容写入磁盘文件，一定要判断返回值，0-成功，其它-失败。
  if (stmt.lobtofile((char *)"pic_out.jpg") != 0)
  {
    printf("stmt.lobtofile() failed.\n%s\n",stmt.m_cda.message); return -1;
  }

  return 0;
}

