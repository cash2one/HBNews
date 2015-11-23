#pragma comment(lib, "libprotobuf.lib")  
#pragma comment(lib, "libprotoc.lib")  

#include "1111.pb.h"

int main()
{
	using namespace x;
	Person p;
	p.set_name("tom");
	p.set_id(88);
	p.set_email("xx@xx.com");
	std::string str;

	p.SerializeToString(&str);//将对象序列化到字符串，除此外还可以序列化到fstream等
	printf("%s\n", str.c_str());
	Person x;
	x.ParseFromString(str);//从字符串反序列化                          
	printf("x.name=%s\n", x.name().c_str());//这里的输出将是tom，说明反序列化正确
	return 0;                                                                        
}