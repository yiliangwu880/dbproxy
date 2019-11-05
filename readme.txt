简介：
	db代理服务器,给业务服务器读写db（mysql,mongodb,redis）用。
	特点：
		部署服务器不需要你写数据库脚本创建db表，dbproxy首次连接db的时候已经为你做了。
		当你修改db表字段时，不需要手工修改db代理服务器代码，只需要更新protobuf导出cpp文件重新编译，就支持读写新的字段。

详细功能说明参考： doc.lua文件	

编译方法：
	整个文件夹放到linux目录，安装cmake gcc git等。

	git submodule init      --更新子模块
	git submodule update			
	git checkout -f	master		--强制删掉本地分支， track远程master分支
	去目录 External里面，参考说明编译每个文件夹，生成依赖库。
	主目录执行：sh clearBuild.sh 完成编译

vs浏览代码：
	执行.\vs\run.bat,生成sln文件

	
目录结构：
	dbproxy_svr ==db代理服务器
	bin			==执行文件
	test 		==测试

使用方法：

