/*
  ���������� svr ,db client����Ϣ��
*/

#pragma once
#include "base_include.h"

class ProtoUtil
{
public:
	//��������
	static void Test();

	//��ȡ�ֶ�ѡ��
	static bool GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, std::string &opt_value);
	static bool GetFieldOpt(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, int32 &opt_value);
	static bool GetFieldOptEnum(const google::protobuf::Message &msg, const std::string &field_name, const std::string &opt_name, uint32 &opt_value);
	//��ȡmsgѡ��
	static bool GetMsgOpt(const google::protobuf::Message &msg, const std::string &opt_name, std::string &opt_value);


	static std::unique_ptr<google::protobuf::Message> CreateMessage(const std::string &msg_name);
};