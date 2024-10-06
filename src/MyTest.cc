#include "MyTest.h"
#include "Person.pb.h"

void MyTest::test()
{
    // ���л�
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

    // ���л����� p, ���յõ�һ���ַ���
    std::string output;
    p.SerializeToString(&output);

    // �����л�����
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