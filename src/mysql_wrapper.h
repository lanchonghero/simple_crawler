#ifndef _MYSQL_WRAPPER_H
#define _MYSQL_WRAPPER_H

#include <mysql/mysql.h>
#include <string>
#include <vector>

struct MysqlField {
  std::string name;
  std::string value;
};

struct MysqlParams {
  std::string               m_sQuery;
  std::vector<std::string>  m_vQueryPre;
  std::vector<std::string>  m_vQueryPost;
  bool                      m_bPrintQueries;

  std::string   m_sUsock;    ///< UNIX socket
  int           m_iFlags;    ///< connection flags
  std::string   m_sSslKey;
  std::string   m_sSslCert;
  std::string   m_sSslCA;

  // connection params
  std::string   m_sHost;
  std::string   m_sUser;
  std::string   m_sPass;
  std::string   m_sDB;
  std::string   m_sDftTable;
  int           m_iPort;

  MysqlParams():m_bPrintQueries(false),m_iPort(3306) {}
};

class MysqlWrapper
{
public:
  MysqlWrapper(const MysqlParams& tParams, bool initConnect=false);
  ~MysqlWrapper();

  virtual void        SqlDismissResult();
  virtual bool        SqlQuery();
  virtual bool        SqlQuery(const char* query);
  virtual bool        SqlIsError();
  virtual int         SqlErrno();
  virtual const std::string  SqlError();
  virtual bool        SqlConnect();
  virtual void        SqlDisconnect();
  virtual int         SqlNumFields();
  virtual bool        SqlFetchRow(std::vector<MysqlField>& vfs);
  virtual bool        SqlFetchRow();
  virtual unsigned long    SqlColumnLength(int index);
  virtual const char*      SqlColumn(int index);
  virtual const char*      SqlFieldName(int index);
  virtual const MysqlParams  GetParams() const;
  virtual bool        SqlRealEscapeString(const std::string& in, std::string& out);

private:
  MYSQL_RES*        m_pMysqlResult;
  MYSQL_FIELD*      m_pMysqlFields;
  MYSQL_ROW         m_tMysqlRow;
  MYSQL             m_tMysqlDriver;
  unsigned long*    m_pMysqlLengths;
  MysqlParams       m_tParams;
  std::string       m_sSqlDSN;
  bool              m_bSqlConnected;
};

#endif //_MYSQL_WRAPPER_H
