#ifndef async_command_module_h__
#define async_command_module_h__

//#include <list>
#include <queue>
#include <string>
#include "uv.h"

#include "def.h"

class ICommand
{
public:
	virtual ~ICommand(){};
	virtual void OnExecute() = 0;			//�����mysql�������ڴ˺�����
	virtual bool GetSql(std::string& strAppendHere) = 0;				//��ȡҪִ�е�sql ����ϲ����� ����Բ�ִ��OnExecute ���Լ��ֶ�ִ��
	virtual bool CanCombine() = 0;			//�Ƿ���Ժϲ�����  ��ִֻ��sql��� �����ð�sql����������ģ��ʱ ����combine
};

class CAsyncCommandModule
{
public:

	enum EWorkType{
		MIN = 0,
		COMMOND_SINGLE = 1,			//����ִ�� ÿ���������Լ���onexcete
		COMMOND_TRY_COMBINE = 2,	//�����ϲ����Ժϲ������һ��ִ�� ��ִ��ÿһ�������onexecute
		MAX = 3
	};

	~CAsyncCommandModule();

	static CAsyncCommandModule *GetInstance();
	
	bool Init(EWorkType eWorkType){
		m_thread = NULL;
		m_shouldStop = false;
		//comandList.reserve(1024*4);

		m_eWorkType = eWorkType;
		return true;
	}

	bool Start();

	bool Join();

	void AddCommand(ICommand* command);

private:
	CAsyncCommandModule();

	static void ProcessCommond(void* arg);

	ICommand* PickOffCommand();
	static CAsyncCommandModule *instance;
	//std::list<ICommand*> comandList;
	std::queue<ICommand*> comandList;


	bool m_shouldStop;
	uv_mutex_t m_mutex;
	uv_thread_t m_thread;
	static EWorkType m_eWorkType;
};

#endif // async_command_module_h__