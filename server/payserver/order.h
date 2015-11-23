#ifndef order_h__
#define order_h__

#include "commodity_mgr.h"

//����
class COrder{

public:
	enum ESTATUS{
		MIN = 0,
		JUST_PAIED = 1,			//�յ��Ѿ�֧���ɹ���֪ͨ
		GS_PROCESING = 2,		//����GS���� ��δ�ظ�
		GS_PROCESS_SUCCESS = 3,	//GS����ɹ���������Ʒ
		GS_PROCESS_FAIL = 4,	//GS����ʧ��
		MAX = 5
	};

	COrder(CCommodity::EPLANTFROM plantform_id,
	const std::string order_id,
	UINT16 area_id,
	UINT32 user_id,
	UINT32 create_time,
	ESTATUS status,
	UINT32 commodity_id,
	UINT32 commodity_num,
	float present_price,
	float original_price){
		m_plantform_id = plantform_id;
		m_order_id = order_id;
		m_area_id = area_id;
		m_user_id = user_id;
		m_create_time = create_time;
		m_status = status;
		m_commodity_id = commodity_id;
		m_commodity_num = commodity_num;
		m_present_price = present_price;
		m_original_price = original_price;
	}

	CCommodity::EPLANTFROM m_plantform_id;
	std::string m_order_id;
	UINT16 m_area_id;//������id ��id
	UINT32 m_user_id;//���id �˻�id  һ���˻����Խ������
	UINT32 m_create_time;
	ESTATUS m_status;
	UINT32 m_commodity_id;
	UINT32 m_commodity_num;
	float m_present_price;
	float m_original_price;
};

#endif // order_h__

