--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--
require "lib"

local action = _ACTION or ""
local outdir = action


WorkSpaceInit  "dbproxy"



Project "dbproxy_svr"
	IncludeFile { 
		"../external/",
		"../external/protobuf/include/",
		"../external/mysql_connector/include/",
		"../proto/",
		"./cpp_cfg/com/",
	}

	SrcPath { 
		"../dbproxy_svr/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../proto/**",
		"./cpp_cfg/**",
	}
	files {
	"../*.txt",
	"../*.lua",
	}
	
    
    

Project "proto"
	files {
	"../proto/*.proto",
	}
	
Project "db_driver"
	IncludeFile { 
		"../external/",
		"../external/protobuf/include/",
		"../proto/",
	}
	files {
		"../db_driver/src/**", 
		"../db_driver/include/**", 
		"../db_driver/*.txt", 
	}
	
Project "test_mysql"
	IncludeFile { 
		"../external/",
		"../external/protobuf/include/",
		"../proto/",
	}
	files {
		"../test/test_mysql/**", 
		"../test/com/**", 
		"../test/test_mysql/*.txt", 
	}





