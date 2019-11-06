简介：
	db代理服务器,给业务服务器读写db（mysql,mongodb,redis）用。
	特点：
		表结构用proto文件表达。用户不需要写db读写的语句（比如sql语句）
		首次连接db,会自动在db创建表。
		当你修改db表字段时，不需要手工修改db代理服务器代码，只需要更新protobuf导出cpp文件重新编译，就支持读写新的字段。

术语:
	dbproxy	  	== db代理服务器，svr读写db都通过dbproxy转发。
	svr  	  	== 需要读写db的业务服务器。 只连接dbproxy, 不连接db。
	db	  		==  mysql, mongodb, redis
	svr_driver  == svr使用的驱动库，处理和dbproxy的交互.
	表			== mysql的表，或者mongodb的集合，这里统一叫表吧。

问题：
	传统项目，svr读写db,一个表（集合）需要加减字段名，都需要人工修改dbproxy的读写字段代码。
	部署阶段，需要写个db脚本，初始化表结构。
	这种做法导致修改一个表字段，需要烦闷地修改db脚本，svr读写代码，dbproxy读写代码。 任意一个地方少改或者改错，就GG。
	开发者直接用db提供的驱动库，通常复杂，容易内存泄漏。 mysql connector 库的接口就是个例子，容易写出泄露内存的代码。

设计目的：
	消灭重复多余人工容易犯错的手动活。
	修改一个表字段，只需要改protobuf文件和svr读写代码，重编译。
	svr不需要写sql语句，只需要读写protobuf对象。
	只提供大多数传统db操作，忽略少用的db操作，目的简化。
	方便转换到不同db。

缺点：
	依赖protobuf3

功能：
	dbproxy_svr:
	{
		用protobuf定义表字段。
		创建db表。
		用户连接限制. 指定svr地址。（可选用）
		兼容多种db. （先实现mysql）
		查询,插入，更新。
	}
	svr_driver:
	{
		查询,插入，更新。
		响应结果。
		断线重连。
	}
	编译脚本，自动检查protobuf依赖关系.

实现方法：
	利用用protobuf3对象在C++内存是个动态对象，能知道每个字段名和值。 动态地实现dbproxy的读写。

协议：
svr和dbproxy通讯tcp pack包含：cmd,msg
cmd 消息号
msg 具体消息内容，protobuf结构。



	

	

	