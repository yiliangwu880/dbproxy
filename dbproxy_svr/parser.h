/*
  解析并处理 svr ,db client的消息。
*/

#pragma once
#include "base_include.h"

class ProtoUtil
{
public:

	static bool GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, std::string &v);
	static bool GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, uint32 &v);
	static bool GetFieldOptEnum(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, uint32 &v);
};