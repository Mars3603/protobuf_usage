#include "MyTest.h"
#include "Person.pb.h"

void MyTest::test()
{
    // 序列化
    Erbing::Person p;
    p.set_id(10);
    p.set_age(32);
    p.set_sex("man");

    p.add_name();
    p.set_name(0,"lufei");
    p.add_name("ais");
    p.add_name("sabo");
    p.mutable_addr()->set_addr("BeiJing");
    p.mutable_addr()->set_num(1001);
    p.set_color(Erbing::Color::Blue);

    // 序列化对象 p, 最终得到一个字符串
    std::string output;
    p.SerializeToString(&output);

    // 反序列化数据
    Erbing::Person pp;
    pp.ParseFromString(output);
    std::cout << pp.id()  << ", " << pp.sex() << ", " << pp.age() << std::endl;
    std::cout << pp.addr().addr() << ", " << pp.addr().num() << std::endl;
    int size = pp.name_size();
    for(int i=0; i<size; ++i)
    {
        std::cout << pp.name(i) << std::endl;
    }
    std::cout << pp.color() << std::endl;
}