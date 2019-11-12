//驱动具体实现

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "libevent_cpp/include/include_all.h"
#include "svr_util/include/singleton.h"
#include "svr_util/include/easy_code.h"
#include "svr_util/include/typedef.h"
#include "db_driver.h"

namespace db {
	static const uint32 RE_CON_INTERVAL_SEC = 10; //x秒尝试重连

	class BaseDbproxy;
	class DbClientCon : public lc::ClientCon
	{
	private:
		BaseDbproxy &m_facade;
		lc::Timer m_recon_tm;
	public:
		DbClientCon(BaseDbproxy &facade);
		~DbClientCon() {};

		virtual void OnRecv(const lc::MsgPack &msg) override final;
		virtual void OnConnected() override final;
		virtual void OnDisconnected() override final;

		bool Send(db::Cmd cmd, const google::protobuf::Message &msg);

	private:
		void Handle_CMD_INIT_TABLE(const char *msg, uint32 msg_len);
		void Handle_CMD_INSERT(const char *msg, uint32 msg_len);
		void Handle_CMD_UPDATE(const char *msg, uint32 msg_len);
		void Handle_CMD_GET(const char *msg, uint32 msg_len);
		void Handle_CMD_DEL(const char *msg, uint32 msg_len);
		void Handle_CMD_SQL(const char *msg, uint32 msg_len);

		void OnTryReconTimeOut();
	};
}//namespace acc