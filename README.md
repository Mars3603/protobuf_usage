# protobuf_usage

protobuf
Latest就是最新的稳定版本。

大致说明：
ProtoBuf是一种序列化数据结构的协议，方便将程序中的数据进行序列化和反序列化。
grpc是网络通信框架，在网络通信时，你也会发送和接收数据。收发的数据使用protobuf进行编解码（序列化和反序列化）
相比 Json 和 xml 有对应的优势。


protobuf 消息体 中的 repeated
使用repeated关键字定义的字段在Protobuf序列化和反序列化时会被当作一个集合或数组（容器）来处理。
使用示例：
message Example {
  repeated string names = 1;
}

使用时
Example example;
example.add_names("Alice");
example.add_names("Bob");

访问元素：
std::string firstName = example.names(0); // 获取第一个名字

获取数量：
int count = example.names_size(); // 获取 names 的数量

清空：
example.clear_names(); // 清空 names

这些API在生成的  Example.pb.h 中可以看到。

总结，使用 repeated 关键字可以当成容器来使用。



根据protobuf的语法, 编辑.proto文件
生成 protobuf 可执行程序时，是需要用到 libprotobuf.so 这种提供的API的，所以需要链接对应的库。


protobuf 的编译方式

待学习： protobuf 和 grpc 的配套使用。

$ protoc --version
protoc: error while loading shared libraries: libprotoc.so.32: cannot open shared object file: No such file or directory

/etc/ld.so.conf 文件用于配置动态链接器（ld.so）的库搜索路径。当你运行一个程序时，动态链接器需要找到它所依赖的共享库。这个文件列出了系统在查找共享库时需要搜索的目录。





