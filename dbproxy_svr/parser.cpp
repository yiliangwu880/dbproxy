#include "parser.h"

using namespace su;
using namespace lc;
using namespace std;
using namespace google::protobuf;
using namespace db;

void ProtoUtil::Test()
{
	TestMsg msg;

	printf("------------------s11-----------------\n");
	{
		string v;
		ProtoUtil::GetMsgOpt(msg, "db.MsgOpt", v);
		printf("MsgOpt:%s\n", v.c_str());
	}
	{
		string v;
		ProtoUtil::GetFieldOpt(msg, "s2", "db.StrOpt", v);
		printf("StrOpt:%s\n", v.c_str());
	}
	{
		int32 v;
		ProtoUtil::GetFieldOpt(msg, "s3", "db.IntOpt", v);
		printf("IntOpt:%d\n", v);
	}
	{
		uint32 v;
		ProtoUtil::GetFieldOptEnum(msg, "id", "db.KeyOpt", v);
		printf("KeyOpt:%d\n", v);
	}
}

bool ProtoUtil::IsErrorTable(const google::protobuf::Message &msg)
{
	//检查主键是否多个
	return false;
}

bool ProtoUtil::GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, string &v)
{
	const Descriptor *des = msg.GetDescriptor();
	COND_F(des);
	const FieldDescriptor* fd = des->FindFieldByName(field_name);
	COND_F(fd);
	const FieldOptions& fo = fd->options();
	const Reflection* ref = fo.GetReflection();
	COND_F(ref);
	const FieldDescriptor* opt_fd = ref->FindKnownExtensionByName(opt_name);
	COND_F(opt_fd);

	v = ref->GetString(fo, opt_fd);
	return true;
}

bool ProtoUtil::GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, int32 &v)
{
	const Descriptor *des = msg.GetDescriptor();
	COND_F(des);
	const FieldDescriptor* fd = des->FindFieldByName(field_name);
	COND_F(fd);
	const FieldOptions& fo = fd->options();
	const Reflection* ref = fo.GetReflection();
	COND_F(ref);
	const FieldDescriptor* opt_fd = ref->FindKnownExtensionByName(opt_name);
	COND_F(opt_fd);

	v = ref->GetInt32(fo, opt_fd);
	return true;
}

bool ProtoUtil::GetFieldOptEnum(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, uint32 &v)
{
	const Descriptor *des = msg.GetDescriptor();
	COND_F(des);
	const FieldDescriptor* fd = des->FindFieldByName(field_name);
	COND_F(fd);
	const FieldOptions& fo = fd->options();
	const Reflection* ref = fo.GetReflection();
	COND_F(ref);
	const FieldDescriptor* opt_fd = ref->FindKnownExtensionByName(opt_name);
	COND_F(opt_fd);

	v = ref->GetEnumValue(fo, opt_fd);
	return true;
}

bool ProtoUtil::GetMsgOpt(const google::protobuf::Message &msg, const std::string &opt_name, std::string &v)
{
	const Descriptor *des = msg.GetDescriptor();
	COND_F(des);
	const MessageOptions& opt = des->options();
	const Reflection* ref = opt.GetReflection();
	COND_F(ref);
	const FieldDescriptor* opt_fd = ref->FindKnownExtensionByName(opt_name);
	COND_F(opt_fd);

	v = ref->GetString(opt, opt_fd);
	return true;
}

bool ProtoUtil::GetMsgMainKeyVal(const google::protobuf::Message &msg, ::uint64 &num_key, std::string &str_key)
{
	str_key.clear();
	num_key = 0;

	const Descriptor *des = msg.GetDescriptor();
	int cnt = des->field_count();
	L_COND_F(cnt > 0, "no field");
	for (int i = 0; i < cnt; i++)
	{
		const FieldDescriptor* field = des->field(i);
		L_COND_F(field);
		KeyType key_type;
		uint32 v;
		GetFieldOptEnum(msg, field->name(), "db.KeyOpt", v);
		key_type = (KeyType)v;
		if (db::K_MAIN_KEY != key_type)
		{
			continue;
		}

		//is main key
		const Reflection* ref = msg.GetReflection();
		L_COND_F(ref);
		if (FieldDescriptor::TYPE_STRING == field->type())
		{
			str_key = ref->GetString(msg, field);
			return true;
		}
		else//其他一致认为是uint64处理
		{
			num_key = ref->GetUInt64(msg, field);
			return true;
		}
	}
	return false;
}

std::unique_ptr<Message> ProtoUtil::CreateMessage(const std::string &msg_name)
{
	unique_ptr<Message> p;
	const Descriptor* des = DescriptorPool::generated_pool()->FindMessageTypeByName(msg_name);
	COND_R(des, nullptr);
	const Message* prototype = MessageFactory::generated_factory()->GetPrototype(des);
	COND_R(prototype, nullptr);
	p.reset(prototype->New());
	return p;
}


