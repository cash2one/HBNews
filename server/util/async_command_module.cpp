#include "async_command_module.h"
#include "time_helper.h"
#include "db_mgr.h"

CAsyncCommandModule::EWorkType CAsyncCommandModule::m_eWorkType = CAsyncCommandModule::MIN;

CAsyncCommandModule *CAsyncCommandModule::instance = NULL;

CAsyncCommandModule::CAsyncCommandModule(){}

CAsyncCommandModule *CAsyncCommandModule::GetInstance()
{
	if (NULL == instance)
	{
		instance = new CAsyncCommandModule;
	}

	return instance; 
}

CAsyncCommandModule::~CAsyncCommandModule()
{
	if (instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}

void CAsyncCommandModule::AddCommand(ICommand* command)
{
	ASSERT(m_thread);//线程必须已经启动！

	if (comandList.size() > 100000){
		//写到日志中
	}

	uv_mutex_lock(&m_mutex);
	//comandList.push_back(command);
	comandList.push(command);
	uv_mutex_unlock(&m_mutex);
}

ICommand* CAsyncCommandModule::PickOffCommand()
{
	ICommand* command = NULL;

	uv_mutex_lock(&m_mutex);
	if (comandList.size() != 0){
		command = comandList.front();
		//comandList.pop_front();
		comandList.pop();
	}
	uv_mutex_unlock(&m_mutex);

	return command;
}

bool CAsyncCommandModule::Start()
{
	int r = uv_mutex_init(&m_mutex);
	ASSERT(r == 0);
	r = uv_thread_create(&m_thread, CAsyncCommandModule:: ProcessCommond, this);
	ASSERT(r == 0);
	return r == 0;
}

bool CAsyncCommandModule::Join()
{
	m_shouldStop = true;
	int r = uv_thread_join(&m_thread);
	ASSERT(r == 0);
	uv_mutex_destroy(&m_mutex);
	return r == 0;
}

void CAsyncCommandModule::ProcessCommond( void* arg )
{
	ICommand* command = NULL;

	CAsyncCommandModule* asynModule = (CAsyncCommandModule*)arg;
	ASSERT(asynModule);

	if (m_eWorkType == COMMOND_SINGLE){
		while (true)
		{
			//假如把锁写在这里 而不是PickOffCommand中 那么假如OnExecute中addCommnd就会导致死锁！
			command = asynModule->PickOffCommand();
			if (command)
			{
				command->OnExecute();
				delete command;
			}
			else if(!asynModule->m_shouldStop){
				// todo sleep
				Sleep(50);
			}
			else{
				break;
			}
		}
	}
	else if (m_eWorkType == COMMOND_TRY_COMBINE){

		std::string sql;
		ICommand* command = NULL;
		static UINT32 last_excute_time = Now();

		MYSQL* con = CDBMgr::GetInstance()->GetConnection();
		ASSERT(con);

		while (true)
		{
			//假如把锁写在这里 而不是PickOffCommand中 那么假如OnExecute中addCommnd就会导致死锁！
			//command = asynModule->PickOffCommand();
			
			UINT32 combine_num = 0;
			sql.clear();
			uv_mutex_lock(&asynModule->m_mutex);//加锁
			//std::vector<ICommand*> v_combines;
			if (asynModule->comandList.size() != 0){

				while(true){

					if (asynModule->comandList.size() == 0){
						break;
					}

					command = asynModule->comandList.front();
					ASSERT(command);
				
					if(command->CanCombine()){
						asynModule->comandList.pop();

						bool r = command->GetSql(sql);
						delete command;
						ASSERT(r);
						//避免命令很多时 sql长度太长拼接太多或者拼接太长时间。
						if(sql.length() >=  1048576 || ++combine_num >= 1000 || (Now() - last_excute_time) >= 30){
							break;
						}
					}
					else{
						//如果前面有sql了 这里就break 并且不pop 等待下一次循环处理这个不能combine的命令
						if (!sql.empty()){
							break;
						}
						asynModule->comandList.pop();
						uv_mutex_unlock(&asynModule->m_mutex);//解锁 虽然也可以不要goto等着解锁 但是毕竟OnExecute可能很久导致一直锁着 而此时其他线程不能add命令了

						command->OnExecute();
						last_excute_time = Now();
						delete command;

						goto END_PROCESS_CANNOT_COMBINE;
					}
				}

				uv_mutex_unlock(&asynModule->m_mutex);//解锁
				if (!sql.empty()){
					int r = CDBMgr::Query(con, sql);
					ASSERT(r == 0);
				}

END_PROCESS_CANNOT_COMBINE:

				;
			}
			else if(!asynModule->m_shouldStop){
				uv_mutex_unlock(&asynModule->m_mutex);//解锁
				// todo sleep
				Sleep(50);
			}
			else{
				uv_mutex_unlock(&asynModule->m_mutex);//解锁
				break;
			}
		}
	}
	else{
		ASSERT(false && "不正确的工作方式");
	}
}