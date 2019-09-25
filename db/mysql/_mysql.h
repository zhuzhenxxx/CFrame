
#ifndef __MYSQL_H
#define __MYSQL_H

// C/C++库常用头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* mysql头文件 */
#include <mysql.h>

// mysql登录环境
struct LOGINENV
{
  char ip[32];
  char user[32];     // 数据库的用户名
  char pass[32];     // 数据库的密码
  char dbname[51];  
  int  port;
};

// mysql执行的结果
struct CDA_DEF
{
  int      rc;             // 执行结果
  unsigned long rpc;       // 执行SQL语句后，影响记录的行数
  char     message[2048];  // 执行SQL语句如果失败，存放错误信息
};

/* 数据库连接池类 */
class connection
{
public:
  MYSQL     *m_conn;

  // 连接状态，0-未连接，1-已连接
  int m_state;

  // 服务器环境句柄
  LOGINENV m_env;

  // 自动提交标志，0-关闭自动提交；1-开启自动提交
  int m_autocommitopt; 

  // 数据库种类，固定取值为mysql
  char m_dbtype[21];

  connection();

 ~connection();

  // 连接数据库，
  // connstr的格式为：ip,username,password,dbname,port
  // autocommitopt：0-关闭自动提交，1-启用自动提交
  int connecttodb(char *connstr,char *charset,unsigned int autocommitopt=0);

  // 从connstr中解析ip,username,password,dbname,port
  void setdbopt(char *connstr);

  // 断开与数据库的连接
  int  disconnect();

  // 设置字符集，要与数据库的一致，否则中文会出现乱码
  void character(char *charset);

  // 提交事务
  int  commit(); 

  // 回滚事务
  int  rollback();

  // 获取错误信息
  void err_report();

  // 用于存放connection操作数据库的错误或最后一次执行SQL语句的结果
  CDA_DEF m_cda;
};

// 定义SQL语句中，输入和输出字段个数的最大值，256是很大的了，可以根据实际情况调整
#define MAXPARAMS  256

/* SQL语言数据操作类 */
class sqlstatement
{
public:
  // 与数据库连接池的状态，0-未绑定，1-已绑定
  int m_state;

  // 语句句柄
  MYSQL_STMT *m_handle;
  
  // 输入参数
  MYSQL_BIND params_in[MAXPARAMS];
  // 输出参数
  MYSQL_BIND params_out[MAXPARAMS];

  connection *m_conn;

  // SQL语句
  char m_sql[10240];

  // 执行结果
  CDA_DEF m_cda;

  int m_sqltype;  // 待执行的SQL语句的类型，0-查询语句；1-非查询语句

  // 自动提交标志，0-关闭自动提交；1-开启自动提交
  int m_autocommitopt; 

  sqlstatement();
  sqlstatement(connection *conn);

 ~sqlstatement();

  // 设定数据库连接池
  int  connect(connection *conn); 

  // 断开与数据库连接池的连接
  int  disconnect();

  // 分析SQL语句,支持存储过程,采用立刻分析的方式,即时返回分析结果
  int  prepare(const char *fmt,...);

  // 绑定输入变量的地址
  int  bindin(unsigned int position,int    *value);
  int  bindin(unsigned int position,long   *value);
  int  bindin(unsigned int position,unsigned int  *value);
  int  bindin(unsigned int position,unsigned long *value);
  int  bindin(unsigned int position,float *value);
  int  bindin(unsigned int position,double *value);
  int  bindin(unsigned int position,char   *value,unsigned int len);

  // 绑定输出变量的地址
  int  bindout(unsigned int position,int    *value);
  int  bindout(unsigned int position,long   *value);
  int  bindout(unsigned int position,unsigned int  *value);
  int  bindout(unsigned int position,unsigned long *value);
  int  bindout(unsigned int position,float *value);
  int  bindout(unsigned int position,double *value);
  int  bindout(unsigned int position,char   *value,unsigned int len);

  // 执行SQL语句
  int  execute();

  // 如果SQL语句不需要输入和输出变量，可以直接执行。
  int  execute(const char *fmt,...);

  // 取下一条记录,只有当SQL语句是查询语句时才有意义 
  int  next();

  // 错误报告
  void err_report();
};

#endif
