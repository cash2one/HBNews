#include "burst_mgr.h"

#include "rand.h"
#include "config.h"
#include "str.h"

#include "db_mgr.h"
#include "db_sql.h"
bool CPacket_Burst1::GetBurst( vector<stGameElement> & ret )
{
	ASSERT(eBurstType == EPACKETBURSTTYPE_1);
	//下面这种根据总的概率来 一定会爆一个的 而没有根据10w来
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
		if ( race >= retProbality)//有等于的情况！
		{
			//如果是嵌套的包 则爆落包
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
			return true;//爆一个即可
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
		if (race <= ele->iProbability)//有等于的情况！
		{
			//如果是嵌套的包 则爆落包
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

	//解析packet表！
	char* sql = SELECT_PACKET;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	MYSQL_RES* res = mysql_store_result(con);   //保存查询结果  

	//检查表结构是否有改变
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
		//	printf("不正确的packet类型");
		//	ASSERT(false);
		//	return false;
		//}

		int burst_type = atoi(row[2]);
		EPACKETBURSTTYPE e_burst_type;
		//3=“N选1，所有概率都一样，概率栏不需要填”，4="N选N,必爆，概率栏不需要填”
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
			ASSERT(false && "不支持的包类型！");
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

		if(burst_type == 3 || burst_type == 4){////3 ：N选N且必掉 此时概率可以不填
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
			//todo 判断具体的物件配置是否存在

			vector<int> num_range;
			split_string_2i(num_range_arr[i], num_range, "-");
			ASSERT(num_range.size() == 2);
			
			stPacketElement* ele = new stPacketElement;
			ele->iElementType = (EELEMENTTYPE)type_id[0];
			ele->iElementID = type_id[1];

			if(burst_type == 3){//3 ：N选1，所有概率都一样，概率栏不需要填
				if (i !=  (type_id_arr.size() -1))
				{
					ele->iProbability = MAXNUM_PROBABILITY/type_id_arr.size();
				}
				else{//最后一个补齐
					ele->iProbability = MAXNUM_PROBABILITY - (type_id_arr.size()-1)*(MAXNUM_PROBABILITY/type_id_arr.size());
				}
			}
			else if(burst_type == 4){//N选N,必爆，概率栏不需要填
				ele->iProbability = MAXNUM_PROBABILITY;
			}
			else{
				ele->iProbability = probability_arr[i];
			}
			ele->iMinNum = num_range[0];
			ele->iMAXNum = num_range[1];

			/*
			马聪  9:20:12
			我想让你做burstpacket的时候做一个判断，就是包里面套包的形式，外包的编号必须大于内包的编号。不然在导入数据的时候会出错。这样的目的是避免填表的时候发生诸如a、b两个包互相包含然后形成死循环的情况。
			马聪  9:22:11
			感觉没说清楚，就是有个需求，“若掉落包内包含另一个掉落包，则后者的编号必须小于前者，若不小于，则会在导入数据时报错”
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