echo "==============export proto cpp=================="
cd `dirname $0` #进入当前脚本文件存放个目录($0为指向脚本命令第一个参数，dirname `取路径)
../external/protobuf/bin/protoc --cpp_out=./ -I=./ ./*.proto  #-I为include目录
exit $?