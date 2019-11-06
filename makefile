#功能：一个命名生成目标文件， 保证依赖关系正确



PROTO_FILE=$(wildcard\
		./proto/*.proto\
		)

PROTO_CPP := $(patsubst %.proto,%.pb.h, $(PROTO_FILE))
		
TARGET = ./bin/dbproxy_svr


all: $(TARGET) $(PROTO_CPP)
	sh build_cmake.sh

$(PROTO_CPP) : $(PROTO_FILE)	
	./proto/proto_2_cpp.sh

	
	
clean:
	rm -f $(TARGET)








