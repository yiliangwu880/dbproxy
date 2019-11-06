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
		"../proto/",
	}

	SrcPath { 
		"../dbproxy_svr/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../proto/**",
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
	}