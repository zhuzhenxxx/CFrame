//
// 本程序演示如何把磁盘文件的文本文件写入Oracle的BLOB字段中。
//

#include "_ooci.h"

int main(int argc,char *argv[])
{
  // 数据库连接池
  connection conn;
  
  // 连接数据库，返回值0-成功，其它-失败
  // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
  if (conn.connecttodb("scott/tiger@orcl11g_193.112.167.234","Simplified Chinese_China.ZHS16GBK") != 0)
  {
    printf("connect database %s failed.\n%s\n","scott/tiger@orcl",conn.m_cda.message); return -1;
  }
  
  // SQL语言操作类
  sqlstatement stmt(&conn);
  
  // 为了方便演示，把goods表中的记录全删掉，再插入一条用于测试的数据。
  // 不需要判断返回值
  stmt.prepare("\
    BEGIN\
      delete from goods;\
      insert into goods(id,name,pic) values(1,'商品名称',empty_blob());\
    END;");
  
  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 使用游标从goods表中提取id为1的pic字段
  // 注意了，同一个sqlstatement可以多次使用
  // 但是，如果它的sql改变了，就要重新prepare和bindin或bindout变量
  stmt.prepare("select pic from goods where id=1 for update");
  stmt.bindblob();

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 获取一条记录，一定要判断返回值，0-成功，1403-无记录，其它-失败。
  if (stmt.next() != 0) return 0;
  
  // 把磁盘文件pic_in.jpg的内容写入BLOB字段，一定要判断返回值，0-成功，其它-失败。
  if (stmt.filetolob((char *)"pic_in.jpg") != 0)
  {
    printf("stmt.filetolob() failed.\n%s\n",stmt.m_cda.message); return -1;
  }

  // 提交事务
  conn.commit();

  return 0;
}

