#include "base_include.h"
#include "svr_util/include/su_mgr.h"
#include "svr_util/include/single_progress.h"
#include <signal.h>
#include "parser.h"

using namespace su;
using namespace lc;
using namespace std;
using namespace google::protobuf;
using namespace db;

class MyLcLog : public lc::ILogPrinter, public Singleton<MyLcLog>
{
public:
	virtual void Printf(lc::LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) override
	{
		su::LogMgr::Obj().Printf((su::LogLv)lv, file, line, fun, pattern, vp);
	}
};


void OnExitProccess()
{
	L_DEBUG("OnExitProccess");
	EventMgr::Obj().StopDispatch();
}
namespace {
 

}
int main(int argc, char* argv[])
{
	ProtoUtil::Test();
	SuMgr::Obj().Init();
	L_COND_F(CfgMgr::Obj().Init());

	if (CfgMgr::Obj().IsDaemon())
	{
		//当nochdir为0时，daemon将更改进城的根目录为root(“ / ”)。
		//当noclose为0是，daemon将进城的STDIN, STDOUT, STDERR都重定向到 / dev / null。
		L_COND_F(0==daemon(1, 0));
	}

	//start or stop proccess
	SPMgr::Obj().Check(argc, argv, "acc_svr", OnExitProccess);

	EventMgr::Obj().Init(&MyLcLog::Obj());



	EventMgr::Obj().Dispatch();
	L_INFO("main end");
	return 0;
}

