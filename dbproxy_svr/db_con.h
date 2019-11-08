/*
 ��������db�ͻ���
*/

#pragma once
#include "base_include.h"

enum class DBType
{
	MYSQL,
	MONGODB,
};

struct DbCfg 
{
	DBType type; 
	std::string ip;
	std::string user;
	std::string psw;
	std::string db_name;
};

class IDbCon
{
public:
	virtual ~IDbCon() {};

	virtual bool ConnectDb() { return false; };

	virtual void InitTable(const std::string &msg_name) {}; //�������ڣ��ⲻ���ٴ���
	virtual bool Insert(const db::ReqInsertData &req) { return false; };
	virtual bool Update(const db::ReqUpdateData &req) { return false; };
	virtual bool Get(const db::ReqGetData &req, db::RspGetData &rsp) { return false; };
	virtual bool Del(const db::ReqDelData &req, db::RspDelData &rsp) { return false; };
	virtual bool ExecuteSql(const std::string &sql_str) { return false; };

};

//��������db�ͻ���
class DbConMgr: public Singleton<DbConMgr>
{

public:
	DbConMgr()
		:m_con(nullptr)
	{
	}
	bool Init(const DbCfg &cfg);

	IDbCon &GetCon();

private:
	IDbCon *m_con;
};

class MongodbConn : public IDbCon
{

};

