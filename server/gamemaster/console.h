#ifndef console_h__
#define console_h__

#include "uv.h"

#include <vector>
class IConsoleCommond{


};

class CConsole{
public:
	
	bool Start();

	bool Join();

	static void run(void* arg);

	static bool ProcessCommond( std::vector<std::string> &args, std::string &ret_info);
	static bool ProcessCommond( std::string &str_commond, std::string &ret_info);
private:
	uv_thread_t m_thread;

};

#endif // console_h__