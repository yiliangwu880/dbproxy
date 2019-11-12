/*
//依赖libevent_cpp库
需要下面写才能工作：

main()
{
	EventMgr::Obj().Init();

	调用本库的api


	EventMgr::Obj().Dispatch();
}

*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "libevent_cpp/include/include_all.h"
#include "svr_util/include/singleton.h"
#include "svr_util/include/easy_code.h"
#include "svr_util/include/typedef.h"
#include "../proto/base.pb.h"

namespace db {

	class DbClientCon;

	//db driver 接口
	class BaseDbproxy 
	{
	private:
		DbClientCon &m_con; //具体状态，和对外接口分离

	public:
		BaseDbproxy();
		~BaseDbproxy();

		bool Connect(const std::string &ip, uint16 port);

		bool InitTable(const db::ReqInitTable &req); //创建表， 检查表是否非法
		bool Insert(const google::protobuf::Message &msg);
		//更新数据，没填值的字段不会更新
		bool Update(const google::protobuf::Message &msg);
		template<class Msg>
		bool Get(const std::string &cond, uint32 limit_num=1);
		template<class Msg>
		bool Del(::uint64 num_key);
		template<class Msg>
		bool Del(std::string str_key);
		//执行mysql sql语句
		bool ExecuteSql(const std::string &sql_str);

	public:
		//连接dbproxy_svr成功
		virtual void OnCon() {};
		virtual void OnDiscon() {};
		virtual void OnRspInitTable(bool is_ok) {};
		virtual void OnRspInsert(const db::RspInsertData &rsp) {};
		virtual void OnRspUpdate(const db::RspUpdateData &rsp) {};
		virtual void OnRspGet(const db::RspGetData &rsp) {};
		virtual void OnRspDel(const db::RspDelData &rsp) {};
		virtual void OnRspSql(bool is_ok) {};

	private:
		bool Get(const std::string &msg_name, const std::string &cond, uint32 limit_num);
		bool Del(const std::string &msg_name, ::uint64 num_key, const std::string &str_key);
	};

	template<class Msg>
	bool db::BaseDbproxy::Del(std::string str_key)
	{
		auto des = Msg::descriptor();
		if (nullptr == des) {
			return false;
		}
		return Del(des->full_name(), 0, str_key);
	}

	template<class Msg>
	bool db::BaseDbproxy::Del(::uint64 num_key)
	{
		auto des = Msg::descriptor();
		if (nullptr == des) {
			return false;
		}
		return Del(des->full_name(), num_key, "");
	}


	template<class Msg>
	bool db::BaseDbproxy::Get(const std::string &cond, uint32 limit_num)
	{
		auto des = Msg::descriptor();
		if (nullptr == des) {
			return false;
		}
		return Get(des->full_name(), cond, limit_num);
	}


}