/*
 ��������db�ͻ���,����ɣ�����mysql�� mongodb
*/

#pragma once
#include "base_include.h"

class IDbCon
{
public:
	virtual ~IDbCon() {};

	virtual bool ConnectDb(const Cfg &cfg) { return false; };

	virtual bool InitTable(const db::ReqInitTable &req, db::RspInitTable &rsp) {return false;}; //������ �����Ƿ�Ƿ�
	virtual bool Insert(const db::ReqInsertData &req) { return false; };
	virtual bool Update(const db::ReqUpdateData &req) { return false; };
	virtual bool Get(const db::ReqGetData &req, db::RspGetData &rsp) { return false; };
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


