/*
 管理连接db客户端
*/

#pragma once
#include "base_include.h"
#include "db_con.h"
#include "mysql_connection.h"
#include "cppconn/resultset.h"

class MysqlCon : public IDbCon
{
public:
	virtual ~MysqlCon();

	virtual bool ConnectDb();

	virtual void InitTable(const std::string &msg_name); //如果表存在，测不会再创建
	virtual bool Insert(const db::ReqInsertData &req);
	virtual bool Update(const db::ReqUpdateData &req);
	virtual bool Get(const db::ReqGetData &req, db::RspGetData &rsp);
	virtual bool Del(const db::ReqDelData &req, db::RspDelData &rsp);
	virtual bool ExecuteSql(const std::string &sql_str);

private:
	void SetInsertPreparePara(sql::PreparedStatement &pstmt, google::protobuf::Message &msg);

	void SetUpdatePreparePara(sql::PreparedStatement &pstmt, google::protobuf::Message &msg);

	bool CreateUpdateSql(const google::protobuf::Message &msg, std::string &sql_str);
	bool SetField(google::protobuf::Message& msg, const google::protobuf::FieldDescriptor &field, const sql::ResultSet& res);

	bool BuildCreateTableSql(const std::string &msg_name, std::string &sql_str);

	std::string GetCreateTypeStr(google::protobuf::FieldDescriptor::Type t);
	bool CreateInsertSql(const google::protobuf::Message &msg, std::string &sql_str);

private:
	std::string m_ip;
	std::string m_user;
	std::string m_psw;
	std::string m_db_name;
	sql::Connection* m_con = nullptr;
};