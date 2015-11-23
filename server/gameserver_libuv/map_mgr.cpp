#include "map_mgr.h"

#include"libxl.h"
using namespace libxl;

#include "str.h"
#include "gs_c_proto.pb.h"

//#include "libxl_helper.h"
#include "db_mgr.h"
#include "db_sql.h"

//bool CMapMgr::Init()
//{
//	/*
//	CMapPoint* a = new CMapPoint();
//	a->iMappointID = 1;
//	mMapMapPointID2MapPoint[1] = a;
//	return true;
//	*/
//
//	
//	libxl::IBookT<wchar_t>* book;
//	bool retLoad;
//	/*int i=0;
//	cin>> i;
//	if(i!=0)
//	{*/
//		book = xlCreateBookW();
//		retLoad=book->load(L"E:/WH/doc/配置表/剧情副本数据表.xls");
//	/*}
//	else
//	{
//		book = xlCreateXMLBookW();
//		retLoad=book->load(L"E:/WH/doc/配置表/剧情副本数据表.xls");
//	}*/
//
//	if(retLoad)
//	{
//		libxl::Sheet * sheet = book->getSheet(0);
//		if(sheet){
//			/*
//			libxl::CellType celltype = sheet->cellType(0,0);
//			switch(celltype)
//			{
//			case CELLTYPE_NUMBER:
//			{
//			double intmy =sheet->readNum(0,0);
//			char csNum[23]={};
//			sprintf(csNum,"num:%ld",intmy);
//			cout<<csNum<<endl;
//			break;
//			}
//			case CELLTYPE_STRING:
//			{
//			cout<<"true\n";
//			const wchar_t * t = sheet->readStr(0,0);
//			char *pcstr = (char *)malloc(sizeof(char)*(2 * wcslen(t)+1));
//			memset(pcstr , 0 , 2 * wcslen(t)+1 );
//			w2c(pcstr,t,2 * wcslen(t)+1);
//			cout<<"result:"<<pcstr<<endl;
//			cout<<strlen(pcstr)<<endl;
//			memcpy(destStr,pcstr,strlen(pcstr));
//			free(pcstr);
//			break;
//			}
//			case libxl::CELLTYPE_EMPTY:
//			cout<<"true\n";
//			break;
//			}
//			*/
//
//			//表格结构
//			/*
//			0	1	2	3	4	5	6	7
//			章节地图表							
//			id	中文名称	英文名称	所属大章	类型	包含系列任务	iconid	解锁需要通关小点
//			1	1	1	1	1	1	1	1
//			id	name_ch	name_en	capterType	pointType	seriesTaskId	iconId	openPointId
//			string	string	string	int	int	string	string	string
//			*/
//
//
//			int i1 = sheet->firstRow();
//			int i2 = sheet->lastCol();
//			ASSERT(sheet->lastCol() == 8);
//			ASSERT( wcscmp(sheet->readStr(2, 0), L"id") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 1), L"中文名称") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 2), L"英文名称") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 3), L"所属大章") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 4), L"类型") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 5), L"包含系列任务") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 6), L"iconid") == 0 );
//			ASSERT( wcscmp(sheet->readStr(2, 7), L"解锁需要通关小点") == 0 );
//
//			gs_c_config_res* config_res = get_gs_c_config_res();
//
//			UINT8 stat_row_idx = 6;
//			for(int row = sheet->firstRow() + stat_row_idx ; row < sheet->lastRow(); ++row)
//			{
//				CMapPoint * pPoint = new CMapPoint();
//				memset(pPoint, 0, sizeof CMapPoint);
//
//				struct_mappoint_info* pPoint_client =  config_res->add_mappoint_info();
//
//				UINT8 col = 0;
//				char* buf = NULL;
//				UINT32 buf_len = 0;
//				//sheet->cellType(row, i++);
//
//				//const wchar_t* id = sheet->readStr(row, col++);
//				buf_len = libxl_helper::read_str_needfree(sheet, row, col++, buf);
//				pPoint_client->set_id(buf, buf_len);
//				delete[] buf;
//
//				//const wchar_t* name_ch = sheet->readStr(row, col++);
//				buf_len = libxl_helper::read_str_needfree(sheet, row, col++, buf);
//				pPoint_client->set_name_ch(buf, buf_len);
//				delete[] buf;
//
//				//const wchar_t* name_en = sheet->readStr(row, col++);
//				buf_len = libxl_helper::read_str_needfree(sheet, row, col++, buf);
//				pPoint_client->set_name_en(buf, buf_len);
//				delete[] buf;
//
//				double capterType = sheet->readNum(row, col++);
//				pPoint_client
//
//				double pointType = sheet->readNum(row, col++);
//
//				//not to config
//				const wchar_t* seriesTaskId = sheet->readStr(row, col++);
//				//buf_len = libxl_helper::read_str_needfree(sheet, row, col++, buf);
//				//pPoint_client->set_iconid(buf, buf_len);
//				//delete[] buf;
//
//				const wchar_t* iconId = sheet->readStr(row, col++);
//				buf_len = libxl_helper::read_str_needfree(sheet, row, col++, buf);
//				pPoint_client->set_iconid(buf, buf_len);
//				delete[] buf;
//
//				const wchar_t* openPointId = sheet->readStr(row, col++);
//				//buf_len = libxl_helper::read_str_needfree(sheet, row, col++, buf);
//				//pPoint_client->set_(buf, buf_len);
//				//delete[] buf;
//
//
//				//for(int col = sheet->firstCol(); col < sheet->lastCol(); ++col)
//				//{
//
//				//	CellType cellType = sheet->cellType(row, col);
//				//	//std::wcout << "(" << row << ", " << col << ") = ";
//				//	if(sheet->isFormula(row, col))
//				//	{
//				//		//const wchar_t* s = sheet->readFormula(row, col);
//				//		//std::wcout << (s ? s : L"null") << " [formula]";
//				//	}
//				//	else
//				//	{
//				//		switch(cellType)
//				//		{
//				//		case CELLTYPE_EMPTY: /*std::wcout << "[empty]";*/ break;
//				//		case CELLTYPE_NUMBER:
//				//			{
//				//				double d = sheet->readNum(row, col);
//				//				//std::wcout << d << " [number]";
//				//				break;
//				//			}
//				//		case CELLTYPE_STRING:
//				//			{
//				//				const wchar_t* s = sheet->readStr(row, col);
//				//				//std::wcout << (s ? s : L"null") << " [string]";
//
//				//				UINT32 suggest_len = 2 * wcslen(s) + 1;
//				//				char *pcstr = (char *)malloc(sizeof(char) * suggest_len);
//				//				memset(pcstr , 0 , suggest_len );
//				//				w2c(pcstr, s, suggest_len);
//				//				//cout<<"result:"<<pcstr<<endl;
//				//				//cout<<strlen(pcstr)<<endl;
//				//				//memcpy(destStr, pcstr,strlen(pcstr));
//
//				//				free(pcstr);
//
//				//				break;        
//				//			}
//				//		case CELLTYPE_BOOLEAN:
//				//			{
//				//				bool b = sheet->readBool(row, col);
//				//				//std::wcout << (b ? "true" : "false") << " [boolean]";
//				//				break;
//				//			}
//				//		case CELLTYPE_BLANK: /*std::wcout << "[blank]";*/ break;
//				//		case CELLTYPE_ERROR: /*std::wcout << "[error]";*/ break;
//				//		}
//				//	}
//				//	//std::wcout << std::endl;
//				//}
//			}
//
//		}        
//		
//		//cout<<"flag\n";
//		//cout<<sheet->lastRow()<<endl;
//		//cout<<sheet->lastCol()<<endl;
//
//
//		book->release();
//	}
//
//	return false;
//}


bool CMapMgr::Init()
{
	MYSQL * con;
	MYSQL_RES* res;
	MYSQL_ROW row;

	con = CDBMgr::GetInstance()->GetConnection();
	//mysql_query(con, SELECT_CHAPTER);

	//需要知道错误码
	//ASSERT( mysql_real_query(con, SELECT_CHAPTER, (unsigned int)strlen(SELECT_CHAPTER)) == 0 );
	
	int code = CDBMgr::Query(con, SELECT_CHAPTER, (unsigned int)strlen(SELECT_CHAPTER));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", SELECT_CHAPTER);  
		return false;  
	}
	//else{
	//	printf("[%s] made...\n", SELECT_CHAPTER);  
	//}

	res = mysql_store_result(con);   //保存查询结果  

	//检查表结构是否有改变
	string need_fields[] = {"id", "chapter_id", "name"};
	std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
	ASSERT(CDBMgr::check_fields(res, v_need_field));

	while (row = mysql_fetch_row(res))
	{     
		//for (int t = 0; t < mysql_num_fields(res); t++)  
		//	printf("%s ", row[t]);  
		//printf("\n");
	}     
	mysql_free_result(res);
	//mysql_close(&mysql);


	code = CDBMgr::Query(con, SELECT_MAPPOINT, (unsigned int)strlen(SELECT_MAPPOINT));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", SELECT_MAPPOINT);  
		return false;  
	}
	//else{
	//	printf("[%s] made...\n", SELECT_MAPPOINT);  
	//}

	res = mysql_store_result(con);
	//检查表结构是否有改变
	string need_mappoint_fields[] = {"id", "mappoint_id", "chapter_id", "type", "name", "joined_mappoint_id_arr", "enter_level", "hidden"};
	std::vector<std::string> v_need_mappoint_fields(need_mappoint_fields, need_mappoint_fields + sizeof(need_mappoint_fields)/sizeof(need_mappoint_fields[0]));
	ASSERT(CDBMgr::check_fields(res, v_need_mappoint_fields));
	while (row = mysql_fetch_row(res))
	{
		//for (int t = 0; t < mysql_num_fields(res); t++)  
		//	printf("%s ", row[t]);  
		//printf("\n");
		CMapPoint * pPoint = new CMapPoint();
		memset(pPoint, 0, sizeof CMapPoint);
		pPoint->iMappointID = atoi(row[1]);
		pPoint->iChapterID = atoi(row[2]);
		pPoint->bType = atoi(row[3]);

		char* name = UTF8ToANSI(row[4]);
		int len = strlen(name);
		ASSERT( len <= MAXLEN_MAPPOINTNAME);
		memcpy(pPoint->strName, name, len);
		free(name);
		
		std::vector<int> joined_points; 
		split_string_2i(string(row[5]), joined_points, "|");
		ASSERT( joined_points.size() > 0 && joined_points.size() <= MAXNUM_JOINEDMAPPOINT);
		int i = 0;
		for (std::vector<int>::iterator it = joined_points.begin() ; it != joined_points.end(); ++it)
		{
			ASSERT(*it);
			pPoint->arrJoinedMapPointID[i++] = *it;
		}

		pPoint->iEnterLevel = atoi(row[6]);

		pPoint->isHidden = (bool)atoi(row[7]);
		
		ASSERT(AddMapPoint(pPoint));
	}

	mysql_free_result(res);

	//判断arrJoinedMapPointID配置的合法性  
	//1. 邻接点存在 2.本点及其邻接点在同一个章节
	for (map<UINT32, CMapPoint*>::iterator it = mMapMapPointID2MapPoint.begin() ; it != mMapMapPointID2MapPoint.end() ; ++it)
	{
		UINT32 (&joins)[MAXNUM_JOINEDMAPPOINT] = it->second->arrJoinedMapPointID;
		for (int i = 0 ; i < MAXNUM_JOINEDMAPPOINT ; ++i)
		{
			UINT32 join_id = joins[i];//邻接点 mappointid
			if ( 0 == join_id)
			{
				break;
			}
			else{
				map<UINT32, CMapPoint*>::iterator it_join = mMapMapPointID2MapPoint.find(join_id);
				if (it_join == mMapMapPointID2MapPoint.end() || it_join->second->iChapterID != it->second->iChapterID)
				{
					return false;
				}
			}
		}
	}

	return true;
}


bool CMapMgr::UnInit()
{
	//delete mMapMapPointID2MapPoint[1];

	for (map<UINT32, CMapPoint*>::iterator it = mMapMapPointID2MapPoint.begin(); it != mMapMapPointID2MapPoint.end() ; ++it)
	{
		delete it->second;
	}
	mMapMapPointID2MapPoint.clear();

	return true;
}

CMapPoint* CMapMgr::GetMapPointByID( INT32 iMapPointID )
{
	map<UINT32, CMapPoint*>::iterator it = mMapMapPointID2MapPoint.find(iMapPointID);
	if (it != mMapMapPointID2MapPoint.end())
	{
		return it->second;
	}
	else{
		return NULL;
	}
}

bool CMapMgr::AddMapPoint( CMapPoint* pPoint )
{
	ASSERT(pPoint && pPoint->iMappointID);
	if (mMapMapPointID2MapPoint.find(pPoint->iMappointID) != mMapMapPointID2MapPoint.end())
	{
		return false;
	}
	mMapMapPointID2MapPoint[pPoint->iMappointID] = pPoint;
	return true;
}