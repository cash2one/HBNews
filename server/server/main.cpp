#include <iostream>
#include <fstream>
#include "person.pb.h"
//#include "tchar.h"
using namespace std;

#pragma comment (lib, "libprotobuf.lib")


//int _tmain(int argc, _TCHAR* argv[])

#include "gs_c_net_mgr.h"

#include "map_mgr.h"

#include "rand.h"
int main(int argc)
{
	//GOOGLE_PROTOBUF_VERIFY_VERSION;

	/*
	Person person;
	person.set_id(8888);
	person.set_name("Kobe");
	person.set_email("1990@qq.com");

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


	for (int i = 0 ; i < 100 ; ++i)
	{
		printf("%d \n", CRandom::GetInstance()->GetRandom(0,10));
	}

	return 1;
	
	CMapMgr::GetInstance()->Init();

	CMapMgr::GetInstance()->UnInit();

	CNetMgr::GetInstance()->Init();

	while (true)
	{
		CNetMgr::GetInstance()->MessageLoop();
	}
	
	CNetMgr::GetInstance()->UnInit(); 

	return 0;
}