#ifndef __COMMAND_H__
#define __COMMAND_H__


class CCommand
{
public:
			
	virtual void onExecute() = 0;			//具体的mysql操作放在此函数中
};

#endif