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
	static bool GetFieldOpt(const google::protobuf::Descriptor &des, const std::string &field_name, const std::string &opt_name, std::string &opt_value);
	static bool GetFieldOpt(const google::protobuf::Descriptor &des, const std::string &field_name, const std::string &opt_name, int32 &opt_value);
	static bool GetFieldOptEnum(const google::protobuf::Descriptor &des, const std::string &field_name, const std::string &opt_name, uint32 &opt_value);
	static bool GetFieldKeyOpt(const google::protobuf::Descriptor &des, const std::string &field_name, db::KeyType &key_type);

	//��ȡmsgѡ��
	static bool GetMsgOpt(const google::protobuf::Descriptor &des, const std::string &opt_name, std::string &opt_value);
	//��ȡ������ ����У���� num_key str_key����֮һ����.
	//@return false��ʾû��
	static bool GetMsgMainKeyVal(const google::protobuf::Message &msg, ::uint64 &num_key, std::string &str_key);
	static std::string GetMsgMainKeyName(const google::protobuf::Descriptor &des);

	//unique ptr���أ���й©���ѡ�
	static std::unique_ptr<google::protobuf::Message> CreateMessage(const std::string &msg_name);
};