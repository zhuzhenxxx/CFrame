#include "bank.h"

int msgid_1(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("select rsts from T_OPERATOR where oper_id=:1 and password=:2");
  stmt.bindin(1,stbank->oper_id,30);
  stmt.bindin(2,stbank->password,30);
  stmt.bindout(1,&stbank->rsts);
  stmt.execute();
  stmt.next();

  if (stbank->rsts==1) return 0;

  return -1;
}

int msgid_2(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("\
    BEGIN\
      insert into T_CUSTOMINFO(userid,password,cardid,username,tel,ye,rsts,crttime)\
             values(:1,:2,:3,:4,:5,0,1,sysdate);\
      insert into T_JYLOG(logid,oper_id,userid,opertype,je,crttime) values(SEQ_JYLOG.nextval,:6,:7,1,0,sysdate);\
    END;");
  stmt.bindin(1,stbank->userid,20);
  stmt.bindin(2,stbank->password,30);
  stmt.bindin(3,stbank->cardid,18);
  stmt.bindin(4,stbank->username,30);
  stmt.bindin(5,stbank->tel,30);
  stmt.bindin(6,stbank->oper_id,30);
  stmt.bindin(7,stbank->userid,20);

  if (stmt.execute() != 0)
  {
    logfile->Write("insert into T_CUSTOMINFO or T_JYLOG failed.\n%s\n",stmt.m_cda.message); 
  }

  return stmt.m_cda.rc;
}

int msgid_3(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("select rsts from T_CUSTOMINFO where userid=:1 and password=:2");
  stmt.bindin(1,stbank->userid,20);
  stmt.bindin(2,stbank->password,30);
  stmt.bindout(1,&stbank->rsts);
  stmt.execute();
  stmt.next();
  if (stbank->rsts==1) return 0;

  return -1;
}

int msgid_7(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("select ye from T_CUSTOMINFO where userid=:1");
  stmt.bindin(1,stbank->userid,20);
  stmt.bindout(1,&stbank->ye);
  stmt.execute();

  if (stmt.next()!=0) return -1;

  return 0;
}

int msgid_4(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("\
    BEGIN\
      update T_CUSTOMINFO set ye=ye+:1 where userid=:2;\
      insert into T_JYLOG(logid,oper_id,userid,opertype,je,crttime) values(SEQ_JYLOG.nextval,:3,:4,2,:5,sysdate);\
    END;");
  stmt.bindin(1,&stbank->je);
  stmt.bindin(2,stbank->userid,20);
  stmt.bindin(3,stbank->oper_id,30);
  stmt.bindin(4,stbank->userid,20);
  stmt.bindin(5,&stbank->je);

  if (stmt.execute() != 0)
  {
    logfile->Write("insert into T_CUSTOMINFO or T_JYLOG failed.\n%s\n",stmt.m_cda.message);
  }

  return stmt.m_cda.rc;
}

// 取款业务，0-成功；-1-余额不足；>0-系统故障
int msgid_5(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("select ye from T_CUSTOMINFO where userid=:1");
  stmt.bindin(1,stbank->userid,20);
  stmt.bindout(1,&stbank->ye);
  if (stmt.execute() != 0)
  {
    logfile->Write("select T_CUSTOMINFO failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }

  stmt.next();

  if (stbank->ye<stbank->je) return -1;

  stmt.prepare("\
    BEGIN\
      update T_CUSTOMINFO set ye=ye-:1 where userid=:2;\
      insert into T_JYLOG(logid,oper_id,userid,opertype,je,crttime) values(SEQ_JYLOG.nextval,:3,:4,3,:5,sysdate);\
    END;");
  stmt.bindin(1,&stbank->je);
  stmt.bindin(2,stbank->userid,20);
  stmt.bindin(3,stbank->oper_id,30);
  stmt.bindin(4,stbank->userid,20);
  stmt.bindin(5,&stbank->je);
  if (stmt.execute() != 0)
  {
    logfile->Write("insert into T_CUSTOMINFO or T_JYLOG failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }

  return 0;
}

// 转帐业务，0-成功；-1-余额不足；-2-转入帐户非法；>0-系统故障
int msgid_6(connection *conn,struct st_bank *stbank,CLogFile *logfile)
{
  sqlstatement stmt(conn);
  stmt.prepare("select ye from T_CUSTOMINFO where userid=:1");
  stmt.bindin(1,stbank->userid,20);
  stmt.bindout(1,&stbank->ye);
  if (stmt.execute() != 0)
  {
    logfile->Write("select T_CUSTOMINFO failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }

  stmt.next();

  // -1-余额不足
  if (stbank->ye<stbank->je) return -1;

  // 判断输入帐户是否合法
  stmt.prepare("select rsts from T_CUSTOMINFO where userid=:1 and rsts in (1,2)");
  stmt.bindin(1,stbank->userid1,20);
  stmt.bindout(1,&stbank->rsts);
  if (stmt.execute() != 0)
  {
    logfile->Write("select T_CUSTOMINFO failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }
  stmt.next();
  
  // 转入帐户非法
  if ( (stbank->rsts!=1) && (stbank->rsts!=2) ) return -2;

  // 转出帐户
  stmt.prepare("\
    BEGIN\
      update T_CUSTOMINFO set ye=ye-:1 where userid=:2;\
      insert into T_JYLOG(logid,oper_id,userid,opertype,je,crttime) values(SEQ_JYLOG.nextval,:3,:4,3,:5,sysdate);\
    END;");
  stmt.bindin(1,&stbank->je);
  stmt.bindin(2,stbank->userid,20);
  stmt.bindin(3,stbank->oper_id,30);
  stmt.bindin(4,stbank->userid,20);
  stmt.bindin(5,&stbank->je);
  if (stmt.execute() != 0)
  {
    logfile->Write("insert into T_CUSTOMINFO or T_JYLOG failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }

  // 转入帐户
  stmt.prepare("\
    BEGIN\
      update T_CUSTOMINFO set ye=ye+:1 where userid=:2;\
      insert into T_JYLOG(logid,oper_id,userid,opertype,je,crttime) values(SEQ_JYLOG.nextval,:3,:4,3,:5,sysdate);\
    END;");
  stmt.bindin(1,&stbank->je);
  stmt.bindin(2,stbank->userid1,20);
  stmt.bindin(3,stbank->oper_id,30);
  stmt.bindin(4,stbank->userid1,20);
  stmt.bindin(5,&stbank->je);
  if (stmt.execute() != 0)
  {
    logfile->Write("insert into T_CUSTOMINFO or T_JYLOG failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }

  return 0;
}

// 查流水
int msgid_8(connection *conn,struct st_bank *stbank,CLogFile *logfile,vector<struct st_bank> &vbank)
{
  struct st_bank sttmp;
  sqlstatement stmt(conn);
  stmt.prepare("select opertype,je,to_char(crttime,'yyyy-mm-dd hh24:mi:ss') from T_JYLOG where userid=:1");
  stmt.bindin(1,stbank->userid,20);
  stmt.bindout(1,&sttmp.opertype);
  stmt.bindout(2,&sttmp.je);
  stmt.bindout(3, sttmp.crttime,19);

  if (stmt.execute() != 0)
  {
    logfile->Write("select T_JYLOG failed.\n%s\n",stmt.m_cda.message); return stmt.m_cda.rc;
  }

  while (TRUE)
  {
    memset(&sttmp,0,sizeof(struct st_bank));
   
    if (stmt.next() != 0) break;

    vbank.push_back(sttmp);
  }

  return 0;
}
