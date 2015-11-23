#include "person.pb.h"
//#include "tchar.h"

//#include <stdio.h>
//#include <iostream>
using namespace std;

#pragma comment (lib, "libprotobuf.lib")


int main(int argc)
{
	//GOOGLE_PROTOBUF_VERIFY_VERSION;

	Person person;
	person.set_id(8888);
	person.set_name("Kobe");
	person.set_email("1990@qq.com");

	/*
	//将数据写到person.pb文件
	fstream out("person.xml", ios::out | ios::binary | ios::trunc);
	person.SerializeToOstream(&out);
	out.close();

	person.SerializeToString();
	person.SerializeToArray();

	person.ParseFromArray()

	//从person.pb文件读取数据
	fstream in("person.xml", ios::in | ios::binary);
	if (!person.ParseFromIstream(&in)) {
	cerr << "Failed to parse person.xml." << endl;
	exit(1);
	}

	cout << "ID: " << person.id() << endl;
	cout << "name: " << person.name() << endl;
	if (person.has_email()) {
	cout << "e-mail: " << person.email() << endl;
	}

	getchar();
	*/

	int len = person.ByteSize();
	char *buffer = new char[len];
	person.SerializeToArray(buffer, len);

	Person p2;
	p2.ParseFromArray(buffer, len);


	return 0;
}