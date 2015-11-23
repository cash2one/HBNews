#include "GameConfig.h"
#include "ConfigMember.h"
#include <mysql.h>
#include <string>
#include <libxl.h>
#include <assert.h>
#include <iostream>
 #pragma comment(lib, "libmysql.lib") 
#pragma comment(lib, "libxl.lib")
MYSQL mysql;
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);  
bool CreateCode(const std::string& tabName, int nStartRow, libxl::Sheet* pSheet);
void c2w(wchar_t *pwstr,size_t len,const char *str);
int main()
{

	GameConfig* pConfig = GameConfig::GetInstance();
	pConfig->ParseAll();

	{
		DBConfig dbConfig = pConfig->GetDBConfig();
		mysql_init(&mysql);
		if(!mysql_real_connect(&mysql, dbConfig.m_IP.c_str(), dbConfig.m_UserName.c_str(), dbConfig.m_PassWord.c_str(), dbConfig.m_DBName.c_str(), dbConfig.m_Port, NULL, 0))
		{
			printf("query failed! [%s] [%d] [%s] \n", "mysql_real_connect", mysql_errno(&mysql), mysql_error(&mysql));
			getchar();
			return false;
		}
	}

	int nStartRow = 1;
	GenerateCodeConfig generateConfig = pConfig->GetGenerateCodeConfig();

	libxl::IBookT<wchar_t>* pBook;
	pBook = xlCreateBookW();
	assert(pBook);
	libxl::Sheet* pSheet = pBook->addSheet(L"表生成数据");
	assert(pSheet);

	//表头
	pSheet->writeStr(0, 0, L"tab_name");
	pSheet->writeStr(0, 1, L"field");
	pSheet->writeStr(0, 2, L"code");
#if 0
	for(auto tab : generateConfig.m_GenerateCodeConfig)
	{
		assert(CreateCode(tab, nStartRow++, pSheet));
	}
#endif
	for (int i =0; i < generateConfig.m_GenerateCodeConfig.size(); ++i)
	{
		assert(CreateCode(generateConfig.m_GenerateCodeConfig[i], nStartRow++, pSheet));
	}
	//保存
	bool bRet = pBook->save(L"../Bin/GenerateCode.xls");
	if (!bRet)
	{
		MessageBoxA(NULL, "关闭打开的excel再试", "提示", MB_OK);
		pBook->release();
		std::cout << "失败" << std::endl;
		getchar();
		return 0;
	}
	pBook->release();
	std::cout << "完成  生成路径为Bin/GenerateCode.xls";
	getchar();
	return 0;


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


bool CreateCode(const std::string& tabName, int nStartRow, libxl::Sheet* pSheet)
{
	char sqlBuf[256] = {0};
	sprintf_s(sqlBuf, sizeof(sqlBuf), "show full columns from %s", tabName.c_str());
	mysql_query(&mysql, "SET NAMES GBK");
	if(mysql_query(&mysql, sqlBuf) != 0)
	{
		printf("query failed! [%s] [%d] [%s] \n", tabName.c_str(), mysql_errno(&mysql), mysql_error(&mysql));
		return false;
	}
	MYSQL_RES* pRes = mysql_store_result(&mysql);
	MYSQL_ROW  row;

	//写入表名
	int nCol = 0;
	wchar_t szBuf[256];
	c2w(szBuf, 256, tabName.c_str());
	pSheet->writeStr(nStartRow, nCol++, szBuf);

	int nIncNum = 0;	//序号 
	int nRow = 0;
	

	std::string strField;
	std::string strCode;
	while(row = mysql_fetch_row(pRes))
	{
		//拼接字段名
		strField += "\"";
		strField += row[0];
		strField += "\","; 

		//拼代码
		strCode += "pItem->set_";

		//转换小写
		std::string strToloser = row[0];
		for (int j = 0; j < strToloser.size(); ++j)
		{
			strToloser[j] = tolower(strToloser[j]);
		}


		strCode += strToloser + std::string("(");

		//根据类型判断
		if (strstr(row[1], "varchar"))
		{
			strCode += "row[col++]);\n";
		}
		else if(strstr(row[1], "int"))
		{
			strCode += "atoi(row[col++]));\n";
		}
		else if(strstr(row[1], "float"))
		{
			strCode += "atof(row[col++]));\n";
		}
		else if(strstr(row[1], "datetime"))
		{
			MessageBoxA(NULL, "不支持的数据类型datetime", "提示", MB_OK);
			assert(0);
		}
		else
		{
			assert(0);
		}
	}
	//去除field最后一个,
	strField.erase(strField.rfind(L','));

	//插入excel
	wchar_t*  wstrFieldBuf = new wchar_t[strlen(strField.c_str()) * 2 + 2];
	c2w(wstrFieldBuf, strlen(strField.c_str()) * 2 + 2, strField.c_str());
	pSheet->writeStr(nStartRow, nCol++, wstrFieldBuf);

	wchar_t*  wstrCodeBuf = new wchar_t[strlen(strCode.c_str()) * 2 + 2];
	c2w(wstrCodeBuf, strlen(strCode.c_str()) * 2 + 2, strCode.c_str());
	pSheet->writeStr(nStartRow, nCol++, wstrCodeBuf);
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
