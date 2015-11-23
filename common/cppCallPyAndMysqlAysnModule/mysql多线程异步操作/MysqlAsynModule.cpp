#include "MysqlAsynModule.h"


CMysqlAsynModule *CMysqlAsynModule::instance = NULL;


CMysqlAsynModule::CMysqlAsynModule(){}

CMysqlAsynModule *CMysqlAsynModule::getInstance()
{
	if (NULL == instance)
	{
		instance = new CMysqlAsynModule;
	}

	return instance; 
}

CMysqlAsynModule::~CMysqlAsynModule()
{
	if (instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}

void CMysqlAsynModule::addCommand(CCommand* command)
{
	if (comandList.size() > 100000)
	{
		//写到日志中
	}

	comandList.push_back(command);
}

CCommand* CMysqlAsynModule::getCommand()
{
	CCommand* command = NULL;

	if (comandList.size() != 0)
	{
		command = comandList.front();
		comandList.pop_front();
	}
	
	return command;
}