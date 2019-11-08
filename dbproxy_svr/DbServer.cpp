#include "DbServer.h"
#include "svr_util/include/read_cfg.h"
#include "google/protobuf/message.h"
#include "base.pb.h"
#include "db_con.h"

using namespace std;
using namespace lc;
using namespace google::protobuf;
using namespace db;

namespace
{
	//简化解包操作。赋值并移动指针
	template<class T>
	void ParseCp(T &dst, const char *&src)
	{
		dst = (decltype(dst))(*src); // 类似 dst = (uint32 &)(*src)
		src = src + sizeof(dst);
	}
}

void InnerSvrCon::Handle_CMD_INIT_TABLE(const char *msg, uint16 msg_len)
{
	ReqInitTable req;
	L_COND(req.ParseFromArray(msg, msg_len), "msg parse fail");

}

void InnerSvrCon::Handle_CMD_INSERT(const char *msg, uint16 msg_len)
{
	ReqInsertData req;
	L_COND(req.ParseFromArray(msg, msg_len), "msg parse fail");
	IDbCon &db_con = DbConMgr::Obj().GetCon();
	RspInsertData rsp;
	rsp.set_msg_name(req.msg_name());
	rsp.set_is_ok(db_con.Insert(req));

	Send(rsp);
}

void InnerSvrCon::Handle_CMD_UPDATE(const char *msg, uint16 msg_len)
{
	ReqUpdateData req;
	L_COND(req.ParseFromArray(msg, msg_len), "msg parse fail");
	IDbCon &db_con = DbConMgr::Obj().GetCon();
	RspUpdateData rsp;
	rsp.set_msg_name(req.msg_name());
	rsp.set_is_ok(db_con.Update(req));

	Send(rsp);
}

void InnerSvrCon::Handle_CMD_GET(const char *msg, uint16 msg_len)
{
	ReqGetData req;
	L_COND(req.ParseFromArray(msg, msg_len), "msg parse fail");
	IDbCon &db_con = DbConMgr::Obj().GetCon();
	RspGetData rsp;
	db_con.Get(req, rsp);
	Send(rsp);
}

void InnerSvrCon::Handle_CMD_DEL(const char *msg, uint16 msg_len)
{
	ReqDelData req;
	L_COND(req.ParseFromArray(msg, msg_len), "msg parse fail");
	IDbCon &db_con = DbConMgr::Obj().GetCon();
	RspDelData rsp;
	db_con.Del(req, rsp);
	Send(rsp);
}

void InnerSvrCon::OnRecv(const MsgPack &msg_pack)
{
	if (msg_pack.len < sizeof(db::Cmd))
	{
		L_WARN("illegal msg. msg.len=%d", msg_pack.len);
		return;
	}
	const char *cur = msg_pack.data;
	ParseCp(m_cur_cmd, cur);
	const char *msg = cur; 
	uint16 msg_len = msg_pack.len - sizeof(db::Cmd);
	switch (m_cur_cmd)
	{
	case CMD_INIT_TABLE: Handle_CMD_INIT_TABLE(msg, msg_len); return;
	case CMD_INSERT    : Handle_CMD_INSERT(msg, msg_len); return;
	case CMD_UPDATE    : Handle_CMD_UPDATE(msg, msg_len); return;
	case CMD_GET       : Handle_CMD_GET(msg, msg_len); return;
	case CMD_DEL       : Handle_CMD_DEL(msg, msg_len); return;
	default:
		L_WARN("illegal msg. cmd=%d", m_cur_cmd); return;
		break;
	}
}

void InnerSvrCon::OnConnected()
{

}

void InnerSvrCon::Send(const google::protobuf::Message &msg)
{
	Send(m_cur_cmd, msg);
}

void InnerSvrCon::Send(db::Cmd cmd, const google::protobuf::Message &msg)
{
	string s;
	bool r = msg.SerializeToString(&s);
	L_COND(r, " msg.SerializeToString fail. cmd=%s", db::Cmd_Name(cmd).c_str());


	string msg_pack;
	msg_pack.append((const char *)&cmd, sizeof(cmd));
	msg_pack.append(s);
	SendPack(msg_pack);
}
