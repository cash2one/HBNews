#include "burst_mgr.h"

#include "rand.h"
#include "config.h"
#include "str.h"

#include "db_mgr.h"
#include "db_sql.h"
bool CPacket_Burst1::GetBurst( vector<stGameElement> & ret )
{
	ASSERT(eBurstType == EPACKETBURSTTYPE_1);
	//�������ָ����ܵĸ����� һ���ᱬһ���� ��û�и���10w��
	//if (!m_bHasCachedTotalProbity)
	//{
	//	for (vector<stPacketElement*>::iterator it = elements.begin(); it != elements.end() ; ++it)
	//	{
	//		ASSERT(*it);
	//		m_iCacheTotalProbity += (*it)->iProbability;
	//	}
	//	m_bHasCachedTotalProbity = true;
	//}
	//UINT32 retProbity = CRandom::GetInstance()->GetRandom(0, m_iCacheTotalProbity);

	UINT32 retProbality = CRandom::GetInstance()->GetRandom(0, MAXNUM_PROBABILITY);
	UINT32 race = 0;
	for (vector<stPacketElement*>::iterator it = elements.begin(); it != elements.end() ; ++it)
	{
		stPacketElement* ele = *it;
		ASSERT(ele);
		race += ele->iProbability;
		if ( race >= retProbality)//�е��ڵ������
		{
			//�����Ƕ�׵İ� �����
			if (ele->iElementType == EELEMENTTYPE_PACKET)
			{
				UINT32 packet_num = CRandom::GetInstance()->GetRandom(ele->iMinNum, ele->iMAXNum);
				ASSERT(packet_num);
				for (int i = 0 ; i < packet_num ; ++i)
				{
					bool r = CBurstMgr::GetInstance()->GetBurst(ele->iElementID, ret);
					ASSERT(r);
				}
			}
			else{
				UINT32 iNum = CRandom::GetInstance()->GetRandom(ele->iMinNum, ele->iMAXNum);
				if (iNum > 0){
					ret.push_back(stGameElement(ele->iElementType, ele->iElementID, NULL, stGameElement::INCREASE, iNum));
				}
			}
			return true;//��һ������
		}
	}
	return false;
}

bool CPacket_BurstN::GetBurst( vector<stGameElement> & ret )
{
	ASSERT(eBurstType == EPACKETBURSTTYPE_N);
	for (vector<stPacketElement*>::iterator it = elements.begin(); it != elements.end() ; ++it)
	{
		stPacketElement* ele = *it;
		ASSERT(ele);
		UINT32 race = CRandom::GetInstance()->GetRandom(0, MAXNUM_PROBABILITY);
		if (race <= ele->iProbability)//�е��ڵ������
		{
			//�����Ƕ�׵İ� �����
			if (ele->iElementType == EELEMENTTYPE_PACKET)
			{
				UINT32 packet_num = CRandom::GetInstance()->GetRandom(ele->iMinNum, ele->iMAXNum);
				ASSERT(packet_num);
				for (int i = 0 ; i < packet_num ; ++i)
				{
					bool r = CBurstMgr::GetInstance()->GetBurst(ele->iElementID, ret);
					ASSERT(r);
				}
			}
			else{
				UINT32 iNum = CRandom::GetInstance()->GetRandom(ele->iMinNum, ele->iMAXNum);
				if (iNum > 0){
					ret.push_back(stGameElement(ele->iElementType, ele->iElementID, NULL, stGameElement::INCREASE, iNum));
				}
			}
		}
	}
	return true;
}

bool CBurstMgr::GetBurst( UINT32 iPacketID, vector<stGameElement> & ret )
{
	if (iPacketID == 0)
	{
		return false;
	}

	map<UINT32, CPacket*>::iterator it = m_mapPacketID2Packet.find(iPacketID);
	if (it == m_mapPacketID2Packet.end())
	{
		ASSERT(false);
		return false;
	}

	ASSERT(it->second);
	it->second->GetBurst(ret);
	return true;
}

bool CBurstMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//����packet��
	char* sql = SELECT_PACKET;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //��0��ѯʧ��  
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	MYSQL_RES* res = mysql_store_result(con);   //�����ѯ���  

	//����ṹ�Ƿ��иı�
	string packet_need_fields[] = {"id", "packet_id", "burst_type", "element_type_id_arr", "element_probability_arr", "element_num_range_arr"};
	std::vector<std::string> v_packet_need_field(packet_need_fields, packet_need_fields + sizeof(packet_need_fields)/sizeof(packet_need_fields[0]));
	ASSERT(CDBMgr::check_fields(res, v_packet_need_field));

	MYSQL_ROW row;
	while (row = mysql_fetch_row(res))
	{
		int col = 1;
		CPacket *pPacket = NULL;

		//EPACKETBURSTTYPE burst_type = (EPACKETBURSTTYPE)atoi(row[2]);
		//if (burst_type == EPACKETBURSTTYPE_1)
		//{
		//	pPacket = new CPacket_Burst1;
		//}
		//else if(burst_type == EPACKETBURSTTYPE_N){
		//	pPacket = new CPacket_BurstN;
		//}
		//else{
		//	printf("����ȷ��packet����");
		//	ASSERT(false);
		//	return false;
		//}

		int burst_type = atoi(row[2]);
		EPACKETBURSTTYPE e_burst_type;
		//3=��Nѡ1�����и��ʶ�һ��������������Ҫ���4="NѡN,�ر�������������Ҫ�
		switch (burst_type)
		{
		case 1:
		case 3:
			e_burst_type = EPACKETBURSTTYPE_1;
			pPacket = new CPacket_Burst1;
			break;
		case 2:
		case 4:
			e_burst_type = EPACKETBURSTTYPE_N;
			pPacket = new CPacket_BurstN;
			break;
		default:
			ASSERT(false && "��֧�ֵİ����ͣ�");
			return false;
		}

		pPacket->iPacketID = atoi(row[col++]);

		//pPacket->eBurstType = (EPACKETBURSTTYPE)atoi(row[col++]);

		col++;
		pPacket->eBurstType = e_burst_type;

		char* pBuf = UTF8ToANSI(row[col++]);
		vector<string> type_id_arr;
		split_string(string(pBuf), type_id_arr, "|");
		free(pBuf);

		pBuf = UTF8ToANSI(row[col++]);
		vector<int> probability_arr;
		split_string_2i(string(pBuf), probability_arr, "|");
		free(pBuf);

		pBuf = UTF8ToANSI(row[col++]);
		vector<string> num_range_arr;
		split_string(string(pBuf), num_range_arr, "|");
		free(pBuf);

		if(burst_type == 3 || burst_type == 4){////3 ��NѡN�ұص� ��ʱ���ʿ��Բ���
			ASSERT( type_id_arr.size() == num_range_arr.size());
		}
		else{
			ASSERT(type_id_arr.size() == probability_arr.size() && type_id_arr.size() == num_range_arr.size());
		}

		for (UINT32 i = 0; i < type_id_arr.size(); ++i)
		{
			vector<int> type_id;
			split_string_2i(type_id_arr[i], type_id, "-");
			
			ASSERT(type_id.size() == 2);
			ASSERT(type_id[0] > EELEMENTTYPE_MIN && type_id[0] < EELEMENTTYPE_MAX);
			//todo �жϾ������������Ƿ����

			vector<int> num_range;
			split_string_2i(num_range_arr[i], num_range, "-");
			ASSERT(num_range.size() == 2);
			
			stPacketElement* ele = new stPacketElement;
			ele->iElementType = (EELEMENTTYPE)type_id[0];
			ele->iElementID = type_id[1];

			if(burst_type == 3){//3 ��Nѡ1�����и��ʶ�һ��������������Ҫ��
				if (i !=  (type_id_arr.size() -1))
				{
					ele->iProbability = MAXNUM_PROBABILITY/type_id_arr.size();
				}
				else{//���һ������
					ele->iProbability = MAXNUM_PROBABILITY - (type_id_arr.size()-1)*(MAXNUM_PROBABILITY/type_id_arr.size());
				}
			}
			else if(burst_type == 4){//NѡN,�ر�������������Ҫ��
				ele->iProbability = MAXNUM_PROBABILITY;
			}
			else{
				ele->iProbability = probability_arr[i];
			}
			ele->iMinNum = num_range[0];
			ele->iMAXNum = num_range[1];

			/*
			���  9:20:12
			����������burstpacket��ʱ����һ���жϣ����ǰ������װ�����ʽ������ı�ű�������ڰ��ı�š���Ȼ�ڵ������ݵ�ʱ������������Ŀ���Ǳ�������ʱ��������a��b�������������Ȼ���γ���ѭ���������
			���  9:22:11
			�о�û˵����������и����󣬡���������ڰ�����һ�������������ߵı�ű���С��ǰ�ߣ�����С�ڣ�����ڵ�������ʱ����
			*/
			if (ele->iElementType == EELEMENTTYPE_PACKET)
			{
				ASSERT(pPacket->iPacketID > ele->iElementID);//
			}

			pPacket->elements.push_back(ele);

		}
		
		ASSERT(AddPacket(pPacket));
	}
	mysql_free_result(res);
	return true;
}