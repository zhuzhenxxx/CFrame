//
// 本程序演示如何执行一个PL/SQL过程
// 在这里说一下我个人的意见，我从不在Oracle数据库中创建PL/SQL过程，也很少使用触发器，原因如下：
// 1、在Oracle数据库中创建PL/SQL过程，程序的调试很麻烦；
// 2、维护工作很麻烦，因为程序员要花时间去了解数据库中的存储过程；
// 3、在我封装的oci中，对oracle的操作已经是非常简单的事情，没必要去折腾存储过程；
//
// 还有，我在oci中也很少用PL/SQL语句，也是因为复杂的PL/SQL调试麻烦。
//
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
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }
  
  // SQL语言操作类
  sqlstatement stmt(&conn);

  // 准备删除记录的PL/SQL，不需要判断返回值
  // 本PL/SQL先删除goods表中的全部记录，再插入一条记录
  stmt.prepare("\
    BEGIN\
      delete from goods;\
      insert into goods(id,name,sal,btime)\
                 values(100,'过程商品',55.65,to_date('2018-01-02 13:00:55','yyyy-mm-dd hh24:mi:ss'));\
    END;");
  // 注意，PL/SQL中的每条SQL需要用分号结束，END之后还有一个分号。

  
  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  printf("exec PL/SQL ok.\n");

  // 提交事务
  conn.commit();

  return 0;
}

