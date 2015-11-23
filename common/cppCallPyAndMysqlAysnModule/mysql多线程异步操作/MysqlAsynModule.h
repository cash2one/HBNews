#ifndef __MYSQLASYNMODULE_H__
#define __MYSQLASYNMODULE_H__

#include <list>


class CCommand;
class CMysqlAsynModule
{
public:
	void addCommand(CCommand* command);
	CCommand* getCommand();
	static CMysqlAsynModule *getInstance();
	virtual ~CMysqlAsynModule();
	
	
private:
	CMysqlAsynModule();

	static CMysqlAsynModule *instance;
	std::list<CCommand*> comandList;
};

#endif