#include "db_driver.h"
#include "connect.h"
#include "log_def.h"

using namespace std;
using namespace lc;
using namespace su;
using namespace db;

DbClientCon::DbClientCon(BaseDbproxy &facade)
	:m_facade(facade)
{
}

void DbClientCon::OnRecv(const lc::MsgPack &msg_pack)
{
	L_COND(msg_pack.len >= sizeof(db::Cmd));
	db::Cmd cmd;
	const char *cur = msg_pack.data;
	cmd = *(const db::Cmd *)cur;
	cur += sizeof(db::Cmd);
	const char *msg = cur;
	uint32 msg_len = msg_pack.len - sizeof(db::Cmd);

	switch (cmd)
	{
	case CMD_INIT_TABLE:Handle_CMD_INIT_TABLE(msg, msg_len); return;
	case CMD_INSERT    :Handle_CMD_INSERT(msg, msg_len); return;
	case CMD_UPDATE    :Handle_CMD_UPDATE(msg, msg_len); return;
	case CMD_GET       :Handle_CMD_GET(msg, msg_len); return;
	case CMD_DEL       :Handle_CMD_DEL(msg, msg_len); return;
	case CMD_SQL       :Handle_CMD_SQL(msg, msg_len); return;
	default:
		L_ERROR("unknow cmd. %d", cmd);
		break;
	}
}

bool db::DbClientCon::Send(db::Cmd cmd, const google::protobuf::Message &msg)
{
	string tcp_pack;
	tcp_pack.append((const char *)&cmd, sizeof(cmd));
	tcp_pack.append(msg.SerializeAsString());
	return SendPack(tcp_pack);
}

void db::DbClientCon::Handle_CMD_INIT_TABLE(const char *msg, uint32 msg_len)
{
	RspInitTable rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len), "parse msg fail");
	m_facade.OnRspInitTable(rsp.is_ok());
}

void db::DbClientCon::Handle_CMD_INSERT(const char *msg, uint32 msg_len)
{
	RspInsertData rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len), "parse msg fail");
	m_facade.OnRspInsert(rsp);
}

void db::DbClientCon::Handle_CMD_UPDATE(const char *msg, uint32 msg_len)
{
	RspUpdateData rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len), "parse msg fail");
	m_facade.OnRspUpdate(rsp);
}

void db::DbClientCon::Handle_CMD_GET(const char *msg, uint32 msg_len)
{
	RspGetData rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len), "parse msg fail");
	m_facade.OnRspGet(rsp);
}

void db::DbClientCon::Handle_CMD_DEL(const char *msg, uint32 msg_len)
{
	RspDelData rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len), "parse msg fail");
	m_facade.OnRspDel(rsp);
}

void db::DbClientCon::Handle_CMD_SQL(const char *msg, uint32 msg_len)
{
	RspSql rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len), "parse msg fail");
	m_facade.OnRspSql(rsp.is_ok());
}

void db::DbClientCon::OnTryReconTimeOut()
{
	TryReconnect();
}

void DbClientCon::OnConnected()
{
	m_facade.OnCon();
}

void DbClientCon::OnDisconnected()
{
	L_DEBUG("start try recon timer , sec=%d", RE_CON_INTERVAL_SEC);
	auto f = std::bind(&DbClientCon::OnTryReconTimeOut, this);
	m_recon_tm.StopTimer();
	m_recon_tm.StartTimer(RE_CON_INTERVAL_SEC*1000, f);
	m_facade.OnDiscon();
}


