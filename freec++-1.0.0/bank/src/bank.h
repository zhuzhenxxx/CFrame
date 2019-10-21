#include "_public.h"
#include "_ooci.h"

struct st_bank
{
  int    msgid;         // 消息类型
  char   oper_id[31];   // 操作员编号
  char   password[31];  // 操作员密码
  char   userid[21];    // 客户银行帐号
  char   cardid[19];    // 客户身份证号码
  char   username[31];  // 客户姓名
  char   tel[31];       // 电话
  double ye;            // 帐户余额
  int    opertype;      // 业务类型
  double je;            // 发生金额
  int    rsts;          // 状态
  char   crttime[20];

  char   userid1[21];    // 客户银行帐号，转入帐号
};

// 操作员登录
int msgid_1(connection *conn,struct st_bank *stbank,CLogFile *logfile);


// 开户业务
int msgid_2(connection *conn,struct st_bank *stbank,CLogFile *logfile);

// 客户身份认证
int msgid_3(connection *conn,struct st_bank *stbank,CLogFile *logfile);

// 存款业务
int msgid_4(connection *conn,struct st_bank *stbank,CLogFile *logfile);

// 取款业务
int msgid_5(connection *conn,struct st_bank *stbank,CLogFile *logfile);

// 转帐业务，0-成功；-1-余额不足；-2-转入帐户非法；>0-系统故障
int msgid_6(connection *conn,struct st_bank *stbank,CLogFile *logfile);

// 查余额
int msgid_7(connection *conn,struct st_bank *stbank,CLogFile *logfile);

// 查流水
int msgid_8(connection *conn,struct st_bank *stbank,CLogFile *logfile,vector<struct st_bank> &vbank);

