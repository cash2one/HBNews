#include "client_for_as.h"
#include "client_for_gs.h"
#include "user_mgr.h"
#include "db_async_command.h"
#include "db_mgr.h"

int main(){

	////单个用户的时候
	//CClientForAS::GetInstance()->Init();
	//CClientForAS::GetInstance()->Start();
	//CClientForAS::GetInstance()->Join();
	////gs的链接在as登陆成功之后！

//多个用户的时候
	//多个用户的时候

	CAsyncCommandModule::GetInstance()->Init(CAsyncCommandModule::COMMOND_SINGLE);
	CAsyncCommandModule::GetInstance()->Start();

	std::vector<CClientForAS*> &vAllusers_as = CUserMgr::GetAllASUsers();
	UINT32 iNum = 1;//1000
	for(UINT32 i = 0; i < iNum ; ++i){
		CClientForAS* client_as = new CClientForAS;
		char username[32] = "iccfdfsdds";
		char password[32] = "111111";

		/*	itoa(2, username, 10);
		itoa(2, password, 10);*/

		client_as->Init();
		client_as->SetUserPwd(std::string(username), std::string(password));
		client_as->Start();
		vAllusers_as.push_back(client_as);
	}

	//等待结束
	for (std::vector<CClientForAS*>::iterator it = vAllusers_as.begin(); it != vAllusers_as.end(); ++it){
		(*it)->Join();
	}

	//等待结束
	std::vector<CClientForGS*> &vAllusers_gs = CUserMgr::GetAllGSUsers();
	for (std::vector<CClientForGS*>::iterator it = vAllusers_gs.begin(); it != vAllusers_gs.end(); ++it){
		(*it)->Join();
	}

	CAsyncCommandModule::GetInstance()->Join();

	CDBMgr::GetInstance()->UnInit();
	
	return 0;
}