/*
 管理连接db客户端
*/

#pragma once
#include "base_include.h"
#include "cpp_cfg/CC_cfg.h"

class CfgMgr : public Singleton<CfgMgr>
{
public:
	CfgMgr() {};
	bool Init();

	const cfg GetCfg() const { return m_cfg; }
private:
	cfg m_cfg;
};