#ifndef item_h__
#define item_h__

#include <vector>
#include <string>

#include "config.h"
#include "def.h"
class struct_game_element;
class IItem{
	public:
		virtual ~IItem(){};
		virtual UINT32 GetNum()	= 0;
		virtual	UINT32 GetOverlapNum()	= 0;
		virtual UINT32 GetId() = 0;
		virtual void   SetId(UINT32 iItemId) = 0;
		virtual UINT16 GetQuality() = 0;                                                          
		virtual void   AddNum(UINT16 iNum = 1) = 0;
		virtual void   DecNum(UINT16 iNum = 1) = 0;
		virtual void   SetNum(UINT16 iNum) = 0;
};


class CItem : public IItem{
public:
	CItem(struct_game_element* pElement);

	virtual UINT32 GetNum();
	virtual void   AddNum(UINT16 iNum = 1);
	virtual void   DecNum(UINT16 iNum = 1);
	virtual void   SetNum(UINT16 iNum);
	
	virtual UINT32 GetId();
	virtual void   SetId(UINT32 iItemId);


	virtual	UINT32 GetOverlapNum();
	virtual UINT16 GetQuality();
private:
	struct_game_element* m_pItem;
	UINT16 m_iOverlapNum;
	UINT16 m_iQuality;
};

class CItemCfg{
public:
	enum EUseType{
		MIN = 0,
		EVOLUTION_MATERIAL = 1,	//1.��������
		EXP = 2,				//2.����ֵ����
		SPEED_KILL_TICKET = 3,	//3.ɨ��ȯ
		COMMON = 4,				//4.����
		MATERIAL_FRAGMENT = 5,	//5.������Ƭ
		HERO_FRAGMENT = 6,		//6.Ӣ����Ƭ
		HEALTH_POTION = 7,		//7.����ҩˮ
		GIFT = 8,				//8.��� 
		OTHER = 9,				//9.����������ߣ�
		MAX = 10
	};

	UINT32 m_iID;			//itemID
	EUseType  m_eUseType;	//ʹ������
	std::string m_name;		//����
	UINT8	m_iQuality;		//Ʒ��
	UINT32 m_iOverlapNum;	//���ɶѵ�����
	UINT32 m_iSellGold;		//���۽��
	std::vector<stGameElement*> m_use_output;	//ʹ��֮��Ĳ���
	std::vector<stGameElement*> m_compose_input;	//�ϳ�����Ҫ������
};

#endif // item_h__

