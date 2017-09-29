#pragma once

#include <libpq-fe.h>

typedef struct
{
	char szHost[128];
	int nHostLen;
	char szPort[128];
	int nPortLen;
	char szOptions[128];
	int nOptionsLen;
	char szTty[128];
	int nTtyLen;
	char szDbName[128];
	int nDbNameLen;
	char szUser[128];
	int nUserLen;
	char szPwd[128];
	int nPwdLen;
} LOG_INFO,*LPLOG_INFO;

class CPostgreSqlBase
{
public:
	CPostgreSqlBase();
	virtual ~CPostgreSqlBase();
	virtual bool Open(LOG_INFO struLogInfo);
	virtual bool Open(const char *strHost, const char *strPort,
		const char *strOptions, const char *strTty,
		const char *strDbName, const char *strUser, const char *strPwd);
	virtual void Close();
	virtual int SetClientEncoding(const char *strEncoding);
	virtual bool Execute(const char* strSql);
	virtual bool ExecuteQuery(const char* strSql);
	virtual bool Commit();
	virtual bool RollBack();
	virtual bool First();
	virtual bool Next();
	virtual bool IsEOF();
	// 通过字段名获取字符串
	virtual std::string GetString(const std::string& strFieldName);

	// 通过下标你获取字符串
	virtual std::string GetString(int nIndex);

protected:
	// 通过字段名获取对应的下标，获取失败返回-1(PQfnumber取不到时默认返回-1)
	virtual int GetFieldIndex(const std::string& strFieldName);
	// 检查列下标是否越界
	virtual bool CheckIndex(int nIndex);
	virtual bool Attach(PGresult* lpRes);
	virtual void Detach();

	bool    m_bEOF;                                   //是否已经是到数据集末尾
	PGconn* m_struConn;
	char* m_strRes;
	PGresult* m_struResSet;

	int m_nCurrentRow;               //当前行
	int m_nRowCount;                 //总行数
	int m_nColCount;                 //字段个数
};

