/*

*/

#include <string>
#include "libevent_cpp/include/include_all.h"
#include "svr_util/include/su_mgr.h"
#include "svr_util/include/single_progress.h"
#include "svr_util/include/read_cfg.h"
#include "unit_test.h"
#include "../db_driver/include/db_driver.h"
#include "../com/cfg.h"
#include "proto/test_data.pb.h"

using namespace std;
using namespace su;
using namespace db;
using namespace lc;


namespace
{
	class DbMgr: public db::BaseDbproxy
	{
	public:
		enum State
		{
			WAIT_CONNECT,
			WAIT_INIT_TALBE,
			WAIT_INSERT,
			WAIT_UPDATE,
		};
		State m_state;
		TestTable m_msg;
		SubMsg m_sub;
		TTT3 m_t;
	public:
		DbMgr();

		void Start();

		virtual void OnCon();
		virtual void OnDiscon();
		virtual void OnRspInitTable(bool is_ok) ;
		virtual void OnRspInsert(const db::RspInsertData &rsp) ;
		virtual void OnRspUpdate(const db::RspUpdateData &rsp) ;
		virtual void OnRspGet(const db::RspGetData &rsp) ;
		virtual void OnRspDel(const db::RspDelData &rsp) ;
		virtual void OnRspSql(bool is_ok);
	};




	DbMgr::DbMgr()
	{
	}


	void DbMgr::Start()
	{
		m_state = WAIT_CONNECT;
		UNIT_INFO("connect %s %d", CfgMgr::Obj().dbproxy_svr_ip.c_str(), CfgMgr::Obj().dbproxy_svr_port);
		Connect(CfgMgr::Obj().dbproxy_svr_ip, CfgMgr::Obj().dbproxy_svr_port);
	}

	void DbMgr::OnCon()
	{
		UNIT_ASSERT(m_state == WAIT_CONNECT);
		m_state = WAIT_INIT_TALBE;
		ReqInitTable req;
		//TestTable a;
		req.add_msg_name("TestTable");
		req.add_msg_name("TTT3");
		InitTable(req);
	}


	void DbMgr::OnDiscon()
	{
		UNIT_INFO("OnDiscon");
	}


	void DbMgr::OnRspInitTable(bool is_ok)
	{
		UNIT_INFO("OnRspInitTable");
		UNIT_ASSERT(WAIT_INIT_TALBE == m_state);
		UNIT_ASSERT(is_ok);
		m_state = WAIT_INSERT;
		{
			m_msg.set_id(1);
			m_msg.set_name_32(1);
			m_msg.set_name_str("a");
			m_msg.set_name_enum(T1);
			m_msg.set_name_bool(true);
			m_msg.set_name_bytes("a", 1);
			Insert(m_msg);
		}
		{
			m_sub.set_name_enum(T1);
			m_sub.set_name_bool(true);

			m_t.set_id(1);
			m_t.set_name("a");
			m_t.mutable_sub_msg()->CopyFrom(m_sub);
			m_t.set_t23("a");
			Insert(m_t);
		}
	}


	void DbMgr::OnRspInsert(const db::RspInsertData &rsp)
	{
		UNIT_INFO("OnRspInsert");
		UNIT_ASSERT(WAIT_INSERT == m_state);
		UNIT_ASSERT(rsp.is_ok());
		if (rsp.msg_name() == "TTT3")
		{
			UNIT_ASSERT(rsp.str_key() == m_t.name());
			UNIT_INFO("start updte");
			m_state = WAIT_UPDATE;
			{
				m_msg.set_name_str("c");
				Update(m_msg);
			}
			{
				m_sub.set_name_enum(T2);
				m_t.set_t23("b");
				Update(m_t);
			}

		}
		else if (rsp.msg_name() == "TestTable")
		{
			UNIT_ASSERT(rsp.num_key() == m_msg.id());
		}

	}


	void DbMgr::OnRspUpdate(const db::RspUpdateData &rsp)
	{
		UNIT_INFO("OnRspUpdate");
		UNIT_ASSERT(WAIT_UPDATE == m_state);
		UNIT_ASSERT(rsp.is_ok());
		if (rsp.msg_name() == "TTT3")
		{
			UNIT_ASSERT(rsp.str_key() == m_t.name());

		}
		else if (rsp.msg_name() == "TestTable")
		{
			UNIT_ASSERT(rsp.num_key() == m_msg.id());
		}
	}

	void DbMgr::OnRspGet(const db::RspGetData &rsp)
	{

	}

	void DbMgr::OnRspDel(const db::RspDelData &rsp)
	{

	}

	void DbMgr::OnRspSql(bool is_ok)
	{

	}

	class TC : public lc::ClientCon
	{
	public:
		//ÿ�ν��ն���������Ϣ��
		virtual void OnRecv(const MsgPack &msg){};
		virtual void OnConnected() {
			UNIT_INFO("OnConnected");
		};
		virtual void OnError(short events) {};
		//����ɾ������ص����Է��Ͽ��������������
		//�����õ�ʱ�� fd, bufferevent ��Դ�Ѿ��ͷ�
		//ɾ�������� ���ᴥ��OnDisconnected��
		virtual void OnDisconnected() {
			UNIT_INFO("OnDisconnected");
		};
	};
}

UNITTEST(test_mysql)
{
	UNIT_ASSERT(CfgMgr::Obj().Init());
	EventMgr::Obj().Init();

	DbMgr db;
	db.Start();

	EventMgr::Obj().Dispatch();
	UNIT_INFO("--------------------test_mysql end--------------------");

}