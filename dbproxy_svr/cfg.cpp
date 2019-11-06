#include "db_con.h"


CfgMgr::CfgMgr()
	:m_inner_port(0)
	, m_ex_port(0)
	, is_daemon(false)
{

}

bool CfgMgr::Init()
{
	L_DEBUG("init cfg");
	su::Config cfg;
	L_COND_F(cfg.init("cfg.txt"));

	m_inner_ip = cfg.GetStr("inner_ip");
	L_DEBUG("inner_ip=%s", m_inner_ip.c_str());
	m_inner_port = (uint16)cfg.GetInt("inner_port");
	L_DEBUG("inner_port=%d", m_inner_port);

	m_ex_ip = cfg.GetStr("ex_ip");
	L_DEBUG("ex_ip=%s", m_ex_ip.c_str());
	m_ex_port = (uint16)cfg.GetInt("ex_port");
	L_DEBUG("ex_port=%d", m_ex_port);

	is_daemon = (bool)cfg.GetInt("is_daemon");
	L_DEBUG("is_daemon=%d", is_daemon);

	return true;
}
