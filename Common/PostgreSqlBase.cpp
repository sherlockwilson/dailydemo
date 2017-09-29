#include "stdafx.h"
#include "PostgreSqlBase.h"


CPostgreSqlBase::CPostgreSqlBase()
{
	m_struConn = NULL;
	m_struResSet = NULL;
	m_struResSet = NULL;
	m_nCurrentRow = 0;
	m_nRowCount = 0;
}


CPostgreSqlBase::~CPostgreSqlBase()
{
	Detach();
}


bool CPostgreSqlBase::Attach(PGresult* lpRes)
{
	Detach();
	m_struResSet = lpRes;
	m_nColCount = PQnfields(m_struResSet);
	m_nRowCount = PQntuples(m_struResSet);
	m_bEOF = (m_nCurrentRow >= m_nRowCount);
	return true;
}

void CPostgreSqlBase::Detach()
{
	if (m_struResSet != NULL)
	{
		PQclear(m_struResSet);
		m_struResSet = NULL;
	}
	m_nCurrentRow = 0;
	m_nRowCount = 0;
	m_nColCount = 0;
	m_bEOF = true;
}

// ָ���һ������
bool CPostgreSqlBase::First()
{

	m_nCurrentRow = 0;
	m_bEOF = (m_nCurrentRow >= m_nRowCount);
	return !IsEOF();

}

// ָ����һ������
bool CPostgreSqlBase::Next()
{
	++m_nCurrentRow;
	m_bEOF = (m_nCurrentRow >= m_nRowCount);
	return !IsEOF();
}

// ���ݼ��Ƿ񵽵�
bool CPostgreSqlBase::IsEOF()
{
	return m_bEOF;
}

bool CPostgreSqlBase::Open(LOG_INFO struLogInfo)
{
	char szHost[128] = {0};
	memcpy_s(szHost,sizeof(szHost)/sizeof(char),struLogInfo.szHost,struLogInfo.nHostLen);
	char szPort[128] = {0};
	memcpy_s(szPort, sizeof(szPort) / sizeof(char), struLogInfo.szPort, struLogInfo.nPortLen);
	char szOptions[128] = {0};
	memcpy_s(szOptions, sizeof(szOptions) / sizeof(char), struLogInfo.szOptions, struLogInfo.nOptionsLen);
	char szTty[128] = {0};
	memcpy_s(szTty, sizeof(szTty) / sizeof(char), struLogInfo.szTty, struLogInfo.nTtyLen);
	char szDbName[128] = {0};
	memcpy_s(szDbName, sizeof(szDbName) / sizeof(char), struLogInfo.szDbName, struLogInfo.nDbNameLen);
	char szUser[128] = {0};
	memcpy_s(szUser,sizeof(szUser)/sizeof(char),struLogInfo.szUser,struLogInfo.nUserLen);
	char szPwd[128] = {0};
	memcpy_s(szPwd,sizeof(szPwd)/sizeof(char),struLogInfo.szPwd,struLogInfo.nPwdLen);

	m_struConn = PQsetdbLogin(szHost, szPort, szOptions, szTty, szDbName, szUser, szPwd);
	if (PQstatus(m_struConn) == CONNECTION_BAD) // or conn==NULL 
	{
		return false;
	}
	return true;
}

bool CPostgreSqlBase::Open(const char *strHost, const char *strPort,
	const char *strOptions, const char *strTty,const char *strDbName, 
	const char *strUser, const char *strPwd)
{
	m_struConn = PQsetdbLogin(strHost, strPort, strOptions, strTty, strDbName, strUser, strPwd);
	if (PQstatus(m_struConn) == CONNECTION_BAD) // or conn==NULL 
	{
		return false;
	}
	return true;
}


int CPostgreSqlBase::SetClientEncoding(const char *strEncoding)
{
	return PQsetClientEncoding(m_struConn, strEncoding);
}

// ִ��SQL��䣬�޷������ݼ��ĵ��ú���
bool CPostgreSqlBase::Execute(const char* strSql)
{
	assert(strSql != "");

	PGresult *ptr_res = PQexec(m_struConn, strSql);
	int status = PQresultStatus(ptr_res);
	if (status != PGRES_COMMAND_OK)
	{
		PQclear(ptr_res);
		return false;
	}
	PQclear(ptr_res);
	return true;
}

// ִ��SQL��䲢�������ݼ�(���ݼ�����)
bool CPostgreSqlBase::ExecuteQuery(const char* strSql)
{
	//-------------------------------------------
	assert(strSql != "");
	Detach();

	PGresult* ptr_res = PQexec(m_struConn, strSql);
	int status = PQresultStatus(ptr_res);
	if (status != PGRES_TUPLES_OK)
	{
		PQclear(ptr_res);
		return false;
	}
	Attach(ptr_res);
	return true;
}

bool CPostgreSqlBase::Commit()
{
	return Execute("commit;");
}

bool CPostgreSqlBase::RollBack()
{
	return Execute("rollback;");
}

// ͨ���ֶ�����ȡ��Ӧ���±꣬��ȡʧ�ܷ���-1(PQfnumberȡ����ʱĬ�Ϸ���-1)
int CPostgreSqlBase::GetFieldIndex(const std::string& strFieldName)
{
	return PQfnumber(m_struResSet, strFieldName.c_str());
}

// ͨ���ֶ�����ȡ�ַ���
std::string CPostgreSqlBase::GetString(const std::string& strFieldName)
{
	const int index = GetFieldIndex(strFieldName);
	return GetString(index);
}

// ͨ���±����ȡ�ַ���
std::string CPostgreSqlBase::GetString(int nIndex)
{
	if (!CheckIndex(nIndex))
	{
		return "";
	}

	const char* ptr_v = PQgetvalue(m_struResSet, m_nCurrentRow, nIndex);
	return ptr_v == NULL ? "" : ptr_v;
}

// ������±��Ƿ�Խ��
bool CPostgreSqlBase::CheckIndex(int nIndex)
{
	return (nIndex >= 0) && (nIndex < m_nColCount);
}

//�ر����ݿ�����
void CPostgreSqlBase::Close()
{
	Detach();
	if (m_struConn != NULL)
	{
		PQfinish(m_struConn);
		m_struConn = NULL;
	}
}