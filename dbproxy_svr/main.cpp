#include "base_include.h"
#include "svr_util/include/su_mgr.h"
#include "svr_util/include/single_progress.h"
#include <signal.h>
#include "parser.h"

using namespace su;
using namespace lc;
using namespace std;
using namespace google::protobuf;
using namespace db_proto;

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
	void t()
	{
		ReqSelectTable msg;

		printf("------------------s11-----------------\n");
		{
			string v;
			ProtoUtil::GetFieldOpt(msg, "s2", "db_proto.StrOpt", v);
			printf("v:%s\n", v.c_str());
		}
		{
			uint32 v;
			ProtoUtil::GetFieldOpt(msg, "s3", "db_proto.IntOpt", v);
			printf("v:%d\n", v);
		}
		{
			uint32 v;
			ProtoUtil::GetFieldOptEnum(msg, "s1", "db_proto.KeyOpt", v);
			printf("KeyOpt:%d\n", v);
		}
		return;

		msg.set_msg_name("a");
		//msg debugstr:msg_name: "a"
		printf("msg debugstr:%s\n", msg.DebugString().c_str());
		const Descriptor *des = msg.GetDescriptor();
	
		{
			const FieldDescriptor* fd = des->FindFieldByName("s1");
			if (fd == nullptr)
			{
				printf("fd == nullptr");
				return;
			}
			const FieldOptions& fo = fd->options();
			//fo typename=google.protobuf.FieldOptions
			printf("fo typename=%s\n", fo.GetTypeName().c_str());
			const Reflection* ref = fo.GetReflection();
			if (ref == nullptr)
			{
				printf("ref == nullptr\n");
				return;
			}
			printf("------------------s1-----------------\n");
			{
				const FieldDescriptor* fd = ref->FindKnownExtensionByName("db_proto.KeyOpt");
				if (fd == nullptr)
				{
					printf("FindKnownExtensionByName fd == nullptr\n");
					return;
				}
				int i = ref->GetEnumValue(fo, fd);
				printf("GetEnumValue %d \n", i);

			}
		}
		{
			const FieldDescriptor* fd = des->FindFieldByName("s2");
			if (fd == nullptr)
			{
				printf("fd == nullptr");
				return;
			}
			const FieldOptions& fo = fd->options();
			//fo typename=google.protobuf.FieldOptions
			printf("fo typename=%s\n", fo.GetTypeName().c_str());
			const Reflection* ref = fo.GetReflection();
			if (ref == nullptr)
			{
				printf("ref == nullptr\n");
				return;
			}
			printf("------------------s1-----------------\n");
			{
				const FieldDescriptor* fd = ref->FindKnownExtensionByName("db_proto.StrOpt");
				if (fd == nullptr)
				{
					printf("FindKnownExtensionByName fd == nullptr\n");
					return;
				}
				auto v = ref->GetString(fo, fd);
				printf("opt= %s \n", v.c_str());

			}
		}
		{
			const FieldDescriptor* fd = des->FindFieldByName("s3");
			if (fd == nullptr)
			{
				printf("fd == nullptr");
				return;
			}
			const FieldOptions& fo = fd->options();
			//fo typename=google.protobuf.FieldOptions
			printf("fo typename=%s\n", fo.GetTypeName().c_str());
			const Reflection* ref = fo.GetReflection();
			if (ref == nullptr)
			{
				printf("ref == nullptr\n");
				return;
			}
			printf("------------------s1-----------------\n");
			{
				const FieldDescriptor* fd = ref->FindKnownExtensionByName("db_proto.IntOpt");
				if (fd == nullptr)
				{
					printf("FindKnownExtensionByName fd == nullptr\n");
					return;
				}
				auto v = ref->GetUInt32(fo, fd);
				printf("opt= %d \n", v);

			}
		}
	
	}

}
int main(int argc, char* argv[])
{
	t();
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

