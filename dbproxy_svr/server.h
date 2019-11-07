#pragma once
#include <string>
#include <vector>
#include "libevent_cpp/include/include_all.h"
#include "log_def.h"
#include "svr_util/include/singleton.h"
#include "svr_util/include/easy_code.h"


class InnerSvrCon : public lc::SvrCon
{
public:

private:
	virtual void OnRecv(const lc::MsgPack &msg) override;
	virtual void OnConnected() override;
};

class Server: public Singleton<Server>
{
public:

public:
	lc::Listener<InnerSvrCon> m_svr_listener;
};
