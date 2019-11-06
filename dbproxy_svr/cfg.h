/*
 管理连接db客户端
*/

#pragma once
#include "base_include.h"


class CfgMgr : public Singleton<CfgMgr>
{
public:
	CfgMgr();
	bool Init();

	unsigned short GetInnerPort() const { return m_inner_port; }
	const char *GetInnerIp() const { return m_inner_ip.c_str(); }
	unsigned short GetExPort() const { return m_ex_port; }
	const char *GetExIp() const { return m_ex_ip.c_str(); }
	bool IsDaemon()const { return is_daemon; }
private:
	unsigned short m_inner_port;
	std::string m_inner_ip;
	//外网地址，给client连接用
	unsigned short m_ex_port;
	std::string m_ex_ip;
	bool is_daemon;
};