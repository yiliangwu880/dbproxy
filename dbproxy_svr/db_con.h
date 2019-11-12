/*
 ��������db�ͻ���,����ɣ�����mysql�� mongodb
*/

#pragma once
#include "base_include.h"

class InnerSvrCon;
class IDbCon
{
public:
	virtual ~IDbCon() {};

	virtual bool ConnectDb(const Cfg &cfg) { return false; };

	virtual bool InitTable(const db::ReqInitTable &req, db::RspInitTable &rsp) {return false;}; //������ �����Ƿ�Ƿ�
	virtual bool Insert(const db::ReqInsertData &req, ::uint64 &num_key, std::string &str_key) { return false; };
	virtual bool Update(const db::ReqUpdateData &req, ::uint64 &num_key, std::string &str_key) { return false; };
	virtual bool Get(const db::ReqGetData &req, InnerSvrCon &con) { return false; };
	virtual bool Del(const db::ReqDelData &req, db::RspDelData &rsp) { return false; };
	virtual bool ExecuteSql(const std::string &sql_str) { return false; };

};

//��������db�ͻ���
class DbConMgr : public Singleton<DbConMgr>
{

public:
	DbConMgr() {}
	~DbConMgr();
	bool Init(const Cfg &cfg);

	IDbCon &GetCon();

private:
	IDbCon *m_con=nullptr;
};


