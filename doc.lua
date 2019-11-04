术语:
	dbproxy	  	== db代理服务器，svr读写db都通过dbproxy做转发
	svr  	  	== 需要读写db的业务服务器。
	db	  		==  mysql,mongodb, redis

问题：
	大多项目，svr读写db,一个表（集合）需要加减字段名，都需要人工修改dbproxy的读写字段代码。
	
设计目的：

实现方法：
	利用用protobuf3对象在C++内存是个动态对象，能知道每个字段名和值。 动态地实现dbproxy的读写。
	

不选用json的原因：
	json太动态了，编译器没发检查人工写出错误字段的错误。


	

	

	