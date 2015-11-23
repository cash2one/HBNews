#include "GameConfig.h"
#include <mysql.h>
#include <iostream>
#include <libxl.h>
#include <assert.h>
#include <vector>
 #pragma comment(lib, "libmysql.lib") 
#pragma comment(lib, "libxl.lib")
MYSQL mysql;
std::vector<std::wstring> wString;
bool GenerateExcel(const std::vector<std::wstring>& vecTabName);
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);
void c2w(wchar_t *pwstr,size_t len,const char *str);
std::string dir;

namespace WRITE
{
	void WriteExcelStr(libxl::Sheet* pSheet, int nRow, int nCol, char* row){
		wchar_t*  wstrBuf = new wchar_t[strlen(row) * 2 + 2];
		c2w(wstrBuf, strlen(row) * 2 + 2, row);
		pSheet->writeStr(nRow, nCol, wstrBuf);
		delete[] wstrBuf;
	}

	void WriteExcelInt(libxl::Sheet* pSheet, int nRow, int nCol, int nNum){
		pSheet->writeNum(nRow, nCol, nNum);
	}
}

int main()
{
	GameConfig* pConfig = GameConfig::GetInstance();
	pConfig->ParseAll();

	//初始化数据库
	{
		mysql_init(&mysql);
		DBConfig dbConfig = pConfig->GetDBConfig();
		if(!mysql_real_connect(&mysql, dbConfig.m_IP.c_str(), dbConfig.m_UserName.c_str(), dbConfig.m_PassWord.c_str(), dbConfig.m_DBName.c_str(), dbConfig.m_Port, NULL, 0))
		{
			printf("query failed! [%s] [%d] [%s] \n", "mysql_real_connect", mysql_errno(&mysql), mysql_error(&mysql));
			getchar();
			return false;
		}
	}
	std::cout << "结果：" << std::endl;
	dir = pConfig->GetGenerateExcelFromTableConfig().m_svnDir;
	std::vector<std::string> vecGenExcelFromTableCfg = pConfig->GetGenerateExcelFromTableConfig().m_GenerateExcelFromTableConfig;
	for (size_t i = 0; i < vecGenExcelFromTableCfg.size(); ++i)
	{
		wchar_t szBuf[512] = {0};
		c2w(szBuf, 512, vecGenExcelFromTableCfg[i].c_str());
		wString.push_back(std::wstring(szBuf));
	}
#if 0
	for (auto config : pConfig->GetGenerateExcelFromTableConfig().m_GenerateExcelFromTableConfig)
	{
		wchar_t szBuf[512] = {0};
		c2w(szBuf, 512, config.c_str());
		wString.push_back(std::wstring(szBuf));
	}
#endif
	GenerateExcel(wString);
	std::cout << std::endl;
	std::cout << "完成" << std::endl;
	getchar();
	getchar();
	return 0;
}

bool GenerateExcel(const std::vector<std::wstring>& vecTabName)
{
	if (!vecTabName.size())
	{
		return false;
	}

	
	int nFlag = 0;

	for (size_t i = 0; i < vecTabName.size(); ++i)
	{
		libxl::Book* pBook = xlCreateBookW();//生成低版本.xls
		char szBuf[256] = {0};
		w2c(szBuf, vecTabName[i].c_str(), 256);

		char sqlBuf[256] = {0};
		sprintf_s(sqlBuf, sizeof(sqlBuf), "show full columns from %s", szBuf);
		mysql_query(&mysql, "SET NAMES GBK");
		if(mysql_query(&mysql, sqlBuf) != 0)
		{
			printf("query failed! [%s] [%d] [%s] \n", szBuf, mysql_errno(&mysql), mysql_error(&mysql));
			return false;
		}
		MYSQL_RES* pRes = mysql_store_result(&mysql);
		MYSQL_ROW  row;


		assert(pBook);
		libxl::Sheet* pSheet = pBook->addSheet(wString[nFlag++].c_str());
		assert(pSheet);

		int nIncNum = 0;	//序号 
		int nRow = 0;
		int nCol = 0;
		std::vector<std::string> vecType;
		while(row = mysql_fetch_row(pRes))
		{
			nRow = 0;
			WRITE::WriteExcelInt(pSheet, nRow++, nCol, nIncNum++);
			WRITE::WriteExcelStr(pSheet, nRow++, nCol, row[8]);
			WRITE::WriteExcelInt(pSheet, nRow++, nCol, 1);//表示服务器配置
			WRITE::WriteExcelStr(pSheet, nRow++, nCol, row[0]);

			if (strstr(row[1], "varchar"))
			{
				WRITE::WriteExcelStr(pSheet, nRow++, nCol, "string");
				vecType.push_back("string");
			}
			else if(strstr(row[1], "int"))
			{
				WRITE::WriteExcelStr(pSheet, nRow++, nCol, "int");
				vecType.push_back("int");
			}
			else if(strstr(row[1], "float"))
			{
				WRITE::WriteExcelStr(pSheet, nRow++, nCol, "float");
				vecType.push_back("float");
			}
			else
			{
				assert(0);
			}

			nCol++;
		}
		
		mysql_free_result(pRes);
		//将记录插入该表
		memset(sqlBuf, 0, sizeof(sqlBuf));
		sprintf_s(sqlBuf, sizeof(sqlBuf), "SELECT * FROM %s", szBuf);
		mysql_query(&mysql, "SET NAMES GBK");
		if(mysql_query(&mysql, sqlBuf) != 0)
		{
			printf("query failed! [%s] [%d] [%s] \n", szBuf, mysql_errno(&mysql), mysql_error(&mysql));
			return false;
		}
		pRes = mysql_store_result(&mysql);
		nRow = 5;
		int nFieldNum  = mysql_field_count(&mysql);
		while(row = mysql_fetch_row(pRes))
		{	
			nCol = 0;
			for (int i = 0; i < nFieldNum; ++i)
			{
				if (vecType[i] == "string")
				{
					if(row[i])
					{
						WRITE::WriteExcelStr(pSheet, nRow, nCol++, row[i]);
					}
				}
				else if(vecType[i] == "int")
				{
					if(row[i])
					{
						WRITE::WriteExcelInt(pSheet, nRow, nCol++, atoi(row[i]));
					}
					
				}
				else if(vecType[i] == "float")
				{
					if(row[i])
					{
						WRITE::WriteExcelInt(pSheet, nRow, nCol++, atof(row[i]));
					}
					
				}
				
			}
			nRow++;
		}
		wchar_t szwBuf[512] = {0};
		c2w(szwBuf, 512, dir.c_str());
		assert(pBook->save((szwBuf + vecTabName[i] + L".xls").c_str()));
		printf("生成表%s成功 目录:%s\n", szBuf, dir.c_str());
		pBook->release();
	}
#if 0
	std::string strTableList;
	for (auto tab : vecTabName)
	{
		char szBuf[256] = {0};
		w2c(szBuf, tab.c_str(), 256);
		strTableList += std::string("client_config_") + szBuf ;
		strTableList += " ";
	}
	strTableList.erase(strTableList.end() - 1);
	char fBuf[256] = {0};
	w2c(fBuf, g_strFileName.c_str(), 256);


	std::string strRawTableList;
	for (auto tab : vecRawTabName)
	{
		char szBuf[256] = {0};
		w2c(szBuf, tab.c_str(), 256);
		strRawTableList += szBuf;
		strRawTableList += " ";
	}
	strRawTableList.erase(strRawTableList.end() - 1);
	printf("[success]成功建立数据库表'%s'从文件：'%s' 中的表：'%s' 导出excel文件名：'%s' 导出目录为当前的bin目录下\n", strTableList.c_str(),
		fBuf, strRawTableList.c_str(), ("client_config_" + std::string(fBuf) + ".xls").c_str());
#endif
	return true;
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
