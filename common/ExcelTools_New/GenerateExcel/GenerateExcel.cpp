#include <iostream>
#include <libxl.h>
#include <string>
#include <assert.h>
#include <locale.h>
#include <tchar.h>
#include <mysql.h>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include "GameConfig.h"
#include "ConfigMember.h"
 #pragma comment(lib, "libmysql.lib") 
#pragma  comment(lib, "libxl.lib")
std::wstring strClientName = L"client_config_";

struct TableAtt
{
	std::wstring strFieldName;
	std::wstring strFieldType;
	std::wstring strFieldComm;

};

struct TableValue
{
	std::wstring strFieldType;
	std::wstring strValue;
};

void GetTabAttAndCreateTab(const libxl::IBookT<wchar_t>* pBook);
bool CreateTable(std::vector<TableAtt>& vecTableAtt, const std::wstring& tabName);
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);
void c2w(wchar_t *pwstr,size_t len,const char *str);
bool GenerateExcel(const std::vector<std::wstring>& vecTabName);
bool ProcessFile(const ConfigItem& strFileName);
void ParseRecord(const libxl::IBookT<wchar_t>* pBook, const std::vector<std::wstring>& vecTab);
bool InsertRecord2Excel(const std::vector<std::vector<TableValue>>& vecAllTabVal);

bool LoadFile(const std::string& strFileName);

std::vector<TableAtt> vecTableAtt;
std::vector<TableValue> vecTabVal;
std::vector<std::wstring> vecTabName;
std::vector<std::wstring> vecRawTabName;//原始表名
MYSQL mysql;
std::wstring g_strFileName;
std::wstring strTabName;
bool bParseRec = false;
std::string fromDir;
std::string toDir;


std::string& trim(std::string &s)   
{  
	if (s.empty())   
	{  
		return s;  
	}  

	s.erase(0,s.find_first_not_of(" "));  
	s.erase(s.find_last_not_of(" ") + 1);  
	return s;  
}  

std::wstring& trim(std::wstring &s)   
{  
	if (s.empty())   
	{  
		return s;  
	}  

	s.erase(0,s.find_first_not_of(L" "));  
	s.erase(s.find_last_not_of(L" ") + 1);  
	return s;  
}  
namespace TIP
{
	void tip()
	{
		char szBuf[512] = {0};
		w2c(szBuf, g_strFileName.c_str(), 512);
		MessageBoxA(NULL, (szBuf + std::string("文件格式可能存在问题，尝试低版本excel")).c_str(), "提示", MB_OK);
		assert(false);
	}
	void ErrorTip()
	{
		MessageBoxA(NULL, "关闭excel,检查路径再试", "提示 ", MB_OK);
	}

	void ErrorFileErrTip()
	{
		MessageBoxA(NULL, "输入文件错误", "提示 ", MB_OK);
	}

	void ErrorTip(const std::string& fileName)
	{
		MessageBoxA(NULL, ("文件" + fileName +"加载错误关闭excel,检查路径再试").c_str(), "提示 ", MB_OK);
	}

	void ErrorFileErrTip(const std::string& fileName)
	{
		MessageBoxA(NULL, ("输入文件" + fileName + "错误").c_str(), "提示 ", MB_OK);
	}
}


int main()
{
	_tsetlocale(LC_ALL, _T("chs"));

	MessageBoxA(NULL, "通过测试发现工具对高版本excel文件支持不好，请使用低版本.xls文件","提示", MB_OK);

	GameConfig* pConfig = GameConfig::GetInstance();
	pConfig->ParseAll();

	//初始化数据库
	{
		mysql_init(&mysql);
		DBConfig dbConfig = pConfig->GetDBConfig();
		if(!mysql_real_connect(&mysql, dbConfig.m_IP.c_str(), dbConfig.m_UserName.c_str(), dbConfig.m_PassWord.c_str(), dbConfig.m_DBName.c_str(), dbConfig.m_Port, NULL, 0))
		{
			printf("query failed! [%s] [%d] [%s] \n", "mysql_real_connect", mysql_errno(&mysql), mysql_error(&mysql));
			return false;
		}
	}
	std::cout << "结果：" << std::endl;
	fromDir = pConfig->GetGenerateExcelConfig().strFromDir;
	toDir = pConfig->GetGenerateExcelConfig().strToDir;

	const std::vector<ConfigItem>& vecGenExcelCfg = pConfig->GetGenerateExcelConfig().m_GenerateExcelConfig;
	for (size_t i = 0; i < vecGenExcelCfg.size(); ++i)
	{
		vecTabName.clear();
		vecRawTabName.clear();
		bParseRec = vecGenExcelCfg[i].m_bDB;
		assert(ProcessFile(vecGenExcelCfg[i]));
	}
#if 0
	for (auto config : pConfig->GetGenerateExcelConfig().m_GenerateExcelConfig)
	{

		vecTabName.clear();
		vecRawTabName.clear();
		bParseRec = config.m_bDB;
		assert(ProcessFile(config));
	}
#endif
	std::cout << std::endl;
	std::cout << "完成" << std::endl;
	getchar();
	getchar();
	return 0;
}


void GetTabAttAndCreateTab(const libxl::IBookT<wchar_t>* pBook)
{
	char tBuf[256] = {0};
	char fBuf[256] = {0};
	for (int i = 0; i < pBook->sheetCount(); ++i)//表单
	{
		libxl::Sheet* pSheet = pBook->getSheet(i);
		strTabName = pSheet->name();
		
		bool isLegal = true;
		//检测名字是否合法
		size_t iter = strTabName.find(L"_");
		if (iter != std::wstring::npos)
		{
			//检测是否是中文_大写的格式
			std::wstring wSubName = strTabName.substr(iter + 1, strTabName.size());
			for (size_t j = 0 ; j < wSubName.size(); ++j)
			{
				if (!(isupper(wSubName[j]) || wSubName[j] == L'_')) 
				{
					memset(tBuf, 0, sizeof(tBuf));
					w2c(tBuf, strTabName.c_str(), 256);

					memset(fBuf, 0, sizeof(fBuf));
					w2c(fBuf, g_strFileName.c_str(), 256);

					char szErrBuf[256] = {0};
					sprintf_s(szErrBuf, sizeof(szErrBuf), "[error]文件[%s] 表名[%s]不符合规范使用中文_大写字母形式", fBuf,tBuf);
					printf(szErrBuf);
					MessageBoxA(NULL, szErrBuf, "提示", MB_OK);
					isLegal = false;
					break;
				}
			}
			//修改表名字
			if (isLegal)
			{
				strTabName = wSubName;
				for (size_t j = 0; j < strTabName.size(); ++j)
				{
					strTabName[j] = tolower(strTabName[j]);
				}
			}
		}

		if (!isLegal)
		{
			continue;
		}

		for (int nRow = pSheet->firstRow(); nRow < pSheet->lastRow(); nRow++)//行
		{
			if (nRow == 2)	//此行表示是否存储数据库配置
			{
				for (int nCol = pSheet->firstCol(); nCol < pSheet->lastCol(); nCol++)//列
				{
					libxl::CellType cellType = pSheet->cellType(nRow, nCol);
					if (libxl::CELLTYPE_BLANK == cellType)//没有填写是否服务器配置
					{
						memset(tBuf, 0, sizeof(tBuf));
						w2c(tBuf, pSheet->name(), 256);

						memset(fBuf, 0, sizeof(fBuf));
						w2c(fBuf, g_strFileName.c_str(), 256);

						char szErrBuf[256] = {0};
						sprintf_s(szErrBuf, sizeof(szErrBuf), "文件[%s] 表[%s] [%d]行 [%d]列没有填写是否服务器配置数据", fBuf,tBuf, nRow, nCol);
						MessageBoxA(NULL, szErrBuf, "提示", MB_OK);
						printf("[error]:文件[%s]表[%s]有字段没有填写是否服务器配置数据\n", fBuf, tBuf);
						break;
					} 
					if (1 == pSheet->readNum(nRow, nCol))//1表示服务器配置
					{

						std::wstring x1 = pSheet->readStr(nRow + 1, nCol);
						if (pSheet == NULL)
						{
							TIP::tip();
						}
						std::wstring x2 = pSheet->readStr(nRow + 2, nCol);
						if (pSheet == NULL)
						{
							TIP::tip();
						}
						std::wstring x3 = pSheet->readStr(nRow - 1, nCol);
						if (pSheet == NULL)
						{
							TIP::tip();
						}
						TableAtt tabAtt = {x1, x2, x3};
						vecTableAtt.push_back(tabAtt);

						
					}
				}
				break;
			}	
#if 0

				libxl::CellType cellType = pSheet->cellType(nRow, nCol);
				switch (cellType)
				{
				case libxl::CELLTYPE_EMPTY:
					std::cout << "[blank]  ";
					break;
				case libxl::CELLTYPE_NUMBER:
					std::cout << pSheet->readNum(nRow, nCol) << " ";
					break;
				case libxl::CELLTYPE_STRING:
					{
						auto str = pSheet->readStr(nRow, nCol) ;
						std::wcout << pSheet->readStr(nRow, nCol) << " ";
						break;

					}

				case libxl::CELLTYPE_BOOLEAN: 
					std::cout << pSheet->readBool(nRow, nCol) << " ";
					break;
				case libxl::CELLTYPE_BLANK:
					std::cout << "[blank] ";
					break;
				case libxl::CELLTYPE_ERROR:
					std::cout << "[error] ";
					break;
				default:
					break;
				}
			}
			std::cout << std::endl;
#endif
		}
		if(!vecTableAtt.size())
		{
			memset(tBuf, 0, sizeof(tBuf));
			w2c(tBuf, pSheet->name(), 256);

			memset(fBuf, 0, sizeof(fBuf));
			w2c(fBuf, g_strFileName.c_str(), 256);

			char szErrBuf[256] = {0};
			sprintf_s(szErrBuf, sizeof(szErrBuf), "文件[%s] 表[%s]没有字段需要录入数据库", fBuf,tBuf);
			printf("[warning:]%s\n", szErrBuf);
			MessageBoxA(NULL, szErrBuf, "提示", MB_OK);
			continue;
		}
		else
		{
			if(CreateTable(vecTableAtt, strTabName))
			{
				vecTabName.push_back(strTabName);
				vecRawTabName.push_back(pSheet->name());
			}
			vecTableAtt.clear();
			strTabName.clear();
			
		}
		
	}
	//std::cout << "数据库表创建成功" << std::endl;
	if(vecTabName.size())
	{
		assert(GenerateExcel(vecTabName));

		//首先看是否配置解析记录
		if(bParseRec)
			ParseRecord(pBook, vecTabName);	
	}
}

void ParseRecord(const libxl::IBookT<wchar_t>* pBook, const std::vector<std::wstring>& vecTab)
{
	for (int i = 0; i < pBook->sheetCount(); ++i)//表单
	{
		std::vector<std::vector<TableValue>> vecAllTabVal;
		libxl::Sheet* pSheet = pBook->getSheet(i);

		std::wstring tabName = strTabName = pSheet->name();
		size_t iterRaw = tabName.find(L"_");
		if (iterRaw != std::wstring::npos)
		{
			tabName = tabName.substr(iterRaw + 1, tabName.size());
			for (size_t j = 0; j < tabName.size(); ++j)
			{
				tabName[j] = tolower(tabName[j]);
			}
		}

		std::vector<std::wstring>::const_iterator iter = std::find(vecTab.begin(), vecTab.end(), tabName);
		if (iter == vecTab.end())
			continue;

		for (int nRow = 5; nRow < pSheet->lastRow(); nRow++)//数据从此行开始
		{
			for (int nCol = pSheet->firstCol(); nCol < pSheet->lastCol(); nCol++)//列
			{
				//获取该列是否要存储
				if (1 == pSheet->readNum(2, nCol))
				{
					TableValue tabVal ;
					tabVal.strFieldType = pSheet->readStr(4, nCol);
					tabVal.strFieldType = trim(tabVal.strFieldType);
					if (!wcscmp(tabVal.strFieldType.c_str(), L"int"))
					{
						wchar_t szBuf[512] = {0};
						swprintf_s(szBuf, L"%d", (int)pSheet->readNum(nRow, nCol));
						tabVal.strValue = szBuf;
					}
					else if(!wcscmp(tabVal.strFieldType.c_str(), L"string"))
					{
						libxl::CellType cellType = pSheet->cellType(nRow, nCol);
						if (libxl::CELLTYPE_NUMBER == cellType)
						{
							//char buf[256];
							//sprintf_s(buf, "%d %d格式不正确，尝试其他方式解析", nRow, nCol);
							//MessageBoxA(NULL, buf, "提示", MB_OK );
							wchar_t szBuf[512];
							swprintf_s(szBuf, L"%d", (int)pSheet->readNum(nRow, nCol));
							tabVal.strValue = szBuf;
						}
						else
						{
							tabVal.strValue = pSheet->readStr(nRow, nCol);//格式不正确
						}
					}
					else if(!wcscmp(tabVal.strFieldType.c_str(), L"float"))
					{
						wchar_t szBuf[512] = {0};
						swprintf_s(szBuf, L"%f", (float)pSheet->readNum(nRow, nCol));
						tabVal.strValue =  szBuf;
					}
					else
					{
						assert(false);
					}
					vecTabVal.push_back(tabVal);
					
				}
			}
			vecAllTabVal.push_back(vecTabVal);//将每行的插入进去
			vecTabVal.clear();
		}
		InsertRecord2Excel(vecAllTabVal);
		strTabName.clear();
		vecAllTabVal.clear();
	}
}
bool InsertRecord2Excel(const std::vector<std::vector<TableValue>>& vecAllTabVal)
{
	//加载表

	libxl::IBookT<wchar_t>* pBook;
	pBook = xlCreateBookW();
	wchar_t szToDir[512] = {0};
	c2w(szToDir, 512, toDir.c_str());
	bool bRetLoad = pBook->load((szToDir + strClientName + g_strFileName).c_str());
	if (!bRetLoad)
	{
		TIP::ErrorTip();
		return false;
	}

	
	std::wstring strVal;
	libxl::Sheet* pSheet = NULL;
	for (int i = 0; i < pBook->sheetCount(); ++i)
	{
		if(pBook->getSheet(i)->name() == strTabName)
		{
			pSheet = pBook->getSheet(i);
			break;
		}
	}

	if (!pSheet)
	{
		return false;
	}

	static int nRowStart = 5;
	for (size_t i = 0; i < vecAllTabVal.size(); ++i)
	{
		for (size_t j = 0; j < vecAllTabVal[i].size(); ++j)
		{
			TableValue value = vecAllTabVal[i][j];
			value.strFieldType = trim(value.strFieldType);
			if (!wcscmp(value.strFieldType.c_str(), L"int"))
			{
				char buf[512] = {0};
				w2c(buf, value.strValue.c_str(), 512);
				bool b =pSheet->writeNum(nRowStart, j, atoi(buf));
			}
			else if(!wcscmp(value.strFieldType.c_str(), L"string"))
			{
				pSheet->writeStr(nRowStart, j, value.strValue.c_str());
			}
			else if(!wcscmp(value.strFieldType.c_str(), L"float"))
			{
				char buf[512] = {0};
				w2c(buf, value.strValue.c_str(), 512);
				bool b =pSheet->writeNum(nRowStart, j, atof(buf));
			}
			else
			{
				assert(false);
			}
		}
		nRowStart++;
	}
#if 0
	for (auto tab : vecAllTabVal)//tab每一行
	{
		for (int i = 0; i < tab.size(); ++i)//针对每行的操作
		{
			tab[i].strFieldType = trim(tab[i].strFieldType);
			if (!wcscmp(tab[i].strFieldType.c_str(), L"int"))
			{
				char buf[512] = {0};
				w2c(buf, tab[i].strValue.c_str(), 512);
				bool b =pSheet->writeNum(nRowStart, i, atoi(buf));
			}
			else if(!wcscmp(tab[i].strFieldType.c_str(), L"string"))
			{
				pSheet->writeStr(nRowStart, i, tab[i].strValue.c_str());
			}
			else if(!wcscmp(tab[i].strFieldType.c_str(), L"float"))
			{
				char buf[512] = {0};
				w2c(buf, tab[i].strValue.c_str(), 512);
				bool b =pSheet->writeNum(nRowStart, i, atof(buf));
			}
			else
			{
				assert(false);
			}
		}
		nRowStart++;
	}
#endif
	nRowStart = 5;
	
	pBook->save((szToDir + strClientName + g_strFileName).c_str());
	pBook->release();
	return true;
}

bool CreateTable(std::vector<TableAtt>& vecTableAtt, const std::wstring& tabName)
{
	//查看数据库是否存在表，如果存在直接删除
	//drop table  if exists monster
	char szBuf[512] = {0};
	w2c(szBuf, tabName.c_str(), 256);
	std::string strSql;
	strSql += "drop table  if exists client_config_" + std::string(szBuf);
	if(mysql_real_query(&mysql, strSql.c_str(), strSql.length()) != 0)
	{
		printf("query failed! [%s] [%d] [%s] \n", szBuf, mysql_errno(&mysql), mysql_error(&mysql));

		return false;
	}
	std::wstring atts;

	for (size_t i = 0; i < vecTableAtt.size(); ++i)
	{
		atts += std::wstring(L"`") + vecTableAtt[i].strFieldName + L"`" + L" ";
		vecTableAtt[i].strFieldType = trim(vecTableAtt[i].strFieldType);
		if (!wcscmp(vecTableAtt[i].strFieldType.c_str(), L"int"))
		{
			atts += L"int(10) comment '" + vecTableAtt[i].strFieldComm + L"',";
		}
		else if(!wcscmp(vecTableAtt[i].strFieldType.c_str(), L"string"))
		{
			atts += L"varchar(100) comment '" + vecTableAtt[i].strFieldComm + L"',";
		}
		else if(!wcscmp(vecTableAtt[i].strFieldType.c_str(), L"float"))
		{
			atts += L"float comment '" + vecTableAtt[i].strFieldComm + L"',";
		}
		else
		{

			printf("error! table [%s] field[%s]", tabName.c_str(), vecTableAtt[i].strFieldName.c_str());
			assert(false);
		}
	}
#if 0
	for (auto att : vecTableAtt)
	{
		atts += att.strFieldName + L" ";
		att.strFieldType = trim(att.strFieldType);
		if (!wcscmp(att.strFieldType.c_str(), L"int"))
		{
			atts += L"int(10) comment '" + att.strFieldComm + L"',";
		}
		else if(!wcscmp(att.strFieldType.c_str(), L"string"))
		{
			atts += L"varchar(100) comment '" + att.strFieldComm + L"',";
		}
		else if(!wcscmp(att.strFieldType.c_str(), L"float"))
		{
			atts += L"float comment '" + att.strFieldComm + L"',";
		}
		else
		{

			printf("error! table [%s] field[%s]", tabName.c_str(), att.strFieldName.c_str());
			assert(false);
		}
	}
#endif
	std::wstring wCreateSql;
	
	atts.erase(atts.rfind(L','));
	wCreateSql += L"CREATE TABLE " + strClientName + tabName + L"(" + atts + L") ";

	int nWsqlL = 2 * wCreateSql.length() + 1;
	char* pCstr = new char[nWsqlL];
	memset(pCstr, 0, nWsqlL);
	w2c(pCstr, wCreateSql.c_str(), nWsqlL);
	mysql_query(&mysql, "SET NAMES GBK");
	if(mysql_real_query(&mysql, pCstr, strlen(pCstr)) != 0)
	{
		char fBuf[256] = {0};
		char tBuf[256] = {0};
		w2c(fBuf, g_strFileName.c_str(), 256);
		w2c(tBuf, strTabName.c_str(), 256);
		sprintf_s(szBuf, sizeof(szBuf),  "文件[%s]表[%s]query failed![%d] [%s]检查是否有非法字段等 \n", fBuf, tBuf, mysql_errno(&mysql), mysql_error(&mysql));
		printf("[error]:%s\n", szBuf);
		MessageBoxA(NULL, szBuf, "提示", MB_OK);

		return false;
	}
	delete[] pCstr;
	return true;
}

void WriteExcelStr(libxl::Sheet* pSheet, int nRow, int nCol, char* row){
	wchar_t*  wstrBuf = new wchar_t[strlen(row) * 2 + 2];
	c2w(wstrBuf, strlen(row) * 2 + 2, row);
	pSheet->writeStr(nRow, nCol, wstrBuf);
	delete[] wstrBuf;
}

void WriteExcelInt(libxl::Sheet* pSheet, int nRow, int nCol, int nNum){
	pSheet->writeNum(nRow, nCol, nNum);
};

bool GenerateExcel(const std::vector<std::wstring>& vecTabName)
{
	if (!vecTabName.size())
	{
		return false;
	}


	libxl::Book* pBook = xlCreateBookW();//生成低版本.xls
	int nFlag = 0;

	for (size_t i = 0; i < vecTabName.size(); ++i)
	{
		char szBuf[256] = {0};
		w2c(szBuf, vecTabName[i].c_str(), 256);

		char sqlBuf[256] = {0};
		sprintf_s(sqlBuf, sizeof(sqlBuf), "show full columns from client_config_%s", szBuf);
		mysql_query(&mysql, "SET NAMES GBK");
		if(mysql_query(&mysql, sqlBuf) != 0)
		{
			printf("query failed! [%s] [%d] [%s] \n", szBuf, mysql_errno(&mysql), mysql_error(&mysql));
			return false;
		}
		MYSQL_RES* pRes = mysql_store_result(&mysql);
		MYSQL_ROW  row;


		assert(pBook);
		libxl::Sheet* pSheet = pBook->addSheet(vecRawTabName[nFlag++].c_str());
		assert(pSheet);

		int nIncNum = 0;	//序号 
		int nRow = 0;
		int nCol = 0;
		while(row = mysql_fetch_row(pRes))
		{
			nRow = 0;
			WriteExcelInt(pSheet, nRow++, nCol, nIncNum++);
			WriteExcelStr(pSheet, nRow++, nCol, row[8]);
			WriteExcelInt(pSheet, nRow++, nCol, 1);//表示服务器配置
			WriteExcelStr(pSheet, nRow++, nCol, row[0]);

			if (strstr(row[1], "varchar"))
			{
				WriteExcelStr(pSheet, nRow++, nCol, "string");
			}
			else if(strstr(row[1], "int"))
			{
				WriteExcelStr(pSheet, nRow++, nCol, "int");
			}
			else if(strstr(row[1], "float"))
			{
				WriteExcelStr(pSheet, nRow++, nCol, "float");
			}
			else
			{
				assert(0);
			}

			nCol++;
		}
	}
#if 0
	for (auto tab : vecTabName)
	{
		char szBuf[256] = {0};
		w2c(szBuf, tab.c_str(), 256);

		char sqlBuf[256] = {0};
		sprintf_s(sqlBuf, sizeof(sqlBuf), "show full columns from client_config_%s", szBuf);
		mysql_query(&mysql, "SET NAMES GBK");
		if(mysql_query(&mysql, sqlBuf) != 0)
		{
			printf("query failed! [%s] [%d] [%s] \n", szBuf, mysql_errno(&mysql), mysql_error(&mysql));
			return false;
		}
		MYSQL_RES* pRes = mysql_store_result(&mysql);
		MYSQL_ROW  row;

		
		assert(pBook);
		libxl::Sheet* pSheet = pBook->addSheet(vecRawTabName[nFlag++].c_str());
		assert(pSheet);

		int nIncNum = 0;	//序号 
		int nRow = 0;
		int nCol = 0;
		while(row = mysql_fetch_row(pRes))
		{
			nRow = 0;
			WriteExcelInt(pSheet, nRow++, nCol, nIncNum++);
			WriteExcelStr(pSheet, nRow++, nCol, row[8]);
			WriteExcelInt(pSheet, nRow++, nCol, 1);//表示服务器配置
			WriteExcelStr(pSheet, nRow++, nCol, row[0]);

			if (strstr(row[1], "varchar"))
			{
				WriteExcelStr(pSheet, nRow++, nCol, "string");
			}
			else if(strstr(row[1], "int"))
			{
				WriteExcelStr(pSheet, nRow++, nCol, "int");
			}
			else if(strstr(row[1], "float"))
			{
				WriteExcelStr(pSheet, nRow++, nCol, "float");
			}
			else
			{
				assert(0);
			}

			nCol++;
		}
	}
#endif
	wchar_t szToDir[512] = {0};
	c2w(szToDir, 512, toDir.c_str());
	assert(pBook->save((szToDir + strClientName + g_strFileName).c_str()));
	pBook->release();

	std::string strTableList;
	for (size_t i = 0; i < vecTabName.size(); ++i)
	{
		char szBuf[256] = {0};
		w2c(szBuf, vecTabName[i].c_str(), 256);
		strTableList += std::string("client_config_") + szBuf ;
		strTableList += " ";
	}
#if 0
	for (auto tab : vecTabName)
	{
		char szBuf[256] = {0};
		w2c(szBuf, tab.c_str(), 256);
		strTableList += std::string("client_config_") + szBuf ;
		strTableList += " ";
	}
#endif
	strTableList.erase(strTableList.end() - 1);
	char fBuf[256] = {0};
	w2c(fBuf, g_strFileName.c_str(), 256);


	std::string strRawTableList;
	for (size_t i = 0; i < vecRawTabName.size(); ++i)
	{
		char szBuf[256] = {0};
		w2c(szBuf, vecRawTabName[i].c_str(), 256);
		strRawTableList += szBuf;
		strRawTableList += " ";
	}
#if 0
	for (auto tab : vecRawTabName)
	{
		char szBuf[256] = {0};
		w2c(szBuf, tab.c_str(), 256);
		strRawTableList += szBuf;
		strRawTableList += " ";
	}
#endif
	strRawTableList.erase(strRawTableList.end() - 1);
	printf("[success]成功建立数据库表'%s'从文件：'%s' 中的表：'%s' 导出excel文件名：'%s' 导出目录为当前的bin目录下\n", strTableList.c_str(),
					fBuf, strRawTableList.c_str(), ("client_config_" + std::string(fBuf)).c_str());
	return true;
}

//wchar_t to char
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len)  
{  
	int nlength=wcslen(pwstr);  
	//获取转换后的长度  
	size_t nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL,0,NULL, NULL );   
	assert(len > nbytes);
	// 通过以上得到的结果，转换unicode 字符为ascii 字符  
	WideCharToMultiByte( 0,0, pwstr, nlength,   pcstr, nbytes, NULL,   NULL );  
	return pcstr ;  
}  

void c2w(wchar_t *pwstr,size_t len,const char *str)
{
	if(str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, NULL, 0);

		assert(len > n);

		MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, pwstr, (int)n);

		pwstr[n] = 0;

	}
}

bool ProcessFile(const ConfigItem& config)
{


	libxl::IBookT<wchar_t>* pBook;
	std::size_t nPointPos = config.m_strFileName.rfind(".");
	if (nPointPos == std::string::npos)
	{
		TIP::ErrorFileErrTip(config.m_strFileName);
		return 0;
	}
	bool bRetLoad;
	std::string subName = config.m_strFileName.substr(nPointPos, config.m_strFileName.length() - nPointPos) ;
	wchar_t szBuf[512] = {0};
	c2w(szBuf, 512, (fromDir + config.m_strFileName).c_str());
	if(subName == ".xls")
	{
		pBook = xlCreateBookW();
		

		bRetLoad = pBook->load(szBuf);
		if (!bRetLoad)
		{
			TIP::ErrorTip(config.m_strFileName);
			return 0;
		}
	}
	else if(subName == ".xlsx")
	{//xlsx
		pBook = xlCreateXMLBookW();
		bRetLoad = pBook->load(szBuf);
		if (!bRetLoad)
		{
			TIP::ErrorTip(config.m_strFileName);
			return 0;
		}
	}
	else
	{
		TIP::ErrorFileErrTip(config.m_strFileName);
		return 0;
	}

///*	std::size_t nSlashPos = config.m_strFileName.rfind("\\");
//	if (nSlashPos == std::string::npos)
//	{
//		ErrorFileErrTip(config.m_strFileName);
//		*/return 0;
//	}
	//int nSlashPos = 0;

	c2w(szBuf, 512, config.m_strFileName.c_str());
	g_strFileName = szBuf;

	GetTabAttAndCreateTab(pBook);
	return true;
}


