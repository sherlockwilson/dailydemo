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
	// ͨ���ֶ�����ȡ�ַ���
	virtual std::string GetString(const std::string& strFieldName);

	// ͨ���±����ȡ�ַ���
	virtual std::string GetString(int nIndex);

protected:
	// ͨ���ֶ�����ȡ��Ӧ���±꣬��ȡʧ�ܷ���-1(PQfnumberȡ����ʱĬ�Ϸ���-1)
	virtual int GetFieldIndex(const std::string& strFieldName);
	// ������±��Ƿ�Խ��
	virtual bool CheckIndex(int nIndex);
	virtual bool Attach(PGresult* lpRes);
	virtual void Detach();

	bool    m_bEOF;                                   //�Ƿ��Ѿ��ǵ����ݼ�ĩβ
	PGconn* m_struConn;
	char* m_strRes;
	PGresult* m_struResSet;

	int m_nCurrentRow;               //��ǰ��
	int m_nRowCount;                 //������
	int m_nColCount;                 //�ֶθ���
};

