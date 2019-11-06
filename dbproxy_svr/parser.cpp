#include "parser.h"

using namespace su;
using namespace lc;
using namespace std;
using namespace google::protobuf;
using namespace db_proto;

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

bool ProtoUtil::GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, uint32 &v)
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

	v = ref->GetUInt32(fo, opt_fd);
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
