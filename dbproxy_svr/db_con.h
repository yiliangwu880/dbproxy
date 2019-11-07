/*
 管理连接db客户端
*/

#pragma once
#include "base_include.h"
#include "mysql_connection.h"
#include "cppconn/resultset.h"

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

	virtual bool ConnectDb();

	virtual void InitTable(const std::string &msg_name); //如果表存在，测不会再创建
	virtual bool Insert(const db::ReqInsertData &req);
	virtual bool Update(const db::ReqUpdateData &req);
	virtual bool Get(const db::ReqGetData &req);
	virtual bool Del(const db::ReqDelData &req);
	virtual bool ExecuteSql(const std::string &sql_str);

};

//管理连接db客户端
class DbConMgr: public Singleton<DbConMgr>
{
	DbConMgr()
		:m_con(nullptr)
	{
	}

public:
	bool Init(const DbCfg &cfg);

	IDbCon &GetCon();

private:
	IDbCon *m_con;
};

class MongodbConn : public IDbCon
{

};

class MysqlCon : public IDbCon
{
public:
	virtual ~MysqlCon();

	virtual bool ConnectDb();

	virtual void InitTable(const std::string &msg_name); //如果表存在，测不会再创建
	virtual bool Insert(const db::ReqInsertData &req);
	virtual bool Update(const db::ReqUpdateData &req);
	virtual bool Get(const db::ReqGetData &req);
	virtual bool Del(const db::ReqDelData &req);
	virtual bool ExecuteSql(const std::string &sql_str);

private:
	void SetInsertPreparePara(sql::PreparedStatement &pstmt, google::protobuf::Message &msg);

	void SetUpdatePreparePara(sql::PreparedStatement &pstmt, google::protobuf::Message &msg);

	bool CreateUpdateSql(const google::protobuf::Message &msg, std::string &sql_str);
	bool SetField(google::protobuf::Message& msg, const google::protobuf::FieldDescriptor &field, const sql::ResultSet& res);

private:
	std::string m_ip;
	std::string m_user;
	std::string m_psw;
	std::string m_db_name;
	sql::Connection* m_con = nullptr;
};