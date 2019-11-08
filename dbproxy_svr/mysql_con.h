/*
 ��������db�ͻ���
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

	virtual bool ConnectDb(const Cfg &cfg) override;

	virtual bool InitTable(const db::ReqInitTable &req, db::RspInitTable &rsp) override; //������ �����Ƿ�Ƿ�
	virtual bool Insert(const db::ReqInsertData &req) override;
	virtual bool Update(const db::ReqUpdateData &req) override;
	virtual bool Get(const db::ReqGetData &req, db::RspGetData &rsp) override;
	virtual bool Del(const db::ReqDelData &req, db::RspDelData &rsp) override;
	virtual bool ExecuteSql(const std::string &sql_str) override;

private:
	void SetInsertPreparePara(sql::PreparedStatement &pstmt, google::protobuf::Message &msg);

	void SetUpdatePreparePara(sql::PreparedStatement &pstmt, google::protobuf::Message &msg);

	bool CreateUpdateSql(const google::protobuf::Message &msg, std::string &sql_str);
	bool SetField(google::protobuf::Message& msg, const google::protobuf::FieldDescriptor &field, const sql::ResultSet& res);

	bool BuildCreateTableSql(const std::string &msg_name, std::string &sql_str);

	std::string GetCreateTypeStr(google::protobuf::FieldDescriptor::Type t);
	bool CreateInsertSql(const google::protobuf::Message &msg, std::string &sql_str);

private:
	sql::Connection* m_con = nullptr;
};