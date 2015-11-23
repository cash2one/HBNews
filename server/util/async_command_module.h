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
	virtual void OnExecute() = 0;			//具体的mysql操作放在此函数中
	virtual bool GetSql(std::string& strAppendHere) = 0;				//获取要执行的sql 如果合并起来 则可以不执行OnExecute 由自己手动执行
	virtual bool CanCombine() = 0;			//是否可以合并起来  当只执行sql语句 而不用把sql结果传给别的模块时 可以combine
};

class CAsyncCommandModule
{
public:

	enum EWorkType{
		MIN = 0,
		COMMOND_SINGLE = 1,			//单挑执行 每个都调用自己的onexcete
		COMMOND_TRY_COMBINE = 2,	//尽量合并可以合并的命令，一起执行 不执行每一条命令的onexecute
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