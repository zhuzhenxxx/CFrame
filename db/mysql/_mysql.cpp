////////////////////////////////////////////////////////
// 本软件由神农开源网提供，开源软件需要大家的努力。
// 技术交流请加QQ群701117364，我们一起把开源软件做得更好。
//
// 软件官方遵守协议约定
//
// 1) 禁止用于技术培训、出版等商业用途。
// 2) 禁止任何侵权行为，包括软件的源码程序、资料文档、文件信息。
// 3) 禁止软件产品插入与软件官方无关的任何广告信息，包括文字、图像、媒体信息。
// 4) 禁止软件产品内置与软件官方程序应用无关的任何第三方插件程序、组件。
// 5) 禁止软件程序存在任何不经过用户许可的远程响应执行的控制程序。
// 6) 禁止软件程序使用任何手段收集用户隐私信息与用户储存的数据。
//
//
//  用户遵守协议约定
//  1) 禁止用户修改软件官方任何的版权说明信息，包括程序的版权协议说明、软件官方链接、文字说明、图像标志、媒体信息。
//  2) 禁止用户通过任何方式破坏、侵犯软件官方的正常发展运营，包括软件官方程序的正常运行与商业授权机制。
//
////////////////////////////////////////////////////////

#include "_mysql.h"

connection::connection()
{ 
  m_conn = NULL;

  m_state = 0; 

  memset(&m_env,0,sizeof(LOGINENV));

  memset(&m_cda,0,sizeof(m_cda));

  m_cda.rc=-1;
  strncpy(m_cda.message,"database not open.",128);

  // 数据库种类
  memset(m_dbtype,0,sizeof(m_dbtype));
  strcpy(m_dbtype,"mysql");
}

connection::~connection()
{
  disconnect();
}

// 从connstr中解析username,password,tnsname
// "120.77.115.3","szidc","SZmb1601","lxqx",3306
void connection::setdbopt(char *connstr)
{
  memset(&m_env,0,sizeof(LOGINENV));

  char *bpos,*epos;

  bpos=epos=0;

  // ip
  bpos=connstr;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.ip,bpos,epos-bpos); 
  }else return;

  // user
  bpos=epos+1;
  epos=0;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.user,bpos,epos-bpos); 
  }else return;

  // pass
  bpos=epos+1;
  epos=0;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.pass,bpos,epos-bpos); 
  }else return;

  // dbname
  bpos=epos+1;
  epos=0;
  epos=strstr(bpos,",");
  if (epos > 0) 
  {
    strncpy(m_env.dbname,bpos,epos-bpos); 
  }else return;

  // port
  m_env.port=atoi(epos+1);
}

int connection::connecttodb(char *connstr,char *charset,unsigned int autocommitopt)
{
  // 如果已连接上数据库，就不再连接
  // 所以，如果想重连数据库，必须显示的调用disconnect()方法后才能重连
  if (m_state == 1) return 0;

  // 从connstr中解析username,password,tnsname
  setdbopt(connstr);

  memset(&m_cda,0,sizeof(m_cda));

  if ( (m_conn = mysql_init(NULL)) == NULL )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"initialize mysql failed.\n",128); return -1;
  }

  if ( mysql_real_connect(m_conn,m_env.ip,m_env.user,m_env.pass,m_env.dbname,m_env.port, NULL, 0 ) == NULL )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  // 设置事务模式，0-关闭自动提交，1-开启自动提交
  m_autocommitopt=autocommitopt;

  if ( mysql_autocommit(m_conn, m_autocommitopt ) != 0 )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  // 设置字符集
  character(charset);

  m_state = 1;

  return 0;
}

// 设置字符集，要与数据库的一致，否则中文会出现乱码
void connection::character(char *charset)
{
  mysql_set_character_set(m_conn,charset);

  return;
}

int connection::disconnect()
{
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return -1;
  }

  rollback();

  mysql_close(m_conn); 

  m_conn=NULL;

  m_state = 0;    

  return 0;
}

int connection::rollback()
{ 
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return -1;
  }

  if ( mysql_rollback(m_conn ) != 0 )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  return 0;    
}

int connection::commit()
{ 
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return -1;
  }

  if ( mysql_commit(m_conn ) != 0 )
  {
    m_cda.rc=mysql_errno(m_conn); strncpy(m_cda.message,mysql_error(m_conn),2000); mysql_close(m_conn); m_conn=NULL;  return -1;
  }

  return 0;
}

void connection::err_report()
{
  if (m_state == 0) 
  { 
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.",128); return;
  }

  memset(&m_cda,0,sizeof(m_cda));

  m_cda.rc=-1;
  strncpy(m_cda.message,"call err_report failed.",128);

  m_cda.rc=mysql_errno(m_conn);

  strncpy(m_cda.message,mysql_error(m_conn),1024);

  return;
}

sqlstatement::sqlstatement()
{
  m_state=0;

  m_handle=NULL;

  memset(&m_cda,0,sizeof(m_cda));

  memset(m_sql,0,sizeof(m_sql));

  m_cda.rc=-1;
  strncpy(m_cda.message,"sqlstatement not connect to connection.\n",128);
}

sqlstatement::sqlstatement(connection *conn)
{
  sqlstatement();

  connect(conn);
}

sqlstatement::~sqlstatement()
{
  disconnect();
}

int sqlstatement::connect(connection *conn)
{
  // 注意，一个sqlstatement在程序中只能连一个connection，不允许连多个connection
  // 所以，只要这个光标已打开，就不允许再次打开，直接返回成功
  if ( m_state == 1 ) return 0;
  
  memset(&m_cda,0,sizeof(m_cda));

  m_conn=conn;

  // 如果数据库连接类的指针为空，直接返回失败
  if (m_conn == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.\n",128); return -1;
  }
  
  // 如果数据库没有连接好，直接返回失败
  if (m_conn->m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"database not open.\n",128); return -1;
  }

  if ( (m_handle=mysql_stmt_init(m_conn->m_conn)) == NULL)
  {
    err_report(); return m_cda.rc;
  }

  m_state = 1;  // 光标成功打开

  m_autocommitopt=m_conn->m_autocommitopt;

  return 0;
}

int sqlstatement::disconnect()
{
  if (m_state == 0) return 0;

  memset(&m_cda,0,sizeof(m_cda));

  mysql_stmt_close(m_handle);

  m_state=0;

  m_handle=NULL;

  memset(&m_cda,0,sizeof(m_cda));

  memset(m_sql,0,sizeof(m_sql));

  m_cda.rc=-1;
  strncpy(m_cda.message,"cursor not open.",128);

  return 0;
}

void sqlstatement::err_report()
{
  // 注意，在该函数中，不可随意用memset(&m_cda,0,sizeof(m_cda))，否则会清空m_cda.rpc的内容
  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return;
  }
  
  memset(&m_conn->m_cda,0,sizeof(m_conn->m_cda));

  m_cda.rc=-1;
  strncpy(m_cda.message,"call err_report() failed.\n",128);

  m_cda.rc=mysql_stmt_errno(m_handle);

  snprintf(m_cda.message,2000,"%d,%s",m_cda.rc,mysql_stmt_error(m_handle));

  m_conn->err_report();

  return;
}
  
int sqlstatement::prepare(const char *fmt,...)
{
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return -1;
  }

  memset(m_sql,0,sizeof(m_sql));

  va_list ap;
  va_start(ap,fmt);
  vsnprintf(m_sql,10000,fmt,ap);

  int ilen=strlen(m_sql);

  // 为了和freeoci兼容，把:1,:2,:3...等替换成?
  for (int ii=0;ii<ilen;ii++)
  {
    if ( (m_sql[ii]==':') && (isdigit(m_sql[ii+1])!=0) )
    {
      m_sql[ii]='?';
      m_sql[ii+1]=' ';
      if (isdigit(m_sql[ii+2])!=0) m_sql[ii+2]=' ';
      if (isdigit(m_sql[ii+3])!=0) m_sql[ii+3]=' ';
    }
  }

  if (mysql_stmt_prepare(m_handle,m_sql,strlen(m_sql)) != 0)
  {
    err_report(); return m_cda.rc;
  }

  // 判断是否是查询语句，如果是，把m_sqltype设为0，其它语句设为1。
  m_sqltype=1;
  
  // 从待执行的SQL语句中截取15个字符，如果这15个字符中包括了“select”，就认为是查询语句
  char strtemp[16]; memset(strtemp,0,sizeof(strtemp)); strncpy(strtemp,m_sql,15);

  if ( (strstr(strtemp,"select") > 0) || (strstr(strtemp,"Select") > 0) || (strstr(strtemp,"SELECT") > 0) ) m_sqltype=0;

  memset(params_in,0,sizeof(params_in));

  memset(params_out,0,sizeof(params_out));

  return 0;
}
  
int sqlstatement::bindin(unsigned int position,int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_in[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindin(unsigned int position,long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_in[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindin(unsigned int position,unsigned int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_in[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindin(unsigned int position,unsigned long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_in[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindin(unsigned int position,char *value,unsigned int len)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_VAR_STRING;
  params_in[position-1].buffer = value;
  params_in[position-1].buffer_length = len;

  return 0;
}

int sqlstatement::bindin(unsigned int position,float *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_FLOAT;
  params_in[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindin(unsigned int position,double *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->param_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_in[position-1].buffer_type = MYSQL_TYPE_DOUBLE;
  params_in[position-1].buffer = value;

  return 0;
}

///////////////////
int sqlstatement::bindout(unsigned int position,int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,unsigned int *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,unsigned long *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_LONGLONG;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,char *value,unsigned int len)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_VAR_STRING;
  params_out[position-1].buffer = value;
  params_out[position-1].buffer_length = len;

  return 0;
}

int sqlstatement::bindout(unsigned int position,float *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_FLOAT;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::bindout(unsigned int position,double *value)
{
  if ( (position<1) || (position>=MAXPARAMS) || (position>m_handle->field_count) )
  {
    m_cda.rc=-1; strncpy(m_cda.message,"array bound.",128);
  }

  params_out[position-1].buffer_type = MYSQL_TYPE_DOUBLE;
  params_out[position-1].buffer = value;

  return 0;
}

int sqlstatement::execute()
{
  memset(&m_cda,0,sizeof(m_cda));

  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return -1;
  }

  if ( (m_handle->param_count>0) && (m_handle->bind_param_done == 0))
  {
    if (mysql_stmt_bind_param(m_handle,params_in) != 0)
    {
      err_report(); return m_cda.rc;
    }
  }


  if ( (m_handle->field_count>0) && (m_handle->bind_result_done == 0) )
  {
    if (mysql_stmt_bind_result(m_handle,params_out) != 0)
    {
      err_report(); return m_cda.rc;
    }
  }

  if (mysql_stmt_execute(m_handle) != 0)
  {
    err_report(); return m_cda.rc;
  }
  
  // 如果不是查询语句，就获取影响记录的行数
  if (m_sqltype == 1) m_cda.rpc=m_handle->affected_rows;
    
  return 0;
}

int sqlstatement::execute(const char *fmt,...)
{
  char strtmpsql[10240];
  memset(strtmpsql,0,sizeof(strtmpsql));

  va_list ap;
  va_start(ap,fmt);
  vsnprintf(strtmpsql,10000,fmt,ap);

  if (prepare(strtmpsql) != 0) return m_cda.rc;

  return execute();
}

int sqlstatement::next()
{
  // 注意，在该函数中，不可随意用memset(&m_cda,0,sizeof(m_cda))，否则会清空m_cda.rpc的内容
  if (m_state == 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"cursor not open.\n",128); return -1;
  }
  
  // 如果语句未执行成功，直接返回失败。
  if (m_cda.rc != 0) return m_cda.rc;
  
  // 判断是否是查询语句，如果不是，直接返回错误
  if (m_sqltype != 0)
  {
    m_cda.rc=-1; strncpy(m_cda.message,"no recordset found.\n",128); return -1;
  }
  
  int ret=mysql_stmt_fetch(m_handle);

  if (ret==0) 
  { 
    m_cda.rpc++; return 0; 
  }
 
  if (ret==1) 
  {
    err_report(); return m_cda.rc;
  }

  if (ret==MYSQL_NO_DATA) return MYSQL_NO_DATA;

  if (ret==MYSQL_DATA_TRUNCATED) 
  {
    m_cda.rpc++; return 0;
  }
  
  return 0;
}
