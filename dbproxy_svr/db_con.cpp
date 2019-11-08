#include "db_con.h"
#include "parser.h"

using namespace su;
using namespace lc;
using namespace std;
using namespace google::protobuf;
using namespace db;

IDbCon & DbConMgr::GetCon()
{
	if (nullptr == m_con)
	{
		L_FATAL("DbConMgr havn't init");
		L_ASSERT(false);
	}
	return *m_con;
}
