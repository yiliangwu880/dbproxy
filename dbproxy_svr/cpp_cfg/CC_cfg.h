
//this file is build by CppCfg Tool
//don't modify by manual
#pragma once
#include <string>
#include <array>
#include "SimpleCfg.h"

struct cfg
{
	struct S_mysql_db
	{
		std::string mysql_db_db_name;
		std::string mysql_db_ip;
		uint16 mysql_db_port;
		std::string mysql_db_psw;
		std::string mysql_db_user;
	};


	////////////////////////define member list////////////////////////
	std::string ip;
	uint16 is_daemon;
	S_mysql_db mysql_db;
	uint16 port;

	////////////////////////method list////////////////////////
	//load or reload cfg file .
	bool LoadFile(const char *file_name=nullptr)
	{
		//default load original cfg file name
		if (nullptr == file_name)
		{
			file_name = "../../bin/dbproxy_svr/cfg.txt";
		}
		SimpleCfg js;
		if (!js.ParseFile(file_name))
		{
			return false;
		}
		return Assign(js);
	}

private:
	template<typename Array>
	inline size_t ArrayLen(const Array &array)
	{
		return sizeof(array) / sizeof(array[0]);
	}
	//initialize or reload cfg content.
	bool Assign(const nlohmann::json &js)
	{
		try
		{

			ip = js["ip"];
			is_daemon = js["is_daemon"];
			mysql_db.mysql_db_db_name = js["mysql_db"]["mysql_db_db_name"];
			mysql_db.mysql_db_ip = js["mysql_db"]["mysql_db_ip"];
			mysql_db.mysql_db_port = js["mysql_db"]["mysql_db_port"];
			mysql_db.mysql_db_psw = js["mysql_db"]["mysql_db_psw"];
			mysql_db.mysql_db_user = js["mysql_db"]["mysql_db_user"];
			port = js["port"];


			return true;
		}
		catch (...)
		{
			//if fail, pls check if your cfg fomart is legal.
			return false;
		}
	}
};
