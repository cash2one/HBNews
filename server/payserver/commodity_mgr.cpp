#include <string>
#include "commodity_mgr.h"
#include "db_mgr.h"
#include "str.h"
bool CCommodityMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	char sql[512] = {0};
	sprintf_s(sql, sizeof sql, "SELECT * FROM `commodity`");

	int r = CDBMgr::Query(con, sql, strlen(sql));
	if (r != 0){
		ASSERT(false);
	}

	int i = 0;
	do{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++){
		case 0:{//commodity
				   {
					   //检查表结构是否有改变
					   std::string need_fields[] = {"id", "pay_plantform", "priority", "name", "present_price", "original_price", "type_id_num_arr"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res)){

					   int col = 0;//
					   UINT32 id = atoi(row[col++]);
					   ASSERT(m_mapCCommodity.find(id) == m_mapCCommodity.end());
					   CHECK_FALSE_RETURN_FALSE(m_mapCCommodity.find(id) == m_mapCCommodity.end());

					   CCommodity::EPLANTFROM ePlantform = (CCommodity::EPLANTFROM)atoi(row[col++]);
					   ASSERT(ePlantform >  CCommodity::EPLANTFROM::MIN && ePlantform <  CCommodity::EPLANTFROM::MAX);
					   CHECK_FALSE_RETURN_FALSE(ePlantform >  CCommodity::EPLANTFROM::MIN && ePlantform <  CCommodity::EPLANTFROM::MAX);

					   CCommodity* pItem = new CCommodity;
					   pItem->id = id;
					   pItem->pay_plantform = ePlantform;
					   pItem->priority = atoi(row[col++]);
					   pItem->name = row[col++];
					   pItem->present_price = atof(row[col++]);
					   pItem->original_price = atof(row[col++]);

					   std::vector<std::string> eles;
					   split_string(std::string(row[col++]), eles, "|");
					   ASSERT(eles.size());
					   for (std::vector<std::string>::iterator it = eles.begin(); it != eles.end(); ++it){
						   std::vector<int> ele;
						   split_string_2i(*it, ele, "-");
						   ASSERT(ele.size() == 3);
						   CHECK_FALSE_RETURN_FALSE(ele.size() == 3);
						   pItem->eles.push_back(stGameElement(ele[0], ele[1], ele[2]));
					   }

					   m_mapCCommodity[pItem->id] = pItem;
					   m_mapPlantformCCommodity[pItem->pay_plantform].push_back(pItem);
				   }

			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

CCommodity* CCommodityMgr::GetCommodityByID( UINT32 iCommodityID )
{
	std::map<UINT32, CCommodity*>::iterator it = m_mapCCommodity.find(iCommodityID);
	if (it != m_mapCCommodity.end()){
		return it->second;
	}

	return NULL;
}

std::vector<CCommodity*>& CCommodityMgr::GetCommodityOfPlantform( CCommodity::EPLANTFROM ePlantform )
{
	static std::vector<CCommodity*> s_empty_list;

	std::map<CCommodity::EPLANTFROM, std::vector<CCommodity*>>::iterator it = m_mapPlantformCCommodity.find(ePlantform);
	if (it != m_mapPlantformCCommodity.end()){
		return it->second;
	}

	return s_empty_list;
}

