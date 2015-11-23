#ifndef sign_mgr_h__
#define sign_mgr_h__
#include <map>
#include "def.h"

struct stGameElement;
class CSignMgr
{
public:
	static CSignMgr* GetInstance(){
		static CSignMgr singleton;
		return &singleton;
	}

	bool Init();
	
	//���ݴ�����ȡ������Ϣ
	stGameElement* GetSign1Reward(UINT32 iYear, UINT32 iMonth, UINT32 iTimes);

	//�����ۼ�ǩ�����ͻ�ȡ������Ϣ
	stGameElement* GetSignNReward(UINT32 iYear, UINT32 iMonth, UINT32 iType);

private:
	CSignMgr(void){};
	std::map<std::pair<UINT32, UINT32>, std::vector<stGameElement>> m_mapSign1Time2Rewards;//first : ���� second:ÿ�ս�������
	std::map<std::pair<UINT32, UINT32>, std::vector<stGameElement>> m_mapSignNTime2Rewards;//first: ���� second:�ۼ�ǩ����������

};

#endif