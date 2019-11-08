/*
  解析并处理 svr ,db client的消息。
*/

#pragma once
#include "base_include.h"

class ProtoUtil
{
public:
	//测试例子
	static void Test();

	//获取字段选项
	static bool GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, std::string &opt_value);
	static bool GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, int32 &opt_value);
	static bool GetFieldOptEnum(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, uint32 &opt_value);
	//获取msg选项
	static bool GetMsgOpt(const google::protobuf::Message &msg, const std::string &opt_name, std::string &opt_value);


	static std::unique_ptr<google::protobuf::Message> CreateMessage(const std::string &msg_name);
};