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

	//>>> ��̬����
	Guid client_id;		//����GS������id û������ʱ ΪĬ�Ϲ���� GUID()
	area::EAreaStatus status ;//����������״��
};

#endif // area_h__