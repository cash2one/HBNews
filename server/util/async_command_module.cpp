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
	ASSERT(m_thread);//�̱߳����Ѿ�������

	if (comandList.size() > 100000){
		//д����־��
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
			//�������д������ ������PickOffCommand�� ��ô����OnExecute��addCommnd�ͻᵼ��������
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
			//�������д������ ������PickOffCommand�� ��ô����OnExecute��addCommnd�ͻᵼ��������
			//command = asynModule->PickOffCommand();
			
			UINT32 combine_num = 0;
			sql.clear();
			uv_mutex_lock(&asynModule->m_mutex);//����
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
						//��������ܶ�ʱ sql����̫��ƴ��̫�����ƴ��̫��ʱ�䡣
						if(sql.length() >=  1048576 || ++combine_num >= 1000 || (Now() - last_excute_time) >= 30){
							break;
						}
					}
					else{
						//���ǰ����sql�� �����break ���Ҳ�pop �ȴ���һ��ѭ�������������combine������
						if (!sql.empty()){
							break;
						}
						asynModule->comandList.pop();
						uv_mutex_unlock(&asynModule->m_mutex);//���� ��ȻҲ���Բ�Ҫgoto���Ž��� ���ǱϾ�OnExecute���ܾܺõ���һֱ���� ����ʱ�����̲߳���add������

						command->OnExecute();
						last_excute_time = Now();
						delete command;

						goto END_PROCESS_CANNOT_COMBINE;
					}
				}

				uv_mutex_unlock(&asynModule->m_mutex);//����
				if (!sql.empty()){
					int r = CDBMgr::Query(con, sql);
					ASSERT(r == 0);
				}

END_PROCESS_CANNOT_COMBINE:

				;
			}
			else if(!asynModule->m_shouldStop){
				uv_mutex_unlock(&asynModule->m_mutex);//����
				// todo sleep
				Sleep(50);
			}
			else{
				uv_mutex_unlock(&asynModule->m_mutex);//����
				break;
			}
		}
	}
	else{
		ASSERT(false && "����ȷ�Ĺ�����ʽ");
	}
}