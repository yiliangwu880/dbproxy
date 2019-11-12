
#include "db_driver.h"
#include "log_def.h"
#include "connect.h"

using namespace std;
using namespace lc;
using namespace su;
using namespace db;


BaseDbproxy::BaseDbproxy()
	:m_con(*(new DbClientCon(*this)))
{
	
}

BaseDbproxy::~BaseDbproxy()
{
	delete &m_con;
}

bool db::BaseDbproxy::Connect(const std::string &ip, uint16 port)
{
	event_base *p = EventMgr::Obj().GetEventBase();
	L_COND_F(p, "must call EventMgr::Obj().Init before this function");
	return m_con.ConnectInit(ip.c_str(), port);
}

bool db::BaseDbproxy::InitTable(const db::ReqInitTable &req)
{
	return m_con.Send(CMD_INIT_TABLE, req);
}

bool db::BaseDbproxy::Insert(const google::protobuf::Message &msg)
{
	ReqInsertData req;
	req.set_msg_name(msg.GetTypeName());
	req.set_data(msg.SerializeAsString());
	return m_con.Send(CMD_INSERT, req);
}

bool db::BaseDbproxy::Update(const google::protobuf::Message &msg)
{
	ReqUpdateData req;
	req.set_msg_name(msg.GetTypeName());
	req.set_data(msg.SerializeAsString());
	return m_con.Send(CMD_UPDATE, req);
}

bool db::BaseDbproxy::Get(const std::string &msg_name, const std::string &cond, uint32 limit_num)
{
	ReqGetData req;
	req.set_msg_name(msg_name);
	req.set_cond(cond);
	req.set_limit_num(limit_num);
	return m_con.Send(CMD_GET, req);
}

bool db::BaseDbproxy::Del(const std::string &msg_name, const std::string &cond, ::uint64 num_key, const std::string &str_key)
{
	ReqDelData req;
	req.set_msg_name(msg_name);
	req.set_cond(cond);
	req.set_num_key(num_key);
	req.set_str_key(str_key);
	return m_con.Send(CMD_DEL, req);
}

bool db::BaseDbproxy::ExecuteSql(const std::string &sql_str)
{
	ReqSql req;
	req.set_exe_str(sql_str);
	return m_con.Send(CMD_SQL, req);
}
