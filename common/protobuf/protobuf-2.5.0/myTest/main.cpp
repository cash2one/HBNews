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

	p.SerializeToString(&str);//���������л����ַ����������⻹�������л���fstream��
	printf("%s\n", str.c_str());
	Person x;
	x.ParseFromString(str);//���ַ��������л�                          
	printf("x.name=%s\n", x.name().c_str());//������������tom��˵�������л���ȷ
	return 0;                                                                        
}