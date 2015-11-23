#ifndef area_h__
#define area_h__

#include <string>
#include "as_c_proto.pb.h"
#include "guid.h"
struct CArea{
	UINT32	area_id;
	std::string	name;
	std::string	ip;
	UINT32	port;
	bool	isHidden;

	//>>> 动态数据
	Guid client_id;		//各个GS的连接id 没有链接时 为默认构造的 GUID()
	area::EAreaStatus status ;//服务器运行状况
};

#endif // area_h__