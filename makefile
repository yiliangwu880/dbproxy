#功能：一个命名生成目标文件， 保证依赖关系正确



PROTO_FILE=$(wildcard\
		./proto/*.proto\
		)

PROTO_CPP := $(patsubst %.proto,%.pb.h, $(PROTO_FILE))
		
TARGET = ./bin/dbproxy_svr/dbproxy_svr


all: $(TARGET) $(PROTO_CPP)
	sh build_cmake.sh

#不需要指令，目的让TARGET文件不存在也能编译
$(TARGET) :$(PROTO_CPP) 
 
#||左边的命令（command1）未执行成功, (会返回非零值),会执行exit
$(PROTO_CPP) : $(PROTO_FILE)	
	./proto/proto_2_cpp.sh || exit $$?;  
	
clean:
	rm -f $(TARGET)








