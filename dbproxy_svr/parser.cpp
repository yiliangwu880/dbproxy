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

bool ProtoUtil::BuildCreateTableSql(const std::string &msg_name, std::string &sql_str)
{
	const Descriptor* des = DescriptorPool::generated_pool()->FindMessageTypeByName(msg_name);
	if (nullptr == des)
	{
		L_ERROR("find msg Descriptor fail. msg name=%s", msg_name.c_str());
		return false;
	}
	if (0 == des->field_count())
	{
		L_ERROR("msg no field. msg name=%s", msg_name.c_str());
		return false;
	}

	sql_str = "CREATE TABLE IF NOT EXISTS `";
	sql_str += des->name(); //不用 msg_name 去掉命名空间, 因为表名有.符号会导致insert sql语句查表名失败
	sql_str += "` (";

	int cnt = des->field_count();
	bool is_find_key = false;
	for (int i = 0; i < cnt; i++)
	{
		//每个域 字符串模板 = "`a` varchar(11) NOT NULL,"
		const FieldDescriptor* field = des->field(i);
		if (nullptr == field)
		{
			L_ERROR("unknow error %d", i);
			return false;
		}
		if (field->is_repeated())
		{
			L_ERROR("not support repeated lable");
			return false;
		}
		sql_str += "`";
		sql_str += field->name();
		sql_str += "` ";
		if ("id" == field->name())
		{
			is_find_key = true;
			sql_str += "bigint(20) NOT NULL";
		}
		else
		{
			sql_str += GetCreateTypeStr(field->type());
		}
		sql_str += ",";
		//L_DEBUG("check field[%s]", field->name().c_str());
	}
	if (!is_find_key)
	{
		L_ERROR("msg need field name [id] for key. msg name=%s", msg_name.c_str());
		return false;
	}
	sql_str += "PRIMARY KEY(`id`)";
	sql_str += ") ENGINE = InnoDB DEFAULT CHARSET = utf8;";
	return true;
}



bool ProtoUtil::CreateInsertSql(const google::protobuf::Message &msg, std::string &sql_str)
{
	const Descriptor* des = msg.GetDescriptor();
	//const Reflection* rfl = msg.GetReflection();
	//if (nullptr == rfl)
	//{
	//	LOG_ERROR("find msg Descriptor or Reflection fail. msg name=%s", des->name().c_str());
	//	return false;
	//}
	if (nullptr == des)
	{
		L_ERROR("find msg Descriptor or Reflection fail. msg name=%s", des->name().c_str());
		return false;
	}
	int count = des->field_count();
	if (0 == count)
	{
		L_ERROR("msg no field. msg name=%s", des->name().c_str());
		return false;
	}

	sql_str += "INSERT INTO ";
	sql_str += des->name();
	sql_str += "(";

	//循环拷贝名字
	for (int i = 0; i < count; i++)
	{
		const FieldDescriptor* field = des->field(i);
		//if (!rfl->HasField(msg, field))
		//{
		//	continue;
		//}
		if (i > 0)
		{
			sql_str += ",";
		}
		sql_str += field->name();
	}

	sql_str += ") VALUES(";

	for (int i = 0; i < count; i++)
	{
		if (i > 0)
		{
			sql_str += ",?";
		}
		else
		{
			sql_str += "?";
		}
	}

	sql_str += ");";
	return true;
}

std::string ProtoUtil::GetCreateTypeStr(FieldDescriptor::Type t)
{
	switch (t)
	{
	default:
		L_ERROR("unknow type %d", (int)t);
		return "unknow type";
	case FieldDescriptor::TYPE_DOUBLE:
	case FieldDescriptor::TYPE_FLOAT:
		return "double NULL DEFAULT NULL";
	case FieldDescriptor::TYPE_INT64:
	case FieldDescriptor::TYPE_SINT64:
	case FieldDescriptor::TYPE_SFIXED64:
	case FieldDescriptor::TYPE_UINT64:
	case FieldDescriptor::TYPE_FIXED64:
	case FieldDescriptor::TYPE_FIXED32:
	case FieldDescriptor::TYPE_UINT32:
		return "bigint(20) NULL DEFAULT NULL";
	case FieldDescriptor::TYPE_INT32:
	case FieldDescriptor::TYPE_ENUM:
	case FieldDescriptor::TYPE_SFIXED32:
	case FieldDescriptor::TYPE_SINT32:
	case FieldDescriptor::TYPE_BOOL:
		return "int(11) NULL DEFAULT NULL";
	case FieldDescriptor::TYPE_STRING:
		return "varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL";
	case FieldDescriptor::TYPE_MESSAGE:
	case FieldDescriptor::TYPE_BYTES:
		return "blob NULL";
	}
}