#include <iostream>
#include <libxl.h>
#include <string>
#include <assert.h>
#include <locale.h>
#include <tchar.h>
#include <mysql.h>
#include <vector>
#include "GameConfig.h"
#pragma comment(lib, "libmysql.lib") 
#pragma comment(lib, "libxl.lib")
struct TableValue
{
	std::wstring strFieldType;
	std::wstring strValue;
};

void ParseRecord(const libxl::IBookT<wchar_t>* pBook);
bool InsertRecord(std::vector<TableValue>& tabVal,  std::wstring& tabName, std::wstring& sql);
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);
bool ProcessFile(const std::string& strFileName);
MYSQL mysql;
std::wstring strTabName;
std::vector<TableValue> vecTabVal;
std::string g_strFileName;

std::string dir;
bool bClient ;

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
	void ErrorTip(const std::string& strFileName)
	{
		char tip[100] = {0};
		sprintf_s(tip, sizeof(tip), "[error]�ļ�%s �ر�excel,���·������", strFileName.c_str());
		MessageBoxA(NULL, tip, "��ʾ ", MB_OK);
	}
	void ErrorFileErrTip(const std::string& strFileName)
	{
		char tip[100] = {0};
		sprintf_s(tip, sizeof(tip), "[error]�ļ�%s �����ļ�����", strFileName.c_str());
		MessageBoxA(NULL, tip, "��ʾ ", MB_OK);
	}
}

int main()
{
	_tsetlocale(LC_ALL, _T("chs"));

	GameConfig* pConfig = GameConfig::GetInstance();
	pConfig->ParseAll();

	{
		DBConfig dbConfig = pConfig->GetDBConfig();
		mysql_init(&mysql);
		if(!mysql_real_connect(&mysql, dbConfig.m_IP.c_str(), dbConfig.m_UserName.c_str(), dbConfig.m_PassWord.c_str(), dbConfig.m_DBName.c_str(), dbConfig.m_Port, NULL, 0))
		{
			printf("query failed! [%s] [%d] [%s] \n", "mysql_real_connect", mysql_errno(&mysql), mysql_error(&mysql));
			return false;
		}
	}

	std::cout << "�����" << std::endl;

	if(pConfig->GetLoadExcelConfigClient().m_active)
	{
		bClient = true;
		dir = pConfig->GetLoadExcelConfigClient().fromDir;
		std::vector<std::string> vecLoadExcelCfg = pConfig->GetLoadExcelConfigClient().m_LoadExcelConfig;
		for (size_t i = 0; i < vecLoadExcelCfg.size(); ++i)
		{
			assert(ProcessFile(vecLoadExcelCfg[i]));
		}
#if 0
		for (auto config : pConfig->GetLoadExcelConfigClient().m_LoadExcelConfig)
		{
			//vecTabName.clear();
			assert(ProcessFile(config));
		}
#endif
	}

	if(pConfig->GetLoadExcelConfigServer().m_active)
	{
		bClient = false;
		dir = pConfig->GetLoadExcelConfigServer().fromDir;
		std::vector<std::string> LoadExcelCfg = pConfig->GetLoadExcelConfigServer().m_LoadExcelConfig;
		for (size_t i = 0; i < LoadExcelCfg.size(); ++i)
		{
			//vecTabName.clear();
			assert(ProcessFile(LoadExcelCfg[i]));
		}
	}

	


	std::cout << std::endl;
	std::cout << "���" << std::endl;
	getchar();
	getchar();
	return 0;
}

void ParseRecord(const libxl::IBookT<wchar_t>* pBook)
{
	for (int i = 0; i < pBook->sheetCount(); ++i)//��
	{
		libxl::Sheet* pSheet = pBook->getSheet(i);
		strTabName = pSheet->name();
		int nLastRow = pSheet->lastRow();
		
		if (5 == nLastRow)
		{
			char tBuf[1024] = {0};
			w2c(tBuf, strTabName.c_str(), 1024);
			printf("[warning]�ļ�:'%s'��:'%s'û�м�¼�������ݿ�\n", g_strFileName.c_str(), tBuf);
		}
		else
		{
			if(bClient)
			{
				//��ձ�
				
				size_t nPos = strTabName.find(L"_");
				if (nPos != std::wstring::npos)
				{
					strTabName = strTabName.substr(nPos + 1, strTabName.size());
					for (size_t i = 0; i < strTabName.size(); ++i)
					{
						strTabName[i] = tolower(strTabName[i]);
					}
				}
				char sqlBuf[512] = {0};
				char buf[512] = {0};
				w2c(buf, strTabName.c_str(), 512);
				sprintf_s(sqlBuf, "delete from client_config_%s", buf);
				if(mysql_real_query(&mysql, sqlBuf, strlen(sqlBuf)))
				{
					printf("query failed! [%s] [%d] [%s] \n", "mysql_real_connect", mysql_errno(&mysql), mysql_error(&mysql));
					return;
				}

			}
			else
			{
				//��ձ�
				char sqlBuf[512] = {0};
				char buf[512] = {0};
				w2c(buf, strTabName.c_str(), 512);
				sprintf_s(sqlBuf, "delete from %s", buf);
				if(mysql_real_query(&mysql, sqlBuf, strlen(sqlBuf)))
				{
					printf("query failed! [%s] [%d] [%s] \n", "mysql_real_connect", mysql_errno(&mysql), mysql_error(&mysql));
					return;
				}
			}
		}
		std::wstring sqlValues;
		for (int nRow = 5; nRow < pSheet->lastRow(); nRow++)//���ݴӴ��п�ʼ
		{
			for (int nCol = pSheet->firstCol(); nCol < pSheet->lastCol(); nCol++)//��
			{
				TableValue tabVal ;
				if(libxl::CELLTYPE_EMPTY == pSheet->cellType(4, nCol) || libxl::CELLTYPE_BLANK == pSheet->cellType(4, nCol))
				{
					char szBuf[512] = {0};
					sprintf_s(szBuf, sizeof(szBuf), "[error]��%s %d��%d����������Ϊ��", g_strFileName.c_str(), nRow, nCol);
					MessageBoxA(NULL,  szBuf, "��ʾ", MB_OK);
					assert(false);
				}
				tabVal.strFieldType = pSheet->readStr(4, nCol);//��ʾ����
				tabVal.strFieldType = trim(tabVal.strFieldType);
				if (!wcscmp(tabVal.strFieldType.c_str(), L"int"))
				{
					libxl::CellType cellType = pSheet->cellType(nRow, nCol);
					if (libxl::CELLTYPE_EMPTY == cellType || libxl::CELLTYPE_BLANK == cellType)
					{
						char szBuf[512] = {0};
						sprintf_s(szBuf, sizeof(szBuf), "[warning]��%s %d��%d����������Ϊ��", g_strFileName.c_str(), nRow, nCol);
						MessageBoxA(NULL, szBuf, "��ʾ", MB_OK);
						continue;
					}
					wchar_t szBuf[512] = {0};
					swprintf_s(szBuf, L"%d", (int)pSheet->readNum(nRow, nCol));
					tabVal.strValue = szBuf;
				}
				else if(!wcscmp(tabVal.strFieldType.c_str(), L"string"))
				{
					libxl::CellType cellType = pSheet->cellType(nRow, nCol);
					//�����ж��Ƿ�Ϊ��
					if (libxl::CELLTYPE_EMPTY == cellType || libxl::CELLTYPE_BLANK == cellType)
					{
						//char szBuf[512] = {0};
						tabVal.strValue = L"";
					}

					else if (libxl::CELLTYPE_NUMBER == cellType)
					{
						//char buf[256];
						//sprintf_s(buf, "%d %d��ʽ����ȷ������������ʽ����", nRow, nCol);
						//MessageBoxA(NULL, buf, "��ʾ", MB_OK );
						wchar_t szBuf[512];
						swprintf_s(szBuf, L"%d", (int)pSheet->readNum(nRow, nCol));
						tabVal.strValue = szBuf;
					}
					else if(libxl::CELLTYPE_EMPTY == cellType)
					{
						tabVal.strValue = L"";
					}
					else
					{

						tabVal.strValue = pSheet->readStr(nRow, nCol);//��ʽ����ȷ
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
			
			assert(InsertRecord(vecTabVal, strTabName, sqlValues));	
			vecTabVal.clear();
		}

		if(sqlValues.size())
		{

			if(bClient)
			{
				//���ȶ�tabNameת��
				size_t nPos = strTabName.find(L"_");
				if (nPos != std::wstring::npos)
				{
					strTabName = strTabName.substr(nPos + 1, strTabName.size());
					for (size_t i = 0; i < strTabName.size(); ++i)
					{
						strTabName[i] = tolower(strTabName[i]);
					}
				}

				sqlValues = sqlValues.erase(sqlValues.rfind(','));
				sqlValues = L"INSERT INTO client_config_" + strTabName +  L" VALUES " + sqlValues;
			}
			else
			{
				sqlValues = sqlValues.erase(sqlValues.rfind(','));
				sqlValues = L"INSERT INTO " + strTabName + L" VALUES " + sqlValues;
			}

			int nWsqlL = 2 * sqlValues.length() + 1;
			char* pCstr = new char[nWsqlL];
			memset(pCstr, 0, nWsqlL);
			w2c(pCstr, sqlValues.c_str(), nWsqlL);

			char tBuf[1024] = {0};
			w2c(tBuf, strTabName.c_str(), 1024);
			mysql_query(&mysql, "SET NAMES GBK");
			if(mysql_real_query(&mysql, pCstr, strlen(pCstr)) != 0)
			{

				char* szBuf =  new char[1024*1000];
				sprintf_s(szBuf, 1024*1000, "[error]:�ļ�:'%s'��:'%s'query failed!---[%s] [%d] [%s]ȷ���Ƿ�����excel�Ƿ���ͬ�����ȴ���\n", g_strFileName.c_str(), tBuf, pCstr, mysql_errno(&mysql), mysql_error(&mysql));
				printf(szBuf);
				MessageBoxA(NULL, szBuf, "��ʾ", MB_OK);
				delete []szBuf;
				//ASSERT(false);
				return;
				//return false;
			}
			delete[] pCstr;
		}

		char tBuf[1024] = {0};
		w2c(tBuf, strTabName.c_str(), 1024);
		if(bClient)
			printf("[success]�ɹ������ļ�:'%s'��'%s'�����ݿ��Ӧ����Ϊclient_config_%s\n", g_strFileName.c_str(), tBuf, tBuf);
		else
			printf("[success]�ɹ������ļ�:'%s'��'%s'�����ݿ��Ӧ����Ϊ%s\n", g_strFileName.c_str(), tBuf, tBuf);
		strTabName.clear();
	}
}

bool InsertRecord(std::vector<TableValue>& tabVal,  std::wstring& tabName, std::wstring& sql)
{
	std::wstring strVal;
	for (size_t i = 0; i < tabVal.size(); ++i)
	{
		tabVal[i].strFieldType = trim(tabVal[i].strFieldType);
		if (!wcscmp(tabVal[i].strFieldType.c_str(), L"int"))
		{
			strVal += tabVal[i].strValue + L",";
		}
		else if(!wcscmp(tabVal[i].strFieldType.c_str(), L"string"))
		{
			strVal += L"'" + tabVal[i].strValue + L"',";
		}
		else if(!wcscmp(tabVal[i].strFieldType.c_str(), L"float"))
		{
			strVal += tabVal[i].strValue + L",";
		}
		else
		{
			assert(false);
		}
	}
	strVal.erase(strVal.rfind(L","));
	if(bClient)
	{


		sql += L"(" + strVal + L"),";



	}
	else
	{
		sql  +=  L"(" + strVal + L"),";

	}


	return true;
}

//wchar_t to char
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len)  
{  
	int nlength=wcslen(pwstr);  
	//��ȡת����ĳ���  
	size_t nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL,0,NULL, NULL );   
	assert(len > nbytes);
	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�  
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

bool ProcessFile(const std::string& strFileName)
{

	libxl::IBookT<wchar_t>* pBook;
	std::size_t nPointPos = strFileName.rfind(".");
	if (nPointPos == std::string::npos)
	{
		TIP::ErrorFileErrTip(strFileName);
		return 0;
	}
	bool bRetLoad;
	std::string subName = strFileName.substr(nPointPos, strFileName.length() - nPointPos) ;
	wchar_t szBuf[512] = {0};
	c2w(szBuf, 512, (dir  + strFileName).c_str());
	if(subName == ".xls")
	{
		pBook = xlCreateBookW();
		bRetLoad = pBook->load(szBuf);
		if (!bRetLoad)
		{
			TIP::ErrorTip(strFileName);
			return 0;
		}
	}
	else if(subName == ".xlsx")
	{//xlsx
		pBook = xlCreateXMLBookW();
		bRetLoad = pBook->load(szBuf);
		if (!bRetLoad)
		{
			TIP::ErrorTip(strFileName);
			return 0;
		}
	}
	else
	{
		TIP::ErrorFileErrTip(strFileName);
		return 0;
	}

	g_strFileName = strFileName;
	ParseRecord(pBook);
	g_strFileName.clear();
	return true;
}